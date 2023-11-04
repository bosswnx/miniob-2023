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

#include "sql/operator/aggre_physical_operator.h"
#include "common/log/log.h"
#include "sql/expr/tuple.h"
#include "sql/operator/physical_operator.h"
#include "sql/operator/project_physical_operator.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
#include "storage/trx/trx.h"
#include <memory>
#include <vector>

RC AggregationPhysicalOperator::open(Trx *trx) {
  if (children_.empty()) {
    LOG_WARN("Aggregation physical operator has to have child physical operator.");
    return RC::INTERNAL;
  }
  std::unique_ptr<PhysicalOperator> &child = children_[0];
  RC rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }
  
  

  trx_ = trx;
  return RC::SUCCESS;
}

RC AggregationPhysicalOperator::next(Tuple *main_query_tuple) {
  if (finished_) {
    return RC::RECORD_EOF;
  }
  finished_ = true;
  RC rc = RC::SUCCESS;
  if (children_.empty()) {
    return RC::INTERNAL;
  }
  auto *child = children_[0].get();
  vector<Value> cells;
  while ((rc = child->next(main_query_tuple)) == RC::SUCCESS) {
    auto tuple = static_cast<ProjectTuple*>(child->current_tuple());
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }
    // 计算聚合函数
    for (int i = 0; i < tuple->cell_num(); i++) { 
      Value cell;
      tuple->cell_at(i, cell);
    }
  }
  auto tuple = static_cast<ProjectTuple*>(child->current_tuple());
  cells.resize(tuple->cell_num());
  for (int i = 0; i < tuple->cell_num(); i++) {
    Value cell;
    tuple->try_cell_at(i, cell);
    cells[i] = cell;
  }
  if (rc != RC::RECORD_EOF) {
    LOG_WARN("failed to get next record: %s", strrc(rc));
    return rc;
  }
  tuple_.set_cells(cells);
  // TODO: rtx
  return RC::SUCCESS;
}

RC AggregationPhysicalOperator::close() {
  children_[0]->close();
  finished_ = false;
  return RC::SUCCESS;
}