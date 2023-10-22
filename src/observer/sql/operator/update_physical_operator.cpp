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
#include "storage/record/record.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"
#include "sql/stmt/update_stmt.h"
#include <utility> // pair

using namespace std;

UpdatePhysicalOperator::UpdatePhysicalOperator(Table *table, std::pair<Field*, Value*> *values_with_field)
    :table_(table), values_with_field_(values_with_field)
{}

RC UpdatePhysicalOperator::open(Trx *trx) {
    if (children_.empty()) {
        return RC::SUCCESS;
    }
    std::unique_ptr<PhysicalOperator> &child = children_[0];
    if (child->type() == PhysicalOperatorType::UPDATE) {
      UpdatePhysicalOperator *update_physical_operator = static_cast<UpdatePhysicalOperator *>(child.get());
      auto test = update_physical_operator->values_with_field();
    }
    RC rc = child->open(trx);
    if (rc != RC::SUCCESS) {
        LOG_WARN("failed to open child operator: %s", strrc(rc));
        return rc;
    }
    trx_ = trx;

    return RC::SUCCESS;
}

RC UpdatePhysicalOperator::next()
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

    RowTuple *row_tuple = static_cast<RowTuple *>(tuple);
    Record &old_record = row_tuple->record();
    // rc = trx_->delete_record(table_, old_record);
    rc = trx_->update_record(table_, old_record, values_with_field_);
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
