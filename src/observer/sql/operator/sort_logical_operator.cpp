#include "sql/operator/sort_logical_operator.h"

SortLogicalOperator::SortLogicalOperator(const std::vector<Field> &order_by_fields, 
std::vector<Field> query_fields, 
std::vector<Field> tables_all_fields) : order_by_fields_(order_by_fields), query_fields_(query_fields), tables_all_fields_(tables_all_fields)
{}
