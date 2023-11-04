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

#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
#include "sql/stmt/stmt.h"
#include "sql/expr/expression.h"

class Db;
class Table;
class FieldMeta;
class SelectStmt;

struct FilterObj 
{
  bool is_expr;
  std::unique_ptr<Expression> expr;
  SelectStmt *sub_select_stmt = nullptr;
  std::vector<Value> *values = nullptr;
  void operator=(FilterObj &obj)
  {
    is_expr = obj.is_expr;
    if (is_expr) {
      expr = std::move(obj.expr);
    } else {
      sub_select_stmt = obj.sub_select_stmt;
      values = obj.values;
    }
  }

  void init_expr(std::unique_ptr<Expression> expr)
  {
    is_expr = true;
    this->expr = std::move(expr);
  }

  void init_sub_select_stmt(SelectStmt *stmt)
  {
    is_expr = false;
    sub_select_stmt = stmt;
  }

  void init_values(std::vector<Value> *values)
  {
    is_expr = false;
    this->values = values;
  }
};

class FilterUnit 
{
public:
  FilterUnit() = default;
  ~FilterUnit()
  {}

  void set_comp(CompOp comp)
  {
    comp_ = comp;
  }

  CompOp comp() const
  {
    return comp_;
  }

  void set_left(FilterObj &obj)
  {
    left_ = obj;
  }
  void set_right(FilterObj &obj)
  {
    right_ = obj;
  }

  FilterObj &left()
  {
    return left_;
  }
  FilterObj &right()
  {
    return right_;
  }

  void set_conjunction_type(char type)
  {
    conjunction_type_ = type;
  }

  char conjunction_type() const
  {
    return conjunction_type_;
  }

private:
  CompOp comp_ = NO_OP;
  FilterObj left_;
  FilterObj right_;
  char conjunction_type_ = 0; // 0: no conjunction, 1: and, 2: or
};

/**
 * @brief Filter/谓词/过滤语句
 * @ingroup Statement
 */
class FilterStmt 
{
public:
  FilterStmt() = default;
  virtual ~FilterStmt();

public:
  std::vector<FilterUnit *> &filter_units()
  {
    return filter_units_;
  }

public:
  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
      const ConditionSqlNode *conditions, int condition_num, FilterStmt *&stmt);

  static RC create_filter_unit(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
      const ConditionSqlNode &condition, FilterUnit *&filter_unit);

private:

  std::vector<FilterUnit *> filter_units_;  // 默认当前都是AND关系
};
