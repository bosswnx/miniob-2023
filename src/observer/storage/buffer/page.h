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
// Created by Wangyunlai on 2023/03/07.
//

#pragma once

#include <stdint.h>
#include "common/types.h"

using TrxID = int32_t;

static constexpr int BP_INVALID_PAGE_NUM = -1;

static constexpr PageNum BP_HEADER_PAGE   = 0;

static constexpr const int BP_PAGE_SIZE = (1 << 13);
static constexpr const int BP_PAGE_DATA_SIZE = (BP_PAGE_SIZE - sizeof(PageNum) - sizeof(LSN));
static constexpr const int BP_TEXTS_SIZE = 65535;  // TEXTS 类型的字段最大长度
static constexpr const int BP_TEXTS_FIELD_SIZE = (BP_TEXTS_SIZE / BP_PAGE_DATA_SIZE + 1) * 4 + 4;  // TEXTS 类型的字段在record中占用的字节数
static constexpr const int BP_TEXTS_PAGE_NUM = (BP_TEXTS_SIZE + BP_PAGE_DATA_SIZE - 1) / BP_PAGE_DATA_SIZE;  // TEXTS 类型的字段占用的页数

/**
 * @brief 表示一个页面，可能放在内存或磁盘上
 * @ingroup BufferPool
 */
struct Page
{
  PageNum page_num;
  LSN     lsn;
  char data[BP_PAGE_DATA_SIZE];
};
