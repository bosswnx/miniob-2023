/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2022/6/6.
//

#include "sql/stmt/select_stmt.h"
#include "common/rc.h"
#include "sql/expr/expression.h"
#include "event/sql_debug.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/filter_stmt.h"
#include "common/log/log.h"
#include "common/lang/string.h"
#include "storage/db/db.h"
#include "storage/field/field.h"
#include "storage/table/table.h"
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

SelectStmt::~SelectStmt()
{
  if (nullptr != filter_stmt_) {
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }
}

// 把该表的所有属性（字段）加入到 query_exprs 中
static void wildcard_fields(Table *table, std::vector<std::unique_ptr<Expression>> &query_exprs, 
    std::vector<Field> &fields, std::vector<string> &query_aliases, bool with_table_name)
{
  const TableMeta &table_meta = table->table_meta();
  const int field_num = table_meta.field_num();
  for (int i = table_meta.sys_field_num(); i < field_num; i++) {
    query_exprs.emplace_back(new FieldExpr(table, table_meta.field(i)));
    fields.emplace_back(table, table_meta.field(i));
    if (with_table_name) {
      string alias = table->name();
      alias += ".";
      alias += table_meta.field(i)->name();
      query_aliases.push_back(alias);
    } else {
      query_aliases.push_back(table_meta.field(i)->name());
    }
  }
}

RC SelectStmt::get_table_and_field(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
    const RelAttrExpr *attr, Table *&table, const FieldMeta *&field)
{
  if (common::is_blank(attr->table_name().c_str())) {
    table = default_table;
  } else if (nullptr != tables) {
    auto iter = tables->find(attr->table_name().c_str());
    if (iter != tables->end()) {
      table = iter->second;
    }
  } else {
    table = db->find_table(attr->table_name().c_str());
  }
  if (nullptr == table) {
    LOG_WARN("No such table: attr.relation_name: %s", attr->table_name().c_str());
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  field = table->table_meta().field(attr->field_name().c_str());
  if (nullptr == field) {
    LOG_WARN("no such field in table: table %s, field %s", table->name(), attr->field_name().c_str());
    table = nullptr;
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }

  return RC::SUCCESS;
}

// 递归处理 expression，将其中的 RelAttrExpr 转换为 FieldExpr
RC SelectStmt::make_field_expr(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
    std::unique_ptr<Expression> &expr, std::vector<Field> &query_fields, std::shared_ptr<std::unordered_map<string,string>> alias2name)
{
  RC rc = RC::SUCCESS;
   if (expr->type() == ExprType::VALUE) {
    return RC::SUCCESS;
  }
  if (expr->type() == ExprType::ARITHMETIC) {
    ArithmeticExpr *arith_expr = static_cast<ArithmeticExpr *>(expr.get());
    if (arith_expr->left() != nullptr) {
      rc = SelectStmt::make_field_expr(db, default_table, tables, arith_expr->left(), query_fields, alias2name);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to make field expr");
        return rc;
      }
    }
    if (arith_expr->right() != nullptr) {
      rc = SelectStmt::make_field_expr(db, default_table, tables, arith_expr->right(), query_fields, alias2name);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to make field expr");
        return rc;
      }
    }
    return rc;
  }
  if (expr->type() == ExprType::AGGREGATION) {
    AggreExpr *aggre_expr = static_cast<AggreExpr *>(expr.get());
    if (aggre_expr->agg_type() == AggreType::CNTALL) {
      return RC::SUCCESS;
    }
    if (aggre_expr->child() == nullptr) {
      LOG_WARN("invalid aggre expr");
      return RC::INVALID_ARGUMENT;
    }
    rc = SelectStmt::make_field_expr(db, default_table, tables, aggre_expr->child(), query_fields, alias2name);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to make field expr");
      return rc;
    }
    return rc;
  }
  if (expr->type() == ExprType::FUNCTION) {
    FuncExpr *func_expr = static_cast<FuncExpr *>(expr.get());
    if (func_expr->child() == nullptr) {
      LOG_WARN("invalid function expr");
      return RC::INVALID_ARGUMENT;
    }
    rc = SelectStmt::make_field_expr(db, default_table, tables, func_expr->child(), query_fields, alias2name);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to make field expr");
      return rc;
    }
    return rc;
  }
  if (expr->type() != ExprType::RELATTR) {
    LOG_WARN("invalid expr type: %d", expr->type());
    return RC::INVALID_ARGUMENT;
  }
  RelAttrExpr *relattr_expr = static_cast<RelAttrExpr *>(expr.get());
  if (relattr_expr->field_name() == "*" || relattr_expr->table_name() == "*") {
    LOG_WARN("can not use * in arithmetic expr");
    return RC::INVALID_ARGUMENT;
  }
  if (tables->size() > 1 && relattr_expr->table_name().empty()) {
    LOG_WARN("invalid field name while table is *. attr=%s", relattr_expr->field_name().c_str());
    return RC::SCHEMA_FIELD_MISSING;
  }
  if (alias2name->find(relattr_expr->table_name()) != alias2name->end()) {
    relattr_expr->set_table_name((*alias2name)[relattr_expr->table_name()]);
  }
  Table *table = nullptr;
  const FieldMeta *field = nullptr;
  rc = SelectStmt::get_table_and_field(db, default_table, tables, relattr_expr, table, field);
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot find attr");
    return rc;
  }
  expr = std::make_unique<FieldExpr>(table, field);
  query_fields.push_back(Field(table, field));
  return rc;
}

