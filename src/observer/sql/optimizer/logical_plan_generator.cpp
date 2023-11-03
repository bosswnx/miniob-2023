/* Copyright (c) 2023 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2023/08/16.
//

#include "sql/optimizer/logical_plan_generator.h"

#include "sql/operator/aggre_logical_operator.h"
#include "sql/operator/logical_operator.h"
#include "sql/operator/calc_logical_operator.h"
#include "sql/operator/sort_logical_operator.h"
#include "sql/operator/project_logical_operator.h"
#include "sql/operator/predicate_logical_operator.h"
#include "sql/operator/table_get_logical_operator.h"
#include "sql/operator/insert_logical_operator.h"
#include "sql/operator/delete_logical_operator.h"
#include "sql/operator/update_logical_operator.h"
#include "sql/operator/join_logical_operator.h"
#include "sql/operator/project_logical_operator.h"
#include "sql/operator/explain_logical_operator.h"

#include "sql/stmt/stmt.h"
#include "sql/stmt/calc_stmt.h"
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/insert_stmt.h"
#include "sql/stmt/delete_stmt.h"
#include "sql/stmt/update_stmt.h"
#include "sql/stmt/explain_stmt.h"
#include <memory>
#include <utility>
#include <vector>

#include "sql/expr/expression.h"
#include "storage/table/table.h"


using namespace std;

RC LogicalPlanGenerator::create(Stmt *stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  RC rc = RC::SUCCESS;
  switch (stmt->type()) {
    case StmtType::CALC: {
      CalcStmt *calc_stmt = static_cast<CalcStmt *>(stmt);
      rc = create_plan(calc_stmt, logical_operator);
    } break;

    case StmtType::SELECT: {
      SelectStmt *select_stmt = static_cast<SelectStmt *>(stmt);
      rc = create_plan(select_stmt, logical_operator);
    } break;

    case StmtType::INSERT: {
      InsertStmt *insert_stmt = static_cast<InsertStmt *>(stmt);
      rc = create_plan(insert_stmt, logical_operator);
    } break;

    case StmtType::UPDATE: {
      UpdateStmt *update_stmt = static_cast<UpdateStmt *>(stmt);
      rc = create_plan(update_stmt, logical_operator);
    } break;

    case StmtType::DELETE: {
      DeleteStmt *delete_stmt = static_cast<DeleteStmt *>(stmt);
      rc = create_plan(delete_stmt, logical_operator);
    } break;

    case StmtType::EXPLAIN: {
      ExplainStmt *explain_stmt = static_cast<ExplainStmt *>(stmt);
      rc = create_plan(explain_stmt, logical_operator);
    } break;
    default: {
      rc = RC::UNIMPLENMENT;
    }
  }
  return rc;
}

RC LogicalPlanGenerator::create_plan(CalcStmt *calc_stmt, std::unique_ptr<LogicalOperator> &logical_operator)
{
  logical_operator.reset(new CalcLogicalOperator(std::move(calc_stmt->expressions())));
  return RC::SUCCESS;
}

// 生成 select 语句的逻辑计划
RC LogicalPlanGenerator::create_plan(
    SelectStmt *select_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  unique_ptr<LogicalOperator> table_oper(nullptr);

  const std::vector<Table *> &tables = select_stmt->tables();
  const auto &all_fields = select_stmt->query_fields();
  auto &all_exprs = select_stmt->query_exprs();
  std::vector<Field> tables_all_fields;

  const auto is_aggre = select_stmt->is_aggre();
  for (Table *table : tables) {

    for (int i = table->table_meta().sys_field_num(); i < table->table_meta().field_num(); i++) {
      const FieldMeta *field_meta = table->table_meta().field(i);
      tables_all_fields.push_back(Field(table, field_meta));
    }

    std::vector<Field> fields;
    for (const Field &field : all_fields) {
      if (0 == strcmp(field.table_name(), table->name())) {
        fields.push_back(field);
      }
    }

    unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, fields, true/*readonly*/));
    if (table_oper == nullptr) {
      table_oper = std::move(table_get_oper);
    } else {
      JoinLogicalOperator *join_oper = new JoinLogicalOperator;
      join_oper->add_child(std::move(table_oper));
      join_oper->add_child(std::move(table_get_oper));
      table_oper = unique_ptr<LogicalOperator>(join_oper);
    }
  }

  unique_ptr<LogicalOperator> predicate_oper;
  RC rc = create_plan(select_stmt->filter_stmt(), predicate_oper);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create predicate logical plan. rc=%s", strrc(rc));
    return rc;
  }

  unique_ptr<LogicalOperator> project_oper(new ProjectLogicalOperator(all_exprs));
  if (predicate_oper) {
    if (table_oper) {
      predicate_oper->add_child(std::move(table_oper));
    }
    project_oper->add_child(std::move(predicate_oper));
  } else {
    if (table_oper) {
      project_oper->add_child(std::move(table_oper));
    }
  }

  if (is_aggre) {
    unique_ptr<LogicalOperator> aggregation_oper(new AggregationLogicalOperator(select_stmt->aggre_types()));
    aggregation_oper->add_child(std::move(project_oper));
    logical_operator.swap(aggregation_oper);
  } else {
    logical_operator.swap(project_oper);
  }

  if (select_stmt->order_by_fields().size() != 0) {
    unique_ptr<LogicalOperator> sort_oper(new SortLogicalOperator(select_stmt->order_by_fields(), all_fields, tables_all_fields));
    sort_oper->add_child(std::move(logical_operator));
    logical_operator.swap(sort_oper);
  }
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(
    FilterStmt *filter_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  std::vector<unique_ptr<Expression>> cmp_exprs;
  std::vector<FilterUnit *> &filter_units = filter_stmt->filter_units();
  for (FilterUnit *filter_unit : filter_units) {
    FilterObj &filter_obj_left = filter_unit->left();
    FilterObj &filter_obj_right = filter_unit->right();

    unique_ptr<Expression> left;
    unique_ptr<Expression> right;

    // unique_ptr<Expression> left(filter_obj_left.is_attr
    //                                   ? static_cast<Expression *>(new FieldExpr(filter_obj_left.field))
    //                                   : static_cast<Expression *>(new ValueExpr(filter_obj_left.value)));
    if (filter_obj_left.is_expr) {
      left = std::move(filter_obj_left.expr);
    } else {
      if (filter_obj_left.sub_select_stmt != nullptr) {
        // 创建子查询的逻辑计划
        // 这里说明目前子查询仅支持单表、单字段查询。如果有多表、多字段，这里默认取第一个。
        const char* table_name = filter_obj_left.sub_select_stmt->tables()[0]->name();
        const char* field_name = filter_obj_left.sub_select_stmt->query_fields()[0].field_name();
        AttrType attr_type = filter_obj_left.sub_select_stmt->query_fields()[0].attr_type();
        unique_ptr<LogicalOperator> sub_query_logical_oper = nullptr;
        RC rc = create_plan(filter_obj_left.sub_select_stmt, sub_query_logical_oper);
        if (rc != RC::SUCCESS) {
          LOG_PANIC("failed to create sub query logical plan. rc=%s", strrc(rc));
          return rc;
        }
        left = unique_ptr<Expression>(static_cast<Expression *>(new SubqueryExpr(attr_type, table_name, field_name, std::move(sub_query_logical_oper))));
      } else if (filter_obj_left.values != nullptr) {
        // 特殊子查询，IN (1, 2, 3)
        left = unique_ptr<Expression>(static_cast<Expression *>(new ValueListExpr(*filter_obj_left.values)));
      } else {
        left = std::move(filter_obj_left.expr);
      }
    }

    // unique_ptr<Expression> right(filter_obj_right.is_attr
    //                                       ? static_cast<Expression *>(new FieldExpr(filter_obj_right.field))
    //                                       : static_cast<Expression *>(new ValueExpr(filter_obj_right.value)));

    if (filter_obj_right.is_expr) {
      right = std::move(filter_obj_right.expr);
    } else {
      if (filter_obj_right.sub_select_stmt != nullptr) {
        // 创建普通select子查询的逻辑计划
        // 这里说明目前子查询仅支持单表、单字段查询。如果有多表、多字段，这里默认取第一个。
        const char* table_name = filter_obj_right.sub_select_stmt->tables()[0]->name();
        const char* field_name = filter_obj_right.sub_select_stmt->query_fields()[0].field_name();
        AttrType attr_type = filter_obj_right.sub_select_stmt->query_fields()[0].attr_type();
        unique_ptr<LogicalOperator> sub_query_logical_oper = nullptr;
        RC rc = create_plan(filter_obj_right.sub_select_stmt, sub_query_logical_oper);
        if (rc != RC::SUCCESS) {
          LOG_PANIC("failed to create sub query logical plan. rc=%s", strrc(rc));
          return rc;
        }
        right = unique_ptr<Expression>(static_cast<Expression *>(new SubqueryExpr(attr_type, table_name, field_name, std::move(sub_query_logical_oper))));
      } else if (filter_obj_right.values != nullptr) {
        // 特殊子查询，IN (1, 2, 3)
        right = unique_ptr<Expression>(static_cast<Expression *>(new ValueListExpr(*filter_obj_right.values)));
      } else {
        right = std::move(filter_obj_right.expr);
      }
    }

    // 对条件进行比较（筛选）
    ComparisonExpr *cmp_expr = new ComparisonExpr(filter_unit->comp(), std::move(left), std::move(right));
    cmp_exprs.emplace_back(cmp_expr);
  }

  unique_ptr<PredicateLogicalOperator> predicate_oper;
  // TODO: 这里只实现了 AND，还需要实现 OR

  // 如果有多个比较条件，需要套上一个ConjunctionExpr。miniob 中暂时只支持 AND 关系
  if (!cmp_exprs.empty()) {
    unique_ptr<ConjunctionExpr> conjunction_expr(new ConjunctionExpr(ConjunctionExpr::Type::AND, cmp_exprs));
    predicate_oper = unique_ptr<PredicateLogicalOperator>(new PredicateLogicalOperator(std::move(conjunction_expr)));
  }

  logical_operator = std::move(predicate_oper);
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(
    InsertStmt *insert_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  Table *table = insert_stmt->table();
  vector<Value> values(insert_stmt->values(), insert_stmt->values() + insert_stmt->value_amount());

  InsertLogicalOperator *insert_operator = new InsertLogicalOperator(table, values);
  logical_operator.reset(insert_operator);
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(
    UpdateStmt *update_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  Table *table = update_stmt->table();
  // const Value *value = update_stmt->values();
  // int value_amount = update_stmt->value_amount();
  auto &field_metas = update_stmt->field_metas();
  auto &targets = update_stmt->targets();
  FilterStmt *filter_stmt = update_stmt->filter_stmt();

  std::vector<Field> fields;
  for (int i = table->table_meta().sys_field_num(); i < table->table_meta().field_num(); i++) {
    const FieldMeta *field_meta = table->table_meta().field(i);
    fields.push_back(Field(table, field_meta));
  }

  // 对可能的子查询创建逻辑计划
  std::vector<UpdateSpecificTarget*> update_targets;
  UpdateSpecificTarget* update_target = nullptr;
  unique_ptr<LogicalOperator> sub_query_logical_oper = nullptr;
  for (int i=0; i<update_stmt->targets().size(); ++i) {
    if (!update_stmt->targets()[i].is_value) {
      sub_query_logical_oper = nullptr;
      RC rc = create_plan(update_stmt->targets()[i].select_stmt, sub_query_logical_oper);
      if (rc != RC::SUCCESS) {
        LOG_PANIC("failed to create sub query logical plan. rc=%s", strrc(rc));
        return rc;
      }
      // update_stmt->targets()[i].sub_select_logical_oper = std::move(sub_query_logical_oper);
      update_target = new UpdateSpecificTarget(std::move(sub_query_logical_oper));
    } else {
      update_target = new UpdateSpecificTarget(update_stmt->targets()[i].value);
    }
    update_targets.emplace_back(update_target);
  }

  // 从表中线性获取数据
  unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, fields, false/*readonly*/));

  unique_ptr<LogicalOperator> predicate_oper;
  RC rc = create_plan(filter_stmt, predicate_oper);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  unique_ptr<LogicalOperator> update_oper(new UpdateLogicalOperator(table, field_metas, update_targets));

  if (predicate_oper) {
    predicate_oper->add_child(std::move(table_get_oper));
    update_oper->add_child(std::move(predicate_oper));
  } else {
    update_oper->add_child(std::move(table_get_oper));
  }

  logical_operator = std::move(update_oper);
  return rc;
}

