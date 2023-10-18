//
// Created by Nelson Boss on 2023/10/17.
//

#pragma once

#include "common/rc.h"

class SQLStageEvent;

/**
 * @brief 描述表的执行器
 * @ingroup Executor
 */
class DropTableExecutor
{
public:
  DropTableExecutor() = default;
  virtual ~DropTableExecutor() = default;

  RC execute(SQLStageEvent *sql_event);
};