RC SelectStmt::check_have_aggre(const std::unique_ptr<Expression> &expr, bool &is_aggre, bool &is_attr) {
  RC rc = RC::SUCCESS;
  if (expr->type() == ExprType::VALUE) {
    return RC::SUCCESS;
  }
  if (expr->type() == ExprType::ARITHMETIC) {
    ArithmeticExpr *arith_expr = static_cast<ArithmeticExpr *>(expr.get());
    bool left_is_attr = false;
    if (arith_expr->left() != nullptr) {
      rc = SelectStmt::check_have_aggre(arith_expr->left(), is_aggre, left_is_attr);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to check have aggre");
        return rc;
      }
    }
    bool right_is_attr = false;
    if (arith_expr->right() != nullptr) {
      rc = SelectStmt::check_have_aggre(arith_expr->right(), is_aggre, right_is_attr);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to check have aggre");
        return rc;
      }
    }
    if (left_is_attr || right_is_attr) {
      is_attr = true;
    }
    if (is_attr && is_aggre) {
      LOG_WARN("both have aggregation and normal selection.");
      return RC::INVALID_ARGUMENT;
    }
    return rc;
  }
  if (expr->type() == ExprType::RELATTR) {
    RelAttrExpr *relattr_expr = static_cast<RelAttrExpr *>(expr.get());
    if (relattr_expr->field_name() == "*" || relattr_expr->table_name() == "*") {
      LOG_WARN("can not use * in arithmetic expr");
      return RC::INVALID_ARGUMENT;
    }
    is_attr = true;
    return rc;
  }
  if (expr->type() == ExprType::FUNCTION) {
    FuncExpr *func_expr = static_cast<FuncExpr *>(expr.get());
    if (func_expr->child() == nullptr) {
      LOG_WARN("invalid function expr");
      return RC::INVALID_ARGUMENT;
    }
    rc = SelectStmt::check_have_aggre(func_expr->child(), is_aggre, is_attr);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to check have aggre");
      return rc;
    }
    return rc;
  }
  // 只剩下聚合函数了
  AggreExpr *aggre_expr = static_cast<AggreExpr *>(expr.get());
  if (aggre_expr->child() == nullptr) {
    LOG_WARN("invalid aggre expr");
    return RC::INVALID_ARGUMENT;
  }
  // 如果是 COUNT(*)，则直接返回
  if (aggre_expr->agg_type() == AggreType::CNT) {
    auto child_expr = aggre_expr->child().get();
    if (child_expr->type() == ExprType::RELATTR) {
      auto relattr_expr = static_cast<RelAttrExpr *>(child_expr);
      if (relattr_expr->table_name() == "*" && relattr_expr->field_name() != "*") {
        LOG_WARN("invalid field name while table is *. attr=%s", relattr_expr->field_name().c_str());
        return RC::INTERNAL;
      }
      if (relattr_expr->field_name() == "*" || relattr_expr->table_name() == "*") {
        aggre_expr->set_agg_type(AggreType::CNTALL);
        is_aggre = true;
        return RC::SUCCESS;
      }
    }
  }
  rc = SelectStmt::check_have_aggre(aggre_expr->child(), is_aggre, is_attr);
  if (rc != RC::SUCCESS || is_aggre) {
    LOG_WARN("failed to check have aggre");
    return rc;
  }
  is_aggre = true;
  is_attr = false;
  return rc;
}

RC SelectStmt::check_parent_relation(Expression *expr,
    std::shared_ptr<std::unordered_map<string, string>> parent_alias2name, const vector<string> &parent_relations, bool &is_parent_relation) {
  if (expr->type() == ExprType::VALUE) {
    return RC::SUCCESS;
  }
  if (expr->type() == ExprType::ARITHMETIC) {
    ArithmeticExpr *arith_expr = static_cast<ArithmeticExpr *>(expr);
    if (arith_expr->left() != nullptr) {
      RC rc = SelectStmt::check_parent_relation(arith_expr->left().get(), parent_alias2name, parent_relations, is_parent_relation);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to check parent relation");
        return rc;
      }
    }
    if (arith_expr->right() != nullptr) {
      RC rc = SelectStmt::check_parent_relation(arith_expr->right().get(), parent_alias2name, parent_relations, is_parent_relation);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to check parent relation");
        return rc;
      }
    }
    return RC::SUCCESS;
  }
  if (expr->type() == ExprType::AGGREGATION) {
    AggreExpr *aggre_expr = static_cast<AggreExpr *>(expr);
    if (aggre_expr->child() == nullptr) {
      LOG_WARN("invalid aggre expr");
      return RC::INVALID_ARGUMENT;
    }
    RC rc = SelectStmt::check_parent_relation(aggre_expr->child().get(), parent_alias2name, parent_relations, is_parent_relation);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to check parent relation");
      return rc;
    }
    return rc;
  }
  // 只剩下 ATTR 了
  if (expr->type() != ExprType::RELATTR) {
    LOG_WARN("invalid expr type: %d", expr->type());
    return RC::INVALID_ARGUMENT;
  }
  auto relattr_expr = static_cast<RelAttrExpr *>(expr);
  if (parent_alias2name->find(relattr_expr->table_name()) != parent_alias2name->end() || 
      std::find(parent_relations.begin(), parent_relations.end(), relattr_expr->table_name()) != parent_relations.end()) {
    is_parent_relation = true;
    relattr_expr->set_is_main_relation(true);
    if (parent_alias2name->find(relattr_expr->table_name()) != parent_alias2name->end()) {
      relattr_expr->set_table_name((*parent_alias2name)[relattr_expr->table_name()]);
    }
  }
  return RC::SUCCESS;
}

