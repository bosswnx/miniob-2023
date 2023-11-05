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

#pragma once

#include <string.h>
#include <memory>
#include <string>

#include "sql/parser/parse_defs.h"
#include "storage/field/field.h"
#include "sql/parser/value.h"
#include "common/log/log.h"


class LogicalOperator;
class PhysicalOperator;
class Trx;

class Tuple;
/**
 * @defgroup Expression
 * @brief 表达式
 */

/**
 * @brief 表达式类型
 * @ingroup Expression
 */
enum class ExprType 
{
  NONE,
  STAR,         ///< 星号，表示所有字段
  FIELD,        ///< 字段。在实际执行时，根据行数据内容提取对应字段的值
  VALUE,        ///< 常量值
  VALUES,       ///< 常量值列表
  CAST,         ///< 需要做类型转换的表达式
  COMPARISON,   ///< 需要做比较的表达式
  CONJUNCTION,  ///< 多个表达式使用同一种关系(AND或OR)来联结
  ARITHMETIC,   ///< 算术运算
  SUBQUERY,     ///< 子查询
  RELATTR,      ///< 字段列表
  AGGREGATION,  ///< 聚合函数
  FUNCTION,     ///< 函数
};

enum class FuncType {
  NONE = 0,
  LENGTH,
  ROUND,
};

/**
 * @brief 表达式的抽象描述
 * @ingroup Expression
 * @details 在SQL的元素中，任何需要得出值的元素都可以使用表达式来描述
 * 比如获取某个字段的值、比较运算、类型转换
 * 当然还有一些当前没有实现的表达式，比如算术运算。
 *
 * 通常表达式的值，是在真实的算子运算过程中，拿到具体的tuple后
 * 才能计算出来真实的值。但是有些表达式可能就表示某一个固定的
 * 值，比如ValueExpr。
 */
class Expression 
{
public:
  Expression() = default;
  virtual ~Expression() = default;

  /**
   * @brief 根据具体的tuple，来计算当前表达式的值。tuple有可能是一个具体某个表的行数据
   */
  virtual RC get_value(const Tuple &tuple, Value &value, Trx *trx = nullptr) = 0;

  /**
   * @brief 在没有实际运行的情况下，也就是无法获取tuple的情况下，尝试获取表达式的值
   * @details 有些表达式的值是固定的，比如ValueExpr，这种情况下可以直接获取值
   */
  virtual RC try_get_value(Value &value) const
  {
    return RC::UNIMPLENMENT;
  }

  /**
   * @brief 表达式的类型
   * 可以根据表达式类型来转换为具体的子类
   */
  virtual ExprType type() const = 0;

  /**
   * @brief 表达式值的类型
   * @details 一个表达式运算出结果后，只有一个值
   */
  virtual AttrType value_type() const = 0;

  /**
   * @brief 表达式的名字，比如是字段名称，或者用户在执行SQL语句时输入的内容
   */
  virtual std::string name() const { return name_; }
  virtual void set_name(std::string name) { name_ = name; }

private:
  std::string  name_;
};


/**
 * @brief 子查询表达式
*/
class SubqueryExpr : public Expression
{
public:
  SubqueryExpr(AttrType attr_type, const char *table_name, const char *field_name, std::unique_ptr<LogicalOperator> logical_operator);
  virtual ~SubqueryExpr() = default;

  ExprType type() const override { return ExprType::SUBQUERY; }
  AttrType value_type() const override { return attr_type_; }

  RC get_value(const Tuple &tuple, Value &value, Trx *trx = nullptr) override;

  std::unique_ptr<LogicalOperator> &logical_operator() { return logical_operator_; }

  std::unique_ptr<PhysicalOperator> &physical_operator() { return physical_operator_; }

  RC set_physical_operator(std::unique_ptr<PhysicalOperator> physical_operator);

  RC set_trx(Trx *trx);

  RC open_physical_operator();

  RC close_physical_operator();


private:
  AttrType attr_type_;
  const char *table_name_;
  const char *field_name_;
  std::unique_ptr<LogicalOperator> logical_operator_;
  std::unique_ptr<PhysicalOperator> physical_operator_;
  Trx *trx_;
  bool is_open_ = false;
};


/**
 * @brief 字段表达式
 * @ingroup Expression
 */
class FieldExpr : public Expression 
{
public:
  FieldExpr() = default;
  FieldExpr(const Table *table, const FieldMeta *field) : field_(table, field)
  {}
  FieldExpr(const Field &field) : field_(field)
  {}

  virtual ~FieldExpr() = default;

