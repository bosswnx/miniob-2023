#pragma once

#include <vector>
#include <memory>

#include "sql/operator/logical_operator.h"
#include "storage/field/field.h"


class SortLogicalOperator : public LogicalOperator 
{
public:
  SortLogicalOperator(const std::vector<Field> &order_by_fields, std::vector<Field> query_fields, std::vector<Field> tables_all_fields);
  virtual ~SortLogicalOperator() = default;

  LogicalOperatorType type() const override
  {
    return LogicalOperatorType::SORT;
  }

  std::vector<std::unique_ptr<Expression>> &expressions()
  {
    return expressions_;
  }
  const std::vector<std::unique_ptr<Expression>> &expressions() const
  {
    return expressions_;
  }
  const std::vector<Field> &order_by_fields() const { return order_by_fields_; }
  std::vector<Field> &query_fields() { return query_fields_; }
  std::vector<Field> &tables_all_fields() {
    return tables_all_fields_;
  }

private:
  //! 聚合类型
  std::vector<Field> order_by_fields_;
  std::vector<Field> query_fields_;
  std::vector<Field> tables_all_fields_;
};
