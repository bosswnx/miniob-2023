/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "sql/operator/logical_operator.h"
#include "sql/operator/physical_operator.h"
#include "sql/parser/value.h"
#include <memory>

class UpdateSpecificTarget {
public:
    UpdateSpecificTarget() = default;
    UpdateSpecificTarget(const Value &value) : is_value_(true), value(value) {}
    UpdateSpecificTarget(std::unique_ptr<LogicalOperator> sub_select_logical_operator)
        : is_value_(false),
          sub_select_logical_operator(std::move(sub_select_logical_operator)) {}

    bool is_value() const { return is_value_; }
    const Value &get_value() const { return value; }
    Value &get_value() { return value; }
    const LogicalOperator *get_sub_select_logical_operator() const { return sub_select_logical_operator.get(); }
    LogicalOperator *get_sub_select_logical_operator() { return sub_select_logical_operator.get(); }
    const PhysicalOperator *get_sub_select_physical_operator() const { return sub_select_physical_operator.get(); }
    PhysicalOperator *get_sub_select_physical_operator() { return sub_select_physical_operator.get(); }
    void set_sub_select_physical_operator(std::unique_ptr<PhysicalOperator> sub_select_physical_operator) {
        this->sub_select_physical_operator = std::move(sub_select_physical_operator);
    }
private:
    bool is_value_;
    Value value;
    std::unique_ptr<LogicalOperator> sub_select_logical_operator = nullptr;
    std::unique_ptr<PhysicalOperator> sub_select_physical_operator = nullptr;
};