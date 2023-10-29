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
// Created by Wangyunlai on 2022/07/05.
//

#include "sql/expr/expression.h"
#include "sql/expr/tuple.h"
#include "sql/operator/physical_operator.h"
#include "sql/operator/logical_operator.h"

class LogicalOperator;
class PhysicalOperator;
class Trx;


using namespace std;

RC FieldExpr::get_value(const Tuple &tuple, Value &value)
{
  return tuple.find_cell(TupleCellSpec(table_name(), field_name()), value);
}

RC ValueExpr::get_value(const Tuple &tuple, Value &value)
{
  value = value_;
  return RC::SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////
CastExpr::CastExpr(unique_ptr<Expression> child, AttrType cast_type)
    : child_(std::move(child)), cast_type_(cast_type)
{}

CastExpr::~CastExpr()
{}

RC CastExpr::cast(const Value &value, Value &cast_value) const
{
  RC rc = RC::SUCCESS;
  if (this->value_type() == value.attr_type()) {
    cast_value = value;
    return rc;
  }

  switch (cast_type_) {
    case BOOLEANS: {
      bool val = value.get_boolean();
      cast_value.set_boolean(val);
    } break;
    default: {
      rc = RC::INTERNAL;
      LOG_WARN("unsupported convert from type %d to %d", child_->value_type(), cast_type_);
    }
  }
  return rc;
}

RC CastExpr::get_value(const Tuple &tuple, Value &cell)
{
  RC rc = child_->get_value(tuple, cell);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  return cast(cell, cell);
}

RC CastExpr::try_get_value(Value &value) const
{
  RC rc = child_->try_get_value(value);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  return cast(value, value);
}


////////////////////////////////////////////////////////////////////////////////
SubqueryExpr::SubqueryExpr(AttrType attr_type, const char *table_name, const char *field_name, unique_ptr<LogicalOperator> logical_operator)
    : attr_type_(attr_type), table_name_(table_name), field_name_(field_name), logical_operator_(std::move(logical_operator))
{}

RC SubqueryExpr::get_value(const Tuple &tuple, Value &value)
{
  RC rc = RC::SUCCESS;
  if (physical_operator_ == nullptr) {
    LOG_WARN("physical operator is null");
    return RC::INVALID_ARGUMENT;
  }

  if (!is_open_) {
    rc = open_physical_operator();
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to open physical operator. rc=%s", strrc(rc));
      return rc;
    }
  }

  // 查询子查询的结果，写入value中
  rc = physical_operator_->next();
  if (rc != RC::SUCCESS) {
    // maybe we reach the end of the result
    if (rc != RC::RECORD_EOF) {
      LOG_PANIC("failed to get next tuple. rc=%s", strrc(rc));
      return rc;
    }
    rc = close_physical_operator();
    if (rc == RC::SUCCESS) {
      rc = RC::RECORD_EOF;
    } else {
      LOG_PANIC("failed to close physical operator. rc=%s", strrc(rc));
    }
    return rc;
  }
  Tuple *another_tuple = physical_operator_->current_tuple();
  // if(another_tuple->type() == TupleType::VALUE_LIST) {
  //   if (another_tuple->cell_num() == 0) {
  //     LOG_WARN("value list tuple cell count is not 1");
  //     return RC::INTERNAL;
  //   }
  //   // ⚠：取第一个
  //   another_tuple->cell_at(0, value);
  //   // UNDEFINED 表示没有匹配的值
  //   if (value.attr_type() == AttrType::UNDEFINED) {
  //     rc = RC::RECORD_EOF;
  //   }
  // } else {
  //   if (another_tuple->cell_num() != 1) {
  //     LOG_WARN("tuple cell count is not 1");
  //     return RC::INTERNAL;
  //   }
  //   rc = another_tuple->find_cell(TupleCellSpec(table_name_, field_name_), value);
  //   if (rc != RC::SUCCESS) {
  //     LOG_WARN("failed to find cell. rc=%s", strrc(rc));
  //     return rc;
  //   }
  // }
  if (another_tuple->cell_num() != 1) {
    LOG_WARN("tuple cell count is not 1");
    return RC::INVALID_ARGUMENT;
  }
  another_tuple->cell_at(0, value);
  return rc;
}

