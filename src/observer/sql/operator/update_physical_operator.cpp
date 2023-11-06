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
// Created by Soulter on 2023/10/21.
//

#include "common/log/log.h"
#include "sql/operator/update_physical_operator.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
#include "storage/record/record.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"
#include "sql/stmt/update_stmt.h"
#include <utility> // pair
#include <vector>
#include "common/update_sub_select.h"

using namespace std;

UpdatePhysicalOperator::UpdatePhysicalOperator(Table *table, const std::vector<FieldMeta> &field_metas, vector<UpdateSpecificTarget*> &targets)
    :table_(table), field_metas_(field_metas), targets_(targets)
{
}

RC UpdatePhysicalOperator::open(Trx *trx) {
    if (children_.empty()) {
        return RC::SUCCESS;
    }

    // 让 update set后面的子查询物理算子先执行，获取到值。
    for (int i=0; i<targets_.size(); ++i) {
      if(targets_[i]->is_value()) {
        values_.push_back(targets_[i]->get_value());
      } else {
        targets_[i]->get_sub_select_physical_operator()->open(trx);
        RC rc = targets_[i]->get_sub_select_physical_operator()->next();
        if (rc != RC::SUCCESS) {
          if (rc == RC::RECORD_EOF) {
            Value cell;
            cell.set_null(true);
            values_.push_back(cell);
            continue;
          }
          LOG_WARN("failed to get next record: %s", strrc(rc));
          return rc;
        }
        Tuple *tuple = targets_[i]->get_sub_select_physical_operator()->current_tuple();
        if (nullptr == tuple) {
          LOG_WARN("failed to get current record: %s", strrc(rc));
          return rc;
        }

        Value cell;
        rc = tuple->cell_at(0, cell);
        if (rc != RC::SUCCESS) {
          LOG_WARN("failed to get cell: %s", strrc(rc));
          return rc;
        }

        rc = targets_[i]->get_sub_select_physical_operator()->next();
        if (rc != RC::RECORD_EOF) {
          // LOG_WARN("failed to get next record: %s", strrc(rc));
          // rc = RC::INVALID_ARGUMENT; // 检测到了多行值。
          // return rc;
          has_failed = true;
        }
        // close
        targets_[i]->get_sub_select_physical_operator()->close();
        // RowTuple *row_tuple = static_cast<RowTuple *>(tuple);
        values_.push_back(cell);
      }
    }

    // 对values参照field_matas进行类型转换
    for (int i=0; i<values_.size(); ++i) {
      if (values_[i].get_null_or_()) {
        continue;
      }
      if (values_[i].attr_type() == field_metas_[i].type()) {
        continue;
      }
      if (values_[i].attr_type() == AttrType::INTS && field_metas_[i].type() == FLOATS) {
        values_[i].set_float((float)values_[i].get_int());
      } else if (values_[i].attr_type() == AttrType::FLOATS && field_metas_[i].type() == INTS) {
        values_[i].set_int((int)values_[i].get_float());
      } else if (values_[i].attr_type() == AttrType::CHARS && field_metas_[i].type() == INTS) {
        values_[i].set_int(atoi(values_[i].get_string().c_str()));
      } else if (values_[i].attr_type() == AttrType::CHARS && field_metas_[i].type() == FLOATS) {
        values_[i].set_float(atof(values_[i].get_string().c_str()));
      } else if (values_[i].attr_type() == AttrType::INTS && field_metas_[i].type() == CHARS) {
        values_[i].set_string(std::to_string(values_[i].get_int()).c_str());
      } else if (values_[i].attr_type() == AttrType::FLOATS && field_metas_[i].type() == CHARS) {
        values_[i].set_string(std::to_string(values_[i].get_float()).c_str());
      } else if (values_[i].attr_type() == CHARS && field_metas_[i].type() == TEXTS) {
        // DO NOTHING
      } else {
        LOG_WARN("failed to convert value type: %s", strrc(RC::INVALID_ARGUMENT));
        return RC::INVALID_ARGUMENT;
      }
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

RC UpdatePhysicalOperator::next(Tuple *main_query_tuple)
{
  RC rc = RC::SUCCESS;
  if (children_.empty()) {
    return RC::RECORD_EOF;
  }

  PhysicalOperator *child = children_[0].get();

  // Record new_record;
  // RC rc = table_->make_record(1, &values_, new_record);
  // if (rc != RC::SUCCESS) {
  //   LOG_WARN("failed to make record. rc=%s", strrc(rc));
  //   return rc;
  // }


  while (RC::SUCCESS == (rc = child->next())) {
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }

    if (has_failed) {
      rc = RC::INVALID_ARGUMENT;
      return rc;
    }

    RowTuple *row_tuple = static_cast<RowTuple *>(tuple);
    Record &old_record = row_tuple->record();
    // rc = trx_->delete_record(table_, old_record);
    rc = trx_->update_record(table_, old_record, field_metas_, values_);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to delete record: %s", strrc(rc));
      return rc;
    }
  }

  return RC::RECORD_EOF;
}

RC UpdatePhysicalOperator::close()
{
  if (!children_.empty()) {
    children_[0]->close();
  }
  return RC::SUCCESS;
}
