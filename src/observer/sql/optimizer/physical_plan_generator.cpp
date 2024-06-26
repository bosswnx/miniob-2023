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
// Created by Wangyunlai on 2022/12/14.
//

#include <memory>
#include <utility>

#include "sql/operator/aggre_logical_operator.h"
#include "sql/operator/aggre_physical_operator.h"
#include "sql/operator/sort_logical_operator.h"
#include "sql/operator/sort_physical_operator.h"
#include "sql/operator/logical_operator.h"
#include "sql/operator/physical_operator.h"
#include "sql/optimizer/physical_plan_generator.h"
#include "sql/operator/table_get_logical_operator.h"
#include "sql/operator/table_scan_physical_operator.h"
#include "sql/operator/index_scan_physical_operator.h"
#include "sql/operator/predicate_logical_operator.h"
#include "sql/operator/predicate_physical_operator.h"
#include "sql/operator/project_logical_operator.h"
#include "sql/operator/project_physical_operator.h"
#include "sql/operator/insert_logical_operator.h"
#include "sql/operator/insert_physical_operator.h"
#include "sql/operator/delete_logical_operator.h"
#include "sql/operator/delete_physical_operator.h"
#include "sql/operator/update_logical_operator.h"
#include "sql/operator/update_physical_operator.h"
#include "sql/operator/explain_logical_operator.h"
#include "sql/operator/explain_physical_operator.h"
#include "sql/operator/join_logical_operator.h"
#include "sql/operator/join_physical_operator.h"
#include "sql/operator/calc_logical_operator.h"
#include "sql/operator/calc_physical_operator.h"
#include "sql/operator/no_table_select_logical_operator.h"
#include "sql/operator/no_table_select_physical_operator.h"
#include "sql/expr/expression.h"
#include "common/log/log.h"
#include "sql/parser/parse_defs.h"
#include "storage/field/field.h"
#include "storage/field/field_meta.h"

using namespace std;

RC PhysicalPlanGenerator::create(LogicalOperator &logical_operator, unique_ptr<PhysicalOperator> &oper)
{
  RC rc = RC::SUCCESS;

  switch (logical_operator.type()) {
    case LogicalOperatorType::CALC: {
      return create_plan(static_cast<CalcLogicalOperator &>(logical_operator), oper);
    } break;

    case LogicalOperatorType::TABLE_GET: {
      return create_plan(static_cast<TableGetLogicalOperator &>(logical_operator), oper);
    } break;

    case LogicalOperatorType::PREDICATE: {
      return create_plan(static_cast<PredicateLogicalOperator &>(logical_operator), oper);
    } break;

    case LogicalOperatorType::PROJECTION: {
      return create_plan(static_cast<ProjectLogicalOperator &>(logical_operator), oper);
    } break;

    case LogicalOperatorType::INSERT: {
      return create_plan(static_cast<InsertLogicalOperator &>(logical_operator), oper);
    } break;

    case LogicalOperatorType::DELETE: {
      return create_plan(static_cast<DeleteLogicalOperator &>(logical_operator), oper);
    } break;

    case LogicalOperatorType::UPDATE: {
      return create_plan(static_cast<UpdateLogicalOperator &>(logical_operator), oper);
    } break;

    case LogicalOperatorType::EXPLAIN: {
      return create_plan(static_cast<ExplainLogicalOperator &>(logical_operator), oper);
    } break;

    case LogicalOperatorType::JOIN: {
      return create_plan(static_cast<JoinLogicalOperator &>(logical_operator), oper);
    } break;
    
    case LogicalOperatorType::AGGREGATION: {
      return create_plan(static_cast<AggregationLogicalOperator &>(logical_operator), oper);
    } break;

    case LogicalOperatorType::SORT: {
      return create_plan(static_cast<SortLogicalOperator &>(logical_operator), oper);
    } break;

    case LogicalOperatorType::NO_TABLE_SELECT: {
      return create_plan(static_cast<NoTableSelectLogicalOperator &>(logical_operator), oper);
    } break;

    default: {
      return RC::INVALID_ARGUMENT;
    }
  }
  return rc;
}

