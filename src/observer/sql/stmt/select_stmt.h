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
// Created by Wangyunlai on 2022/6/5.
//

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "common/rc.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/stmt.h"
#include "storage/field/field.h"

class FieldMeta;
class FilterStmt;
class Db;
class Table;

/**
 * @brief 表示select语句
 * @ingroup Statement
 */
class SelectStmt : public Stmt 
{
public:
  SelectStmt() = default;
  ~SelectStmt() override;

  StmtType type() const override
  {
    return StmtType::SELECT;
  }

public:
  static RC create(Db *db, SelectSqlNode &select_sql, Stmt *&stmt, 
      std::shared_ptr<std::unordered_map<string,string>> name2alias = nullptr,
      std::shared_ptr<std::unordered_map<string,string>> alias2name = nullptr,
      std::shared_ptr<std::vector<string>> main_relation_names = nullptr);

public:
  const std::vector<Table *> &tables() const
  {
    return tables_;
  }
  const std::vector<Field> &query_fields() const
  {
    return query_fields_;
  }
  FilterStmt *filter_stmt() const
  {
    return filter_stmt_;
  }
  bool is_aggre() const { return is_aggre_; }
  const std::vector<AggreType> &aggre_types() const { return aggre_types_; }
  const std::vector<Field> &order_by_fields() const { return order_by_fields_; }

private:
  std::vector<Field> query_fields_;
  std::vector<AggreType> aggre_types_;
  std::vector<Table *> tables_;
  bool is_aggre_;
  FilterStmt *filter_stmt_ = nullptr;
  std::vector<Field> order_by_fields_;
};
