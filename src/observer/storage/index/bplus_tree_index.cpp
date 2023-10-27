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
// Created by wangyunlai.wyl on 2021/5/19.
//

#include "storage/index/bplus_tree_index.h"
#include "common/log/log.h"
#include "storage/index/index.h"

BplusTreeIndex::~BplusTreeIndex() noexcept
{
  close();
}

RC BplusTreeIndex::create(const char *file_name, const IndexMeta &index_meta, const vector<FieldMeta> &field_metas)
{
  if (inited_) {
    // TODO: fix this log
    // LOG_WARN("Failed to create index due to the index has been created before. file_name:%s, index:%s, field:%s",
    //     file_name,
    //     index_meta.name(),
    //     index_meta.field());
    return RC::RECORD_OPENNED;
  }

  Index::init(index_meta, field_metas);

  vector<AttrType> types;
  vector<int> lens;
  for (const auto &field_meta : field_metas) {
    types.emplace_back(field_meta.type());
    lens.emplace_back(field_meta.len());
  }
  RC rc = index_handler_.create(file_name, index_meta.is_unique(), types, lens);
  if (RC::SUCCESS != rc) {
    // TODO: fix this log
    // LOG_WARN("Failed to create index_handler, file_name:%s, index:%s, field:%s, rc:%s",
    //     file_name,
    //     index_meta.name(),
    //     index_meta.field(),
    //     strrc(rc));
    return rc;
  }

  inited_ = true;
  // TODO: fix this log
  // LOG_INFO(
  //     "Successfully create index, file_name:%s, index:%s, field:%s", file_name, index_meta.name(), index_meta.field());
  return RC::SUCCESS;
}

RC BplusTreeIndex::open(const char *file_name, const IndexMeta &index_meta, const std::vector<FieldMeta> &field_metas)
{
  if (inited_) {
    // TODO: fix this log
    // LOG_WARN("Failed to open index due to the index has been initedd before. file_name:%s, index:%s, field:%s",
    //     file_name,
    //     index_meta.name(),
    //     index_meta.field());
    return RC::RECORD_OPENNED;
  }

  Index::init(index_meta, field_metas);

  RC rc = index_handler_.open(file_name);
  if (RC::SUCCESS != rc) {
    // TODO: fix this log
    // LOG_WARN("Failed to open index_handler, file_name:%s, index:%s, field:%s, rc:%s",
    //     file_name,
    //     index_meta.name(),
    //     index_meta.field(),
    //     strrc(rc));
    return rc;
  }

  inited_ = true;
  // TODO: fix this log
  // LOG_INFO(
  //     "Successfully open index, file_name:%s, index:%s, field:%s", file_name, index_meta.name(), index_meta.field());
  return RC::SUCCESS;
}

RC BplusTreeIndex::close()
{
  if (inited_) {
    // TODO: fix this log
    // LOG_INFO("Begin to close index, index:%s, field:%s", index_meta_.name(), index_meta_.field());
    index_handler_.close();
    inited_ = false;
  }
  LOG_INFO("Successfully close index.");
  return RC::SUCCESS;
}

RC BplusTreeIndex::insert_entry(const char *record, const RID *rid)
{
  // record 是整行记录的起始地址，需要加上字段的偏移量
  vector<const char *> user_keys;
  for (const auto &field_meta : field_metas_) {
    user_keys.emplace_back(record + field_meta.offset());
  }
  return index_handler_.insert_entry(user_keys, rid);
}

RC BplusTreeIndex::delete_entry(const char *record, const RID *rid)
{
  vector<const char *> user_keys;
  for (const auto &field_meta : field_metas_) {
    user_keys.emplace_back(record + field_meta.offset());
  }
  return index_handler_.delete_entry(user_keys, rid);
}

// RC BplusTreeIndex::update_entry(const RID *rid, const char *old_record_data)
// {
//   // 其实就是先删除再插入
//   // return index_handler_.update_entry(old_record_data + field_meta_.offset(), rid, values_with_field);
//   RC rc = index_handler_.delete_entry(old_record_data + field_meta_.offset(), rid);
//   if (rc != RC::SUCCESS) {
//     LOG_WARN("failed to delete entry. rc=%d:%s", rc, strrc(rc));
//     return rc;
//   }
//   rc = index_handler_.insert_entry(old_record_data + field_meta_.offset(), rid);
//   if (rc != RC::SUCCESS) {
//     LOG_WARN("failed to insert entry. rc=%d:%s", rc, strrc(rc));
//     return rc;
//   }
//   return RC::SUCCESS;
// }

IndexScanner *BplusTreeIndex::create_scanner(const char *left_key, int left_len, bool left_inclusive, const char *right_key,
    int right_len, bool right_inclusive) {
  vector<const char *> left_keys;
  vector<int> left_lens;
  vector<const char *> right_keys;
  vector<int> right_lens;
  left_keys.emplace_back(left_key);
  left_lens.emplace_back(left_len);
  right_keys.emplace_back(right_key);
  right_lens.emplace_back(right_len);
  return create_scanner(left_keys, left_lens, left_inclusive, right_keys, right_lens, right_inclusive);
}

IndexScanner *BplusTreeIndex::create_scanner(
    const vector<const char *> &left_keys, const vector<int> &left_lens, bool left_inclusive, const vector<const char *> &right_keys, const vector<int> &right_lens, bool right_inclusive)
{
  BplusTreeIndexScanner *index_scanner = new BplusTreeIndexScanner(index_handler_);
  RC rc = index_scanner->open(left_keys, left_lens, left_inclusive, right_keys, right_lens, right_inclusive);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open index scanner. rc=%d:%s", rc, strrc(rc));
    delete index_scanner;
    return nullptr;
  }
  return index_scanner;
}

RC BplusTreeIndex::sync()
{
  return index_handler_.sync();
}

////////////////////////////////////////////////////////////////////////////////
BplusTreeIndexScanner::BplusTreeIndexScanner(BplusTreeHandler &tree_handler) : tree_scanner_(tree_handler)
{}

BplusTreeIndexScanner::~BplusTreeIndexScanner() noexcept
{
  tree_scanner_.close();
}

RC BplusTreeIndexScanner::open(
    const vector<const char *> &left_keys, const vector<int> &left_lens, bool left_inclusive, const vector<const char *> &right_keys, const vector<int> &right_lens, bool right_inclusive)
{
  return tree_scanner_.open(left_keys, left_lens, left_inclusive, right_keys, right_lens, right_inclusive);
}

RC BplusTreeIndexScanner::next_entry(RID *rid)
{
  return tree_scanner_.next_entry(*rid);
}

RC BplusTreeIndexScanner::destroy()
{
  delete this;
  return RC::SUCCESS;
}