RC PhysicalPlanGenerator::create_plan(TableGetLogicalOperator &table_get_oper, unique_ptr<PhysicalOperator> &oper)
{
  vector<unique_ptr<Expression>> &predicates = table_get_oper.predicates();
  // 看看是否有可以用于索引查找的表达式
  Table *table = table_get_oper.table();

  // TODO: 修改为多列索引
  // 先把所有的表达式都拿出来，然后遍历所有的索引，看看是否有索引可以使用
  // 需要保证之前已经把表达式能合并的都合并了
  // 表达式左边是字段，右边是值

  vector<string> fields_name;
  vector<Value> values;
  vector<CompOp> comps;
  for (auto &expr : predicates) {
    if (expr->type() == ExprType::COMPARISON) {
      auto comparison_expr = static_cast<ComparisonExpr *>(expr.get());
      unique_ptr<Expression> &left_expr = comparison_expr->left();
      unique_ptr<Expression> &right_expr = comparison_expr->right();
      // 左右比较的一边最少是一个值
      if (left_expr->type() != ExprType::VALUE && right_expr->type() != ExprType::VALUE) {
        continue;
      }
      ValueExpr *value_expr = nullptr;
      FieldExpr *field_expr = nullptr;
      bool swaped = false;
      if (left_expr->type() == ExprType::FIELD) {
        ASSERT(right_expr->type() == ExprType::VALUE, "right expr should be a value expr while left is field expr");
        field_expr = static_cast<FieldExpr *>(left_expr.get());
        value_expr = static_cast<ValueExpr *>(right_expr.get());
      } else if (right_expr->type() == ExprType::FIELD) {
        ASSERT(left_expr->type() == ExprType::VALUE, "left expr should be a value expr while right is a field expr");
        field_expr = static_cast<FieldExpr *>(right_expr.get());
        value_expr = static_cast<ValueExpr *>(left_expr.get());
        swaped = true;
      }

      if (field_expr == nullptr) {
        continue;
      }
      fields_name.push_back(field_expr->field().field_name());
      values.push_back(value_expr->get_value());
      switch (comparison_expr->comp()) {
        case EQUAL_TO: comps.push_back(EQUAL_TO); break;
        case NOT_EQUAL: comps.push_back(NOT_EQUAL); break;
        case LESS_THAN: comps.push_back(swaped ? GREAT_THAN : LESS_THAN); break;
        case LESS_EQUAL: comps.push_back(swaped ? GREAT_EQUAL : LESS_EQUAL); break;
        case GREAT_THAN: comps.push_back(swaped ? LESS_THAN : GREAT_THAN); break;
        case GREAT_EQUAL: comps.push_back(swaped ? LESS_EQUAL : GREAT_EQUAL); break;
        case LIKE: comps.push_back(LIKE); break;
        case NOT_LIKE: comps.push_back(NOT_LIKE); break;
        default: {
          LOG_WARN("unsupported comparison type %d", comparison_expr->comp());
          return RC::INTERNAL;
        }
      }
    }
  }

  vector<Value> l_values;
  vector<Value> r_values;
  bool l_inclusive = false;
  bool r_inclusive = false;

  Index *index = table->find_index_by_fields(fields_name, comps, values, l_values, r_values, l_inclusive, r_inclusive);



  if (index != nullptr) {
    // ASSERT(value_expr != nullptr, "got an index but value expr is null ?");
    // TODO: 需要处理出索引扫描的区间

    IndexScanPhysicalOperator *index_scan_oper = new IndexScanPhysicalOperator(
          table, index, table_get_oper.readonly(), 
          l_values, l_inclusive /*left_inclusive*/, 
          r_values, r_inclusive /*right_inclusive*/);
          
    index_scan_oper->set_predicates(std::move(predicates));
    oper = unique_ptr<PhysicalOperator>(index_scan_oper);
    LOG_TRACE("use index scan");
  } else {
    auto table_scan_oper = new TableScanPhysicalOperator(table, table_get_oper.readonly());
    table_scan_oper->set_predicates(std::move(predicates));
    oper = unique_ptr<PhysicalOperator>(table_scan_oper);
    LOG_TRACE("use table scan");
  }

  return RC::SUCCESS;
}

