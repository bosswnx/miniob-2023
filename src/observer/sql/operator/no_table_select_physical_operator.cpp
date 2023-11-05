//
// Created by Nelson Boss on 2023/11/5.
//

#include "sql/operator/no_table_select_physical_operator.h"
#include "common/rc.h"
#include "event/sql_debug.h"

using namespace std;

RC NoTableSelectPhysicalOperator::open(Trx *trx)
{
  trx_ = trx;
  return RC::SUCCESS;
}

RC NoTableSelectPhysicalOperator::next(Tuple *main_query_tuple)
{
  if (init_) {
    return RC::RECORD_EOF;
  }
  init_ = true;
  return RC::SUCCESS;
}

RC NoTableSelectPhysicalOperator::close()
{
  return RC::SUCCESS;
}

Tuple *NoTableSelectPhysicalOperator::current_tuple()
{
  return &tuple_;
}

string NoTableSelectPhysicalOperator::param() const
{
  return "Unimplemented";
}
