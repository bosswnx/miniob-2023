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
// Created by Wangyunlai on 2022/6/5.
//

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "common/rc.h"
#include "sql/expr/expression.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
#include "sql/stmt/stmt.h"
#include "storage/field/field.h"

class FieldMeta;
class FilterStmt;
class Db;
class Table;

/**
 * @brief 表示select语句
 * @ingroup Statement
 */
class SelectStmt : public Stmt 
{
public:
  SelectStmt() = default;
  ~SelectStmt() override;

  StmtType type() const override
  {
    return StmtType::SELECT;
  }

public:
  static RC create(Db *db, SelectSqlNode &select_sql, Stmt *&stmt, 
      std::shared_ptr<std::unordered_map<string,string>> name2alias = nullptr,
      std::shared_ptr<std::unordered_map<string,string>> alias2name = nullptr,
      std::shared_ptr<std::vector<string>> main_relation_names = nullptr);

public:
  const std::vector<Table *> &tables() const
  {
    return tables_;
  }
  std::vector<std::unique_ptr<Expression>> &query_exprs()
  {
    return query_exprs_;
  }
  const std::vector<Field> &query_fields() const
  {
    return query_fields_;
  }
  const std::vector<std::string> &query_aliases() const
  {
    return query_aliases_;
  }
  FilterStmt *filter_stmt() const
  {
    return filter_stmt_;
  }
  bool is_aggre() const { return is_aggre_; }
  const std::vector<Field> &order_by_fields() const { return order_by_fields_; }

  const std::vector<Field> &group_by_fields() const { return group_by_fields_; }

  static RC get_table_and_field(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
    const RelAttrExpr *attr, Table *&table, const FieldMeta *&field);

  static RC make_field_expr(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
    std::unique_ptr<Expression> &expr, std::vector<Field> &query_fields, std::shared_ptr<std::unordered_map<string,string>> alias2name);

  static RC check_have_aggre(const std::unique_ptr<Expression> &expr, bool &is_aggre, bool &is_attr);

  static RC check_parent_relation(Expression *expr, 
      std::shared_ptr<std::unordered_map<string, string>> parent_alias2name, 
      const std::vector<string> &main_relation_names, bool &is_parent_relation);

  static RC convert_alias_to_name(Expression *expr, 
      std::shared_ptr<std::unordered_map<string, string>> alias2name);

  static RC check_have_relattr(Expression *expr, bool &have_relattr);

  // get query field by expr。
  // 不维护偏移量和index
  // 此方法用于create table select
  std::vector<FieldMeta> true_query_fields() {
    std::vector<FieldMeta> ret;
    
    for (int i=0; i<query_exprs_.size(); ++i) {
      // 类型转换，分情况讨论。有 FieldExpr，有 AggreExpr，有 ArithmeticExpr
      if (query_exprs_[i]->type() == ExprType::FIELD) {
        FieldExpr *field_expr = static_cast<FieldExpr *>(query_exprs_[i].get());
        ret.push_back(*field_expr->field().meta());
      } else if (query_exprs_[i]->type() == ExprType::ARITHMETIC) {
        ArithmeticExpr *arithmetic_expr = static_cast<ArithmeticExpr *>(query_exprs_[i].get());
        std::string name = arithmetic_expr->name();
        AttrType type = arithmetic_expr->value_type();
        int len;
        switch (type) {
          case AttrType::INTS:
            len = sizeof(int);
            break;
          case AttrType::FLOATS:
            len = sizeof(float);
            break;
          case AttrType::CHARS:
            len = 1024;
            break;
          default:
            return ret;
        }
        FieldMeta field_meta;
        field_meta.init(name.c_str(), type, 0, len, true, true, 0);
        ret.push_back(field_meta);
      } else if (query_exprs_[i]->type() == ExprType::VALUE) {
        ValueExpr *value_expr = static_cast<ValueExpr *>(query_exprs_[i].get());
        std::string name = value_expr->name();
        AttrType type = value_expr->value_type();
        Value value = value_expr->get_value();
        FieldMeta field_meta;
        field_meta.init(name.c_str(), type, 0, value.length(), true, value.get_null_or_(), 0);
        ret.push_back(field_meta);
      }
    }
    return ret;
  }

private:
  std::vector<std::unique_ptr<Expression>> query_exprs_;
  std::vector<Field> query_fields_;
  std::vector<std::string> query_aliases_;
  std::vector<Table *> tables_;
  bool is_aggre_;
  FilterStmt *filter_stmt_ = nullptr;
  std::vector<Field> order_by_fields_;
  std::vector<Field> group_by_fields_;
};
