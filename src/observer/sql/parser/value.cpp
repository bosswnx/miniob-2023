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
// Created by WangYunlai on 2023/06/28.
//

#include <sstream>
#include "sql/parser/value.h"
#include "common/log/log.h"
#include "common/lang/comparator.h"
#include "common/lang/string.h"
#include "value.h"

const char *ATTR_TYPE_NAME[] = {"undefined", "chars", "ints", "floats", "booleans", "dates"};

const char *attr_type_to_string(AttrType type)
{
  if (type >= UNDEFINED && type <= FLOATS) {
    return ATTR_TYPE_NAME[type];
  }
  return "unknown";
}
AttrType attr_type_from_string(const char *s)
{
  for (unsigned int i = 0; i < sizeof(ATTR_TYPE_NAME) / sizeof(ATTR_TYPE_NAME[0]); i++) {
    if (0 == strcmp(ATTR_TYPE_NAME[i], s)) {
      return (AttrType)i;
    }
  }
  return UNDEFINED;
}

Value::Value(int val)
{
  set_int(val);
}

Value::Value(float val)
{
  set_float(val);
}

Value::Value(bool val)
{
  set_boolean(val);
}

Value::Value(date val) { set_date(val); }
Value::Value(const char *s, int len /*= 0*/) { set_string(s, len); }

void Value::set_data(char *data, int length)
{
  switch (attr_type_) {
    case CHARS: {
      set_string(data, length);
    } break;
    case INTS: {
      num_value_.int_value_ = *(int *)data;
      length_ = length;
    } break;
    case FLOATS: {
      num_value_.float_value_ = *(float *)data;
      length_ = length;
    } break;
    case BOOLEANS: {
      num_value_.bool_value_ = *(int *)data != 0;
      length_ = length;
    } break;
    case DATES: {
      
      date_value_ = date(data);
      length_ = length;
    } break;
    default: {
      LOG_WARN("unknown data type: %d", attr_type_);
    } break;
  }
}
void Value::set_int(int val)
{
  attr_type_ = INTS;
  num_value_.int_value_ = val;
  length_ = sizeof(val);
}

void Value::set_float(float val)
{
  attr_type_ = FLOATS;
  num_value_.float_value_ = val;
  length_ = sizeof(val);
}
void Value::set_boolean(bool val)
{
  attr_type_ = BOOLEANS;
  num_value_.bool_value_ = val;
  length_ = sizeof(val);
}
void Value::set_date(date val)
{
  attr_type_ = DATES;
  date_value_ = val;
  length_ = sizeof(val.get_date_value());

}
void Value::set_string(const char *s, int len /*= 0*/)
{
  attr_type_ = CHARS;
  if (len > 0) {
    len = strnlen(s, len);
    str_value_.assign(s, len);
  } else {
    str_value_.assign(s);
  }
  length_ = str_value_.length();
}

void Value::set_value(const Value &value)
{
  switch (value.attr_type_) {
    case INTS: {
      set_int(value.get_int());
    } break;
    case FLOATS: {
      set_float(value.get_float());
    } break;
    case CHARS: {
      set_string(value.get_string().c_str());
    } break;
    case BOOLEANS: {
      set_boolean(value.get_boolean());
    } break;
    case DATES: {
      set_date(value.get_date());
    } break;
    case UNDEFINED: {
      ASSERT(false, "got an invalid value type");
    } break;
  }
}

const char *Value::data() const
{
  switch (attr_type_) {
    case CHARS: {
      return str_value_.c_str();
    } break;
    case DATES: {
      return (const char *)date_value_.get_date_value_addr();
    }
    default: {
      return (const char *)&num_value_;
    } break;
  }
}

std::string Value::to_string() const
{
  std::stringstream os;
  switch (attr_type_) {
    case INTS: {
      os << num_value_.int_value_;
    } break;
    case FLOATS: {
      os << common::double_to_str(num_value_.float_value_);
    } break;
    case BOOLEANS: {
      os << num_value_.bool_value_;
    } break;
    case DATES: {
      os << get_date_str();
    } break;
    case CHARS: {
      os << str_value_;
    } break;
    default: {
      LOG_WARN("unsupported attr type: %d", attr_type_);
    } break;
  }
  return os.str();
}

int Value::compare(const Value &other) const
{
  if (this->attr_type_ == other.attr_type_) {
    switch (this->attr_type_) {
      case INTS: {
        return common::compare_int((void *)&this->num_value_.int_value_, (void *)&other.num_value_.int_value_);
      } break;
      case FLOATS: {
        return common::compare_float((void *)&this->num_value_.float_value_, (void *)&other.num_value_.float_value_);
      } break;
      case DATES: {
        int32_t this_date = get_date().get_date_value();
        int32_t other_date = other.get_date().get_date_value();
        return common::compare_int((void *)&this_date, (void *)&other_date);
      } break;
      case CHARS: {
        return common::compare_string((void *)this->str_value_.c_str(),
            this->str_value_.length(),
            (void *)other.str_value_.c_str(),
            other.str_value_.length());
      } break;
      case BOOLEANS: {
        return common::compare_int((void *)&this->num_value_.bool_value_, (void *)&other.num_value_.bool_value_);
      }
      default: {
        LOG_WARN("unsupported type: %d", this->attr_type_);
      }
    }
  } else if (this->attr_type_ == INTS && other.attr_type_ == FLOATS) {
    float this_data = this->num_value_.int_value_;
    return common::compare_float((void *)&this_data, (void *)&other.num_value_.float_value_);
  } else if (this->attr_type_ == FLOATS && other.attr_type_ == INTS) {
    float other_data = other.num_value_.int_value_;
    return common::compare_float((void *)&this->num_value_.float_value_, (void *)&other_data);
  }
  LOG_WARN("not supported");
  return -1;  // TODO return rc?
}

