//
// Created by Nelson Boss on 2023/10/17.
//

#include "sql/executor/drop_table_executor.h"

#include "session/session.h"
#include "common/log/log.h"
#include "sql/stmt/drop_table_stmt.h"
#include "storage/table/table.h"
#include "event/sql_event.h"
#include "event/session_event.h"
#include "storage/db/db.h"

RC DropTableExecutor::execute(SQLStageEvent *sql_event)
{
  Stmt *stmt = sql_event->stmt();
  Session *session = sql_event->session_event()->session();
  ASSERT(stmt->type() == StmtType::DROP_TABLE, 
         "drop table executor can not run this command: %d", static_cast<int>(stmt->type()));

  DropTableStmt *drop_table_stmt = static_cast<DropTableStmt *>(stmt);

  const char *table_name = drop_table_stmt->table_name().c_str();
  RC rc = session->get_current_db()->drop_table(table_name);

  return rc;
}