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
// Created by Wangyunlai on 2021/5/14.
//

#pragma once

#include <memory>
#include <vector>
#include <string>

#include "common/log/log.h"
#include "common/rc.h"
#include "sql/expr/tuple_cell.h"
#include "sql/parser/parse.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
#include "sql/expr/expression.h"
#include "storage/record/record.h"

class Table;


enum class TupleType 
{
  BASIC,
  ROW,
  PROJECT,
  EXPRESSION,
  VALUE_LIST,
  JOINED,
  NONE,
};

/**
 * @defgroup Tuple
 * @brief Tuple 元组，表示一行数据，当前返回客户端时使用
 * @details 
 * tuple是一种可以嵌套的数据结构。
 * 比如select t1.a+t2.b from t1, t2;
 * 需要使用下面的结构表示：
 * @code {.cpp}
 *  Project(t1.a+t2.b)
 *        |
 *      Joined
 *      /     \
 *   Row(t1) Row(t2)
 * @endcode
 * 
 */

/**
 * @brief 元组的结构，包含哪些字段(这里成为Cell)，每个字段的说明
 * @ingroup Tuple
 */
class TupleSchema 
{
public:
  void append_cell(const TupleCellSpec &cell)
  {
    cells_.push_back(cell);
  }
  void append_cell(const char *table, const char *field)
  {
    append_cell(TupleCellSpec(table, field));
  }
  void append_cell(const char *alias)
  {
    append_cell(TupleCellSpec(alias));
  }
  int cell_num() const
  {
    return static_cast<int>(cells_.size());
  }
  const TupleCellSpec &cell_at(int i) const
  {
    return cells_[i];
  }

private:
  std::vector<TupleCellSpec> cells_;
};

/**
 * @brief 元组的抽象描述
 * @ingroup Tuple
 */
class Tuple 
{
public:
  Tuple() = default;
  virtual ~Tuple() = default;

  /**
   * @brief 获取元组中的Cell的个数
   * @details 个数应该与tuple_schema一致
   */
  virtual int cell_num() const = 0;

  // 别用我，用cell_num()
  virtual int all_c_num()
  {
    return -1;
  }

  /**
   * @brief 获取指定位置的Cell
   * 
   * @param index 位置
   * @param[out] cell  返回的Cell
   */
  virtual RC cell_at(int index, Value &cell) const = 0;

  // 别用我，用cell_at()
  virtual RC all_c_at(int index, Value &cell)
  {
    return RC::NOTFOUND;
  }

  // 不要用我
  virtual std::vector<TupleCellSpec *> &speces()
  {
    static std::vector<TupleCellSpec *> empty;
    return empty;
  }

  virtual std::vector<TupleCellSpec *> &all_speces()
  {
    static std::vector<TupleCellSpec *> empty;
    return empty;
  }

  virtual TupleType type() const
  {
    return TupleType::BASIC;
  }

  /**
   * @brief 根据cell的描述，获取cell的值
   * 
   * @param spec cell的描述
   * @param[out] cell 返回的cell
   */
  virtual RC find_cell(const TupleCellSpec &spec, Value &cell) const = 0;

  virtual std::string to_string() const
  {
    std::string str;
    const int cell_num = this->cell_num();
    for (int i = 0; i < cell_num - 1; i++) {
      Value cell;
      cell_at(i, cell);
      str += cell.to_string();
      str += ", ";
    }

    if (cell_num > 0) {
      Value cell;
      cell_at(cell_num - 1, cell);
      str += cell.to_string();
    }
    return str;
  }
};

/**
 * @brief 一行数据的元组
 * @ingroup Tuple
 * @details 直接就是获取表中的一条记录
 */
class RowTuple : public Tuple 
{
public:
  RowTuple() = default;
  virtual ~RowTuple()
  {
    for (FieldExpr *spec : speces_) {
      delete spec;
    }
    speces_.clear();
  }

  void set_record(Record *record)
  {
    this->record_ = record;
  }

  TupleType type() const override
  {
    return TupleType::ROW;
  }

  void set_schema(Table *table, const std::vector<FieldMeta> *fields)
  {
    table_ = table;
    speces_.clear();
    this->speces_.reserve(fields->size());
    for (const FieldMeta &field : *fields) {
      speces_.push_back(new FieldExpr(table, &field));
    }
  }

  int cell_num() const override
  {
    return speces_.size();
  }