  ExprType type() const override { return ExprType::FIELD; }
  AttrType value_type() const override { return field_.attr_type(); }

  Field &field() { return field_; }

  const Field &field() const { return field_; }

  const char *table_name() const { return field_.table_name(); }

  const char *field_name() const { return field_.field_name(); }

  RC get_value(const Tuple &tuple, Value &value, Trx *trx = nullptr) override;

private:
  Field field_;
};

/**
 * @brief 字段列表表达式
 * @ingroup Expression
 */
class RelAttrExpr : public Expression
{
public:
  RelAttrExpr() = default;
  RelAttrExpr(const char *table_name, const char *field_name) : table_name_(table_name), field_name_(field_name) {}
  RelAttrExpr(const std::string &table_name, const std::string &field_name) : table_name_(table_name), field_name_(field_name) {}

  virtual ~RelAttrExpr() = default;

  ExprType type() const override { return ExprType::RELATTR; }
  AttrType value_type() const override { return UNDEFINED; }

  RC get_value(const Tuple &tuple, Value &value, Trx *trx = nullptr) override { return RC::UNIMPLENMENT; }
  RC try_get_value(Value &value) const override { return RC::UNIMPLENMENT; }

  const string &table_name() const { return table_name_; }
  const string &field_name() const { return field_name_; }

  void set_table_name(const string &table_name) { table_name_ = table_name; }
  void set_field_name(const string &field_name) { field_name_ = field_name; }

  void set_is_main_relation(bool is_main_relation) { is_main_relation_ = is_main_relation; }
  const bool is_main_relation() const { return is_main_relation_; }

private:
  bool is_main_relation_ = false;
  std::string table_name_;
  std::string field_name_;
};

/**
 * @brief 常量值表达式
 * @ingroup Expression
 */
class ValueExpr : public Expression 
{
public:
  ValueExpr() = default;
  explicit ValueExpr(const Value &value) : value_(value)
  {}

  virtual ~ValueExpr() = default;

  RC get_value(const Tuple &tuple, Value &value, Trx *trx = nullptr) override;
  RC try_get_value(Value &value) const override { value = value_; return RC::SUCCESS; }

  ExprType type() const override { return ExprType::VALUE; }

  AttrType value_type() const override { return value_.attr_type(); }

  void get_value(Value &value) const { value = value_; }

  const Value &get_value() const { return value_; }

private:
  Value value_;
};

/**
 * @brief 常量值列表表达式
 * @ingroup Expression
 */
class ValueListExpr : public Expression
{
public:
  ValueListExpr() = default;
  explicit ValueListExpr(const std::vector<Value> &values) : values_(values)
  {}

  virtual ~ValueListExpr() = default;

  RC get_value(const Tuple &tuple, Value &value, Trx *trx = nullptr) override;
  RC try_get_value(Value &value) const override { value = values_[0]; return RC::SUCCESS; }

  ExprType type() const override { return ExprType::VALUES; }

  AttrType value_type() const override { return values_[0].attr_type(); }

  void set_index(int index) { index_ = index; }

  const std::vector<Value> &get_values() const { return values_; }

private:
  std::vector<Value> values_;
  int index_ = 0;
};
/**
 * @brief 类型转换表达式
 * @ingroup Expression
 */
class CastExpr : public Expression 
{
public:
  CastExpr(std::unique_ptr<Expression> child, AttrType cast_type);
  virtual ~CastExpr();

  ExprType type() const override
  {
    return ExprType::CAST;
  }
  RC get_value(const Tuple &tuple, Value &value, Trx *trx = nullptr) override;

  RC try_get_value(Value &value) const override;

  AttrType value_type() const override { return cast_type_; }

  std::unique_ptr<Expression> &child() { return child_; }

private:
  RC cast(const Value &value, Value &cast_value) const;

private:
  std::unique_ptr<Expression> child_;  ///< 从这个表达式转换
  AttrType cast_type_;  ///< 想要转换成这个类型
};

/**
 * @brief 比较表达式
 * @ingroup Expression
 */
class ComparisonExpr : public Expression 
{
public:
  ComparisonExpr(CompOp comp, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);
  virtual ~ComparisonExpr();

  ExprType type() const override { return ExprType::COMPARISON; }

  RC get_value(const Tuple &tuple, Value &value, Trx *trx = nullptr) override;

  AttrType value_type() const override { return BOOLEANS; }

  CompOp comp() const { return comp_; }

  std::unique_ptr<Expression> &left()  { return left_;  }
  std::unique_ptr<Expression> &right() { return right_; }

