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
#include "sql/stmt/filter_stmt.h"
#include "common/log/log.h"
#include "common/lang/string.h"
#include "storage/db/db.h"
#include "storage/table/table.h"

UpdateStmt::UpdateStmt(Table *table, Value *values, int value_amount)
    : table_(table), values_(values), value_amount_(value_amount)
{}

RC UpdateStmt::create(Db *db, const UpdateSqlNode &update, Stmt *&stmt)
{
  // TODO

  if (nullptr == db) {
    LOG_WARN("[UpdateStmt] invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }

  // std::vector<Table *> tables;
  std::unordered_map<std::string, Table *> table_map;
  std::vector<Field> query_fields;

  const char *table_name = update.relation_name.c_str();
  const char *field_name = update.attribute_name.c_str();

  if (table_name == nullptr) {
    LOG_WARN("[UpdateStmt] invalid argument. relation name is null.");
    return RC::INVALID_ARGUMENT;
  }

  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("[UpdateStmt] no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  table_map.insert(std::pair<std::string, Table *>(table_name, table));

  const FieldMeta *field_meta = table->table_meta().field(field_name);
  if (nullptr == field_meta) {
    LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), field_name);
    return RC::SCHEMA_FIELD_MISSING;
  }

  query_fields.push_back(Field(table, field_meta));

  // FilterStmt *filter_stmt = nullptr;
  // RC rc = FilterStmt::create(db, 
  //     table,
  //     &table_map,
  //     update.conditions.data(),
  //     static_cast<int>(update.conditions.size()),
  //     filter_stmt
  // );
  // if (rc != RC::SUCCESS) {
  //   LOG_WARN("cannot construct filter stmt");
  //   return rc;
  // }

  // 转换类型 update.value 到 value
  Value *value = new Value();
  value->set_value(update.value);

  UpdateStmt *update_stmt = new UpdateStmt(table, value, 1);
  stmt = update_stmt;
  return RC::SUCCESS;
}
