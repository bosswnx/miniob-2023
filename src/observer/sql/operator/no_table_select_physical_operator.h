//
// Created by Nelson Boss on 2023/11/05.
//

#pragma once

#include "sql/expr/tuple.h"
#include "sql/operator/physical_operator.h"
#include "common/rc.h"

class Table;

/**
 * @brief 无表选择物理算子
 * @ingroup PhysicalOperator
 */
class NoTableSelectPhysicalOperator : public PhysicalOperator
{
public:
  NoTableSelectPhysicalOperator() 
  {}

  virtual ~NoTableSelectPhysicalOperator() = default;

  std::string param() const override;

  PhysicalOperatorType type() const override
  {
    return PhysicalOperatorType::NO_TABLE_SELECT;
  }

  RC open(Trx *trx) override;
  RC next(Tuple *main_query_tuple = nullptr) override;
  RC close() override;

  Tuple *current_tuple() override;

private:
  Trx *trx_ = nullptr;
  bool init_ = false;
  ValueListTuple tuple_;  // 实际没啥用，只是为了返回一个tuple
};