RC PhysicalPlanGenerator::create_plan(PredicateLogicalOperator &pred_oper, unique_ptr<PhysicalOperator> &oper)
{
  vector<unique_ptr<LogicalOperator>> &children_opers = pred_oper.children();
  ASSERT(children_opers.size() == 1, "predicate logical operator's sub oper number should be 1");

  LogicalOperator &child_oper = *children_opers.front();

  unique_ptr<PhysicalOperator> child_phy_oper;
  RC rc = create(child_oper, child_phy_oper);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create child operator of predicate operator. rc=%s", strrc(rc));
    return rc;
  }

  vector<unique_ptr<Expression>> &expressions = pred_oper.expressions();

  ASSERT(expressions.size() == 1, "predicate logical operator's children should be 1");

  unique_ptr<Expression> expression = std::move(expressions.front());


  // 在这里，如果是子查询的Expr，先让其生成子查询的物理计划
  if (expression->type() == ExprType::CONJUNCTION) {
    // 转成 ConjunctionExpr
    ConjunctionExpr &conjunction_expr = static_cast<ConjunctionExpr &>(*expression);
    vector<unique_ptr<Expression>> &exprs = conjunction_expr.children();
    for (auto &expr : exprs) {
      if (expr->type() == ExprType::COMPARISON) {
        // 转成 SubqueryExpr
        ComparisonExpr &comparison_expr = static_cast<ComparisonExpr &>(*expr);
        if (comparison_expr.left()->type() == ExprType::SUBQUERY) {
          SubqueryExpr *subquery_expr = static_cast<SubqueryExpr *>(comparison_expr.left().get());
          unique_ptr<PhysicalOperator> subquery_phy_oper = nullptr;
          rc = create(*subquery_expr->logical_operator(), subquery_phy_oper);
          subquery_expr->set_physical_operator(std::move(subquery_phy_oper));
        }
        if (comparison_expr.right()->type() == ExprType::SUBQUERY) {
          SubqueryExpr *subquery_expr = static_cast<SubqueryExpr *>(comparison_expr.right().get());
          unique_ptr<PhysicalOperator> subquery_phy_oper = nullptr;
          if (subquery_expr->logical_operator() == nullptr) {
            subquery_expr->set_physical_operator(nullptr);
          } else {
            rc = create(*subquery_expr->logical_operator(), subquery_phy_oper);
            subquery_expr->set_physical_operator(std::move(subquery_phy_oper));
          }
        }
      }
    }
  }

  oper = unique_ptr<PhysicalOperator>(new PredicatePhysicalOperator(std::move(expression)));
  oper->add_child(std::move(child_phy_oper));
  return rc;
}

RC PhysicalPlanGenerator::create_plan(ProjectLogicalOperator &project_oper, unique_ptr<PhysicalOperator> &oper)
{
  vector<unique_ptr<LogicalOperator>> &child_opers = project_oper.children();

  unique_ptr<PhysicalOperator> child_phy_oper;

  RC rc = RC::SUCCESS;
  if (!child_opers.empty()) {
    LogicalOperator *child_oper = child_opers.front().get();
    rc = create(*child_oper, child_phy_oper);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to create project logical operator's child physical operator. rc=%s", strrc(rc));
      return rc;
    }
  }

  ProjectPhysicalOperator *project_operator = new ProjectPhysicalOperator;
  
  auto &project_exprs = project_oper.expressions();
  project_operator->add_expressions(std::move(project_exprs));

  if (child_phy_oper) {
    project_operator->add_child(std::move(child_phy_oper));
  }

  oper = unique_ptr<PhysicalOperator>(project_operator);

  LOG_TRACE("create a project physical operator");
  return rc;
}

