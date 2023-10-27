/* Copyright (c) OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Soulter on 2023/10/18
//
#pragma once

#include <vector>

#include "sql/operator/logical_operator.h"
#include "sql/parser/parse_defs.h"
#include "storage/field/field_meta.h"
#include <utility> // pair

/**
 * @brief Update 逻辑算子
 * @ingroup LogicalOperator
 */
class UpdateLogicalOperator : public LogicalOperator
{
public:
  UpdateLogicalOperator(Table *table, const vector<FieldMeta> &field_metas, const vector<Value> &values);
  virtual ~UpdateLogicalOperator() = default;

  LogicalOperatorType type() const override
  {
    return LogicalOperatorType::UPDATE;
  }

  Table *table() const { return table_; }
  // const Value &values() const { return values_; } // 暂时支持单个值
  // Value &values() { return values_; } // 暂时支持单个值
  const vector<FieldMeta> &field_metas() const { return field_metas_; }
  const vector<Value> &values() const { return values_; }

private:
  Table *table_ = nullptr;
//   std::vector<Value> values_;
  // Value values_; // 暂时支持单个值
  vector<FieldMeta> field_metas_;
  vector<Value> values_;
};