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
// Created by Wangyunlai on 2023/6/13.
//

#include "sql/executor/create_table_executor.h"

#include "common/rc.h"
#include "session/session.h"
#include "common/log/log.h"
#include "sql/parser/parse_defs.h"
#include "storage/table/table.h"
#include "sql/stmt/create_table_stmt.h"
#include "event/sql_event.h"
#include "event/session_event.h"
#include "storage/db/db.h"
#include "storage/trx/trx.h"

RC CreateTableExecutor::execute(SQLStageEvent *sql_event)
{
  Stmt *stmt = sql_event->stmt();
  Session *session = sql_event->session_event()->session();
  ASSERT(stmt->type() == StmtType::CREATE_TABLE, 
         "create table executor can not run this command: %d", static_cast<int>(stmt->type()));

  CreateTableStmt *create_table_stmt = static_cast<CreateTableStmt *>(stmt);

  const int attribute_count = static_cast<int>(create_table_stmt->attr_infos().size());

  const char *table_name = create_table_stmt->table_name().c_str();

  RC rc = RC::SUCCESS;
  if (create_table_stmt->physical_operator() != nullptr) {
    // create table as select
    std::vector<AttrInfoSqlNode> attr_infos;
    for (int i = 0; i < create_table_stmt->query_fields().size(); ++i) {
      AttrInfoSqlNode attr_info;
      attr_info.is_null = create_table_stmt->query_fields()[i].meta()->is_null();
      attr_info.name = create_table_stmt->query_fields()[i].meta()->name();
      attr_info.type = create_table_stmt->query_fields()[i].meta()->type();
      attr_info.length = create_table_stmt->query_fields()[i].meta()->len();
      attr_infos.push_back(attr_info);
    }
    rc = session->get_current_db()->create_table(table_name, create_table_stmt->query_fields().size(), attr_infos.data());

    // open physical operator
    Trx *trx = session->current_trx();
    rc = create_table_stmt->physical_operator()->open(session->current_trx());
    Table *table_ = session->get_current_db()->find_table(table_name);
    while (create_table_stmt->physical_operator()->next() == RC::SUCCESS) {
      // get tuple
      Tuple *tuple = create_table_stmt->physical_operator()->current_tuple();
      Record record;
      std::vector<Value> values_;
      for (int i = 0; i < tuple->cell_num(); ++i) {
        Value value;
        tuple->cell_at(i, value);
        values_.push_back(value);
      }
      rc = table_->make_record(static_cast<int>(values_.size()), values_.data(), record);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to make record. rc=%s", strrc(rc));
        return rc;
      }
      rc = trx->insert_record(table_, record);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to insert record by transaction. rc=%s", strrc(rc));
      }
    }
  } else {
    rc = session->get_current_db()->create_table(table_name, attribute_count, create_table_stmt->attr_infos().data());
  }


  return rc;
}