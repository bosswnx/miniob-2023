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
// Created by Wangyunlai on 2022/07/05.
//

#pragma once

#include "storage/table/table.h"
#include "storage/field/field_meta.h"

/**
 * @brief 字段
 * 
 */
class Field // 存放有字段的数据
{
public:
  Field() = default;
  Field(const Table *table, const FieldMeta *field) : table_(table), field_(field) {}
  Field(const Table *table, const FieldMeta *field, const string &table_alias, const string &field_alias) : table_(table), field_(field), table_alias_(table_alias), field_alias_(field_alias) {}
  Field(const Table *table, const FieldMeta *field, const string &table_alias, const string &field_alias, const bool is_asc) : table_(table), field_(field), table_alias_(table_alias), field_alias_(field_alias), is_asc_(is_asc) {}

  Field(const Field &) = default;

  const Table *table() const { return table_; }
  const FieldMeta *meta() const{ return field_; }
  AttrType attr_type() const { return field_->type(); }
  const char *table_name() const { return table_->name(); }
  const char *field_name() const { return field_->name(); }
  const bool have_table_alias() const { return table_alias_.size(); }
  const char *table_alias() const { return table_alias_.c_str();}
  const bool have_field_alias() const { return !field_alias_.empty(); }
  const char *field_alias() const { return field_alias_.c_str(); }
  const bool is_null(){
    return field_->is_null();
  }
  void set_table(const Table *table) { this->table_ = table; }
  void set_field(const FieldMeta *field) { this->field_ = field; }

  void set_int(Record &record, int value);
  int  get_int(const Record &record);

  const char *get_data(const Record &record);

  const bool is_asc() const {return is_asc_;}

  //set asc
  void set_asc(bool is_asc) {is_asc_ = is_asc;}

private:
  const Table *table_ = nullptr;  // 字段所在的表
  string table_alias_;
  string field_alias_;
  const FieldMeta *field_ = nullptr;  // 字段的元数据
  bool is_asc_ = true;                // 是否升序
};
