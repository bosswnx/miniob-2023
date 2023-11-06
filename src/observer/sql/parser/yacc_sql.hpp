/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_YACC_SQL_HPP_INCLUDED
# define YY_YY_YACC_SQL_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    SEMICOLON = 258,               /* SEMICOLON  */
    CREATE = 259,                  /* CREATE  */
    DROP = 260,                    /* DROP  */
    TABLE = 261,                   /* TABLE  */
    TABLES = 262,                  /* TABLES  */
    INDEX = 263,                   /* INDEX  */
    CALC = 264,                    /* CALC  */
    SELECT = 265,                  /* SELECT  */
    DESC = 266,                    /* DESC  */
    SHOW = 267,                    /* SHOW  */
    SYNC = 268,                    /* SYNC  */
    INSERT = 269,                  /* INSERT  */
    DELETE = 270,                  /* DELETE  */
    UPDATE = 271,                  /* UPDATE  */
    LBRACE = 272,                  /* LBRACE  */
    RBRACE = 273,                  /* RBRACE  */
    COMMA = 274,                   /* COMMA  */
    TRX_BEGIN = 275,               /* TRX_BEGIN  */
    TRX_COMMIT = 276,              /* TRX_COMMIT  */
    TRX_ROLLBACK = 277,            /* TRX_ROLLBACK  */
    INT_T = 278,                   /* INT_T  */
    STRING_T = 279,                /* STRING_T  */
    DATE_T = 280,                  /* DATE_T  */
    FLOAT_T = 281,                 /* FLOAT_T  */
    HELP = 282,                    /* HELP  */
    EXIT = 283,                    /* EXIT  */
    DOT = 284,                     /* DOT  */
    INTO = 285,                    /* INTO  */
    VALUES = 286,                  /* VALUES  */
    FROM = 287,                    /* FROM  */
    INNER_JOIN = 288,              /* INNER_JOIN  */
    WHERE = 289,                   /* WHERE  */
    HAVING = 290,                  /* HAVING  */
    ORDER_BY = 291,                /* ORDER_BY  */
    GROUP_BY = 292,                /* GROUP_BY  */
    ASC = 293,                     /* ASC  */
    IN = 294,                      /* IN  */
    NI = 295,                      /* NI  */
    EXISTS = 296,                  /* EXISTS  */
    NOT_EXISTS = 297,              /* NOT_EXISTS  */
    AND = 298,                     /* AND  */
    OR = 299,                      /* OR  */
    SET = 300,                     /* SET  */
    ON = 301,                      /* ON  */
    LOAD = 302,                    /* LOAD  */
    DATA = 303,                    /* DATA  */
    INFILE = 304,                  /* INFILE  */
    NOT_NULL = 305,                /* NOT_NULL  */
    EXPLAIN = 306,                 /* EXPLAIN  */
    UNIQUE = 307,                  /* UNIQUE  */
    AS = 308,                      /* AS  */
    EQ = 309,                      /* EQ  */
    LT = 310,                      /* LT  */
    GT = 311,                      /* GT  */
    LE = 312,                      /* LE  */
    GE = 313,                      /* GE  */
    NE = 314,                      /* NE  */
    MAX = 315,                     /* MAX  */
    MIN = 316,                     /* MIN  */
    SUM = 317,                     /* SUM  */
    CNT = 318,                     /* CNT  */
    AVG = 319,                     /* AVG  */
    LK = 320,                      /* LK  */
    NLK = 321,                     /* NLK  */
    IS_NOT = 322,                  /* IS_NOT  */
    IS = 323,                      /* IS  */
    LENGTH = 324,                  /* LENGTH  */
    ROUND = 325,                   /* ROUND  */
    DATE_FORMAT = 326,             /* DATE_FORMAT  */
    P_INT = 327,                   /* P_INT  */
    P_FLOAT = 328,                 /* P_FLOAT  */
    ID = 329,                      /* ID  */
    DATE_STR = 330,                /* DATE_STR  */
    SSS = 331,                     /* SSS  */
    NULL_T = 332,                  /* NULL_T  */
    UMINUS = 333                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 128 "yacc_sql.y"

  ParsedSqlNode *                   sql_node;
  ConditionSqlNode *                condition;
  ConditionSqlNode *                join_condition;
  Value *                           value;
  enum CompOp                       comp;
  RelAttrSqlNode *                  rel_attr;
  RelationSqlNode *                 relation;
  std::vector<AttrInfoSqlNode> *    attr_infos;
  AttrInfoSqlNode *                 attr_info;
  UpdateTarget *                    update_target_t;
  Expression *                      expression;
  std::vector<Expression *> *       expression_list;
  std::vector<Value> *              value_list;
  std::vector<ConditionSqlNode> *   condition_list;
  std::vector<RelAttrSqlNode> *     rel_attr_list;
  std::vector<RelationSqlNode> *    relation_list;
  std::vector<RelAttrSqlNode> *     order_by_list;
  std::vector<RelAttrSqlNode> *     group_by_body;
  std::vector<RelAttrSqlNode> *     group_by_list;
  std::vector<RelAttrSqlNode> *     order_by_body;
  bool                              order_by_type;
  std::vector<std::string> *        ID_list;
  std::vector<JoinSqlNode> *        join_list;
  UpdateSqlNode *                   update_info;
  char *                            string;
  int                               number;
  float                             floats;
  AggreType                         aggre_type;

#line 173 "yacc_sql.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner);


#endif /* !YY_YY_YACC_SQL_HPP_INCLUDED  */