RC SubqueryExpr::set_physical_operator(unique_ptr<PhysicalOperator> physical_operator)
{
  physical_operator_ = std::move(physical_operator);
  return RC::SUCCESS;
}

RC SubqueryExpr::set_trx(Trx *trx)
{
  trx_ = trx;
  return RC::SUCCESS;
}

RC SubqueryExpr::open_physical_operator()
{
  if (physical_operator_ == nullptr) {
    LOG_WARN("physical operator is null");
    return RC::INVALID_ARGUMENT;
  }
  RC rc = physical_operator_->open(trx_);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open physical operator. rc=%s", strrc(rc));
  } else {
    is_open_ = true;
  }
  return rc;
}

RC SubqueryExpr::close_physical_operator()
{
  if (physical_operator_ == nullptr) {
    LOG_WARN("physical operator is null");
    return RC::INVALID_ARGUMENT;
  }
  RC rc = physical_operator_->close();
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to close physical operator. rc=%s", strrc(rc));
  } else {
    is_open_ = false;
  }
  return rc;
}
////////////////////////////////////////////////////////////////////////////////

ComparisonExpr::ComparisonExpr(CompOp comp, unique_ptr<Expression> left, unique_ptr<Expression> right)
    : comp_(comp), left_(std::move(left)), right_(std::move(right))
{}

ComparisonExpr::~ComparisonExpr()
{}

RC ComparisonExpr::compare_value(const Value &left, const Value &right, bool &result) const
{
  RC rc = RC::SUCCESS;
  result = false;
  // 这里把 like 算作一种特殊的算符（comporator）
  if (comp_ == CompOp::LIKE) {
    if (left.attr_type() != AttrType::CHARS || right.attr_type() != AttrType::CHARS) {
      LOG_PANIC("like operator only support string type now");
      return RC::INVALID_ARGUMENT;
    }
    result = left.like(left.data(), right.data());
    return rc;
  } else if (comp_ == CompOp::NOT_LIKE) {
    if (left.attr_type() != AttrType::CHARS || right.attr_type() != AttrType::CHARS) {
      LOG_PANIC("like operator only support string type now");
      return RC::INVALID_ARGUMENT;
    }
    result = !left.like(left.data(), right.data());
    return rc;
  }
  int cmp_result = left.compare(right);
  switch (comp_) {
    case IN_:
    case EQUAL_TO: {
      result = (0 == cmp_result);
    } break;
    case LESS_EQUAL: {
      result = (cmp_result <= 0);
    } break;
    case NOT_IN:
    case NOT_EQUAL: {
      result = (cmp_result != 0);
    } break;
    case LESS_THAN: {
      result = (cmp_result < 0);
    } break;
    case GREAT_EQUAL: {
      result = (cmp_result >= 0);
    } break;
    case GREAT_THAN: {
      result = (cmp_result > 0);
    } break;
    default: {
      LOG_WARN("unsupported comparison. %d", comp_);
      rc = RC::INTERNAL;
    } break;
  }

  return rc;
}

RC ComparisonExpr::try_get_value(Value &cell) const
{
  if (left_->type() == ExprType::VALUE && right_->type() == ExprType::VALUE) {
    ValueExpr *left_value_expr = static_cast<ValueExpr *>(left_.get());
    ValueExpr *right_value_expr = static_cast<ValueExpr *>(right_.get());
    const Value &left_cell = left_value_expr->get_value();
    const Value &right_cell = right_value_expr->get_value();

    bool value = false;
    RC rc = compare_value(left_cell, right_cell, value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to compare tuple cells. rc=%s", strrc(rc));
    } else {
      cell.set_boolean(value);
    }
    return rc;
  }

  return RC::INVALID_ARGUMENT;
}

