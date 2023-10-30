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
#include "sql/parser/parse_defs.h"
#include "sql/stmt/filter_stmt.h"
#include "common/log/log.h"
#include "common/lang/string.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include <cstddef>

SelectStmt::~SelectStmt()
{
  if (nullptr != filter_stmt_) {
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }
}

// 把该表的所有属性（字段）加入到field_metas中
static void wildcard_fields(Table *table, std::vector<Field> &field_metas)
{
  const TableMeta &table_meta = table->table_meta();
  const int field_num = table_meta.field_num();
  for (int i = table_meta.sys_field_num(); i < field_num; i++) {
    field_metas.push_back(Field(table, table_meta.field(i)));
  }
}

RC check_sub_select_legal(Db *db, ParsedSqlNode *sub_select)
{
  // 当左子查询的属性不止一个时，报错（注意这里没有判断*，需要到后面的步骤判断）
  if (sub_select->selection.attributes.size() != 1) {
    LOG_WARN("invalid subquery attributes");
    return RC::INVALID_ARGUMENT;
  }
  // 如果是*，需要先获得table，然后看其中的fields是不是1，如果不是，报错
  if (sub_select->selection.attributes.size() > 0 && sub_select->selection.attributes[0].attribute_name == "*") {
    int fields_num = 0;
    for (size_t j = 0; j < sub_select->selection.relations.size(); ++j) {
      const char *table_name = sub_select->selection.relations[j].c_str();
      if (nullptr == table_name) {
        LOG_WARN("invalid argument. relation name is null. index=%d", j);
        return RC::INVALID_ARGUMENT;
      }
      Table *table = db->find_table(table_name);
      if (nullptr == table) {
        LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
        return RC::SCHEMA_TABLE_NOT_EXIST;
      }
      fields_num += table->table_meta().field_num();
    }
    if (fields_num != 1) {
      LOG_WARN("invalid subquery attributes");
      return RC::INVALID_ARGUMENT;
    }
  }
  return RC::SUCCESS;
}

