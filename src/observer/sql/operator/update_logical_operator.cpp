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
// Created by Soulter on 2023/10/18.
//

#include "sql/operator/update_physical_operator.h"
#include "sql/stmt/update_stmt.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"

using namespace std;

UpdatePhysicalOperator::UpdatePhysicalOperator(Table *table, Value &&values)
    : table_(table), values_(std::move(values))
{}

RC UpdatePhysicalOperator::open(Trx *trx)
{
  Record record;
  RC rc = table_->make_record(1, &values_, record); // 暂时支持单个值
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to make record. rc=%s", strrc(rc));
    return rc;
  }

  rc = trx->update_record(table_, record);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to update record by transaction. rc=%s", strrc(rc));
  }
  return rc;
}

RC UpdatePhysicalOperator::next()
{
  return RC::RECORD_EOF;
}

RC UpdatePhysicalOperator::close()
{
  return RC::SUCCESS;
}
