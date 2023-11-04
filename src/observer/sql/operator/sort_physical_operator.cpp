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

#include "sql/operator/sort_physical_operator.h"
#include "common/log/log.h"
#include "sql/expr/tuple.h"
#include "sql/operator/physical_operator.h"
#include "sql/operator/project_physical_operator.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"
#include <algorithm>
#include <memory>
#include <vector>

SortPhysicalOperator::SortPhysicalOperator(std::vector<Field> &order_by_fields, std::vector<Field> &query_fields, std::vector<Field> tables_all_fields): order_by_fields_(order_by_fields), query_fields_(query_fields), tables_all_fields_(tables_all_fields) {}

RC SortPhysicalOperator::open(Trx *trx) {
  if (children_.empty()) {
    LOG_WARN("SORT physical operator has to have child physical operator.");
    return RC::INTERNAL;
  }
  std::unique_ptr<PhysicalOperator> &child = children_[0];
  RC rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  for (int i=0; i<order_by_fields_.size(); ++i) {
    for(int j=0; j<tables_all_fields_.size(); ++j) {
      if (order_by_fields_[i].table_name() == tables_all_fields_[j].table_name() && order_by_fields_[i].field_name() == tables_all_fields_[j].field_name()) {
        order_by_field_indexes_.push_back(j);
        break;
      }
    }
  }

  // we need to get all the records from the child operator
  // and sort them

  while (rc != RC::RECORD_EOF) {
    ValueListTuple tuple;
    rc = child->next();
    if (rc == RC::RECORD_EOF) {
      break;
    } else if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get next tuple from child operator: %s", strrc(rc));
      return rc;
    }
    std::vector<Value> cells;
    // int test = child->current_tuple()->all_c_num();
    for (int i=0; i<child->current_tuple()->all_c_num(); ++i) {
        Value cell;
        child->current_tuple()->all_c_at(i, cell);
        cells.push_back(cell);
    }
    tuple.set_cells(cells);
    tuples_.push_back(tuple);
  }

    // sort the tuples
    std::stable_sort(tuples_.begin(), tuples_.end(), [this](const ValueListTuple &a, const ValueListTuple &b) {
        int ret = 0;
        for (int i=0; i<order_by_fields_.size(); ++i) {
          Value cell_a, cell_b;
            // a.cell_at(order_by_fields_[i].meta()->index(), cell_a);
            // b.cell_at(order_by_fields_[i].meta()->index(), cell_b);
            a.cell_at(order_by_field_indexes_[i], cell_a);
            b.cell_at(order_by_field_indexes_[i], cell_b);
            ret = cell_a.compare(cell_b);
            if (ret != 0) {
                if (order_by_fields_[i].is_asc()) {
                    return ret < 0;
                } else {
                    return ret > 0;
                }
            }
        }
        return ret < 0;
    });

  if (tuples_.size() > 0) {
    //pre loading
    for (int i=0; i<query_fields_.size(); ++i) {
      for (int j=0; j<tables_all_fields_.size(); ++j) {
        if (query_fields_[i].table_name() == tables_all_fields_[j].table_name() && query_fields_[i].field_name() == tables_all_fields_[j].field_name()) {
          tuple_.add_position(j);
          break;
        }
      }
    }
  }

  trx_ = trx;
  return RC::SUCCESS;
}

bool cmp(const ValueListTuple &a, const ValueListTuple &b, std::vector<Field> &order_by_fields) {
  int ret = 0;
  for (int i=0; i<order_by_fields.size(); ++i) {
    Value cell_a, cell_b;
    a.cell_at(i, cell_a);
    b.cell_at(i, cell_b);
    ret = cell_a.compare(cell_b);
    if (ret != 0) {
      if (order_by_fields[i].is_asc()) {
        return ret < 0;
      } else {
        return ret > 0;
      }
    }
  }
  return ret < 0;
}

RC SortPhysicalOperator::next(Tuple *main_query_tuple) {
    if (index_ < tuples_.size()) {
        tuple_.set_tuple(&tuples_[index_++]);
        return RC::SUCCESS;
    } else {
        // wocaonima BEGIN
        order_by_fields_.clear();
        order_by_field_indexes_.clear();
        tables_all_fields_.clear();
        query_fields_.clear();
        tuples_.clear();
        index_ = 0;
        // wocaonima END
        return RC::RECORD_EOF;
    }
}

RC SortPhysicalOperator::close() {
  children_[0]->close();
  return RC::SUCCESS;
}