RC ComparisonExpr::get_value(const Tuple &tuple, Value &value)
{
  Value left_value;
  Value right_value;
  RC rc;

  if (left_->type() == ExprType::SUBQUERY && right_->type() == ExprType::SUBQUERY) {
    LOG_WARN("not support subquery in both side of comparison");
    return RC::INVALID_ARGUMENT;
  }
  
  if (left_->type() == ExprType::SUBQUERY || right_->type() == ExprType::SUBQUERY) {
    SubqueryExpr *subquery_expr;
    Value* sub_query_value;
    if (left_->type() == ExprType::SUBQUERY) {
      subquery_expr = static_cast<SubqueryExpr *>(left_.get());
      rc = right_->get_value(tuple, right_value); // 假设右边不是子查询
      sub_query_value = &left_value;
    } else {
      subquery_expr = static_cast<SubqueryExpr *>(right_.get());
      rc = left_->get_value(tuple, left_value); // 假设左边不是子查询
      sub_query_value = &right_value;
    }
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }

    bool bool_value = false;
    // 循环next子查询的算子，直到找到一个满足条件的值
    while((rc = subquery_expr->get_value(tuple, *sub_query_value)) == RC::SUCCESS) {
      if (sub_query_value->attr_type() == UNDEFINED) {
        rc = RC::RECORD_EOF; // maybe wrong
        break;
      }

      // 当comp_为=或者<>时，子查询的结果只能是一个值
      if (comp_ == CompOp::EQUAL_TO || comp_ == CompOp::NOT_EQUAL) {
        if (has_sub_queried_) {
          has_sub_queried_ = false; // 良好的习惯，重置
          rc = RC::INVALID_ARGUMENT;
          break;
        } else {
          has_sub_queried_ = true;
        }
      }
      
      rc = compare_value(left_value, right_value, bool_value);

      // 当CompOp不是NOT_IN时，只要找到一个满足条件的值就可以返回
      // 当CompOp是NOT_IN时，必须得遍历完所有的值才能知道
      if (rc == RC::SUCCESS && comp_ != CompOp::NOT_IN && bool_value ) {
        value.set_boolean(bool_value);
        break;
      } else if (rc == RC::SUCCESS && comp_ == CompOp::NOT_IN && !bool_value) {
        // 当CompOp==NOT_IN，且左右value相等（因为是NOTIN，所以bool_value前要加!），直接结束循环
        value.set_boolean(bool_value);
        break;
      }
      
    }

    if (rc == RC::INVALID_ARGUMENT) return rc;

    // NOT_IN 判断
    
    if (rc == RC::RECORD_EOF) {
      if (comp_ == CompOp::NOT_IN) {
        value.set_boolean(true);
        rc = RC::SUCCESS;
        return rc;
      }
    }
    if (rc != RC::SUCCESS && rc != RC::RECORD_EOF) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    }


    // 关闭算子
    // 可优化 static_cast 潜在的开销
    rc = subquery_expr->close_physical_operator();
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to close physical operator. rc=%s", strrc(rc));
    }
  } else {
    rc = left_->get_value(tuple, left_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
      return rc;
    }
    rc = right_->get_value(tuple, right_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }
    bool bool_value = false;
    rc = compare_value(left_value, right_value, bool_value);
    if (rc == RC::SUCCESS) {
      value.set_boolean(bool_value);
    }
  }
  has_sub_queried_ = false;
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
ConjunctionExpr::ConjunctionExpr(Type type, vector<unique_ptr<Expression>> &children)
    : conjunction_type_(type), children_(std::move(children))
{}