  RC cell_at(int index, Value &cell) const override
  {
    if (index < 0 || index >= static_cast<int>(speces_.size())) {
      LOG_WARN("invalid argument. index=%d", index);
      return RC::INVALID_ARGUMENT;
    }

    FieldExpr *field_expr = speces_[index];
    const FieldMeta *field_meta = field_expr->field().meta();
    cell.set_type(field_meta->type() == TEXTS ? CHARS : field_meta->type());
    //this->record_,判断第index个字段是否为为1，如果为1则将cell.is_null_置为true
    if(this->record_->data()[index] == 1)
    {
      cell.set_null(true);
    }
    else
    {
      cell.set_data(this->record_->data() + field_meta->offset(), field_meta->len());
    }
    
    return RC::SUCCESS;
  }

  RC find_cell(const TupleCellSpec &spec, Value &cell) const override
  {
    const char *table_name = spec.table_name();
    const char *field_name = spec.field_name();
    if (0 != strcmp(table_name, table_->name())) {
      return RC::NOTFOUND;
    }

    for (size_t i = 0; i < speces_.size(); ++i) {
      const FieldExpr *field_expr = speces_[i];
      const Field &field = field_expr->field();
      if (0 == strcmp(field_name, field.field_name())) {
        return cell_at(i, cell);
      }
    }
    return RC::NOTFOUND;
  }

#if 0
  RC cell_spec_at(int index, const TupleCellSpec *&spec) const override
  {
    if (index < 0 || index >= static_cast<int>(speces_.size())) {
      LOG_WARN("invalid argument. index=%d", index);
      return RC::INVALID_ARGUMENT;
    }
    spec = speces_[index];
    return RC::SUCCESS;
  }
#endif

  Record &record()
  {
    return *record_;
  }

  const Record &record() const
  {
    return *record_;
  }

private:
  Record *record_ = nullptr;  // tuple 的数据
  const Table *table_ = nullptr;  // tuple 所在的表
  std::vector<FieldExpr *> speces_;  // 每个 cell 对应的字段，可以是一个表达式
};

/**
 * @brief 从一行数据中，选择部分字段组成的元组，也就是投影操作
 * @ingroup Tuple
 * @details 一般在select语句中使用。
 * 投影也可以是很复杂的操作，比如某些字段需要做类型转换、重命名、表达式运算、函数计算等。
 * 当前的实现是比较简单的，只是选择部分字段，不做任何其他操作。
 */
class ProjectTuple : public Tuple 
{
public:
  ProjectTuple() = default;
  ~ProjectTuple() override = default;

  void set_tuple(Tuple *tuple)
  {
    this->tuple_ = tuple;
  }

  TupleType type() const override
  {
    return TupleType::PROJECT;
  }

  void add_expr(std::unique_ptr<Expression> &&expr)
  {
    exprs_.push_back(std::move(expr));
  }
  int cell_num() const override
  {
    return exprs_.size();
  }

  RC cell_at(int index, Value &cell) const override
  {
    if (index < 0 || index >= static_cast<int>(exprs_.size())) {
      return RC::INTERNAL;
    }
    if (tuple_ == nullptr) {
      return RC::INTERNAL;
    }
    Expression *expr = exprs_[index].get();
    return expr->get_value(*tuple_, cell);
  }

  RC try_cell_at(int index, Value &cell) const
  {
    if (index < 0 || index >= static_cast<int>(exprs_.size())) {
      return RC::INTERNAL;
    }
    Expression *expr = exprs_[index].get();
    return expr->try_get_value(cell);
  }

  RC find_cell(const TupleCellSpec &spec, Value &cell) const override
  {
    for (const std::unique_ptr<Expression> &expr : exprs_) {
      if (0 == strcmp(spec.alias(), expr->name().c_str())) {
        return expr->get_value(*tuple_, cell);
      }
    }
    return RC::NOTFOUND;
  }

  int all_c_num() override
  {
    return tuple_->cell_num();
  }

  RC all_c_at(int index, Value &cell) override
  {
    return tuple_->cell_at(index, cell);
  }

#if 0
  RC cell_spec_at(int index, const TupleCellSpec *&spec) const override
  {
    if (index < 0 || index >= static_cast<int>(speces_.size())) {
      return RC::NOTFOUND;
    }
    spec = speces_[index];
    return RC::SUCCESS;
  }
#endif
private:
  std::vector<std::unique_ptr<Expression>> exprs_;  // 选择的字段
  Tuple *tuple_ = nullptr;  // 指向一个其他的 tuple 的派生类
};

class ExpressionTuple : public Tuple 
{
public:
  ExpressionTuple(std::vector<std::unique_ptr<Expression>> &expressions)
    : expressions_(expressions)
  {
  }
  
