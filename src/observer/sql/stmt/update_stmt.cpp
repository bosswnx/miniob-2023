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
#include <cstdio>
#include <utility> // pair

UpdateStmt::UpdateStmt(Table *table, FilterStmt *filter_stmt, std::pair<Field*, Value*> *values_with_field, int value_amount)
    : table_(table), filter_stmt_(filter_stmt), values_with_field_(values_with_field), value_amount_(value_amount)
{}

UpdateStmt::~UpdateStmt()
{
  if (nullptr != filter_stmt_) {
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }
}

RC UpdateStmt::create(Db *db, const UpdateSqlNode &update, Stmt *&stmt)
{
  // printf("create update stmt.\n");
  const char *table_name = update.relation_name.c_str();
  if (nullptr == db || nullptr == table_name) {
    LOG_WARN("[UpdateStmt] invalid argument. db=%p, table_name=%p", db, table_name);
    return RC::INVALID_ARGUMENT;
  }

  const char *field_name = update.attribute_name.c_str();

  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("[UpdateStmt] no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  // std::vector<Field> query_fields;
  
  std::unordered_map<std::string, Table *> table_map;
  table_map.insert(std::pair<std::string, Table *>(table_name, table));

  // printf("update field_name: %s\n", field_name);
  const FieldMeta *field_meta = table->table_meta().field(field_name);
  if (nullptr == field_meta) {
    LOG_WARN("no such field. field=%s.%s.%s\n", db->name(), table->name(), field_name);
    return RC::SCHEMA_FIELD_MISSING;
  }

  // 字段类型检查
  auto table_meta_ = table->table_meta();

  if (field_meta->type() != update.value.attr_type()) {
    LOG_ERROR("Invalid value type. table name=%s, field name=%s, type=%d, but given=%d",
              table_meta_.name(), field_meta->name(), field_meta->type(), update.value.attr_type());
    return RC::SCHEMA_FIELD_TYPE_MISMATCH;
  }


  Field* query_field = new Field(table, field_meta); // 当前只支持单字段更新

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
  Value *value = new Value();
  value->set_value(update.value);
  std::pair<Field*, Value*> *values_with_field = new std::pair<Field*, Value*>(query_field, value);
  UpdateStmt *update_stmt = new UpdateStmt(table, filter_stmt, values_with_field, 1);
  stmt = update_stmt;
  return RC::SUCCESS;
}
