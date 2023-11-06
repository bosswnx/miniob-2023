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
#include "sql/expr/tuple_cell.h"
#include "sql/operator/physical_operator.h"
#include "sql/operator/project_physical_operator.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
#include "storage/field/field_meta.h"
#include "storage/table/table.h"
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

  RC rc = RC::SUCCESS;

  if (children_.empty()) {
    return RC::INTERNAL;
  }
  auto *child = children_[0].get();

  // 预取
  if (group_by_fields_.size() != 0) {
    if (!has_fetched_) {
      // all_table_fields_ 
      for (int i = 0; i < group_by_fields_.size(); ++i) {
        TupleCellSpec spec(group_by_fields_[i].table_name(), group_by_fields_[i].field_name(), group_by_fields_[i].table_alias());
        group_by_field_specs_.push_back(spec);
      }
      // 如果有group_by子句 并且 本算子之前没有fetch过tuple
      // 那么就需要先fetch tuple
      while ((rc = child->next(main_query_tuple)) == RC::SUCCESS) {
        auto tuple = static_cast<ProjectTuple*>(child->current_tuple());
        if (nullptr == tuple) {
          LOG_WARN("failed to get current record: %s", strrc(rc));
          return rc;
        }
        GroupByTuple t;
        for (int i = 0; i < tuple->all_c_num(); i++) {
          Value cell;
          tuple->all_c_at(i, cell);
          t.append_cell(cell);
        }
        t.set_schema(all_table_fields_);
        tuples_.push_back(t);
      }
      has_fetched_ = true;
      if (rc != RC::RECORD_EOF) {
        LOG_WARN("failed to get next record: %s", strrc(rc));
        return rc;
      }
    }
    auto tuple = static_cast<ProjectTuple*>(child->current_tuple());
    for (int i=0; i<tuple->cell_num(); ++i) {
      tuple->reset_aggre_expr(i);
    }
    // 基于 tuples 的第一项从tuples中选出所有符合条件的tuple
    // if (tuples_.empty()) return RC::RECORD_EOF;
    bool done = true;
    int start = 0;
    for (; start<tuples_.size(); ++start) {
      if (tuples_[start].get_mark()) continue;
      else {
        done = false;
        tuples_[start].set_mark(true);
        break;
      }
    }
    if (done) {
      finished_ = true;
      return RC::RECORD_EOF;
    }

    GroupByTuple t = tuples_[start];

    tuple->set_tuple(&t);
    // 计算聚合函数
    for (int i = 0; i < tuple->cell_num(); i++) { 
      Value cell;
      tuple->cell_at(i, cell);
    }

    // 设置条件
    std::vector<Value> conditions;
    for (int i = 0; i < group_by_fields_.size(); i++) {
      Value condition;
      t.find_cell(group_by_field_specs_[i], condition);
      conditions.push_back(condition);
    }
    // 找到满足条件的其他项的tuple
    for (int i = start + 1; i < tuples_.size(); i++) {
      bool flag = true;
      for (int j = 0; j < group_by_fields_.size(); j++) {
        Value condition;
        tuples_[i].find_cell(group_by_field_specs_[j], condition);
        if (condition.compare(conditions[j]) != 0) {
          flag = false;
          break;
        }
      }
      if (!flag) continue; // 这个tuple不满足
      // 到这里已经找到满足条件的tuple
      tuples_[i].set_mark(true);
      tuple->set_tuple(&tuples_[i]);
      // 计算聚合函数
      for (int i = 0; i < tuple->cell_num(); i++) { 
        Value cell;
        tuple->cell_at(i, cell);
      }
    }

    // 到这里 该组的聚合函数计算完毕
    // conditions.size() + tuple.cell_num()
    vector<Value> cells;
    cells.resize(conditions.size() + tuple->cell_num());
    for (int i = 0; i < conditions.size(); i++) {
      cells[i] = conditions[i];
    }
    for (int i = 0; i < tuple->cell_num(); i++) {
      Value cell;
      tuple->try_cell_at(i, cell);
      cells[i + conditions.size()] = cell;
    }
    tuple_.set_cells(cells);

    tuple = static_cast<ProjectTuple*>(child->current_tuple());
    for (int i=0; i<tuple->cell_num(); ++i) {
      tuple->reset_aggre_expr(i);
    }
  } else {

    finished_ = true;

    vector<Value> cells;
    bool has = false;
    while ((rc = child->next(main_query_tuple)) == RC::SUCCESS) {
      has = true;
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
      if (!has) {
        cell.set_null(true);
        // 类型要吗？
      } else {
        tuple->try_cell_at(i, cell);
        cells[i] = cell;
      }
    }
    if (rc != RC::RECORD_EOF) {
      LOG_WARN("failed to get next record: %s", strrc(rc));
      return rc;
    }
    tuple_.set_cells(cells);
    // TODO: rtx
    return RC::SUCCESS;

  }

  return RC::SUCCESS;
}

RC AggregationPhysicalOperator::close() {
  children_[0]->close();
  finished_ = false;
  return RC::SUCCESS;
}