RC LogicalPlanGenerator::create_plan(
    DeleteStmt *delete_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  Table *table = delete_stmt->table();
  FilterStmt *filter_stmt = delete_stmt->filter_stmt();
  std::vector<Field> fields;
  for (int i = table->table_meta().sys_field_num(); i < table->table_meta().field_num(); i++) {
    const FieldMeta *field_meta = table->table_meta().field(i);
    fields.push_back(Field(table, field_meta));
  }
  unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, fields, false/*readonly*/));

  unique_ptr<LogicalOperator> predicate_oper;
  RC rc = create_plan(filter_stmt, predicate_oper);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  unique_ptr<LogicalOperator> delete_oper(new DeleteLogicalOperator(table));

  if (predicate_oper) {
    predicate_oper->add_child(std::move(table_get_oper));
    delete_oper->add_child(std::move(predicate_oper));
  } else {
    delete_oper->add_child(std::move(table_get_oper));
  }

  logical_operator = std::move(delete_oper);
  return rc;
}

RC LogicalPlanGenerator::create_plan(
    ExplainStmt *explain_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  Stmt *child_stmt = explain_stmt->child();
  unique_ptr<LogicalOperator> child_oper;
  RC rc = create(child_stmt, child_oper);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create explain's child operator. rc=%s", strrc(rc));
    return rc;
  }

  logical_operator = unique_ptr<LogicalOperator>(new ExplainLogicalOperator);
  logical_operator->add_child(std::move(child_oper));
  return rc;
}