// like 语句比较
bool Value::like(const Value &tmplt) const
{
  const char *s = this->data();
  const char *tmplt_s = tmplt.data();
  int s_len = this->length_;
  int tmplt_len = tmplt.length_;

  // fast-fail [deprecated]
  // 不能这样判断，因为tmplt可以有连续的%，比如%hello%
  // if (s_len < tmplt_len) {
  //   return false;
  // }

  int i = 0, j = 0; // i: tmplt_s; j: s
  bool is_wildcard = false; // %: 0 or more characters; _: 1 character

  // helloworld
  // h%   _orld

  while (j < s_len) {
    if (i < tmplt_len && tmplt_s[i] == '%') {
      is_wildcard = true;
      while (i < tmplt_len && tmplt_s[i] == '%') ++i; // skip continuous %
    }

    if (is_wildcard && i < tmplt_len && tmplt_s[i] == s[j]) {
      is_wildcard = false;
    }
  
    if (i < tmplt_len && s[j] != tmplt_s[i] && !is_wildcard && tmplt_s[i] != '_') {
      return false;
    }

    ++j;
    if (!is_wildcard) ++i;

    if (i >= tmplt_len && j < s_len && !is_wildcard) return false;
  }

  if (is_wildcard && i < tmplt_len && tmplt_s[i] != '%') {
    return false;
  }
  return true;
}

bool Value::check_date(date val) const {
  int year = 0;
  int month = 0;
  int day = 0;
  int32_t int_value = val.get_date_value();
  year = int_value / 10000;
  month = (int_value % 10000) / 100;
  day = int_value % 100;
  if((year < 1900 || year > 9999) || (month <= 0 || month > 12) || (day <= 0 || day > 31))
  {
    return false;
  }
  int max_day_in_month[] = {31,29,31,30,31,30,31,31,30,31,30,31};
  const int max_day = max_day_in_month[month - 1];
  if(day > max_day)
  {
    return false;
  }
  if (month == 2 && day > 28 && !val.is_leap_year(year))
  {
    return false;
  }
  return true;
}

int Value::get_int() const
{
  switch (attr_type_) {
    case CHARS: {
      try {
        return (int)(std::stol(str_value_));
      } catch (std::exception const &ex) {
        LOG_TRACE("failed to convert string to number. s=%s, ex=%s", str_value_.c_str(), ex.what());
        return 0;
      }
    } break;
    case INTS: {
      return num_value_.int_value_;
    } break;
    case FLOATS: {
      return (int)(num_value_.float_value_);
    } break;
    case DATES: {
      return get_date().get_date_value();
    } break;
    case BOOLEANS: {
      return (int)(num_value_.bool_value_);
    } break;
    default: {
      LOG_WARN("unknown data type. type=%d", attr_type_);
      return 0;
    } break;
  }
  return 0;
}

date Value::get_date() const
{
  return date_value_;
}

// ;get date string
std::string Value::get_date_str() const
{
  // date_value_ 转成 char* 类型。date_value_ 是一个date类型，本质是一个int32_t类型的数据, 存储方式：YYYYMMDD
  auto date_value = date_value_.get_date_value();
  int year = date_value / 10000;
  int month = (date_value % 10000) / 100;
  int day = date_value % 100;
  std::string res;
  res += std::to_string(year);
  if(month < 10)
  {
    res += "-0" + std::to_string(month);
  } else {
    res += "-" + std::to_string(month);
  }
  if(day < 10) {
    res += "-0" + std::to_string(day);
  } else {
    res += "-" + std::to_string(day);
  }
  return res;
}

float Value::get_float() const
{
  switch (attr_type_) {
    case CHARS: {
      try {
        return std::stof(str_value_);
      } catch (std::exception const &ex) {
        LOG_TRACE("failed to convert string to float. s=%s, ex=%s", str_value_.c_str(), ex.what());
        return 0.0;
      }
    } break;
    case INTS: {
      return float(num_value_.int_value_);
    } break;
    case FLOATS: {
      return num_value_.float_value_;
    } break;
    case DATES: {
      return float(get_date().get_date_value());
    }break;
    case BOOLEANS: {
      return float(num_value_.bool_value_);
    } break;
    default: {
      LOG_WARN("unknown data type. type=%d", attr_type_);
      return 0;
    }
  }
  return 0;
}

std::string Value::get_string() const
{
  return this->to_string();
}

bool Value::get_boolean() const
{
  switch (attr_type_) {
    case CHARS: {
      try {
        float val = std::stof(str_value_);
        if (val >= EPSILON || val <= -EPSILON) {
          return true;
        }

        int int_val = std::stol(str_value_);
        if (int_val != 0) {
          return true;
        }

        return !str_value_.empty();
      } catch (std::exception const &ex) {
        LOG_TRACE("failed to convert string to float or integer. s=%s, ex=%s", str_value_.c_str(), ex.what());
        return !str_value_.empty();
      }
    } break;
    case INTS: {
      return num_value_.int_value_ != 0;
    } break;
    case DATES :{
      
      return get_date().get_date_value() != 0;
    }
    case FLOATS: {
      float val = num_value_.float_value_;
      return val >= EPSILON || val <= -EPSILON;
    } break;
    case BOOLEANS: {
      return num_value_.bool_value_;
    } break;
    default: {
      LOG_WARN("unknown data type. type=%d", attr_type_);
      return false;
    }
  }
  return false;
}