RC PhysicalPlanGenerator::create_plan(InsertLogicalOperator &insert_oper, unique_ptr<PhysicalOperator> &oper)
{
  Table *table = insert_oper.table();
  vector<Value> &values = insert_oper.values();
  InsertPhysicalOperator *insert_phy_oper = new InsertPhysicalOperator(table, std::move(values));
  oper.reset(insert_phy_oper);
  return RC::SUCCESS;
}

RC PhysicalPlanGenerator::create_plan(UpdateLogicalOperator &update_oper, unique_ptr<PhysicalOperator> &oper)
{
  vector<unique_ptr<LogicalOperator>> &child_opers = update_oper.children(); // 把逻辑算子生成的children拿出来

  unique_ptr<PhysicalOperator> child_physical_oper;

  RC rc = RC::SUCCESS;
  if (!child_opers.empty()) {
    LogicalOperator *child_oper = child_opers.front().get(); 

    rc = create(*child_oper, child_physical_oper);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to create physical operator. rc=%s", strrc(rc));
      return rc;
    }
  }

  for (int i=0; i<update_oper.targets().size(); ++i) {
    if (!update_oper.targets()[i]->is_value()) {
      // 创建物理计划
      unique_ptr<PhysicalOperator> sub_select_physical_oper = nullptr;
      rc = create(*update_oper.targets()[i]->get_sub_select_logical_operator(), sub_select_physical_oper);
      update_oper.targets()[i]->set_sub_select_physical_operator (std::move(sub_select_physical_oper));
    }
  }

  oper = unique_ptr<PhysicalOperator>(new UpdatePhysicalOperator(update_oper.table(), update_oper.field_metas(), update_oper.targets()));
  if (child_physical_oper) {
    oper->add_child(std::move(child_physical_oper));
  }
  return rc;
}

RC PhysicalPlanGenerator::create_plan(DeleteLogicalOperator &delete_oper, unique_ptr<PhysicalOperator> &oper)
{
  vector<unique_ptr<LogicalOperator>> &child_opers = delete_oper.children();

  unique_ptr<PhysicalOperator> child_physical_oper;

  RC rc = RC::SUCCESS;
  if (!child_opers.empty()) {
    LogicalOperator *child_oper = child_opers.front().get();
    rc = create(*child_oper, child_physical_oper);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to create physical operator. rc=%s", strrc(rc));
      return rc;
    }
  }

  oper = unique_ptr<PhysicalOperator>(new DeletePhysicalOperator(delete_oper.table()));

  if (child_physical_oper) {
    oper->add_child(std::move(child_physical_oper));
  }
  return rc;
}

RC PhysicalPlanGenerator::create_plan(ExplainLogicalOperator &explain_oper, unique_ptr<PhysicalOperator> &oper)
{
  vector<unique_ptr<LogicalOperator>> &child_opers = explain_oper.children();

  RC rc = RC::SUCCESS;
  unique_ptr<PhysicalOperator> explain_physical_oper(new ExplainPhysicalOperator);
  for (unique_ptr<LogicalOperator> &child_oper : child_opers) {
    unique_ptr<PhysicalOperator> child_physical_oper;
    rc = create(*child_oper, child_physical_oper);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to create child physical operator. rc=%s", strrc(rc));
      return rc;
    }

    explain_physical_oper->add_child(std::move(child_physical_oper));
  }

  oper = std::move(explain_physical_oper);
  return rc;
}

RC PhysicalPlanGenerator::create_plan(JoinLogicalOperator &join_oper, unique_ptr<PhysicalOperator> &oper)
{
  RC rc = RC::SUCCESS;

  vector<unique_ptr<LogicalOperator>> &child_opers = join_oper.children();
  if (child_opers.size() != 2) {
    LOG_WARN("join operator should have 2 children, but have %d", child_opers.size());
    return RC::INTERNAL;
  }

  unique_ptr<PhysicalOperator> join_physical_oper(new NestedLoopJoinPhysicalOperator);
  for (auto &child_oper : child_opers) {
    unique_ptr<PhysicalOperator> child_physical_oper;
    rc = create(*child_oper, child_physical_oper);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to create physical child oper. rc=%s", strrc(rc));
      return rc;
    }

    join_physical_oper->add_child(std::move(child_physical_oper));
  }

  oper = std::move(join_physical_oper);
  return rc;
}

