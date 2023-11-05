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
// Created by Longda on 2021/4/13.
//

#include <string.h>
#include <string>

#include "resolve_stage.h"

#include "common/conf/ini.h"
#include "common/io/io.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "event/sql_event.h"
#include "event/session_event.h"
#include "session/session.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/create_table_stmt.h"
#include "sql/stmt/stmt.h"

using namespace common;

RC ResolveStage::handle_request(SQLStageEvent *sql_event)
{
  RC rc = RC::SUCCESS;
  SessionEvent *session_event = sql_event->session_event();
  SqlResult *sql_result = session_event->sql_result();

  Db *db = session_event->session()->get_current_db();
  if (nullptr == db) {
    LOG_ERROR("cannot find current db");
    rc = RC::SCHEMA_DB_NOT_EXIST;
    sql_result->set_return_code(rc);
    sql_result->set_state_string("no db selected");
    return rc;
  }

  ParsedSqlNode *sql_node = sql_event->sql_node().get();


  SelectStmt *select_stmt = nullptr;
  Stmt *stmt_ = nullptr;
  if (sql_node->flag == SCF_CREATE_TABLE) {
    if (sql_node->create_table.sub_select != nullptr) {
      // create table select
      rc = Stmt::create_stmt(db, *sql_node->create_table.sub_select,
                                   stmt_);
      if (rc != RC::SUCCESS && rc != RC::UNIMPLENMENT) {
        LOG_WARN("failed to create stmt. rc=%d:%s", rc, strrc(rc));
        sql_result->set_return_code(rc);
        return rc;
      }
      // cast to SelectStmt
      select_stmt = static_cast<SelectStmt *>(stmt_);
    }
  }

  Stmt *stmt = nullptr;
  rc = Stmt::create_stmt(db, *sql_node, stmt);
  if (rc != RC::SUCCESS && rc != RC::UNIMPLENMENT) {
    LOG_WARN("failed to create stmt. rc=%d:%s", rc, strrc(rc));
    sql_result->set_return_code(rc);
    return rc;
  }

  // cast to CreateTableStmt
  if (sql_node->flag == SCF_CREATE_TABLE) {
    if (sql_node->create_table.sub_select != nullptr) {
      CreateTableStmt *create_table_stmt = static_cast<CreateTableStmt *>(stmt);
      create_table_stmt->set_select_stmt(select_stmt);
      // 将select_stmt的query_exprs赋值给create_table_stmt的query_names
      // create_table_stmt->set_query_names(select_stmt->query_names());
      // create_table_stmt->set_query_exprs(select_stmt->query_exprs());
      create_table_stmt->set_query_fields(select_stmt->true_query_fields());
    }
  }

  sql_event->set_stmt(stmt);

  return rc;
}
