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

#include <math.h>
#include <sstream>
#include "sql/parser/value.h"
#include "common/log/log.h"
#include "common/lang/comparator.h"
#include "common/lang/string.h"
#include "value.h"
#include <cstring>

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
  is_null_ = false;
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
  is_null_ = false;
  num_value_.int_value_ = val;
  length_ = sizeof(val);
}

void Value::set_float(float val)
{
  attr_type_ = FLOATS;
  is_null_ = false;
  num_value_.float_value_ = val;
  length_ = sizeof(val);
}
void Value::set_boolean(bool val)
{
  attr_type_ = BOOLEANS;
  is_null_ = false;
  num_value_.bool_value_ = val;
  length_ = sizeof(val);
}
void Value::set_date(date val)
{
  attr_type_ = DATES;
  is_null_ = false;
  date_value_ = val;
  length_ = sizeof(val.get_date_value());

}
void Value::set_string(const char *s, int len /*= 0*/)
{
  attr_type_ = CHARS;
  is_null_ = false;
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
  if(value.get_null_or_() == true)
  {
    set_null(true);
    return;
  }
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
  if (get_null_or_() == true){
    os << "NULL";
    return os.str();
  }
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
  if (this->is_null_) {
    // null 是最小的
    if (other.is_null_) {
      return 0;
    } else {
      return -1;
    }
  } else if (other.is_null_) {
    return 1;
  }
  // if (this->is_null_ || other.is_null_) {
  //   return -1;
  // }
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
  } else if ((this->attr_type_ == CHARS && other.attr_type_ == FLOATS) || (this->attr_type_ == CHARS && other.attr_type_ == INTS)) {
    float this_value = db_str_to_float(this->str_value_.c_str());
    float other_value;
    if (other.attr_type_ == INTS) {
      other_value = other.num_value_.int_value_;
    } else {
      other_value = other.num_value_.float_value_;
    }
    return common::compare_float((void *)&this_value, (void *)&other_value);
  } else if ((this->attr_type_ == FLOATS && other.attr_type_ == CHARS) || (this->attr_type_ == INTS && other.attr_type_ == CHARS)) {
    float other_value = db_str_to_float(other.str_value_.c_str());
    float this_value;
    if (this->attr_type_ == INTS) {
      this_value = this->num_value_.int_value_;
    } else {
      this_value = this->num_value_.float_value_;
    }
    return common::compare_float((void *)&this_value, (void *)&other_value);
  } else if (this->attr_type_ == CHARS && other.attr_type_ == DATES) {
    int32_t this_date = get_date().get_date_value();
    int32_t other_date = other.get_date().get_date_value();
    return common::compare_int((void *)&this_date, (void *)&other_date);
  }

  // todo: 还要实现date等其他类型的比较
  LOG_WARN("not supported");
  return -1;  // TODO return rc?
}

float Value::db_str_to_float(const char *str) const {
    float this_value = 0;
    bool entering_dot = false;
    int dot_index = 1;
    for (int i = 0; i < strlen(str); i++) {
      if (str[i] >= 48 && str[i] <= 57) {
        if (!entering_dot) {
          this_value = this_value * 10 + str[i] - 48;
        } else {
          this_value += (str[i] - 48) * pow(0.1, dot_index++);
        }
      } else {
        if (str[i] == '.' && !entering_dot) {
          entering_dot = true;
        } else break;
      }
    }
    return this_value;
}

// like 语句比较
bool Value::like(const char* s, const char *tmplt_s) const {
  int s_len = strlen(s);
  int tmplt_len = strlen(tmplt_s);

  // fast-fail [deprecated]
  // 不能这样判断，因为tmplt可以有连续的%，比如%hello%
  // if (s_len < tmplt_len) {
  //   return false;
  // }

  int i = 0, j = 0; // i: tmplt_s; j: s
  bool is_wildcard = false; // %: 0 or more characters; _: 1 character
  int t = -1; // Initialize t to -1.

  while (j <= s_len) {
      if (i < tmplt_len && tmplt_s[i] == '%') {
          is_wildcard = true;
          t = j; // Remember the starting position for %
          while (i < tmplt_len && tmplt_s[i] == '%') ++i; // Skip continuous %

          if (i >= tmplt_len) return true;

          while (j <= s_len) {
              if (like(s + j, tmplt_s + i))
                  return true;
              ++j;
          }
      }

      if (is_wildcard && i < tmplt_len && (tmplt_s[i] == s[j] || tmplt_s[i] == '_')) {
          is_wildcard = false;
      } else if (!is_wildcard && i < tmplt_len && s[j] != tmplt_s[i] && tmplt_s[i] != '_') {
          return false;
      }

      ++j;
      if (!is_wildcard) ++i;

      if (i >= tmplt_len && j < s_len && !is_wildcard) {
          return false;
      }
  }

  if (i >= tmplt_len) return true; // If we reached the end of the template string

  return false;
}

bool Value::check_date(date val) const {
  
  int year = 0;
  int month = 0;
  int day = 0;
  int32_t int_value = val.get_date_value();
  year = int_value / 10000;
  month = (int_value % 10000) / 100;
  day = int_value % 100;
  if((year < 1000 || year > 9999) || (month <= 0 || month > 12) || (day <= 0 || day > 31))
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

Value Value::max_value(AttrType type) {
  Value value;
  switch (type) {
    case INTS: {
      value.set_int(2147483647);
    } break;
    case FLOATS: {
      value.set_float(3.40282e+038);
    } break;
    case DATES: {
      value.set_date(99991231);
    } break;
    default: {
      LOG_WARN("unsupported type: %d", type);
    }
  }
  return value;
}

Value Value::min_value(AttrType type) {
  Value value;
  switch (type) {
    case INTS: {
      value.set_int(-2147483648);
    } break;
    case FLOATS: {
      value.set_float(-3.40282e+038);
    } break;
    case DATES: {
      value.set_date(10000101);
    } break;
    default: {
      LOG_WARN("unsupported type: %d", type);
    }
  }
  return value;
}

int Value::get_int() const
{
  if (get_null_or_()){
    return 0;
  }
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
  if (get_null_or_()) {
    return 0;
  }
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

// overide operator
bool Value::operator<(const Value &other) const
{
  return compare(other) < 0;
}

bool Value::operator>(const Value &other) const
{
  return compare(other) > 0;
}
