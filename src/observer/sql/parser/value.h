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
// Created by Wangyunlai 2023/6/27
//

#pragma once

#include <cstdint>
#include <string>
#include "src/observer/common/rc.h"
#include <cstring>

/**
 * @brief 属性的类型
 * 
 */
enum AttrType
{
  UNDEFINED,
  CHARS,          ///< 字符串类型
  INTS,           ///< 整数类型(4字节)
  FLOATS,         ///< 浮点数类型(4字节)
  DATES,          ///< date int32_t
  BOOLEANS,       ///< boolean类型，当前不是由parser解析出来的，是程序内部使用的
  TEXTS,          ///< text类型
};

const char *attr_type_to_string(AttrType type);
AttrType attr_type_from_string(const char *s);


//class date
class date
{
 public:
  date() = default;
  date(int32_t val){ datevalue = val; }
  date(const char *str)
  {
    string_to_date(str);
    
  }
  const int32_t *get_date_value_addr() const { return &datevalue; }
  const int32_t get_date_value() const { return datevalue; }
  const int get_year() const { return datevalue / 10000; }
  const int get_month() const { return (datevalue % 10000) / 100; }
  const int get_day() const { return datevalue % 100; }
  const std::string get_month_english() const
  {
    std::string month_english[12] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
    return month_english[get_month() - 1];
  }
  const std::string get_day_english() const
  {
    std::string result = std::to_string(get_day());
    if (get_day() % 10 == 1) {
      result += "st";
    } else if (get_day() == 2) {
      result += "nd";
    } else if (get_day() == 3) {
      result += "rd";
    } else {
      result += "th";
    }
    return result;
  }
  void set_date_value(int32_t val){ datevalue = val; }
  /**
   * @brief 将字符串转换为date
   * 
   * @param str 
   * @param date 
   * @return RC 
   */
  inline RC string_to_date(const char *str)
  {  
    int year = 0;
    int month = 0;
    int day = 0;
    int ret = sscanf(str, "%d-%d-%d", &year, &month, &day);
    int32_t int_value = 0;
    if (ret != 3) // 其实就是等于 
    {
      memcpy(&int_value, str, sizeof(int32_t));
    } 
    else {
      int_value = year * 10000 + month * 100 + day;
    }
    set_date_value(int_value);
    return RC::SUCCESS;
  }

  /**
   * @brief 判断是否是闰年
   * 
   * @param year 
   * @return true 
   * @return false 
   */
  inline bool is_leap_year(int year)
  {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 ==0;
  }
private:
 int32_t datevalue;

};
// class date

/**
 * @brief 属性的值
 * 
 */
class Value 
{
public:
  Value() = default;

  Value(AttrType attr_type, char *data, int length = 4) : attr_type_(attr_type)
  {
    this->set_data(data, length);
  }

  explicit Value(int val);
  explicit Value(float val);
  explicit Value(bool val);
  explicit Value(date val);
  explicit Value(const char *s, int len = 0);
  explicit Value(bool is_null,bool mOOn){ set_null(is_null);}

  Value(const Value &other) = default;
  Value &operator=(const Value &other) = default;

  void set_type(AttrType type)
  {
    this->attr_type_ = type;
  }
  void set_data(char *data, int length);
  void set_data(const char *data, int length)
  {
    this->set_data(const_cast<char *>(data), length);
  }
  void set_int(int val);
  void set_float(float val);
  void set_boolean(bool val);
  void set_date(date val);
  void set_string(const char *s, int len = 0);
  void set_string(const std::string &s) { set_string(s.c_str()); }
  void set_null(bool is_null){ is_null_ = is_null; }
  void set_value(const Value &value);

  bool check_date(date val) const;

  // bool cast(AttrType type_);

  std::string to_string() const;

  int compare(const Value &other) const;

  /*
    * @brief 判断当前的值是否与模板匹配(like)
    * 
    * @param s
    * @param tmplt_s
    * @return true
    * @return false
    * @auther Soulter
  */
  bool like(const char *s, const char *tmplt_s) const;

  /*
    将字符串转换为整数（MySQL字符串和数字比较的坑：比较时会把字符串类型转成整数类型，从首字母开始，遇到非数字类型后终止。）
  */
  float db_str_to_float(const char *str) const;

  const char *data() const;
  int length() const
  {
    return length_;
  }

  AttrType attr_type() const
  {
    return attr_type_;
  }

  static Value max_value(AttrType type);
  static Value min_value(AttrType type);

public:
  /**
   * 获取对应的值
   * 如果当前的类型与期望获取的类型不符，就会执行转换操作
   */
  int get_int() const;
  float get_float() const;
  std::string get_string() const;
  bool get_boolean() const;
  date get_date() const;
  std::string get_date_str() const;
  bool get_null_or_() const{ return is_null_;};

  // 重载运算符 <
  bool operator<(const Value &other) const;
  bool operator>(const Value &other) const;
private:
  AttrType attr_type_ = UNDEFINED;
  int length_ = 0;

  union {
    int int_value_;
    float float_value_;
    bool bool_value_;
  } num_value_;
  date date_value_;
  std::string str_value_;
  bool is_null_ = false;
};