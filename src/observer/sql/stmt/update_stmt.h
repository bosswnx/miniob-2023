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

#include "common/rc.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
#include "sql/stmt/stmt.h"
#include "storage/field/field_meta.h"
#include <utility> // pair
#include <vector>

class Table;
class FilterStmt;
class Field;

/**
 * @brief 更新语句
 * @ingroup Statement
 */
class UpdateStmt : public Stmt 
{
public:
  // 使用 pair 存储字段名和 Value
  UpdateStmt(Table *table, FilterStmt *filter_stmt, 
      const std::vector<FieldMeta> &field_metas, std::vector<UpdateTarget> &targets, int value_amount);

  ~UpdateStmt() override;
  FilterStmt *filter_stmt() const
  {
    return filter_stmt_;
  }
  StmtType type() const override
  {
    return StmtType::UPDATE;
  }

public:
  static RC create(Db *db, UpdateSqlNode &update_sql, Stmt *&stmt);

public:
  Table *table() const
  {
    return table_;
  }
  int value_amount() const
  {
    return value_amount_;
  }
  const std::vector<FieldMeta> &field_metas() const {
    return field_metas_;
  }
  // const std::vector<Value> &values() const {
  //   return values_;
  // }
  std::vector<UpdateTarget> &targets() {
    return targets_;
  }

private:
  Table *table_ = nullptr;
  std::vector<FieldMeta> field_metas_;
  std::vector<UpdateTarget> targets_;
  int value_amount_ = 0;
  FilterStmt *filter_stmt_ = nullptr;
};
