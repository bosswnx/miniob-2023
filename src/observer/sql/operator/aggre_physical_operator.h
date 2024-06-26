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
// Created by Wangyunlai on 2022/07/08.
//

#pragma once

#include "sql/operator/physical_operator.h"
#include "sql/expr/tuple.h"
#include "sql/parser/parse_defs.h"
#include "storage/record/record_manager.h"
#include <memory>

/**
 * @brief 聚合函数物理算子
 * @ingroup PhysicalOperator
 */
class AggregationPhysicalOperator : public PhysicalOperator
{
public:
  AggregationPhysicalOperator() {}

  virtual ~AggregationPhysicalOperator() = default;

  PhysicalOperatorType type() const override
  {
    return PhysicalOperatorType::AGGREGATION;
  }

  std::string param() const override { return "Unimplemented";}
  std::string name() const override { return "Unimplemented";}

  RC open(Trx *trx) override;
  RC next(Tuple *main_query_tuple = nullptr) override;
  RC close() override;

  Tuple * current_tuple() override { return &tuple_; }

private:
  Trx * trx_ = nullptr;  // trx 用于处理并发
  ValueListTuple tuple_;
  bool finished_ = false;
};