RC ConjunctionExpr::get_value(const Tuple &tuple, Value &value)
{
  RC rc = RC::SUCCESS;
  if (children_.empty()) {
    value.set_boolean(true);
    return rc;
  }

  Value tmp_value;
  for (const unique_ptr<Expression> &expr : children_) {
    rc = expr->get_value(tuple, tmp_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value by child expression. rc=%s", strrc(rc));
      return rc;
    }
    bool bool_value = tmp_value.get_boolean();
    if ((conjunction_type_ == Type::AND && !bool_value) || (conjunction_type_ == Type::OR && bool_value)) {
      value.set_boolean(bool_value);
      return rc;
    }
  }

  bool default_value = (conjunction_type_ == Type::AND);
  value.set_boolean(default_value);
  return rc;
}

////////////////////////////////////////////////////////////////////////////////

ArithmeticExpr::ArithmeticExpr(ArithmeticExpr::Type type, Expression *left, Expression *right)
    : arithmetic_type_(type), left_(left), right_(right)
{}
ArithmeticExpr::ArithmeticExpr(ArithmeticExpr::Type type, unique_ptr<Expression> left, unique_ptr<Expression> right)
    : arithmetic_type_(type), left_(std::move(left)), right_(std::move(right))
{}

AttrType ArithmeticExpr::value_type() const
{
  if (!right_) {
    return left_->value_type();
  }

  if (left_->value_type() == AttrType::INTS &&
      right_->value_type() == AttrType::INTS &&
      arithmetic_type_ != Type::DIV) {
    return AttrType::INTS;
  }
  
  return AttrType::FLOATS;
}

RC ArithmeticExpr::calc_value(const Value &left_value, const Value &right_value, Value &value) const
{
  RC rc = RC::SUCCESS;

  const AttrType target_type = value_type();

  switch (arithmetic_type_) {
    case Type::ADD: {
      if (target_type == AttrType::INTS) {
        value.set_int(left_value.get_int() + right_value.get_int());
      } else {
        value.set_float(left_value.get_float() + right_value.get_float());
      }
    } break;

    case Type::SUB: {
      if (target_type == AttrType::INTS) {
        value.set_int(left_value.get_int() - right_value.get_int());
      } else {
        value.set_float(left_value.get_float() - right_value.get_float());
      }
    } break;

    case Type::MUL: {
      if (target_type == AttrType::INTS) {
        value.set_int(left_value.get_int() * right_value.get_int());
      } else {
        value.set_float(left_value.get_float() * right_value.get_float());
      }
    } break;

    case Type::DIV: {
      if (target_type == AttrType::INTS) {
        if (right_value.get_int() == 0) {
          // NOTE: 设置为整数最大值是不正确的。通常的做法是设置为NULL，但是当前的miniob没有NULL概念，所以这里设置为整数最大值。
          value.set_int(numeric_limits<int>::max());
        } else {
          value.set_int(left_value.get_int() / right_value.get_int());
        }
      } else {
        if (right_value.get_float() > -EPSILON && right_value.get_float() < EPSILON) {
          // NOTE: 设置为浮点数最大值是不正确的。通常的做法是设置为NULL，但是当前的miniob没有NULL概念，所以这里设置为浮点数最大值。
          value.set_float(numeric_limits<float>::max());
        } else {
          value.set_float(left_value.get_float() / right_value.get_float());
        }
      }
    } break;

    case Type::NEGATIVE: {
      if (target_type == AttrType::INTS) {
        value.set_int(-left_value.get_int());
      } else {
        value.set_float(-left_value.get_float());
      }
    } break;

    default: {
      rc = RC::INTERNAL;
      LOG_WARN("unsupported arithmetic type. %d", arithmetic_type_);
    } break;
  }
  return rc;
}

RC ArithmeticExpr::get_value(const Tuple &tuple, Value &value)
{
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;

  rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  rc = right_->get_value(tuple, right_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    return rc;
  }
  return calc_value(left_value, right_value, value);
}

RC ArithmeticExpr::try_get_value(Value &value) const
{
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;

  rc = left_->try_get_value(left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }

  if (right_) {
    rc = right_->try_get_value(right_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }
  }

  return calc_value(left_value, right_value, value);
}