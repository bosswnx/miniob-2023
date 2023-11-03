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
// Created by Wangyunlai on 2022/5/22.
//

#include "common/rc.h"
#include "common/log/log.h"
#include "common/lang/string.h"
#include "sql/expr/expression.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/parser/value.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include <memory>

FilterStmt::~FilterStmt()
{
  for (FilterUnit *unit : filter_units_) {
    delete unit;
  }
  filter_units_.clear();
}

RC FilterStmt::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
    const ConditionSqlNode *conditions, int condition_num, FilterStmt *&stmt)
{
  RC rc = RC::SUCCESS;
  stmt = nullptr;

  FilterStmt *tmp_stmt = new FilterStmt();
  for (int i = 0; i < condition_num; i++) {
    FilterUnit *filter_unit = nullptr;
    rc = create_filter_unit(db, default_table, tables, conditions[i], filter_unit);
    if (rc != RC::SUCCESS) {
      delete tmp_stmt;
      LOG_WARN("failed to create filter unit. condition index=%d", i);
      return rc;
    }
    tmp_stmt->filter_units_.push_back(filter_unit);
  }

  stmt = tmp_stmt;
  return rc;
}

RC get_table_and_field(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
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

RC make_field_expr(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables, std::unique_ptr<Expression> &expr)
{
  RC rc = RC::SUCCESS;
  if (expr->type() == ExprType::VALUE) {
    return RC::SUCCESS;
  }
  if (expr->type() == ExprType::ARITHMETIC) {
    ArithmeticExpr *arith_expr = static_cast<ArithmeticExpr *>(expr.get());
    if (arith_expr->left() != nullptr) {
      rc = make_field_expr(db, default_table, tables, arith_expr->left());
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to make field expr");
        return rc;
      }
    }
    if (arith_expr->right() != nullptr) {
      rc = make_field_expr(db, default_table, tables, arith_expr->right());
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to make field expr");
        return rc;
      }
    }
    return rc;
  }
  if (expr->type() != ExprType::RELATTR) {
    LOG_WARN("invalid expr type: %d", expr->type());
    return RC::INVALID_ARGUMENT;
  }
  RelAttrExpr *relattr_expr = static_cast<RelAttrExpr *>(expr.get());
  Table *table = nullptr;
  const FieldMeta *field = nullptr;
  rc = get_table_and_field(db, default_table, tables, relattr_expr, table, field);
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot find attr");
    return rc;
  }
  expr = std::make_unique<FieldExpr>(table, field);
  return rc;
}

RC FilterStmt::create_filter_unit(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
    const ConditionSqlNode &condition, FilterUnit *&filter_unit)
    
{
  RC rc = RC::SUCCESS;

  CompOp comp = condition.comp;
  if (comp < EQUAL_TO || comp >= NO_OP) {
    LOG_WARN("invalid compare operator : %d", comp);
    return RC::INVALID_ARGUMENT;
  }

  filter_unit = new FilterUnit;

  Value right_value;
  bool right_is_value = false;
  Value left_value;
  bool left_is_value = false;
  rc = condition.left_expr->try_get_value(left_value);
  left_is_value = rc == RC::SUCCESS;
  rc = condition.right_expr->try_get_value(right_value);
  right_is_value = rc == RC::SUCCESS;
  rc = RC::SUCCESS;
  if (right_is_value && right_value.attr_type() == DATES) {
    //判断日期是否合法
    if (!right_value.check_date(right_value.get_date())) {
      // TODO: fix this log
      // LOG_WARN("date is invalid. table=%s, field=%s, field type=%d, value_type=%d",
      //     condition.left_attr.relation_name.c_str(), condition.left_attr.attribute_name.c_str(),
      //     right_value.attr_type(), right_value.attr_type());
      return RC::SCHEMA_FIELD_TYPE_MISMATCH;
    }
  }

  // 左 filterObj
  if (condition.sub_select == 1) { // 如果是子查询
    if(condition.left_sub_select->flag == SCF_SELECT) {
      // 普通的子查询语句（select）
      FilterObj filter_obj;
      filter_obj.init_sub_select_stmt(condition.left_select_stmt);
      filter_unit->set_left(filter_obj);
    } else if (condition.left_sub_select->flag == SCF_SOME_VALUES) {
      // select * from tb where xx in (1, 2, 3) 类似的情况
      FilterObj filter_obj;
      filter_obj.init_values(&condition.left_sub_select->some_values.values);
      filter_unit->set_left(filter_obj);
    }
  } else {  // 如果不是子查询，那么就是表达式
    std::unique_ptr<Expression> left_expr(condition.left_expr);
    rc = make_field_expr(db, default_table, tables, left_expr);
    if (rc != RC::SUCCESS) {
      LOG_WARN("cannot find attr");
      return rc;
    }
    FilterObj filter_obj;
    filter_obj.init_expr(std::move(left_expr));
    filter_unit->set_left(filter_obj);
  }
  // 右 filterObj
  if (condition.sub_select == 2) {  // 如果是子查询
    // FilterObj filter_obj;
    // filter_obj.init_sub_select_stmt(condition.right_select_stmt);
    // filter_unit->set_right(filter_obj);
    if (condition.right_sub_select->flag == SCF_SELECT) {
      // 普通的子查询语句（select）
      FilterObj filter_obj;
      filter_obj.init_sub_select_stmt(condition.right_select_stmt);
      filter_unit->set_right(filter_obj);
    } else if (condition.right_sub_select->flag == SCF_SOME_VALUES) {
      // select * from tb where xx in (1, 2, 3) 类似的情况
      FilterObj filter_obj;
      filter_obj.init_values(&condition.right_sub_select->some_values.values);
      filter_unit->set_right(filter_obj);
    }
  } else {  // 如果不是子查询，那么就是表达式
    std::unique_ptr<Expression> right_expr(condition.right_expr);
    rc = make_field_expr(db, default_table, tables, right_expr);
    if (rc != RC::SUCCESS) {
      LOG_WARN("cannot find attr");
      return rc;
    }
    FilterObj filter_obj;
    filter_obj.init_expr(std::move(right_expr));
    filter_unit->set_right(filter_obj);
  }

  filter_unit->set_comp(comp);

  // 检查两个类型是否能够比较
  return rc;
}