  virtual ~ExpressionTuple()
  {
  }

  int cell_num() const override
  {
    return expressions_.size();
  }

  RC cell_at(int index, Value &cell) const override
  {
    if (index < 0 || index >= static_cast<int>(expressions_.size())) {
      return RC::INTERNAL;
    }

    const Expression *expr = expressions_[index].get();
    return expr->try_get_value(cell);
  }

  TupleType type() const override
  {
    return TupleType::EXPRESSION;
  }

  RC find_cell(const TupleCellSpec &spec, Value &cell) const override
  {
    for (const std::unique_ptr<Expression> &expr : expressions_) {
      if (0 == strcmp(spec.alias(), expr->name().c_str())) {
        return expr->try_get_value(cell);
      }
    }
    return RC::NOTFOUND;
  }


private:
  const std::vector<std::unique_ptr<Expression>> &expressions_;
};

/**
 * @brief 一些常量值组成的Tuple
 * @ingroup Tuple
 */
class ValueListTuple : public Tuple 
{
public:
  ValueListTuple() = default;
  virtual ~ValueListTuple() = default;

  void set_cells(const std::vector<Value> &cells)
  {
    cells_ = cells;
  }

  virtual int cell_num() const override
  {
    return static_cast<int>(cells_.size());
  }

  virtual RC cell_at(int index, Value &cell) const override
  {
    if (index < 0 || index >= cell_num()) {
      return RC::NOTFOUND;
    }

    cell = cells_[index];
    return RC::SUCCESS;
  }

  TupleType type() const override
  {
    return TupleType::VALUE_LIST;
  }

  virtual RC find_cell(const TupleCellSpec &spec, Value &cell) const override
  {
    return RC::INTERNAL;
  }

private:
  std::vector<Value> cells_;
};


class SortProjectTuple : public Tuple
{
  public:
  SortProjectTuple() = default;
  virtual ~SortProjectTuple()
  {
  }

  void set_tuple(ValueListTuple *tuple)
  {
    this->tuple_ = tuple;
  }

  TupleType type() const override
  {
    return TupleType::PROJECT;
  }

  int cell_num() const override
  {
    return position.size();
  }

  RC cell_at(int index, Value &cell) const override
  {
    if (index < 0 || index >= position.size()) {
      return RC::INTERNAL;
    }
    if (tuple_ == nullptr) {
      return RC::INTERNAL;
    }

    const int pos = position[index];
    return tuple_->cell_at(pos, cell);
  }

  RC find_cell(const TupleCellSpec &spec, Value &cell) const override
  {
    return tuple_->find_cell(spec, cell);
  }

  void add_position(int pos)
  {
    position.push_back(pos);
  }

private:
  std::vector<int> position;
  ValueListTuple *tuple_ = nullptr; 

};



/**
 * @brief 将两个tuple合并为一个tuple
 * @ingroup Tuple
 * @details 在join算子中使用
 */
class JoinedTuple : public Tuple 
{
public:
  JoinedTuple() = default;
  virtual ~JoinedTuple() = default;

  void set_left(Tuple *left)
  {
    left_ = left;
  }
  void set_right(Tuple *right)
  {
    right_ = right;
  }

  int cell_num() const override
  {
    return left_->cell_num() + right_->cell_num();
  }

  RC cell_at(int index, Value &value) const override
  {
    const int left_cell_num = left_->cell_num();
    if (index >= 0 && index < left_cell_num) {
      return left_->cell_at(index, value);
    }

    if (index >= left_cell_num && index < left_cell_num + right_->cell_num()) {
      return right_->cell_at(index - left_cell_num, value);
    }

    return RC::NOTFOUND;
  }

  TupleType type() const override
  {
    return TupleType::JOINED;
  }

  RC find_cell(const TupleCellSpec &spec, Value &value) const override
  {
    RC rc = left_->find_cell(spec, value);
    if (rc == RC::SUCCESS || rc != RC::NOTFOUND) {
      return rc;
    }

    return right_->find_cell(spec, value);
  }

private:
  Tuple *left_ = nullptr;
  Tuple *right_ = nullptr;
};


// 占位符
class NoneTuple : public Tuple {
public: 
  NoneTuple() = default;
  virtual ~NoneTuple() = default;
  int cell_num() const override {
    return 0;
  }
  TupleType type() const override {
    return TupleType::NONE;
  }
  RC cell_at(int index, Value &value) const override {
    return RC::NOTFOUND;
  }
  RC find_cell(const TupleCellSpec &spec, Value &value) const override {
    return RC::NOTFOUND;
  }
};