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
// Created by Soulter on 2023/10/18.
//

#include "sql/operator/update_logical_operator.h"
#include "sql/parser/parse_defs.h"
#include <utility>

UpdateLogicalOperator::UpdateLogicalOperator(Table *table, const std::vector<FieldMeta> &field_metas, vector<UpdateSpecificTarget*> &targets)
    : table_(table), field_metas_(field_metas), targets_(targets)
{}