RC SelectStmt::convert_alias_to_name(Expression *expr, std::shared_ptr<std::unordered_map<string, string>> alias2name) {
   if (expr->type() == ExprType::VALUE) {
    return RC::SUCCESS;
  }
  if (expr->type() == ExprType::ARITHMETIC) {
    ArithmeticExpr *arith_expr = static_cast<ArithmeticExpr *>(expr);
    if (arith_expr->left() != nullptr) {
      RC rc = SelectStmt::convert_alias_to_name(arith_expr->left().get(), alias2name);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to check parent relation");
        return rc;
      }
    }
    if (arith_expr->right() != nullptr) {
      RC rc = SelectStmt::convert_alias_to_name(arith_expr->right().get(), alias2name);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to check parent relation");
        return rc;
      }
    }
    return RC::SUCCESS;
  }
  if (expr->type() == ExprType::AGGREGATION) {
    AggreExpr *aggre_expr = static_cast<AggreExpr *>(expr);
    if (aggre_expr->child() == nullptr) {
      LOG_WARN("invalid aggre expr");
      return RC::INVALID_ARGUMENT;
    }
    RC rc = SelectStmt::convert_alias_to_name(aggre_expr->child().get(), alias2name);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to check parent relation");
      return rc;
    }
    return rc;
  } 
  if (expr->type() == ExprType::FUNCTION) {
    FuncExpr *func_expr = static_cast<FuncExpr *>(expr);
    if (func_expr->child() == nullptr) {
      LOG_WARN("invalid function expr");
      return RC::INVALID_ARGUMENT;
    }
    RC rc = SelectStmt::convert_alias_to_name(func_expr->child().get(), alias2name);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to check parent relation");
      return rc;
    }
    return rc;
  }
  // 只剩下 ATTR 了
  if (expr->type() != ExprType::RELATTR) {
    LOG_WARN("invalid expr type: %d", expr->type());
    return RC::INVALID_ARGUMENT;
  }
  auto relattr_expr = static_cast<RelAttrExpr *>(expr);
  if (alias2name->find(relattr_expr->table_name()) != alias2name->end()) {
    relattr_expr->set_table_name((*alias2name)[relattr_expr->table_name()]);
  }
  return RC::SUCCESS;
}

RC SelectStmt::check_have_relattr(Expression *expr, bool &have_relattr) {
  if (expr->type() == ExprType::VALUE) {
    return RC::SUCCESS;
  }
  if (expr->type() == ExprType::ARITHMETIC) {
    ArithmeticExpr *arith_expr = static_cast<ArithmeticExpr *>(expr);
    if (arith_expr->left() != nullptr) {
      RC rc = SelectStmt::check_have_relattr(arith_expr->left().get(), have_relattr);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to check have relattr");
        return rc;
      }
    }
    if (arith_expr->right() != nullptr) {
      RC rc = SelectStmt::check_have_relattr(arith_expr->right().get(), have_relattr);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to check have relattr");
        return rc;
      }
    }
    return RC::SUCCESS;
  }
  if (expr->type() == ExprType::AGGREGATION) {
    LOG_WARN("invalid aggre expr");
    return RC::INVALID_ARGUMENT;
  }
  if (expr->type() == ExprType::FUNCTION) {
    FuncExpr *func_expr = static_cast<FuncExpr *>(expr);
    if (func_expr->child() == nullptr) {
      LOG_WARN("invalid function expr");
      return RC::INVALID_ARGUMENT;
    }
    RC rc = SelectStmt::check_have_relattr(func_expr->child().get(), have_relattr);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to check have relattr");
      return rc;
    }
    return rc;
  }
  // 只剩下 ATTR 了
  if (expr->type() != ExprType::RELATTR) {
    LOG_WARN("invalid expr type: %d", expr->type());
    return RC::INVALID_ARGUMENT;
  }
  have_relattr = true;
  return RC::SUCCESS;
}

