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
// Created by Wangyunlai on 2022/6/6.
//

#include "sql/stmt/select_stmt.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/filter_stmt.h"
#include "common/log/log.h"
#include "common/lang/string.h"
#include "storage/db/db.h"
#include "storage/table/table.h"

SelectStmt::~SelectStmt()
{
  if (nullptr != filter_stmt_) {
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }
}

// 把该表的所有属性（字段）加入到field_metas中
static void wildcard_fields(Table *table, std::vector<Field> &field_metas)
{
  const TableMeta &table_meta = table->table_meta();
  const int field_num = table_meta.field_num();
  for (int i = table_meta.sys_field_num(); i < field_num; i++) {
    field_metas.push_back(Field(table, table_meta.field(i)));
  }
}

RC SelectStmt::create(Db *db, const SelectSqlNode &select_sql, Stmt *&stmt)
{
  if (nullptr == db) {
    LOG_WARN("invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }

  if (select_sql.attributes.empty()) {
    LOG_WARN("invalid argument. select attributes is empty");
    return RC::INVALID_ARGUMENT;
  }

  // collect tables in `from` statement
  std::vector<Table *> tables;
  std::unordered_map<std::string, Table *> table_map;

  // 遍历查询的表
  // 1. 检查表是否存在
  // 2. 将表加入到tables中
  // 3. 将表加入到table_map中
  for (size_t i = 0; i < select_sql.relations.size(); i++) {
    const char *table_name = select_sql.relations[i].c_str();
    if (nullptr == table_name) {
      LOG_WARN("invalid argument. relation name is null. index=%d", i);
      return RC::INVALID_ARGUMENT;
    }

    Table *table = db->find_table(table_name);
    if (nullptr == table) {
      LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }

    tables.push_back(table);
    table_map.insert(std::pair<std::string, Table *>(table_name, table));
  }

  // collect query fields in `select` statement
  std::vector<Field> query_fields;
  std::vector<AggreType> aggre_types;
  int aggre_stat = 0;  //  用于表示当前是否有聚合函数  01->普通  10->聚合
  for (int i = static_cast<int>(select_sql.attributes.size()) - 1; i >= 0; i--) {
    const RelAttrSqlNode &relation_attr = select_sql.attributes[i];
    aggre_stat |= relation_attr.aggre_type == AggreType::NONE ? 1 : 2;
    if (aggre_stat == 3) {
      LOG_WARN("both have aggregation and normal selection.");
      return RC::INVALID_ARGUMENT;
    }

    // 如果表名为空，且属性名为*，则将所有表的所有属性加入到query_fields中
    if (common::is_blank(relation_attr.relation_name.c_str()) &&
        0 == strcmp(relation_attr.attribute_name.c_str(), "*")) {
      if (relation_attr.aggre_type == AggreType::NONE) {
        for (Table *table : tables) {
          // wildcard: 通配符
        wildcard_fields(table, query_fields);
        }
      } else if (relation_attr.aggre_type == AggreType::CNT) {
        auto *table = tables[0];
        auto *field_meta = table->table_meta().field(0);
        query_fields.push_back(Field(table, field_meta));
        aggre_types.push_back(AggreType::CNTALL);
      } else {
        auto table_meta = tables[0]->table_meta();
        if (tables.size() > 1 || table_meta.field_num() > 1) {
          LOG_WARN("too many fields in aggregation function.");
          return RC::INVALID_ARGUMENT;
        }
        auto *table = tables[0];
        auto *field_meta = table->table_meta().field(0);
        if ((field_meta->type() == CHARS || field_meta->type() == DATES) && relation_attr.aggre_type != AggreType::MAX && relation_attr.aggre_type != AggreType::MIN) {
          LOG_WARN("invalid aggregation function on DATES type.");
          return RC::INVALID_ARGUMENT;
        }
        query_fields.push_back(Field(table, field_meta));
        aggre_types.push_back(relation_attr.aggre_type);
      }

    } else if (!common::is_blank(relation_attr.relation_name.c_str())) {
      const char *table_name = relation_attr.relation_name.c_str();
      const char *field_name = relation_attr.attribute_name.c_str();

      // table_name为*，且field_name不为*，则报错
      if (0 == strcmp(table_name, "*")) {
        if (0 != strcmp(field_name, "*")) {
          LOG_WARN("invalid field name while table is *. attr=%s", field_name);
          return RC::SCHEMA_FIELD_MISSING;
        }
        if (relation_attr.aggre_type == AggreType::NONE) {
          for (Table *table : tables) {
            wildcard_fields(table, query_fields);
          }
        } else if (relation_attr.aggre_type == AggreType::CNT) {
          auto *table = tables[0];
          auto *field_meta = table->table_meta().field(0);
          query_fields.push_back(Field(table, field_meta));
          aggre_types.push_back(AggreType::CNT);
        } else {
          auto table_meta = tables[0]->table_meta();
          if (tables.size() > 1 || table_meta.field_num() > 1) {
            LOG_WARN("too many fields in aggregation function.");
            return RC::INVALID_ARGUMENT;
          }
          auto *table = tables[0];
          auto *field_meta = table->table_meta().field(0);
          if ((field_meta->type() == CHARS || field_meta->type() == DATES) && relation_attr.aggre_type != AggreType::MAX && relation_attr.aggre_type != AggreType::MIN) {
            LOG_WARN("invalid aggregation function on DATES type.");
            return RC::INVALID_ARGUMENT;
          }
          query_fields.push_back(Field(table, field_meta));
          aggre_types.push_back(relation_attr.aggre_type);
        }
      } else {
        auto iter = table_map.find(table_name);
        if (iter == table_map.end()) {
          LOG_WARN("no such table in from list: %s", table_name);
          return RC::SCHEMA_FIELD_MISSING;
        }

        Table *table = iter->second;
        if (0 == strcmp(field_name, "*")) {
          if (relation_attr.aggre_type == AggreType::NONE) {
            wildcard_fields(table, query_fields);
          } else if (relation_attr.aggre_type == AggreType::CNT) {
            auto *field_meta = table->table_meta().field(0);
            query_fields.push_back(Field(table, field_meta));
            aggre_types.push_back(AggreType::CNT);
          } else {
            auto table_meta = table->table_meta();
            if (table_meta.field_num() > 1) {
              LOG_WARN("too many fields in aggregation function.");
              return RC::INVALID_ARGUMENT;
            }
            auto *field_meta = table->table_meta().field(0);
            if ((field_meta->type() == CHARS || field_meta->type() == DATES) && relation_attr.aggre_type != AggreType::MAX && relation_attr.aggre_type != AggreType::MIN) {
              LOG_WARN("invalid aggregation function on DATES type.");
              return RC::INVALID_ARGUMENT;
            }
            query_fields.push_back(Field(table, field_meta));
            aggre_types.push_back(relation_attr.aggre_type);
          }
        } else {
          const FieldMeta *field_meta = table->table_meta().field(field_name);
          if (nullptr == field_meta) {
            LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), field_name);
            return RC::SCHEMA_FIELD_MISSING;
          }

          query_fields.push_back(Field(table, field_meta));
          if (relation_attr.aggre_type != AggreType::NONE) {
            aggre_types.push_back(relation_attr.aggre_type);
          }
        }
      }
    } else {
      if (tables.size() != 1) {
        LOG_WARN("invalid. I do not know the attr's table. attr=%s", relation_attr.attribute_name.c_str());
        return RC::SCHEMA_FIELD_MISSING;
      }

      Table *table = tables[0];
      const FieldMeta *field_meta = table->table_meta().field(relation_attr.attribute_name.c_str());
      if (nullptr == field_meta) {
        LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), relation_attr.attribute_name.c_str());
        return RC::SCHEMA_FIELD_MISSING;
      }
      if ((field_meta->type() == CHARS || field_meta->type() == DATES) && relation_attr.aggre_type != AggreType::MAX && relation_attr.aggre_type != AggreType::MIN) {
        LOG_WARN("invalid aggregation function on DATES type.");
        return RC::INVALID_ARGUMENT;
      }
      query_fields.push_back(Field(table, field_meta));
      if (relation_attr.aggre_type != AggreType::NONE) {
        aggre_types.push_back(relation_attr.aggre_type);
      }
    }
  }

  LOG_INFO("got %d tables in from stmt and %d fields in query stmt", tables.size(), query_fields.size());

  Table *default_table = nullptr;
  if (tables.size() == 1) {
    default_table = tables[0];
  }

  // create filter statement in `where` statement
  FilterStmt *filter_stmt = nullptr;
  RC rc = FilterStmt::create(db,
      default_table,
      &table_map,
      select_sql.conditions.data(),
      static_cast<int>(select_sql.conditions.size()),
      filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot construct filter stmt");
    return rc;
  }

  // everything alright
  SelectStmt *select_stmt = new SelectStmt();
  // TODO add expression copy
  select_stmt->is_aggre_ = aggre_stat == 2;
  select_stmt->tables_.swap(tables);
  select_stmt->query_fields_.swap(query_fields);
  select_stmt->aggre_types_.swap(aggre_types);
  select_stmt->filter_stmt_ = filter_stmt;
  stmt = select_stmt;
  return RC::SUCCESS;
}
