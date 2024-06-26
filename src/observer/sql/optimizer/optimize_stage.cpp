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

#include "event/sql_debug.h"
#include "optimize_stage.h"

#include "common/conf/ini.h"
#include "common/io/io.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "sql/expr/expression.h"
#include "sql/operator/logical_operator.h"
#include "sql/executor/sql_result.h"
#include "sql/stmt/stmt.h"
#include "event/sql_event.h"
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/create_table_stmt.h"
#include "event/session_event.h"

using namespace std;
using namespace common;

RC OptimizeStage::handle_request(SQLStageEvent *sql_event)
{
  unique_ptr<LogicalOperator> logical_operator;
  RC rc = create_logical_plan(sql_event, logical_operator);  // 生成逻辑计划
  if (rc != RC::SUCCESS) {
    if (rc != RC::UNIMPLENMENT) {
      LOG_WARN("failed to create logical plan. rc=%s", strrc(rc));
    }
    return rc;
  }

  // rc = rewrite(logical_operator);  // 优化逻辑计划
  // if (rc != RC::SUCCESS) {
  //   LOG_WARN("failed to rewrite plan. rc=%s", strrc(rc));
  //   return rc;
  // }

  rc = optimize(logical_operator);  // 优化逻辑计划
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to optimize plan. rc=%s", strrc(rc));
    return rc;
  }

  unique_ptr<PhysicalOperator> physical_operator;
  rc = generate_physical_plan(logical_operator, physical_operator);  // 将逻辑计划转换为物理计划
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to generate physical plan. rc=%s", strrc(rc));
    return rc;
  }

  sql_event->set_operator(std::move(physical_operator));


  // create table set physical oper
  Stmt *stmt = sql_event->stmt();
  if (stmt->type() == StmtType::CREATE_TABLE) {
    CreateTableStmt *create_table_stmt = static_cast<CreateTableStmt *>(stmt);
    if (create_table_stmt->select_stmt() != nullptr) {
      create_table_stmt->set_physical_operator(std::move(sql_event->physical_operator()));
    }
  }


  return rc;
}

RC OptimizeStage::optimize(unique_ptr<LogicalOperator> &oper)
{
  // do nothing
  return RC::SUCCESS;
}

RC OptimizeStage::generate_physical_plan(
    unique_ptr<LogicalOperator> &logical_operator, unique_ptr<PhysicalOperator> &physical_operator)
{
  // 有且仅有主查询会调用这个函数
  RC rc = RC::SUCCESS;
  rc = physical_plan_generator_.create(*logical_operator, physical_operator);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create physical operator. rc=%s", strrc(rc));
  }
  return rc;
}

RC OptimizeStage::rewrite(unique_ptr<LogicalOperator> &logical_operator)
{
  RC rc = RC::SUCCESS;
  
  bool change_made = false;
  do {
    change_made = false;
    rc = rewriter_.rewrite(logical_operator, change_made);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to do expression rewrite on logical plan. rc=%s", strrc(rc));
      return rc;
    }
  } while (change_made);

  return rc;
}

RC OptimizeStage::create_logical_plan(SQLStageEvent *sql_event, unique_ptr<LogicalOperator> &logical_operator)
{
  Stmt *stmt = sql_event->stmt();
  if (nullptr == stmt) {
    return RC::UNIMPLENMENT;
  }

  if (stmt->type() == StmtType::CREATE_TABLE) {
    CreateTableStmt *create_table_stmt = static_cast<CreateTableStmt *>(stmt);
    if (create_table_stmt->select_stmt() != nullptr) {
      // create table xx as select ...
      stmt = create_table_stmt->select_stmt();
    }
  }

  if(stmt->type() == StmtType::SELECT) {
    SelectStmt *select_stmt = static_cast<SelectStmt *>(stmt);
    if (select_stmt->order_by_fields().size() != 0) {
      sql_debug(sql_event->sql().c_str());
    }
  }

  return logical_plan_generator_.create(stmt, logical_operator);
}
