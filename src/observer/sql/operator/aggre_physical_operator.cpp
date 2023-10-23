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

AggregationPhysicalOperator::AggregationPhysicalOperator(std::vector<AggreType> &aggre_types): aggre_types_(aggre_types) {}

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

RC AggregationPhysicalOperator::next() {
  if (finished_) {
    return RC::RECORD_EOF;
  }
  finished_ = true;
  RC rc = RC::SUCCESS;
  if (children_.empty()) {
    return RC::INTERNAL;
  }
  auto *child = children_[0].get();
  vector<Value> cells(aggre_types_.size());
  bool initialized = false;
  int count = 0;
  while ((rc = child->next()) == RC::SUCCESS) {
    auto tuple = static_cast<ProjectTuple*>(child->current_tuple());
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }
    if (!initialized) {
      initialized = true;
      for (int i = 0; i < aggre_types_.size(); i++) {
        Value cell;
        tuple->cell_at(i, cell);
        switch (aggre_types_[i]) {
          case AggreType::MAX: 
          case AggreType::MIN:
          case AggreType::SUM: {
            switch (cell.attr_type()) {
              case INTS: {
                cells[i].set_type(INTS);
                cells[i].set_int(cell.get_int());
              } break;
              case FLOATS: {
                cells[i].set_type(FLOATS);
                cells[i].set_float(cell.get_float());
              } break;
              default: {
                LOG_WARN("invalid attrebute type.");
                return RC::INVALID_ARGUMENT;
              }
            }
          } break;
          case AggreType::AVG: {
            switch (cell.attr_type()) {
              case INTS:
              case FLOATS: {
                cells[i].set_type(FLOATS);
                cells[i].set_float(cell.get_float());
              } break;
              default: {
                LOG_WARN("invalid attrebute type.");
                return RC::INVALID_ARGUMENT;
              }
            }
          } break;
          case AggreType::CNT: {
            cells[i].set_type(INTS);
            cells[i].set_int(1);
          } break;
          default: {
            LOG_WARN("NONE aggregation type in operator");
            return RC::INTERNAL;
          }
        }
      }
      count++;
      continue;
    }
    for (int i = 0; i < aggre_types_.size(); i++) {
      Value cell;
      tuple->cell_at(i, cell);
      switch (aggre_types_[i]) {
        case AggreType::MAX: {
          if (cells[i].attr_type() == INTS) {
            cells[i].set_int(std::max(cells[i].get_int(), cell.get_int()));
          } else {
            cells[i].set_float(std::max(cells[i].get_float(), cell.get_float()));
          }
        } break;
        case AggreType::MIN: {
          if (cells[i].attr_type() == INTS) {
            cells[i].set_int(std::min(cells[i].get_int(), cell.get_int()));
          } else {
            cells[i].set_float(std::min(cells[i].get_float(), cell.get_float()));
          }
        } break;
        case AggreType::SUM: {
          if (cells[i].attr_type() == INTS) {
            cells[i].set_int(cells[i].get_int() + cell.get_int());
          } else {
            cells[i].set_float(cells[i].get_float() + cells[i].get_float());
          }
        } break;
        case AggreType::AVG: {
          cells[i].set_float(cells[i].get_float() + cell.get_float());
        }
        default: break;
      }
      count++;
    }
  }
  // todo: trx
  for (int i = 0; i < aggre_types_.size(); i++) {
    switch (aggre_types_[i]) {
      case AggreType::AVG: {
        cells[i].set_float(cells[i].get_float() / count);
      } break;
      case AggreType::CNT: {
        cells[i].set_int(count);
      }
      default: break;
    }
  }
  tuple_.set_cells(cells);
  return RC::SUCCESS;
}

RC AggregationPhysicalOperator::close() {
  children_[0]->close();
  return RC::SUCCESS;
}