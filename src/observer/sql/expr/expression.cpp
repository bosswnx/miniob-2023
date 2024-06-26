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
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/expr/tuple.h"
#include "sql/expr/tuple_cell.h"
#include "sql/operator/physical_operator.h"
#include "sql/operator/logical_operator.h"
#include "sql/parser/value.h"
#include "storage/buffer/page.h"
#include "storage/index/index.h"
#include "gtest/gtest.h"
#include <cmath>
#include <cstddef>
#include <cstring>
#include <string>

class LogicalOperator;
class PhysicalOperator;
class Trx;

#include "sql/parser/parse_defs.h"

using namespace std;

RC FieldExpr::get_value(const Tuple &tuple, Value &value, Trx *trx)
{
  RC rc = tuple.find_cell(TupleCellSpec(table_name(), field_name()), value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("field expr get value fail");
  }
  if (field_.attr_type() != TEXTS) {
    return rc;
  }
  // 如果当前字段是 TEXTS，需要把对应的数据取出来
  auto data = value.data();
  int length = *(int *)data;
  char *text_data = new char[length];
  memset(text_data, 0, length);
  int offset = 0;
  for (int i = 0; i < BP_TEXTS_PAGE_NUM; i++) {
    Frame *frame = nullptr;
    PageNum page_num = *(PageNum*)(data + (i + 1) * 4);
    if (page_num <= 0) {
      break;
    }
    rc = field_.table()->data_buffer_pool()->get_this_page(page_num, &frame);
    if (rc != RC::SUCCESS) {
      LOG_WARN("get page fail in get value");
      return RC::INTERNAL;
    }
    int len = std::min(length - offset, BP_PAGE_DATA_SIZE);
    memcpy(text_data + offset, frame->data(), len);
    offset += len;
  }
  value.set_type(CHARS);
  value.set_data(text_data, length);
  delete[] text_data;
  return RC::SUCCESS;
}

RC ValueExpr::get_value(const Tuple &tuple, Value &value, Trx *trx)
{
  value = value_;
  return RC::SUCCESS;
}