RC PhysicalPlanGenerator::create_plan(CalcLogicalOperator &logical_oper, std::unique_ptr<PhysicalOperator> &oper)
{
  RC rc = RC::SUCCESS;
  CalcPhysicalOperator *calc_oper = new CalcPhysicalOperator(std::move(logical_oper.expressions()));
  oper.reset(calc_oper);
  return rc;
}

RC PhysicalPlanGenerator::create_plan(AggregationLogicalOperator &aggre_oper, std::unique_ptr<PhysicalOperator> &oper) {
  RC rc = RC::SUCCESS;
  vector<unique_ptr<LogicalOperator>> &child_opers = aggre_oper.children();
  if (child_opers.size() != 1) {
    LOG_WARN("Aggregation operator should have 1 child, but have %d", child_opers.size());
    return RC::INTERNAL;
  }
  if (child_opers[0]->type() != LogicalOperatorType::PROJECTION) {
    LOG_WARN("Aggregation operator should only have projection operator as child, but doesn't.");
    return RC::INTERNAL;
  }

  unique_ptr<PhysicalOperator> child_phy_oper;
  LogicalOperator *child_oper = child_opers.front().get();
  rc = create(*child_oper, child_phy_oper);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create project logical operator's child physical operator. rc=%s", strrc(rc));
    return rc;
  }
  unique_ptr<PhysicalOperator> aggre_phy_oper(new AggregationPhysicalOperator());
  aggre_phy_oper->add_child(std::move(child_phy_oper));
  oper = std::move(aggre_phy_oper);
  return rc;
}


RC PhysicalPlanGenerator::create_plan(SortLogicalOperator &sort_oper, std::unique_ptr<PhysicalOperator> &oper) {
  RC rc = RC::SUCCESS;
  vector<unique_ptr<LogicalOperator>> &child_opers = sort_oper.children();
  if (child_opers.size() != 1) {
    LOG_WARN("sort operator should have 1 child, but have %d", child_opers.size());
    return RC::INTERNAL;
  }
  // if (child_opers[0]->type() != LogicalOperatorType::PROJECTION) {
  //   LOG_WARN("sort operator should only have projection operator as child, but doesn't.");
  //   return RC::INTERNAL;
  // }
  unique_ptr<PhysicalOperator> child_phy_oper;
  LogicalOperator *child_oper = child_opers.front().get();
  rc = create(*child_oper, child_phy_oper);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create project logical operator's child physical operator. rc=%s", strrc(rc));
    return rc;
  }
  auto order_by_fields = sort_oper.order_by_fields();
  unique_ptr<PhysicalOperator> sort_phy_oper(new SortPhysicalOperator(order_by_fields, sort_oper.query_fields(), sort_oper.tables_all_fields()));
  sort_phy_oper->add_child(std::move(child_phy_oper));
  oper = std::move(sort_phy_oper);
  return rc;
}

RC PhysicalPlanGenerator::create_plan(NoTableSelectLogicalOperator &no_table_select_oper, std::unique_ptr<PhysicalOperator> &oper) {
  RC rc = RC::SUCCESS;
  vector<unique_ptr<LogicalOperator>> &child_opers = no_table_select_oper.children();
  if (child_opers.size() != 0) {
    LOG_WARN("NoTableSelectLogicalOperator operator should have 0 child, but have %d", child_opers.size());
    return RC::INTERNAL;
  }
  unique_ptr<PhysicalOperator> no_table_select_phy_oper(new NoTableSelectPhysicalOperator());
  oper = std::move(no_table_select_phy_oper);
  return rc;
}