  /**
   * 尝试在没有tuple的情况下获取当前表达式的值
   * 在优化的时候，可能会使用到
   */
  RC try_get_value(Value &value) const override;

  /**
   * compare the two tuple cells
   * @param value the result of comparison
   */
  RC compare_value(const Value &left, const Value &right, bool &value) const;

  
private:
  CompOp comp_;
  std::unique_ptr<Expression> left_;
  std::unique_ptr<Expression> right_;
  // 用于子查询与其他condition的“=”比较。这种情况下，如果为true则报错。只会统计“=”的情况。
  bool has_sub_queried_ = false;
};

/**
 * @brief 联结表达式
 * @ingroup Expression
 * 多个表达式使用同一种关系(AND或OR)来联结
 * 当前miniob仅有AND操作
 */
class ConjunctionExpr : public Expression 
{
public:
  enum class Type {
    AND,
    OR,
  };

public:
  ConjunctionExpr(Type type, std::vector<std::unique_ptr<Expression>> &children);
  virtual ~ConjunctionExpr() = default;

  ExprType type() const override { return ExprType::CONJUNCTION; }

  AttrType value_type() const override { return BOOLEANS; }

  RC get_value(const Tuple &tuple, Value &value, Trx *trx = nullptr) override;

  Type conjunction_type() const { return conjunction_type_; }

  std::vector<std::unique_ptr<Expression>> &children() { return children_; }

private:
  Type conjunction_type_;
  std::vector<std::unique_ptr<Expression>> children_;
};

/**
 * @brief 算术表达式
 * @ingroup Expression
 */
class ArithmeticExpr : public Expression 
{
public:
  enum class Type {
    ADD,
    SUB,
    MUL,
    DIV,
    NEGATIVE,
  };

public:
  ArithmeticExpr(Type type, Expression *left, Expression *right);
  ArithmeticExpr(Type type, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);
  virtual ~ArithmeticExpr() = default;

  ExprType type() const override { return ExprType::ARITHMETIC; }

  AttrType value_type() const override;

  RC get_value(const Tuple &tuple, Value &value, Trx *trx = nullptr) override;
  RC try_get_value(Value &value) const override;

  Type arithmetic_type() const { return arithmetic_type_; }

  std::unique_ptr<Expression> &left() { return left_; }
  std::unique_ptr<Expression> &right() { return right_; }

private:
  RC calc_value(const Value &left_value, const Value &right_value, Value &value) const;
  
private:
  Type arithmetic_type_;
  std::unique_ptr<Expression> left_;
  std::unique_ptr<Expression> right_;
};

/**
 * @brief 聚合函数表达式
 * @ingroup Expression
 */
class AggreExpr : public Expression 
{
public:
  AggreExpr(AggreType type, Expression *child): type_(type), child_(child) {}
  AggreExpr(AggreType type, std::unique_ptr<Expression> child): type_(type), child_(std::move(child)) {}
  virtual ~AggreExpr() = default;

  ExprType type() const override { return ExprType::AGGREGATION; }

  AttrType value_type() const override;

  RC get_value(const Tuple &tuple, Value &value, Trx *trx = nullptr) override;
  RC try_get_value(Value &value) const override;

  AggreType agg_type() const { return type_; }
  void set_agg_type(AggreType type) { type_ = type; }

  std::unique_ptr<Expression> &child() { return child_; }

private:
  AggreType type_;
  Value value_;
  int cnt_ = 0;
  std::unique_ptr<Expression> child_;
};
  
/**
 * @brief 函数表达式
 * @ingroup Expression
 */
class FuncExpr : public Expression 
{
public:
  FuncExpr(FuncType type, std::unique_ptr<Expression> child): type_(type), child_(std::move(child)) {};
  FuncExpr(FuncType type, Expression *child): type_(type), child_(child) {};
  virtual ~FuncExpr() = default;

  ExprType type() const override { return ExprType::FUNCTION; }

  AttrType value_type() const override;

  RC get_value(const Tuple &tuple, Value &value, Trx *trx = nullptr) override;
  RC try_get_value(Value &value) const override;

  RC get_round_digits(int &round_digits, Trx *trx = nullptr);
  void set_round_digits(std::unique_ptr<Expression> round_digits) { 
    round_digits_ = std::move(round_digits); 
  }
  void set_round_digits(Expression *round_digits) { 
    round_digits_.reset(round_digits); 
  }

  FuncType func_type() const { return type_; }

  std::unique_ptr<Expression> &child() { return child_; }

private:
  FuncType type_;
  std::unique_ptr<Expression> round_digits_;
  std::unique_ptr<Expression> child_;
};