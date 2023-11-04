
#pragma once

#include "sql/operator/physical_operator.h"
#include "sql/expr/tuple.h"
#include "sql/parser/parse_defs.h"
#include "storage/record/record_manager.h"
#include "storage/table/table.h"
#include <vector>

/**
 * @brief 聚合函数物理算子
 * @ingroup PhysicalOperator
 */
class SortPhysicalOperator : public PhysicalOperator
{
public:
  SortPhysicalOperator(std::vector<Field> &order_by_fields, std::vector<Field> &query_fields, std::vector<Field> tables_all_fields);

  virtual ~SortPhysicalOperator() = default;

  PhysicalOperatorType type() const override
  {
    return PhysicalOperatorType::SORT;
  }

  std::string param() const override { return "Unimplemented";}
  std::string name() const override { return "Unimplemented";}

  RC open(Trx *trx) override;
  RC next(Tuple *main_query_tuple = nullptr) override;
  RC close() override;

  Tuple * current_tuple() override { return &tuple_; }

private:
  Trx * trx_ = nullptr;  // trx 用于处理并发

  std::vector<Field> order_by_fields_;
  std::vector<int> order_by_field_indexes_;
  std::vector<Field> tables_all_fields_;

  std::vector<Field> query_fields_;
  SortProjectTuple tuple_;

  std::vector<ValueListTuple> tuples_;  // 用于存储所有的 tuple
  int index_ = 0;
};
