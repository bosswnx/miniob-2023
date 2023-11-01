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
// Created by Soulter on 2022/10/18.
//

#pragma once

#include "sql/operator/physical_operator.h"
#include "sql/parser/parse.h"
#include "sql/parser/parse_defs.h"
#include <memory>
#include <utility> // pair

class Trx;
class UpdateStmt;
class UpdateSpecificTarget;

/**
 * @brief Update 物理算子
 * @ingroup UpdateOperator
 */
class UpdatePhysicalOperator : public PhysicalOperator
{
public:
  UpdatePhysicalOperator(Table *table, const vector<FieldMeta> &field_metas, vector<UpdateSpecificTarget*> &targets);

  virtual ~UpdatePhysicalOperator() = default;

  PhysicalOperatorType type() const override
  {
    return PhysicalOperatorType::UPDATE;
  }

  const vector<FieldMeta> &field_metas() const { return field_metas_; }
  // const vector<Value> &values() const { return values_; }
  const vector<UpdateSpecificTarget*> &targets() const { return targets_; }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override { return nullptr; }

private:
  Table *table_ = nullptr;
  // std::vector<Value> values_;
  // Value values_; // 暂时支持单个值
  vector<FieldMeta> field_metas_;
  vector<UpdateSpecificTarget*> targets_;
  vector<Value> values_;
  bool has_failed = false;
  Trx *trx_ = nullptr;
};