RC SelectStmt::create(Db *db, SelectSqlNode &select_sql, Stmt *&stmt, 
    std::shared_ptr<std::unordered_map<string, string>> name2alias,
    std::shared_ptr<std::unordered_map<string, string>> alias2name,
    std::shared_ptr<std::vector<string>> main_relation_names) // main_relations_name: 外层查询的relations name
{
  if (nullptr == db) {
    LOG_WARN("invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }

  if (select_sql.attributes.empty()) {
    LOG_WARN("invalid argument. select attributes is empty");
    return RC::INVALID_ARGUMENT;
  }

  bool tag_ = false;
  if (name2alias == nullptr) {
    name2alias = std::make_shared<std::unordered_map<string, string>>();
    tag_ = true;
  }
  if (alias2name == nullptr) {
    alias2name = std::make_shared<std::unordered_map<string, string>>();
    tag_ = true;
  }
  if (main_relation_names == nullptr) {
    main_relation_names = std::make_shared<std::vector<string>>();
    tag_ = true;
  }


  // 检查 conditions 内的 relation 是否存在（包括可能的父查询）
  // 如果有非本查询的 relation，将其转换为 sub_select
  if (!tag_) {
      for (auto &condition: select_sql.conditions) {
        // 不是子查询 且 是expression
        if (condition.sub_select == 0 && condition.left_is_expr) {
          SelectStmt::check_parent_relation(condition.left_expr, alias2name, *main_relation_names, condition.is_left_main_);
        }
        if (condition.sub_select == 0 && condition.right_is_expr) {
          SelectStmt::check_parent_relation(condition.right_expr, alias2name, *main_relation_names, condition.is_right_main_);
        }
      }
  }


  // 上下的相对顺序必须这样⬆️ ⬇️。

  // 检查当前查询的 alias 是否重复

  for (int i = 0; i < select_sql.relations.size(); i++) {
    for (int j = i + 1; j < select_sql.relations.size(); j++) {
      if (select_sql.relations[i].alias.empty() || select_sql.relations[j].alias.empty()) {
        continue;
      }
      if (select_sql.relations[i].alias == select_sql.relations[j].alias) {
        LOG_WARN("duplicate alias: %s", select_sql.relations[i].alias.c_str());
        return RC::INVALID_ARGUMENT;
      }
    }
  }

  for (size_t i = 0; i < select_sql.relations.size(); ++i) {
    if (select_sql.relations[i].alias.size()) {
      (*alias2name)[select_sql.relations[i].alias] = select_sql.relations[i].name;
      (*name2alias)[select_sql.relations[i].name] = select_sql.relations[i].alias;
    }
    main_relation_names->push_back(select_sql.relations[i].name);
  }

  // 将 where 语句 conditions 里 relation 的 alias 还原为 name
  // TODO: 这里的处理方式不太好，应该在解析语法树的时候就把 alias 还原为 name
  // for (auto &condition: select_sql.conditions) {
  //   if (alias2name->find(condition.left_attr.relation_name) != alias2name->end()) {
  //     condition.left_attr.alias = condition.left_attr.relation_name;
  //     condition.left_attr.relation_name = (*alias2name)[condition.left_attr.alias];
  //   }
  //   if (alias2name->find(condition.right_attr.relation_name) != alias2name->end()) {
  //     condition.right_attr.alias = condition.right_attr.relation_name;
  //     condition.right_attr.relation_name = (*alias2name)[condition.right_attr.alias];
  //   }
  // }
  for (auto &condition: select_sql.conditions) {
    if (condition.left_is_expr) {
      SelectStmt::convert_alias_to_name(condition.left_expr, alias2name);
    }
    if (condition.right_is_expr) {
      SelectStmt::convert_alias_to_name(condition.right_expr, alias2name);
    }
  }


  // 子查询，先检测conditions中是否有子查询condition，如果有，先为其转成stmt放在condition node中备用。
  for (size_t i = 0; i < select_sql.conditions.size(); ++i) {
    if (select_sql.conditions[i].sub_select != 0) {
      if (select_sql.conditions[i].sub_select == 1 || select_sql.conditions[i].sub_select == 3) { // 1为左子查询
        auto subquery = select_sql.conditions[i].left_sub_select;
        if (subquery->flag != SCF_SELECT && subquery->flag != SCF_SOME_VALUES) {
          LOG_WARN("invalid subquery type");
          return RC::INVALID_ARGUMENT;
        }
        if (subquery->flag == SCF_SOME_VALUES) break; // 如果子查询是 SOME VALUES，不需要生成 stmt
        
        RC rc_ = check_sub_select_legal(db, subquery, select_sql.relations);
        if (rc_ != RC::SUCCESS) {
          return rc_;
        }

        Stmt *subquery_stmt = nullptr;
        // 生成左子查询的 stmt （如果有的话）
        RC rc = SelectStmt::create(db, subquery->selection, subquery_stmt, name2alias, alias2name, main_relation_names);
        if (rc != RC::SUCCESS) {
          LOG_WARN("cannot construct subquery stmt");
          return rc;
        }
        // 类型转换为 SelectStmt
        auto *subquery_select_stmt = dynamic_cast<SelectStmt *>(subquery_stmt);
        select_sql.conditions[i].left_select_stmt = subquery_select_stmt;
      }
      if (select_sql.conditions[i].sub_select == 2 || select_sql.conditions[i].sub_select == 3) { // 2为右子查询
        auto subquery = select_sql.conditions[i].right_sub_select;
        if (subquery->flag != SCF_SELECT && subquery->flag != SCF_SOME_VALUES) {
          LOG_WARN("invalid subquery type");
          return RC::INVALID_ARGUMENT;
        }

        if (subquery->flag == SCF_SOME_VALUES) break; // 如果子查询是 SOME VALUES，不需要生成 stmt

        RC rc_ = check_sub_select_legal(db, subquery, select_sql.relations);
        if (rc_ != RC::SUCCESS) {
          return rc_;
        }

        Stmt *subquery_stmt = nullptr;
        // 生成右子查询的 stmt （如果有的话）
        RC rc = SelectStmt::create(db, subquery->selection, subquery_stmt, name2alias, alias2name, main_relation_names);
        if (rc != RC::SUCCESS) {
          LOG_WARN("cannot construct subquery stmt");
          return rc;
        }
        // 类型转换为 SelectStmt
        auto *subquery_select_stmt = dynamic_cast<SelectStmt *>(subquery_stmt);
        select_sql.conditions[i].right_select_stmt = subquery_select_stmt;
      }
    } 

  }

  // collect tables in `from` statement
  std::vector<Table *> tables;
  std::unordered_map<std::string, Table *> table_map;

  // 遍历查询的表
  // 1. 检查表是否存在
  // 2. 将表加入到tables中
  // 3. 将表加入到table_map中
  for (size_t i = 0; i < select_sql.relations.size(); i++) {
    const char *table_name = select_sql.relations[i].name.c_str();
    if (nullptr == table_name) {
      LOG_WARN("invalid argument. relation name is null. index=%d", i);
      return RC::INVALID_ARGUMENT;
    }

    Table *table = db->find_table(table_name);
    if (nullptr == table) {
      LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }
    tables.push_back(table);
    table_map.insert(std::pair<std::string, Table *>(table_name, table));
  }

  // 还要将 joins 的表加入到tables和table_map中
  // 优化：这个地方有潜在的优化空间，可以在解析语法树的时候就直接把join的表加入到select_sql.relations内
  // 已优化↑
  // for (size_t i = 0; i< select_sql.joins.size(); ++i) {
  //   const char *table_name = select_sql.joins[i].relation_name.c_str();
  //   if (nullptr == table_name) {
  //     LOG_WARN("invalid argument. relation name is null. index=%d", i);
  //     return RC::INVALID_ARGUMENT;
  //   }

  //   Table *table = db->find_table(table_name);
  //   if (nullptr == table) {
  //     LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
  //     return RC::SCHEMA_TABLE_NOT_EXIST;
  //   }
  //   tables.push_back(table);
  //   table_map.insert(std::pair<std::string, Table *>(table_name, table));
  // }
  // 还要将 joins 的 conditions 加入到 select_sql.conditions 中
  // for (size_t i = 0; i< select_sql.joins.size(); ++i) {
  //   for (size_t j = 0; j < select_sql.joins[i].conditions.size(); ++j) {
  //     select_sql.conditions.push_back(select_sql.joins[i].conditions[j]);
  //   }
  // }



  // group by
  std::vector<Field> group_by_fields;
  for (int i=0; i<select_sql.group_attrs.size(); ++i) {
    const RelAttrSqlNode &relation_attr = select_sql.group_attrs[i]; // order by's relation attr
    auto *relattr_expr = static_cast<RelAttrExpr*>(relation_attr.expr);
    if (relattr_expr->table_name().empty() && relattr_expr->field_name() == "*") {
      LOG_WARN("invalid selection. * cannot have alias.");
      return RC::INVALID_ARGUMENT;
    } else if (relattr_expr->table_name().size()) {
      const auto &table_name = relattr_expr->table_name();
      if (alias2name->find(table_name) != alias2name->end()) {  // 如果表名在name2alias中，说明有别名
        relattr_expr->set_table_name((*alias2name)[table_name]);
      }
      const auto &field_name = relattr_expr->field_name();
      if (table_name == "*") {  // 表名为*
        if (field_name != "*") {  // 属性名不为*，报错
          LOG_WARN("invalid field name while table is *. attr=%s", field_name.c_str());
          return RC::SCHEMA_FIELD_MISSING;
        }
        LOG_WARN("invalid selection. * cannot have alias.");
        return RC::INVALID_ARGUMENT;
      } else {  // 表名不是*，是具体的表名
        auto iter = table_map.find(table_name);
        if (iter == table_map.end()) {
          LOG_WARN("no such table in from list: %s", table_name.c_str());
          return RC::SCHEMA_FIELD_MISSING;
        }
        Table *table = iter->second;
        if (field_name == "*") {
          LOG_WARN("invalid selection. * cannot have alias.");
          return RC::INVALID_ARGUMENT;
        } else {
          const FieldMeta *field_meta = table->table_meta().field(field_name.c_str());
          if (nullptr == field_meta) {
            LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), field_name.c_str());
            return RC::SCHEMA_FIELD_MISSING;
          }
          group_by_fields.push_back(Field(table, field_meta, (*name2alias)[table->name()], relation_attr.alias, relation_attr.is_asc));
        }
      }
    } else {
      if (tables.size() != 1) {
        LOG_WARN("invalid. I do not know the attr's table. attr=%s", relattr_expr->field_name().c_str());
        return RC::SCHEMA_FIELD_MISSING;
      }
      Table *table = tables[0];
      const FieldMeta *field_meta = table->table_meta().field(relattr_expr->field_name().c_str());
      if (nullptr == field_meta) {
        LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), relattr_expr->field_name().c_str());
        return RC::SCHEMA_FIELD_MISSING;
      }
      group_by_fields.push_back(Field(table, field_meta, (*name2alias)[table->name()], relation_attr.alias, relation_attr.is_asc));
    }

    // 检查group_by
    bool ok = false;
    for (int i=0; i<select_sql.attributes.size(); ++i) {
      const RelAttrSqlNode &relation_attr1 = select_sql.attributes[i]; // order by's relation attr
      auto *relattr_expr1 = static_cast<RelAttrExpr*>(relation_attr1.expr);
      if (relattr_expr1->table_name() == relattr_expr->table_name() && relattr_expr1->field_name() == relattr_expr->field_name()) {
        ok = true;
      }
    }
    if (!ok) {
      LOG_WARN("invalid group by. group by attr must be in select attrs.");
      return RC::INVALID_ARGUMENT;
    }
  }

  // collect query fields in `select` statement
  std::vector<std::unique_ptr<Expression>> query_exprs;  // 需要查询的表达式
  std::vector<std::string> query_aliases;  // 查询的名称
  std::vector<Field> query_fields;  // 查询需要用到的字段
  int aggre_stat = 0;  //  用于表示当前是否有聚合函数  01->普通  10->聚合
  bool with_table_name = tables.size() > 1;  // 是否需要表名
  for (int i = static_cast<int>(select_sql.attributes.size()) - 1; i >= 0; i--) {

    if(!group_by_fields.empty()) {
      bool tag = false;
      for (int j=0; j<group_by_fields.size(); ++j) {
        if (select_sql.attributes[i].expr->type() != ExprType::AGGREGATION) {
          tag = true;
          break;
        }
      }
      if (tag) continue;
    }

    const RelAttrSqlNode &relation_attr = select_sql.attributes[i];
    // 如果 relation 为空，说明是一个表达式，检查没有 attrexpr 后直接加入到 query_exprs 中
    RC rc = RC::SUCCESS;
    auto expr = std::unique_ptr<Expression>(relation_attr.expr);
    if (tables.empty()) {
      bool have_relattr = false;
      rc = SelectStmt::check_have_relattr(expr.get(), have_relattr);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to check have relattr");
        return rc;
      }
      if (have_relattr) {
        LOG_WARN("invalid selection. no relation in from list.");
        return RC::INVALID_ARGUMENT;
      }
      if (relation_attr.alias.size()) {
        query_aliases.push_back(relation_attr.alias);
      } else {
        query_aliases.push_back(expr->name());
      }
      query_exprs.push_back(std::move(expr));
      continue;
    }
    bool is_aggre = false;
    bool is_attr = false;
    rc = SelectStmt::check_have_aggre(expr, is_aggre, is_attr);
    aggre_stat |= (is_aggre ? 2 : 1);
    if (aggre_stat == 3) {
      if (select_sql.group_attrs.empty()) {
        LOG_WARN("both have aggregation and normal selection.");
        return RC::INVALID_ARGUMENT;
      } else {
        aggre_stat = 2;
      }
    }
    // 如果表达式直接就是表名和属性名，那么进行判断（需要支持"*"）
    if (expr->type() == ExprType::RELATTR) {
      auto relattr_expr = static_cast<RelAttrExpr *>(expr.get());
      // 如果表名为空，且属性名为*，则将所有表的所有属性加入到query_fields中
      if (relattr_expr->field_name() == "*" && relattr_expr->table_name().empty()) {
        if (relation_attr.aggre_type == AggreType::NONE) {
          if (relation_attr.alias.size()) {  // 如果有别名，报错
            LOG_WARN("invalid selection. * cannot have alias.");
            return RC::INVALID_ARGUMENT;
          }
          for (Table *table : tables) {
            // wildcard: 通配符
            wildcard_fields(table, query_exprs, query_fields, query_aliases, with_table_name);
          }
        } else if (relation_attr.aggre_type == AggreType::CNT) {  // CNT可以
          auto *table = tables[0];
          auto *field_meta = table->table_meta().field(0);
          Field field(table, field_meta);
          query_fields.push_back(field);
          query_exprs.emplace_back(new FieldExpr(field));
          if (with_table_name) {
            string alias = table->name();
            alias += ".";
            alias += field_meta->name();
            query_aliases.push_back(alias);
          } else {
            query_aliases.push_back(field_meta->name());
          }
        } else {  // 如果是聚合函数，报错
          LOG_WARN("invalid selection in aggregation function.");
          return RC::INVALID_ARGUMENT;
        }

      } else if (relattr_expr->table_name().size()) {  // 如果表名不为空
        auto &table_name = relattr_expr->table_name();
        if (alias2name->find(table_name) != alias2name->end()) {  // 如果表名在name2alias中，说明有别名
          relattr_expr->set_table_name((*alias2name)[table_name]);
        }
        auto &field_name = relattr_expr->field_name();

        if (table_name == "*") {  // 表名为*
          if (field_name != "*") {  // 属性名不为*，报错
            LOG_WARN("invalid field name while table is *. attr=%s", field_name.c_str());
            return RC::SCHEMA_FIELD_MISSING;
          }
          // 和 * 一样
          if (relation_attr.alias.size()) {
            LOG_WARN("invalid selection. * cannot have alias.");
            return RC::INVALID_ARGUMENT;
          }
          if (relation_attr.aggre_type == AggreType::NONE) {  // 如果是普通的查询
            for (Table *table : tables) {
              wildcard_fields(table, query_exprs, query_fields, query_aliases, with_table_name);
            }
          } else if (relation_attr.aggre_type == AggreType::CNT) {
            auto *table = tables[0];
            auto *field_meta = table->table_meta().field(0);
            Field field(table, field_meta);
            query_fields.push_back(field);
            query_exprs.emplace_back(new FieldExpr(field));
            if (with_table_name) {
              string alias = table->name();
              alias += ".";
              alias += field_meta->name();
              query_aliases.push_back(alias);
            } else {
              query_aliases.push_back(field_meta->name());
            }
          } else {
            LOG_WARN("invalid selection in aggregation function.");
            return RC::INVALID_ARGUMENT;
          }
        } else {  // 表名不是*，是具体的表名
          auto iter = table_map.find(table_name);
          if (iter == table_map.end()) {
            LOG_WARN("no such table in from list: %s", table_name.c_str());
            return RC::SCHEMA_FIELD_MISSING;
          }

          Table *table = iter->second;
          if (field_name == "*") {
            if (relation_attr.alias.size()) {
              LOG_WARN("invalid selection. * cannot have alias.");
              return RC::INVALID_ARGUMENT;
            }
            if (relation_attr.aggre_type == AggreType::NONE) {
              wildcard_fields(table, query_exprs, query_fields, query_aliases, with_table_name);
            } else if (relation_attr.aggre_type == AggreType::CNT) {
              auto *field_meta = table->table_meta().field(0);
              Field field(table, field_meta);
              query_fields.push_back(field);
              query_exprs.emplace_back(new FieldExpr(field));
              if (with_table_name) {
                string alias = table->name();
                alias += ".";
                alias += field_meta->name();
                query_aliases.push_back(alias);
              } else {
                query_aliases.push_back(field_meta->name());
              }
            } else {
              LOG_WARN("invalid selection in aggregation function.");
              return RC::INVALID_ARGUMENT;
            }
          } else {
            const FieldMeta *field_meta = table->table_meta().field(field_name.c_str());
            if (nullptr == field_meta) {
              LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), field_name.c_str());
              return RC::SCHEMA_FIELD_MISSING;
            }
            Field field(table, field_meta, (*name2alias)[table->name()], relation_attr.alias);
            query_fields.push_back(field);
            query_exprs.emplace_back(new FieldExpr(field));
            // 如果有别名，就用别名，否则就用属性名
            if (relation_attr.alias.size()) {
              query_aliases.push_back(relation_attr.alias);
            } else if (with_table_name) {
              string alias = table->name();
              alias += ".";
              alias += field_meta->name();
              query_aliases.push_back(alias);
            } else {
              query_aliases.push_back(field_meta->name());
            }
            if (relation_attr.aggre_type != AggreType::NONE) {
            }
          }
        }
      } else {  // 只有属性名，没有表名
        if (tables.size() != 1) {
          LOG_WARN("invalid. I do not know the attr's table. attr=%s", relattr_expr->field_name().c_str());
          return RC::SCHEMA_FIELD_MISSING;
        }

        Table *table = tables[0];
        const FieldMeta *field_meta = table->table_meta().field(relattr_expr->field_name().c_str());
        if (nullptr == field_meta) {
          LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), relattr_expr->field_name().c_str());
          return RC::SCHEMA_FIELD_MISSING;
        }
        if (field_meta->type() == CHARS || field_meta->type() == DATES) {
          switch (relation_attr.aggre_type) {
            case AggreType::SUM:
            case AggreType::AVG:
              LOG_WARN("invalid aggregation function on CHARS type.");
              return RC::INVALID_ARGUMENT;
            default:
              break;
          }
        }
        Field field(table, field_meta, (*name2alias)[table->name()], relation_attr.alias);
        query_fields.push_back(field);
        query_exprs.emplace_back(new FieldExpr(field));
        if (relation_attr.alias.size()) {
          query_aliases.push_back(relation_attr.alias);
        } else if (with_table_name) {
          string alias = table->name();
          alias += ".";
          alias += field_meta->name();
          query_aliases.push_back(alias);
        } else {
          query_aliases.push_back(field_meta->name());
        }
      }
      // 否则就是复杂的表达式，将其中的 RelAttrExpr 转换为 FieldExpr 后，直接加入到query_exprs中
    } else {
      expr->set_name(expr->name());
      RC rc = SelectStmt::make_field_expr(db, tables[0], &table_map, expr, query_fields, alias2name);
      if (rc != RC::SUCCESS) {
        LOG_WARN("cannot construct field expr");
        return rc;
      }
      if (relation_attr.alias.size()) {
        query_aliases.push_back(relation_attr.alias);
      } else {
        query_aliases.push_back(expr->name());
      }
      query_exprs.emplace_back(std::move(expr));
    }
  }

  LOG_INFO("got %d tables in from stmt and %d fields in query stmt", tables.size(), query_exprs.size());

  // order by
  std::vector<Field> order_by_fields;
  for (int i=0; i<select_sql.order_attrs.size(); ++i) {
    const RelAttrSqlNode &relation_attr = select_sql.order_attrs[i]; // order by's relation attr
    auto *relattr_expr = static_cast<RelAttrExpr*>(relation_attr.expr);
    if (relattr_expr->table_name().empty() && relattr_expr->field_name() == "*") {
      LOG_WARN("invalid selection. * cannot have alias.");
      return RC::INVALID_ARGUMENT;
    } else if (relattr_expr->table_name().size()) {
      const auto &table_name = relattr_expr->table_name();
      if (alias2name->find(table_name) != alias2name->end()) {  // 如果表名在name2alias中，说明有别名
        relattr_expr->set_table_name((*alias2name)[table_name]);
      }
      const auto &field_name = relattr_expr->field_name();
      if (table_name == "*") {  // 表名为*
        if (field_name != "*") {  // 属性名不为*，报错
          LOG_WARN("invalid field name while table is *. attr=%s", field_name.c_str());
          return RC::SCHEMA_FIELD_MISSING;
        }
        LOG_WARN("invalid selection. * cannot have alias.");
        return RC::INVALID_ARGUMENT;
      } else {  // 表名不是*，是具体的表名
        auto iter = table_map.find(table_name);
        if (iter == table_map.end()) {
          LOG_WARN("no such table in from list: %s", table_name.c_str());
          return RC::SCHEMA_FIELD_MISSING;
        }
        Table *table = iter->second;
        if (field_name == "*") {
          LOG_WARN("invalid selection. * cannot have alias.");
          return RC::INVALID_ARGUMENT;
        } else {
          const FieldMeta *field_meta = table->table_meta().field(field_name.c_str());
          if (nullptr == field_meta) {
            LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), field_name.c_str());
            return RC::SCHEMA_FIELD_MISSING;
          }
          order_by_fields.push_back(Field(table, field_meta, (*name2alias)[table->name()], relation_attr.alias, relation_attr.is_asc));
        }
      }
    } else {
      if (tables.size() != 1) {
        LOG_WARN("invalid. I do not know the attr's table. attr=%s", relattr_expr->field_name().c_str());
        return RC::SCHEMA_FIELD_MISSING;
      }
      Table *table = tables[0];
      const FieldMeta *field_meta = table->table_meta().field(relattr_expr->field_name().c_str());
      if (nullptr == field_meta) {
        LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), relattr_expr->field_name().c_str());
        return RC::SCHEMA_FIELD_MISSING;
      }
      order_by_fields.push_back(Field(table, field_meta, (*name2alias)[table->name()], relation_attr.alias, relation_attr.is_asc));
    }
  }

  // group by 复制给 order by
  for (int i=0; i<group_by_fields.size(); ++i) {
    Field field = group_by_fields[i];
    field.set_asc(true);
    order_by_fields.push_back(field);
  }

  Table *default_table = nullptr;
  if (tables.size() == 1) {
    default_table = tables[0];
  }

  // create filter statement in `where` statement
  FilterStmt *filter_stmt = nullptr;
  RC rc = FilterStmt::create(db,
      default_table,
      &table_map,
      select_sql.conditions.data(), // vector.data() 返回指向vector第一个元素的指针
      static_cast<int>(select_sql.conditions.size()),
      filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot construct filter stmt");
    return rc;
  }


  // create filter statement in `where` statement
  FilterStmt *filter_stmt2 = nullptr;

  if (!select_sql.havings.empty()) {
    rc = FilterStmt::create(db,
        default_table,
        &table_map,
        select_sql.havings.data(), // vector.data() 返回指向vector第一个元素的指针
        static_cast<int>(select_sql.havings.size()),
        filter_stmt2);
    if (rc != RC::SUCCESS) {
      LOG_WARN("cannot construct filter stmt");
      return rc;
    }
  }


  // everything alright
  SelectStmt *select_stmt = new SelectStmt();
  // TODO add expression copy
  select_stmt->is_aggre_ = aggre_stat == 2;
  select_stmt->tables_.swap(tables);
  select_stmt->query_exprs_.swap(query_exprs);
  select_stmt->query_fields_.swap(query_fields);
  select_stmt->query_aliases_.swap(query_aliases);
  select_stmt->filter_stmt_ = filter_stmt;
  select_stmt->having_stmt_ = filter_stmt2;
  select_stmt->order_by_fields_.swap(order_by_fields);
  select_stmt->group_by_fields_.swap(group_by_fields);
  stmt = select_stmt;
  return RC::SUCCESS;
}