RC SelectStmt::create(Db *db, SelectSqlNode &select_sql, Stmt *&stmt)
{
  if (nullptr == db) {
    LOG_WARN("invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }

  if (select_sql.attributes.empty()) {
    LOG_WARN("invalid argument. select attributes is empty");
    return RC::INVALID_ARGUMENT;
  }


  // 子查询，先检测conditions中是否有子查询condition，如果有，先为其转成stmt放在condition node中备用。
  for (size_t i = 0; i < select_sql.conditions.size(); ++i) {
    if (select_sql.conditions[i].sub_select != 0) {
      if (select_sql.conditions[i].sub_select == 1) { // 1为左子查询
        auto subquery = select_sql.conditions[i].left_sub_select;
        if (subquery->flag != SCF_SELECT && subquery->flag != SCF_SOME_VALUES) {
          LOG_WARN("invalid subquery type");
          return RC::INVALID_ARGUMENT;
        }
        if (subquery->flag == SCF_SOME_VALUES) break; // 如果子查询是 SOME VALUES，不需要生成 stmt
        
        RC rc_ = check_sub_select_legal(db, subquery);
        if (rc_ != RC::SUCCESS) {
          return rc_;
        }

        Stmt *subquery_stmt = nullptr;
        // 生成左子查询的 stmt （如果有的话）
        RC rc = SelectStmt::create(db, subquery->selection, subquery_stmt);
        if (rc != RC::SUCCESS) {
          LOG_WARN("cannot construct subquery stmt");
          return rc;
        }
        // 类型转换为 SelectStmt
        auto *subquery_select_stmt = dynamic_cast<SelectStmt *>(subquery_stmt);
        select_sql.conditions[i].left_select_stmt = subquery_select_stmt;
      }
      if (select_sql.conditions[i].sub_select == 2) { // 2为右子查询
        auto subquery = select_sql.conditions[i].right_sub_select;
        if (subquery->flag != SCF_SELECT && subquery->flag != SCF_SOME_VALUES) {
          LOG_WARN("invalid subquery type");
          return RC::INVALID_ARGUMENT;
        }

        if (subquery->flag == SCF_SOME_VALUES) break; // 如果子查询是 SOME VALUES，不需要生成 stmt

        RC rc_ = check_sub_select_legal(db, subquery);
        if (rc_ != RC::SUCCESS) {
          return rc_;
        }

        Stmt *subquery_stmt = nullptr;
        // 生成右子查询的 stmt （如果有的话）
        RC rc = SelectStmt::create(db, subquery->selection, subquery_stmt);
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
    const char *table_name = select_sql.relations[i].c_str();
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

  // collect query fields in `select` statement
  std::vector<Field> query_fields;
  std::vector<AggreType> aggre_types;
  int aggre_stat = 0;  //  用于表示当前是否有聚合函数  01->普通  10->聚合
  for (int i = static_cast<int>(select_sql.attributes.size()) - 1; i >= 0; i--) {
    const RelAttrSqlNode &relation_attr = select_sql.attributes[i];
    aggre_stat |= relation_attr.aggre_type == AggreType::NONE ? 1 : 2;
    if (aggre_stat == 3) {
      LOG_WARN("both have aggregation and normal selection.");
      return RC::INVALID_ARGUMENT;
    }

    // 如果表名为空，且属性名为*，则将所有表的所有属性加入到query_fields中
    if (common::is_blank(relation_attr.relation_name.c_str()) &&
        0 == strcmp(relation_attr.attribute_name.c_str(), "*")) {
      if (relation_attr.aggre_type == AggreType::NONE) {
        for (Table *table : tables) {
          // wildcard: 通配符
        wildcard_fields(table, query_fields);
        }
      } else if (relation_attr.aggre_type == AggreType::CNT) {
        auto *table = tables[0];
        auto *field_meta = table->table_meta().field(0);
        query_fields.push_back(Field(table, field_meta));
        aggre_types.push_back(AggreType::CNTALL);
      } else {
        LOG_WARN("invalid selection in aggregation function.");
        return RC::INVALID_ARGUMENT;
      }

    } else if (!common::is_blank(relation_attr.relation_name.c_str())) {
      const char *table_name = relation_attr.relation_name.c_str();
      const char *field_name = relation_attr.attribute_name.c_str();

      // table_name为*，且field_name不为*，则报错
      if (0 == strcmp(table_name, "*")) {
        if (0 != strcmp(field_name, "*")) {
          LOG_WARN("invalid field name while table is *. attr=%s", field_name);
          return RC::SCHEMA_FIELD_MISSING;
        }
        if (relation_attr.aggre_type == AggreType::NONE) {
          for (Table *table : tables) {
            wildcard_fields(table, query_fields);
          }
        } else if (relation_attr.aggre_type == AggreType::CNT) {
          auto *table = tables[0];
          auto *field_meta = table->table_meta().field(0);
          query_fields.push_back(Field(table, field_meta));
          aggre_types.push_back(AggreType::CNT);
        } else {
          LOG_WARN("invalid selection in aggregation function.");
          return RC::INVALID_ARGUMENT;
        }
      } else {
        auto iter = table_map.find(table_name);
        if (iter == table_map.end()) {
          LOG_WARN("no such table in from list: %s", table_name);
          return RC::SCHEMA_FIELD_MISSING;
        }

        Table *table = iter->second;
        if (0 == strcmp(field_name, "*")) {
          if (relation_attr.aggre_type == AggreType::NONE) {
            wildcard_fields(table, query_fields);
          } else if (relation_attr.aggre_type == AggreType::CNT) {
            auto *field_meta = table->table_meta().field(0);
            query_fields.push_back(Field(table, field_meta));
            aggre_types.push_back(AggreType::CNT);
          } else {
            LOG_WARN("invalid selection in aggregation function.");
            return RC::INVALID_ARGUMENT;
          }
        } else {
          const FieldMeta *field_meta = table->table_meta().field(field_name);
          if (nullptr == field_meta) {
            LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), field_name);
            return RC::SCHEMA_FIELD_MISSING;
          }

          query_fields.push_back(Field(table, field_meta));
          if (relation_attr.aggre_type != AggreType::NONE) {
            aggre_types.push_back(relation_attr.aggre_type);
          }
        }
      }
    } else {
      if (tables.size() != 1) {
        LOG_WARN("invalid. I do not know the attr's table. attr=%s", relation_attr.attribute_name.c_str());
        return RC::SCHEMA_FIELD_MISSING;
      }

      Table *table = tables[0];
      const FieldMeta *field_meta = table->table_meta().field(relation_attr.attribute_name.c_str());
      if (nullptr == field_meta) {
        LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), relation_attr.attribute_name.c_str());
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
      query_fields.push_back(Field(table, field_meta));
      if (relation_attr.aggre_type != AggreType::NONE) {
        aggre_types.push_back(relation_attr.aggre_type);
      }
    }
  }

  LOG_INFO("got %d tables in from stmt and %d fields in query stmt", tables.size(), query_fields.size());

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

  // everything alright
  SelectStmt *select_stmt = new SelectStmt();
  // TODO add expression copy
  select_stmt->is_aggre_ = aggre_stat == 2;
  select_stmt->tables_.swap(tables);
  select_stmt->query_fields_.swap(query_fields);
  select_stmt->aggre_types_.swap(aggre_types);
  select_stmt->filter_stmt_ = filter_stmt;
  stmt = select_stmt;
  return RC::SUCCESS;
}