RC ValueListExpr::get_value(const Tuple &tuple, Value &value, Trx *trx)
{
  if (index_ >= values_.size()) {
    index_ = 0;
    return RC::RECORD_EOF;
  }
  value = values_[index_++];
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

RC CastExpr::get_value(const Tuple &tuple, Value &cell, Trx *trx)
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

RC SubqueryExpr::get_value(const Tuple &tuple, Value &value, Trx *trx)
{ 
  RC rc = RC::SUCCESS;
  if (logical_operator_ == nullptr && physical_operator_ == nullptr) {
    return RC::RECORD_EOF;
  }
  if (physical_operator_ == nullptr) {
    LOG_WARN("physical operator is null");
    return RC::INVALID_ARGUMENT;
  }

  trx_ = trx;

  if (!is_open_) {
    rc = open_physical_operator();
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to open physical operator. rc=%s", strrc(rc));
      return rc;
    }
  }

  // 查询子查询的结果，写入value中
  Tuple *non_const_ptr = const_cast<Tuple*>(&tuple);
  rc = physical_operator_->next(non_const_ptr);
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
  if (another_tuple->cell_num() > 1) {
    LOG_WARN("tuple cell count is not 1");
    return RC::INVALID_ARGUMENT;
  }
  if (another_tuple->cell_num() == 0) {
    value.set_null(true);
  } else {
    another_tuple->cell_at(0, value);
  }
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
  if(comp_ == CompOp::IS_NOT_ ) {
    if(left.get_null_or_() == false) {
      result = true;
    } else {
      result = false;
    }
    return rc;
  }
  if(comp_ == CompOp::IS_ ) {
    if(left.get_null_or_() == true || (right.get_null_or_() == true && left.get_null_or_() == right.get_null_or_())) {
      result = true;
    } else {
      result = false;
    }
    return rc;
  }
  if(left.get_null_or_() == true || right.get_null_or_() == true) {
    result = false;
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

RC ComparisonExpr::get_value(const Tuple &tuple, Value &value, Trx *trx)
{
  Value left_value;
  Value right_value;
  RC rc;

  if (left_->type() == ExprType::SUBQUERY && right_->type() == ExprType::SUBQUERY) {
    // 双边的值都只能有一个
    SubqueryExpr *left_subquery_expr;
    SubqueryExpr *right_subquery_expr;
    Value* left_sub_query_value;
    Value* right_sub_query_value;
    left_subquery_expr = static_cast<SubqueryExpr *>(left_.get());
    right_subquery_expr = static_cast<SubqueryExpr *>(right_.get());
    rc = left_->get_value(tuple, left_value); 
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
      return rc;
    }
    Value test;
    rc = left_->get_value(tuple, test);
    if (rc != RC::RECORD_EOF) {
      LOG_WARN("we only support 1 rows for subquery result rc=%s", strrc(rc));
      return rc;
    }
    rc = right_->get_value(tuple, right_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }
    rc = right_->get_value(tuple, test);
    if (rc != RC::RECORD_EOF) {
      LOG_WARN("we only support 1 rows for subquery result rc=%s", strrc(rc));
      return rc;
    }
    bool bool_value = false;
    rc = compare_value(left_value, right_value, bool_value);
    if (rc == RC::SUCCESS) {
      value.set_boolean(bool_value);
    }
    // WARN: 这里有一个非常大的问题: 在mysql中，是支持一行多个cells比较的，而这里如果有多个cells则会直接报错
  } else if (left_->type() == ExprType::SUBQUERY || right_->type() == ExprType::SUBQUERY) {
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
    while((rc = subquery_expr->get_value(tuple, *sub_query_value, trx)) == RC::SUCCESS) {
      if (sub_query_value->attr_type() == UNDEFINED && !sub_query_value->get_null_or_()) {
        rc = RC::RECORD_EOF; // maybe wrong
        break;
      }

      if (comp_ == CompOp::EXISTS_) {
        // 当comp_为EXISTS时，直接返回true
        value.set_boolean(true);
        break;
      } else if (comp_ == CompOp::NOT_EXISTS_) {
        value.set_boolean(false);
        break;
      } else if (comp_ != CompOp::IN_ && comp_ != CompOp::NOT_IN && comp_ != CompOp::EXISTS_ && comp_ != CompOp::NOT_EXISTS_) {
        // 当comp_不是IN、NOT_IN、EXISTS、NOT_EXISTS时，子查询的结果只能是一个值
        if (has_sub_queried_) {
          has_sub_queried_ = false; // 良好的习惯，重置
          rc = RC::INVALID_ARGUMENT;
          break;
        } else {
          has_sub_queried_ = true;
        }
      }

      rc = compare_value(left_value, right_value, bool_value);

      // 当CompOp不是NOT_IN时，只要找到一个满足条件的值就可以返回 (这句话是错误的)
      // 当CompOp是NOT_IN时，必须得遍历完所有的值才能知道
      // if (rc == RC::SUCCESS && comp_ != CompOp::NOT_IN && bool_value ) {
      //   value.set_boolean(bool_value);
      //   break;
      // } else if (rc == RC::SUCCESS && comp_ == CompOp::NOT_IN && !bool_value) {
      //   // 当CompOp==NOT_IN，且左右value相等（因为是NOTIN，所以bool_value前要加!），直接结束循环
      //   value.set_boolean(bool_value);
      //   break;
      // }
      // 当CompOp是IN时，只要找到一个满足条件的值就可以返回
      // 当CompOp是NOT_IN时，必须得遍历完所有的值才能知道
      if (rc == RC::SUCCESS && comp_ == CompOp::IN_ && bool_value ) {
        value.set_boolean(bool_value);
        break;
      } else if (rc == RC::SUCCESS && comp_ == CompOp::NOT_IN && !bool_value) {
        // 当CompOp==NOT_IN，且左右value相等（因为是NOTIN，所以bool_value前要加!），直接结束循环
        value.set_boolean(bool_value);
        break;
      }
      
    }

    if (rc == RC::INVALID_ARGUMENT) return rc;

    // EOF 判断
    if (rc == RC::RECORD_EOF) {
      if (comp_ == CompOp::NOT_IN || comp_ == CompOp::NOT_EXISTS_) {
        value.set_boolean(true);
        rc = RC::SUCCESS;
        return rc;
      } else if (comp_ == CompOp::IN_ || comp_ == CompOp::EXISTS_) {
        value.set_boolean(false);
        rc = RC::SUCCESS;
        return rc;
      }
    }
    if (rc != RC::SUCCESS && rc != RC::RECORD_EOF) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    }

    value.set_boolean(bool_value);

    // 关闭算子
    // 可优化 static_cast 潜在的开销
    if (subquery_expr->physical_operator() != nullptr) {
      rc = subquery_expr->close_physical_operator();
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to close physical operator. rc=%s", strrc(rc));
      }
    }

  } else if (left_->type() == ExprType::VALUES || right_->type() == ExprType::VALUES) {
    ValueListExpr *value_list_expr;
    Value* value_list_value;
    if (left_->type() == ExprType::VALUES) {
      value_list_expr = static_cast<ValueListExpr *>(left_.get());
      rc = right_->get_value(tuple, right_value); // 假设右边不是value list
      value_list_value = &left_value;
    } else {
      value_list_expr = static_cast<ValueListExpr *>(right_.get());
      rc = left_->get_value(tuple, left_value); // 假设左边不是value list
      value_list_value = &right_value;
    }
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }

    bool bool_value = false;

    // 循环next子查询的算子，直到找到一个满足条件的值
    while((rc = value_list_expr->get_value(tuple, *value_list_value)) == RC::SUCCESS) {
      if (value_list_value->attr_type() == UNDEFINED) {
        rc = RC::RECORD_EOF; // maybe wrong
        break;
      }

      if (comp_ == CompOp::EXISTS_) {
        // 当comp_为EXISTS时，直接返回true
        value.set_boolean(true);
        break;
      } else if (comp_ == CompOp::NOT_EXISTS_) {
        value.set_boolean(false);
        break;
      } else if (comp_ == CompOp::EQUAL_TO || comp_ == CompOp::NOT_EQUAL) {
        // 当comp_为=或者<>时，子查询的结果只能是一个值
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

    // EOF判断
    if (rc == RC::RECORD_EOF) {
      if (comp_ == CompOp::NOT_IN || comp_ == CompOp::NOT_EXISTS_) {
        value.set_boolean(true);
        rc = RC::SUCCESS;
        return rc;
      } else if (comp_ == CompOp::IN_ || comp_ == CompOp::EXISTS_) {
        value.set_boolean(false);
        rc = RC::SUCCESS;
        return rc;
      }
    }
    if (rc != RC::SUCCESS && rc != RC::RECORD_EOF) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    }

    value_list_expr->set_index(0); // 重置index

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
  if (rc == RC::RECORD_EOF) {
    rc = RC::SUCCESS;
  }
  has_sub_queried_ = false;
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
ConjunctionExpr::ConjunctionExpr(Type type, vector<unique_ptr<Expression>> &children)
    : conjunction_type_(type), children_(std::move(children))
{}

RC ConjunctionExpr::get_value(const Tuple &tuple, Value &value, Trx *trx)
{
  RC rc = RC::SUCCESS;
  if (children_.empty()) {
    value.set_boolean(true);
    return rc;
  }

  Value tmp_value;
  for (const unique_ptr<Expression> &expr : children_) {
    rc = expr->get_value(tuple, tmp_value, trx);
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

  if (left_value.get_null_or_() || right_value.get_null_or_()) {
    value.set_null(true);
    return rc;
  }

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
          // 除以0，设置为NULL
          value.set_null(true);
        } else {
          value.set_int(left_value.get_int() / right_value.get_int());
        }
      } else {
        if (right_value.get_float() > -EPSILON && right_value.get_float() < EPSILON) {
          // 除以0，设置为NULL
          value.set_null(true);
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

RC ArithmeticExpr::get_value(const Tuple &tuple, Value &value, Trx *trx)
{
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;

  if (left_ != nullptr) {
    rc = left_->get_value(tuple, left_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
      return rc;
    }
  }
  if (right_ != nullptr) {
    rc = right_->get_value(tuple, right_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }
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

AttrType AggreExpr::value_type() const {
  switch (type_) {
    case AggreType::CNT:
    case AggreType::CNTALL: {
      return INTS;
    }
    case AggreType::MIN:
    case AggreType::MAX:
    case AggreType::SUM: {
      return child_->value_type();
    }
    case AggreType::AVG: {
      return FLOATS;
    }
    default: {
      LOG_WARN("unsupported aggre type. %d", type_);
      return UNDEFINED;
    }
  }
}

RC AggreExpr::get_value(const Tuple &tuple, Value &value, Trx *trx) {
  RC rc = RC::SUCCESS;
  if (type_ == AggreType::CNTALL) {
    cnt_++;
    value.set_int(cnt_);
    value_.set_int(cnt_);
    return RC::SUCCESS;
  }
  rc = child_->get_value(tuple, value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of child expression. rc=%s", strrc(rc));
    return rc;
  }
  if (value.get_null_or_()) {
    value = value_;
    if (cnt_ == 0) {
      LOG_WARN("aggregate function has no value");
      value.set_null(true);
      return RC::SUCCESS;
    }
    if (type_ == AggreType::AVG) {
      float tmp = value.get_float() / (float)cnt_;
      value.set_float(tmp);
    }
    return RC::SUCCESS;
  }
  cnt_++;
  if (cnt_ == 1) {
    switch (type_) {
      case AggreType::CNT:
      case AggreType::CNTALL: {
        value_.set_int(1);
      } break;
      case AggreType::MAX:
      case AggreType::MIN: {
        switch (value.attr_type()) {
          case INTS: {
            value_.set_int(value.get_int());
          } break;
          case FLOATS: {
            value_.set_float(value.get_float());
          } break;
          case DATES: {
            value_.set_date(value.get_date());
          } break;
          case CHARS: {
            value_.set_string(value.get_string());
          } break;
          default: {
            LOG_WARN("unsupported attr type. %d", value.attr_type());
            return RC::INTERNAL;
          } break;
        }
      } break;
      case AggreType::AVG: {
        value_.set_float(value.get_float());
      } break;
      case AggreType::SUM: {
        switch (value.attr_type()) {
          case INTS: {
            value_.set_int(value.get_int());
          } break;
          case FLOATS: {
            value_.set_float(value.get_float());
          } break;
          default: {
            LOG_WARN("unsupported attr type. %d", value.attr_type());
            return RC::INTERNAL;
          } break;
        }
      } break;
      default: {
        LOG_WARN("unsupported aggre type. %d", type_);
        return RC::INTERNAL;
      } break;
    }
  } else {
    switch (type_) {
      case AggreType::CNT:
      case AggreType::CNTALL: {
        value_.set_int(cnt_);
      } break;
      case AggreType::MAX: {
        switch (value.attr_type()) {
          case INTS: {
            value_.set_int(std::max(value.get_int(), value_.get_int()));
          } break;
          case FLOATS: {
            value_.set_float(std::max(value.get_float(), value_.get_float()));
          } break;
          case DATES: {
            value_.set_date(date(std::max(value.get_date().get_date_value(), value_.get_date().get_date_value())));
          } break;
          case CHARS: {
            value_.set_string(std::max(value.get_string(), value_.get_string()));
          } break;
          default: {
            LOG_WARN("unsupported attr type. %d", value.attr_type());
            return RC::INTERNAL;
          } break;
        }
      } break;
      case AggreType::MIN: {
        switch (value.attr_type()) {
          case INTS: {
            value_.set_int(std::min(value.get_int(), value_.get_int()));
          } break;
          case FLOATS: {
            value_.set_float(std::min(value.get_float(), value_.get_float()));
          } break;
          case DATES: {
            value_.set_date(date(std::min(value.get_date().get_date_value(), value_.get_date().get_date_value())));
          } break;
          case CHARS: {
            value_.set_string(std::min(value.get_string(), value_.get_string()));
          } break;
          default: {
            LOG_WARN("unsupported attr type. %d", value.attr_type());
            return RC::INTERNAL;
          } break;
        }
      } break;
      case AggreType::AVG: {
        value_.set_float(value.get_float() + value_.get_float());
      } break;
      case AggreType::SUM: {
        switch (value.attr_type()) {
          case INTS: {
            value_.set_int(value.get_int() + value_.get_int());
          } break;
          case FLOATS: {
            value_.set_float(value.get_float() + value_.get_float());
          } break;
          default: {
            LOG_WARN("unsupported attr type. %d", value.attr_type());
            return RC::INTERNAL;
          } break;
        }
      } break;
      default: {
        LOG_WARN("unsupported aggre type. %d", type_);
        return RC::INTERNAL;
      } break;
    }
  }
  value = value_;
  if (type_ == AggreType::AVG) {
    float tmp = value.get_float() / (float)cnt_;
    value.set_float(tmp);
  }
  return RC::SUCCESS;
}

RC AggreExpr::try_get_value(Value &value) const {
  value = value_;
  if (cnt_ == 0) {
    if (type_ == AggreType::CNT || type_ == AggreType::CNTALL) {
      value.set_int(0);
    } else {
      value.set_null(true);
    }
    return RC::SUCCESS;
  }
  if (type_ == AggreType::AVG) {
    value.set_float(value.get_float() / cnt_);
  }
  return RC::SUCCESS;
}

AttrType FuncExpr::value_type() const {
  switch (type_) {
    case FuncType::LENGTH: {
      return INTS;
    }
    default: {
      LOG_WARN("unsupported func type. %d", type_);
      return UNDEFINED;
    }
  }
}

RC FuncExpr::get_value(const Tuple &tuple, Value &value, Trx *trx) {
  RC rc = RC::SUCCESS;
  Value child_value;
  rc = child_->get_value(tuple, child_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of child expression. rc=%s", strrc(rc));
    return rc;
  }
  switch (type_) {
    case FuncType::LENGTH: {
      if (child_value.attr_type() != CHARS) {
        LOG_WARN("unsupported attr type. %d", child_value.attr_type());
        return RC::INVALID_ARGUMENT;
      }
      value.set_int(child_value.get_string().length());
    } break;
    case FuncType::ROUND: {
      Value round_digits;
      if (child_value.attr_type() != INTS && child_value.attr_type() != FLOATS) {
        LOG_WARN("unsupported attr type. %d", child_value.attr_type());
        return RC::INVALID_ARGUMENT;
      }
      rc = param_->try_get_value(round_digits);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to get value of round digits expression. rc=%s", strrc(rc));
        return rc;
      }
      if (round_digits.attr_type() != INTS) {
        LOG_WARN("unsupported attr type. %d", round_digits.attr_type());
        return RC::INVALID_ARGUMENT;
      }
      int rd = round_digits.get_int();
      if (rd >= 0) {
        float num = child_value.get_float();
        float tmp = num * pow(10, rd);
        int tmp_int = (int)tmp;
        float tmp_float = (float)tmp_int;
        if (tmp - tmp_float >= 0.5) {
          tmp_int++;
        }
        tmp = (float)tmp_int;
        tmp /= pow(10, rd);
        value.set_float(tmp);
      } else {
        float num = child_value.get_float();
        float tmp = num / pow(10, -rd);
        int tmp_int = (int)tmp;
        float tmp_float = (float)tmp_int;
        if (tmp - tmp_float >= 0.5) {
          tmp_int++;
        }
        tmp = (float)tmp_int;
        tmp *= pow(10, -rd);
        value.set_float(tmp);
      }
    } break;
    case FuncType::DATE_FORMAT: {
      Value format;
      if (child_value.attr_type() != DATES) {
        LOG_WARN("unsupported attr type. %d", child_value.attr_type());
        return RC::INVALID_ARGUMENT;
      }
      rc = param_->try_get_value(format);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to get value of format expression. rc=%s", strrc(rc));
        return rc;
      }
      if (format.attr_type() != CHARS) {
        LOG_WARN("unsupported attr type. %d", format.attr_type());
        return RC::INVALID_ARGUMENT;
      }
      string fmt = format.get_string();
      date d = child_value.get_date();
      int idx = fmt.find("%Y");  // 四位数年份
      if (idx >= 0) {
        fmt.replace(idx, 2, std::to_string(d.get_year()));
      }
      idx = fmt.find("%y");  // 两位数年份
      if (idx >= 0) {
        fmt.replace(idx, 2, (d.get_year() % 100 < 10 ? "0" : "") + std::to_string(d.get_year() % 100));
      }
      idx = fmt.find("%M");  // 月份英文
      if (idx >= 0) {
        fmt.replace(idx, 2, d.get_month_english());
      }
      idx = fmt.find("%m");  // 月份 01-12
      if (idx >= 0) {
        fmt.replace(idx, 2, (d.get_month() < 10 ? "0" : "") + std::to_string(d.get_month()));
      }
      idx = fmt.find("%c");  // 月份 1-12
      if (idx >= 0) {
        fmt.replace(idx, 2, std::to_string(d.get_month()));
      }
      idx = fmt.find("%D");  // 月份英文
      if (idx >= 0) {
        fmt.replace(idx, 2, d.get_day_english());
      }
      idx = fmt.find("%d");  // 日期
      if (idx >= 0) {
        fmt.replace(idx, 2, (d.get_day() < 10 ? "0" : "") + std::to_string(d.get_day()));
      }
      idx = fmt.find("%e");  // 日期
      if (idx >= 0) {
        fmt.replace(idx, 2, std::to_string(d.get_day()));
      }
      value.set_string(fmt);
    } break;
    default: {
      LOG_WARN("unsupported func type. %d", type_);
      return RC::INTERNAL;
    } break;
  }
  return RC::SUCCESS;
}

RC FuncExpr::try_get_value(Value &value) const {
  RC rc = RC::SUCCESS;
  Value child_value;
  rc = child_->try_get_value(child_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of child expression. rc=%s", strrc(rc));
    return rc;
  }
  if (child_value.attr_type() != CHARS) {
    LOG_WARN("unsupported attr type. %d", child_value.attr_type());
    return RC::INVALID_ARGUMENT;
  }
  switch (type_) {
    case FuncType::LENGTH: {
      value.set_int(child_value.get_string().length());
    } break;
    default: {
      LOG_WARN("unsupported func type. %d", type_);
      return RC::INTERNAL;
    } break;
  }
  return RC::SUCCESS;
}