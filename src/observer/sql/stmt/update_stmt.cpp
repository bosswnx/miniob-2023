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
// Created by Wangyunlai on 2022/5/22.
//

#include "sql/stmt/update_stmt.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
#include "sql/stmt/filter_stmt.h"
#include "common/log/log.h"
#include "common/lang/string.h"
#include "storage/db/db.h"
#include "storage/field/field_meta.h"
#include "storage/table/table.h"
#include <cstdio>
#include <utility> // pair
#include "sql/stmt/select_stmt.h"

UpdateStmt::UpdateStmt(Table *table, FilterStmt *filter_stmt, 
    const std::vector<FieldMeta> &field_metas, std::vector<UpdateTarget> &targets, int value_amount)
    : table_(table), filter_stmt_(filter_stmt), field_metas_(field_metas), value_amount_(value_amount)
{
  targets_ = std::move(targets);
}

UpdateStmt::~UpdateStmt()
{
  if (nullptr != filter_stmt_) {
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }
}

RC UpdateStmt::create(Db *db, UpdateSqlNode &update, Stmt *&stmt)
{
  // printf("create update stmt.\n");
  const char *table_name = update.relation_name.c_str();
  if (nullptr == db || nullptr == table_name) {
    LOG_WARN("[UpdateStmt] invalid argument. db=%p, table_name=%p", db, table_name);
    return RC::INVALID_ARGUMENT;
  }

  vector<string> fields_name = update.attributes_name;

  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("[UpdateStmt] no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  // 对 update.targets 遍历
  for (int i=0; i<update.targets.size(); ++i) {
    if (!update.targets[i].is_value) {
      Stmt *subquery_stmt = nullptr;
      RC rc = SelectStmt::create(db, update.targets[i].sub_select->selection, subquery_stmt);
      if (rc != RC::SUCCESS) {
        LOG_WARN("cannot construct subquery stmt");
        return rc;
      }
      auto *subquery_select_stmt = dynamic_cast<SelectStmt *>(subquery_stmt);
      update.targets[i].select_stmt = subquery_select_stmt;
    }
  }

  // std::vector<Field> query_fields;
  
  std::unordered_map<std::string, Table *> table_map;
  table_map.insert(std::pair<std::string, Table *>(table_name, table));

  // printf("update field_name: %s\n", field_name);
  vector<FieldMeta> field_metas;
  for (auto &field_name : fields_name) {
    const FieldMeta *field_meta = table->table_meta().field(field_name.c_str());
    if (nullptr == field_meta) {
      LOG_WARN("no such field. field=%s.%s.%s\n", db->name(), table->name(), field_name.c_str());
      return RC::SCHEMA_FIELD_MISSING;
    }
    field_metas.push_back(*field_meta);
  }

  // 字段类型检查
  auto table_meta_ = table->table_meta();

  for (int i = 0; i < field_metas.size(); i++) {
    if(update.targets[i].is_value) {
      if (field_metas[i].type() != update.targets[i].value.attr_type()) {
        AttrType to_type = field_metas[i].type();
        AttrType from_type = update.targets[i].value.attr_type();

        if (to_type == AttrType::INTS && from_type == AttrType::FLOATS) {

          update.targets[i].value.set_int((int)update.targets[i].value.get_float());
          update.targets[i].value.set_type(AttrType::INTS);
        } else if (to_type == AttrType::FLOATS && from_type == AttrType::INTS) {

          update.targets[i].value.set_float((float)update.targets[i].value.get_int());
          update.targets[i].value.set_type(AttrType::FLOATS);
        } else if (to_type == AttrType::CHARS && from_type == AttrType::INTS) {

          update.targets[i].value.set_string(std::to_string(update.targets[i].value.get_int()).c_str());
          update.targets[i].value.set_type(AttrType::CHARS);
        } else if (to_type == AttrType::CHARS && from_type == AttrType::FLOATS) {

          update.targets[i].value.set_string(std::to_string(update.targets[i].value.get_float()).c_str());
          update.targets[i].value.set_type(AttrType::CHARS);
        } else if (update.targets[i].value.get_null_or_()) {
           
        } else {
          LOG_WARN("type mismatch. to_type=%d, from_type=%d", to_type, from_type);
          return RC::INVALID_ARGUMENT;
        }
      }
    }
  }
  

  FilterStmt *filter_stmt = nullptr;
  RC rc = FilterStmt::create(db, 
      table,
      &table_map,
      update.conditions.data(),
      static_cast<int>(update.conditions.size()),
      filter_stmt
  );
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot construct filter stmt");
    return rc;
  }

  // 转换类型 update.value 到 value
  // vector<Value> values;
  // for (auto &value : update.values) {
  //   values.push_back(value);
  // }

  // vector<UpdateTarget> targets;
  // for (auto &target : update.targets) {
  //   targets.push_back(target);
  // }

  UpdateStmt *update_stmt = new UpdateStmt(table, filter_stmt, field_metas, update.targets, update.targets.size());
  stmt = update_stmt;
  return RC::SUCCESS;
}
