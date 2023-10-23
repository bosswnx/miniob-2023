/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "yacc_sql.y"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "common/log/log.h"
#include "common/lang/string.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.hpp"
#include "sql/parser/lex_sql.h"
#include "sql/expr/expression.h"

using namespace std;

string token_name(const char *sql_string, YYLTYPE *llocp)
{
  return string(sql_string + llocp->first_column, llocp->last_column - llocp->first_column + 1);
}

int yyerror(YYLTYPE *llocp, const char *sql_string, ParsedSqlResult *sql_result, yyscan_t scanner, const char *msg)
{
  std::unique_ptr<ParsedSqlNode> error_sql_node = std::make_unique<ParsedSqlNode>(SCF_ERROR);
  error_sql_node->error.error_msg = msg;
  error_sql_node->error.line = llocp->first_line;
  error_sql_node->error.column = llocp->first_column;
  sql_result->add_sql_node(std::move(error_sql_node));
  return 0;
}

ArithmeticExpr *create_arithmetic_expression(ArithmeticExpr::Type type,
                                             Expression *left,
                                             Expression *right,
                                             const char *sql_string,
                                             YYLTYPE *llocp)
{
  ArithmeticExpr *expr = new ArithmeticExpr(type, left, right);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}


#line 115 "yacc_sql.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "yacc_sql.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SEMICOLON = 3,                  /* SEMICOLON  */
  YYSYMBOL_CREATE = 4,                     /* CREATE  */
  YYSYMBOL_DROP = 5,                       /* DROP  */
  YYSYMBOL_TABLE = 6,                      /* TABLE  */
  YYSYMBOL_TABLES = 7,                     /* TABLES  */
  YYSYMBOL_INDEX = 8,                      /* INDEX  */
  YYSYMBOL_CALC = 9,                       /* CALC  */
  YYSYMBOL_SELECT = 10,                    /* SELECT  */
  YYSYMBOL_DESC = 11,                      /* DESC  */
  YYSYMBOL_SHOW = 12,                      /* SHOW  */
  YYSYMBOL_SYNC = 13,                      /* SYNC  */
  YYSYMBOL_INSERT = 14,                    /* INSERT  */
  YYSYMBOL_DELETE = 15,                    /* DELETE  */
  YYSYMBOL_UPDATE = 16,                    /* UPDATE  */
  YYSYMBOL_LBRACE = 17,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 18,                    /* RBRACE  */
  YYSYMBOL_COMMA = 19,                     /* COMMA  */
  YYSYMBOL_TRX_BEGIN = 20,                 /* TRX_BEGIN  */
  YYSYMBOL_TRX_COMMIT = 21,                /* TRX_COMMIT  */
  YYSYMBOL_TRX_ROLLBACK = 22,              /* TRX_ROLLBACK  */
  YYSYMBOL_INT_T = 23,                     /* INT_T  */
  YYSYMBOL_STRING_T = 24,                  /* STRING_T  */
  YYSYMBOL_DATE_T = 25,                    /* DATE_T  */
  YYSYMBOL_FLOAT_T = 26,                   /* FLOAT_T  */
  YYSYMBOL_HELP = 27,                      /* HELP  */
  YYSYMBOL_EXIT = 28,                      /* EXIT  */
  YYSYMBOL_DOT = 29,                       /* DOT  */
  YYSYMBOL_INTO = 30,                      /* INTO  */
  YYSYMBOL_VALUES = 31,                    /* VALUES  */
  YYSYMBOL_FROM = 32,                      /* FROM  */
  YYSYMBOL_WHERE = 33,                     /* WHERE  */
  YYSYMBOL_AND = 34,                       /* AND  */
  YYSYMBOL_SET = 35,                       /* SET  */
  YYSYMBOL_ON = 36,                        /* ON  */
  YYSYMBOL_LOAD = 37,                      /* LOAD  */
  YYSYMBOL_DATA = 38,                      /* DATA  */
  YYSYMBOL_INFILE = 39,                    /* INFILE  */
  YYSYMBOL_EXPLAIN = 40,                   /* EXPLAIN  */
  YYSYMBOL_EQ = 41,                        /* EQ  */
  YYSYMBOL_LT = 42,                        /* LT  */
  YYSYMBOL_GT = 43,                        /* GT  */
  YYSYMBOL_LE = 44,                        /* LE  */
  YYSYMBOL_GE = 45,                        /* GE  */
  YYSYMBOL_NE = 46,                        /* NE  */
<<<<<<< HEAD
  YYSYMBOL_LK = 47,                        /* LK  */
  YYSYMBOL_NLK = 48,                       /* NLK  */
  YYSYMBOL_NUMBER = 49,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 50,                     /* FLOAT  */
  YYSYMBOL_ID = 51,                        /* ID  */
  YYSYMBOL_DATE_STR = 52,                  /* DATE_STR  */
  YYSYMBOL_SSS = 53,                       /* SSS  */
  YYSYMBOL_54_ = 54,                       /* '+'  */
  YYSYMBOL_55_ = 55,                       /* '-'  */
  YYSYMBOL_56_ = 56,                       /* '*'  */
  YYSYMBOL_57_ = 57,                       /* '/'  */
  YYSYMBOL_UMINUS = 58,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 59,                  /* $accept  */
  YYSYMBOL_commands = 60,                  /* commands  */
  YYSYMBOL_command_wrapper = 61,           /* command_wrapper  */
  YYSYMBOL_exit_stmt = 62,                 /* exit_stmt  */
  YYSYMBOL_help_stmt = 63,                 /* help_stmt  */
  YYSYMBOL_sync_stmt = 64,                 /* sync_stmt  */
  YYSYMBOL_begin_stmt = 65,                /* begin_stmt  */
  YYSYMBOL_commit_stmt = 66,               /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 67,             /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 68,           /* drop_table_stmt  */
  YYSYMBOL_show_tables_stmt = 69,          /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 70,           /* desc_table_stmt  */
  YYSYMBOL_create_index_stmt = 71,         /* create_index_stmt  */
  YYSYMBOL_drop_index_stmt = 72,           /* drop_index_stmt  */
  YYSYMBOL_create_table_stmt = 73,         /* create_table_stmt  */
  YYSYMBOL_attr_def_list = 74,             /* attr_def_list  */
  YYSYMBOL_attr_def = 75,                  /* attr_def  */
  YYSYMBOL_number = 76,                    /* number  */
  YYSYMBOL_type = 77,                      /* type  */
  YYSYMBOL_insert_stmt = 78,               /* insert_stmt  */
  YYSYMBOL_value_list = 79,                /* value_list  */
  YYSYMBOL_value = 80,                     /* value  */
  YYSYMBOL_delete_stmt = 81,               /* delete_stmt  */
  YYSYMBOL_update_stmt = 82,               /* update_stmt  */
  YYSYMBOL_select_stmt = 83,               /* select_stmt  */
  YYSYMBOL_calc_stmt = 84,                 /* calc_stmt  */
  YYSYMBOL_expression_list = 85,           /* expression_list  */
  YYSYMBOL_expression = 86,                /* expression  */
  YYSYMBOL_select_attr = 87,               /* select_attr  */
  YYSYMBOL_rel_attr = 88,                  /* rel_attr  */
  YYSYMBOL_attr_list = 89,                 /* attr_list  */
  YYSYMBOL_rel_list = 90,                  /* rel_list  */
  YYSYMBOL_where = 91,                     /* where  */
  YYSYMBOL_condition_list = 92,            /* condition_list  */
  YYSYMBOL_condition = 93,                 /* condition  */
  YYSYMBOL_comp_op = 94,                   /* comp_op  */
  YYSYMBOL_load_data_stmt = 95,            /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 96,              /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 97,         /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 98              /* opt_semicolon  */
=======
  YYSYMBOL_MAX = 47,                       /* MAX  */
  YYSYMBOL_MIN = 48,                       /* MIN  */
  YYSYMBOL_SUM = 49,                       /* SUM  */
  YYSYMBOL_CNT = 50,                       /* CNT  */
  YYSYMBOL_AVG = 51,                       /* AVG  */
  YYSYMBOL_NUMBER = 52,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 53,                     /* FLOAT  */
  YYSYMBOL_ID = 54,                        /* ID  */
  YYSYMBOL_DATE_STR = 55,                  /* DATE_STR  */
  YYSYMBOL_SSS = 56,                       /* SSS  */
  YYSYMBOL_57_ = 57,                       /* '+'  */
  YYSYMBOL_58_ = 58,                       /* '-'  */
  YYSYMBOL_59_ = 59,                       /* '*'  */
  YYSYMBOL_60_ = 60,                       /* '/'  */
  YYSYMBOL_UMINUS = 61,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 62,                  /* $accept  */
  YYSYMBOL_commands = 63,                  /* commands  */
  YYSYMBOL_command_wrapper = 64,           /* command_wrapper  */
  YYSYMBOL_exit_stmt = 65,                 /* exit_stmt  */
  YYSYMBOL_help_stmt = 66,                 /* help_stmt  */
  YYSYMBOL_sync_stmt = 67,                 /* sync_stmt  */
  YYSYMBOL_begin_stmt = 68,                /* begin_stmt  */
  YYSYMBOL_commit_stmt = 69,               /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 70,             /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 71,           /* drop_table_stmt  */
  YYSYMBOL_show_tables_stmt = 72,          /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 73,           /* desc_table_stmt  */
  YYSYMBOL_create_index_stmt = 74,         /* create_index_stmt  */
  YYSYMBOL_drop_index_stmt = 75,           /* drop_index_stmt  */
  YYSYMBOL_create_table_stmt = 76,         /* create_table_stmt  */
  YYSYMBOL_attr_def_list = 77,             /* attr_def_list  */
  YYSYMBOL_attr_def = 78,                  /* attr_def  */
  YYSYMBOL_number = 79,                    /* number  */
  YYSYMBOL_type = 80,                      /* type  */
  YYSYMBOL_insert_stmt = 81,               /* insert_stmt  */
  YYSYMBOL_value_list = 82,                /* value_list  */
  YYSYMBOL_value = 83,                     /* value  */
  YYSYMBOL_delete_stmt = 84,               /* delete_stmt  */
  YYSYMBOL_update_stmt = 85,               /* update_stmt  */
  YYSYMBOL_select_stmt = 86,               /* select_stmt  */
  YYSYMBOL_calc_stmt = 87,                 /* calc_stmt  */
  YYSYMBOL_expression_list = 88,           /* expression_list  */
  YYSYMBOL_expression = 89,                /* expression  */
  YYSYMBOL_select_attr = 90,               /* select_attr  */
  YYSYMBOL_aggre_attr = 91,                /* aggre_attr  */
  YYSYMBOL_aggre_list = 92,                /* aggre_list  */
  YYSYMBOL_rel_attr = 93,                  /* rel_attr  */
  YYSYMBOL_attr_list = 94,                 /* attr_list  */
  YYSYMBOL_rel_list = 95,                  /* rel_list  */
  YYSYMBOL_where = 96,                     /* where  */
  YYSYMBOL_condition_list = 97,            /* condition_list  */
  YYSYMBOL_condition = 98,                 /* condition  */
  YYSYMBOL_comp_op = 99,                   /* comp_op  */
  YYSYMBOL_load_data_stmt = 100,           /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 101,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 102,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 103             /* opt_semicolon  */
>>>>>>> dda9a4c (fix: parser)
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  72
/* YYLAST -- Last index in YYTABLE.  */
<<<<<<< HEAD
#define YYLAST   156

/* YYNTOKENS -- Number of terminals.  */
<<<<<<< HEAD
#define YYNTOKENS  59
=======
#define YYNTOKENS  60
>>>>>>> efbad1e (finish parser)
=======
#define YYLAST   192

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  62
>>>>>>> dda9a4c (fix: parser)
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
<<<<<<< HEAD
#define YYNRULES  93
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  167

/* YYMAXUTOK -- Last valid token kind.  */
<<<<<<< HEAD
#define YYMAXUTOK   309
=======
#define YYMAXUTOK   310
>>>>>>> efbad1e (finish parser)
=======
#define YYNRULES  105
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  202

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   312
>>>>>>> dda9a4c (fix: parser)


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
<<<<<<< HEAD
<<<<<<< HEAD
       2,     2,    56,    54,     2,    55,     2,    57,     2,     2,
=======
       2,     2,    57,    55,     2,    56,     2,    58,     2,     2,
>>>>>>> efbad1e (finish parser)
=======
       2,     2,    59,    57,     2,    58,     2,    60,     2,     2,
>>>>>>> dda9a4c (fix: parser)
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
<<<<<<< HEAD
      45,    46,    47,    48,    49,    50,    51,    52,    53,    58
=======
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
<<<<<<< HEAD
      59
>>>>>>> efbad1e (finish parser)
=======
      55,    56,    61
>>>>>>> dda9a4c (fix: parser)
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
<<<<<<< HEAD
       0,   177,   177,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   208,   214,   219,   225,   231,   237,   243,
     250,   256,   264,   278,   288,   307,   310,   323,   331,   341,
     344,   345,   346,   347,   350,   366,   369,   380,   384,   388,
     394,   402,   414,   429,   451,   461,   466,   477,   480,   483,
     486,   489,   493,   496,   504,   511,   523,   528,   539,   542,
     556,   559,   572,   575,   581,   584,   589,   596,   608,   620,
     632,   647,   648,   649,   650,   651,   652,   653,   654,   658,
     671,   679,   689,   690
=======
       0,   182,   182,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   213,   219,   224,   230,   236,   242,   248,
     255,   261,   269,   283,   293,   312,   315,   328,   336,   346,
     349,   350,   351,   352,   355,   371,   374,   385,   389,   393,
     399,   407,   419,   434,   456,   466,   471,   482,   485,   488,
     491,   494,   498,   501,   509,   518,   530,   533,   536,   539,
     542,   545,   555,   565,   575,   585,   599,   602,   611,   623,
<<<<<<< HEAD
     628,   635,   643,   646,   660,   663,   676,   679,   685,   688,
     693,   700,   712,   724,   736,   751,   752,   753,   754,   755,
     756,   760,   773,   781,   791,   792
>>>>>>> dda9a4c (fix: parser)
=======
     630,   638,   648,   651,   665,   668,   681,   684,   690,   693,
     698,   705,   717,   729,   741,   756,   757,   758,   759,   760,
     761,   765,   778,   786,   796,   797
>>>>>>> 7bf3cb6 (fix(yacc_sql.y): rel_attr aggre_type to NONE)
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "SEMICOLON", "CREATE",
  "DROP", "TABLE", "TABLES", "INDEX", "CALC", "SELECT", "DESC", "SHOW",
  "SYNC", "INSERT", "DELETE", "UPDATE", "LBRACE", "RBRACE", "COMMA",
  "TRX_BEGIN", "TRX_COMMIT", "TRX_ROLLBACK", "INT_T", "STRING_T", "DATE_T",
  "FLOAT_T", "HELP", "EXIT", "DOT", "INTO", "VALUES", "FROM", "WHERE",
  "AND", "SET", "ON", "LOAD", "DATA", "INFILE", "EXPLAIN", "EQ", "LT",
<<<<<<< HEAD
  "GT", "LE", "GE", "NE", "LK", "NLK", "NUMBER", "FLOAT", "ID", "DATE_STR",
  "SSS", "'+'", "'-'", "'*'", "'/'", "UMINUS", "$accept", "commands",
  "command_wrapper", "exit_stmt", "help_stmt", "sync_stmt", "begin_stmt",
  "commit_stmt", "rollback_stmt", "drop_table_stmt", "show_tables_stmt",
  "desc_table_stmt", "create_index_stmt", "drop_index_stmt",
  "create_table_stmt", "attr_def_list", "attr_def", "number", "type",
  "insert_stmt", "value_list", "value", "delete_stmt", "update_stmt",
  "select_stmt", "calc_stmt", "expression_list", "expression",
  "select_attr", "rel_attr", "attr_list", "rel_list", "where",
=======
  "GT", "LE", "GE", "NE", "MAX", "MIN", "SUM", "CNT", "AVG", "NUMBER",
  "FLOAT", "ID", "DATE_STR", "SSS", "'+'", "'-'", "'*'", "'/'", "UMINUS",
  "$accept", "commands", "command_wrapper", "exit_stmt", "help_stmt",
  "sync_stmt", "begin_stmt", "commit_stmt", "rollback_stmt",
  "drop_table_stmt", "show_tables_stmt", "desc_table_stmt",
  "create_index_stmt", "drop_index_stmt", "create_table_stmt",
  "attr_def_list", "attr_def", "number", "type", "insert_stmt",
  "value_list", "value", "delete_stmt", "update_stmt", "select_stmt",
  "calc_stmt", "expression_list", "expression", "select_attr",
  "aggre_attr", "aggre_list", "rel_attr", "attr_list", "rel_list", "where",
>>>>>>> dda9a4c (fix: parser)
  "condition_list", "condition", "comp_op", "load_data_stmt",
  "explain_stmt", "set_variable_stmt", "opt_semicolon", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

<<<<<<< HEAD
#define YYPACT_NINF (-100)
=======
#define YYPACT_NINF (-134)
>>>>>>> dda9a4c (fix: parser)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
<<<<<<< HEAD
      61,    -1,    78,    -7,   -47,   -36,    18,  -100,     1,   -26,
     -19,  -100,  -100,  -100,  -100,  -100,     2,     9,    61,    62,
      60,  -100,  -100,  -100,  -100,  -100,  -100,  -100,  -100,  -100,
    -100,  -100,  -100,  -100,  -100,  -100,  -100,  -100,  -100,  -100,
    -100,    16,    17,    28,    29,    -7,  -100,  -100,  -100,  -100,
      -7,  -100,  -100,    -5,    40,  -100,    53,    68,  -100,  -100,
      43,    44,    64,    56,    63,  -100,  -100,  -100,  -100,    83,
      69,  -100,    70,   -16,  -100,    -7,    -7,    -7,    -7,    -7,
      52,    57,    58,  -100,    73,    74,    59,   -23,    65,    66,
      71,    72,  -100,  -100,   -45,   -45,  -100,  -100,  -100,    92,
      68,    95,     8,  -100,    75,  -100,    84,    67,    94,    98,
    -100,    76,    74,  -100,   -23,   -24,   -24,  -100,    85,   -23,
     114,  -100,  -100,  -100,  -100,   104,    66,   106,    77,    92,
    -100,   107,  -100,  -100,  -100,  -100,  -100,  -100,  -100,  -100,
       8,     8,     8,    74,    79,    80,    94,  -100,   113,  -100,
     -23,   115,  -100,  -100,  -100,  -100,  -100,  -100,  -100,  -100,
     116,  -100,  -100,   107,  -100,  -100,  -100
=======
     107,     4,    42,   -13,    43,   -29,    22,  -134,    11,    15,
       9,  -134,  -134,  -134,  -134,  -134,    27,    36,   107,    88,
      92,  -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,
    -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,
    -134,    44,    45,    51,    52,   -13,  -134,  -134,  -134,  -134,
     -13,  -134,  -134,    -1,    79,    96,    97,    98,   108,    95,
    -134,    94,   112,   112,  -134,  -134,    78,    82,   102,    99,
     100,  -134,  -134,  -134,  -134,   116,   105,  -134,   109,    -5,
    -134,   -13,   -13,   -13,   -13,   -13,     6,    10,    12,    13,
      14,    84,    89,    43,  -134,  -134,   115,   117,   101,    48,
      93,   103,   104,   106,  -134,  -134,    20,    20,  -134,  -134,
    -134,   129,  -134,   129,  -134,   129,  -134,   129,  -134,   129,
    -134,   132,   112,   112,   135,   -33,  -134,   113,  -134,   123,
      -9,   137,   142,  -134,   -48,   143,   144,   145,   146,   147,
     114,   117,  -134,  -134,    48,    -8,    -8,  -134,   133,    48,
     160,  -134,  -134,  -134,  -134,   152,   103,   154,   119,   129,
    -134,  -134,  -134,  -134,  -134,   132,  -134,   155,  -134,  -134,
    -134,  -134,  -134,  -134,   -33,   -33,   -33,   117,   121,   124,
     137,  -134,   159,  -134,  -134,    48,   161,  -134,  -134,  -134,
    -134,  -134,  -134,  -134,  -134,   162,  -134,  -134,   155,  -134,
    -134,  -134
>>>>>>> dda9a4c (fix: parser)
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
       0,    26,    27,    28,    24,    23,     0,     0,     0,     0,
<<<<<<< HEAD
      92,    22,    21,    14,    15,    16,    17,     9,    10,    11,
      12,    13,     8,     5,     7,     6,     4,     3,    18,    19,
      20,     0,     0,     0,     0,     0,    47,    48,    49,    50,
       0,    63,    54,    55,    66,    64,     0,    68,    31,    30,
       0,     0,     0,     0,     0,    90,     1,    93,     2,     0,
       0,    29,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,    72,     0,     0,     0,     0,
       0,     0,    61,    56,    57,    58,    59,    60,    67,    70,
      68,     0,    74,    51,     0,    91,     0,     0,    35,     0,
      33,     0,    72,    69,     0,     0,     0,    73,    75,     0,
       0,    40,    41,    43,    42,    38,     0,     0,     0,    70,
      53,    45,    81,    82,    83,    84,    85,    88,    86,    87,
       0,     0,    74,    72,     0,     0,    35,    34,     0,    71,
       0,     0,    78,    80,    77,    79,    76,    52,    89,    39,
       0,    36,    32,    45,    44,    37,    46
=======
     104,    22,    21,    14,    15,    16,    17,     9,    10,    11,
      12,    13,     8,     5,     7,     6,     4,     3,    18,    19,
      20,     0,     0,     0,     0,     0,    47,    48,    49,    50,
       0,    63,    54,    55,     0,     0,     0,     0,     0,    79,
      81,     0,    76,    76,    31,    30,     0,     0,     0,     0,
       0,   102,     1,   105,     2,     0,     0,    29,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    64,     0,    86,     0,     0,
       0,     0,     0,     0,    61,    56,    57,    58,    59,    60,
      66,    82,    67,    82,    68,    82,    69,    82,    70,    82,
      80,    84,    76,    76,     0,    88,    51,     0,   103,     0,
       0,    35,     0,    33,     0,     0,     0,     0,     0,     0,
       0,    86,    77,    78,     0,     0,     0,    87,    89,     0,
       0,    40,    41,    43,    42,    38,     0,     0,     0,    82,
      71,    72,    73,    74,    75,    84,    53,    45,    95,    96,
      97,    98,    99,   100,     0,     0,    88,    86,     0,     0,
      35,    34,     0,    83,    85,     0,     0,    92,    94,    91,
      93,    90,    52,   101,    39,     0,    36,    32,    45,    44,
      37,    46
>>>>>>> dda9a4c (fix: parser)
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
<<<<<<< HEAD
    -100,  -100,   117,  -100,  -100,  -100,  -100,  -100,  -100,  -100,
    -100,  -100,  -100,  -100,  -100,   -21,     6,  -100,  -100,  -100,
     -25,   -86,  -100,  -100,  -100,  -100,    81,   -42,  -100,    -4,
      39,    11,   -99,     0,  -100,    25,  -100,  -100,  -100,  -100
=======
    -134,  -134,   163,  -134,  -134,  -134,  -134,  -134,  -134,  -134,
    -134,  -134,  -134,  -134,  -134,    -2,    26,  -134,  -134,  -134,
     -15,   -98,  -134,  -134,  -134,  -134,   110,    25,  -134,    91,
     -61,    -4,  -110,    21,  -133,    16,  -134,    39,  -134,  -134,
    -134,  -134
>>>>>>> dda9a4c (fix: parser)
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
<<<<<<< HEAD
      28,    29,    30,    31,    32,   127,   108,   160,   125,    33,
     151,    51,    34,    35,    36,    37,    52,    53,    56,   116,
      83,   112,   103,   117,   118,   140,    38,    39,    40,    68
=======
      28,    29,    30,    31,    32,   157,   131,   195,   155,    33,
     186,    51,    34,    35,    36,    37,    52,    53,    61,    62,
      94,   146,   135,   141,   126,   147,   148,   174,    38,    39,
      40,    74
>>>>>>> dda9a4c (fix: parser)
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
<<<<<<< HEAD
      57,   105,    92,    73,    54,    41,    61,    42,    74,    55,
      45,    78,    79,   130,    75,    58,   115,   132,   133,   134,
     135,   136,   137,   138,   139,    59,    46,    47,   131,    48,
      49,    60,    62,   143,    94,    95,    96,    97,    76,    77,
      78,    79,    46,    47,   157,    48,    49,    64,    50,    76,
      77,    78,    79,    63,   152,   154,   115,    46,    47,    54,
      48,    49,    66,    67,   163,     1,     2,    69,    70,    80,
       3,     4,     5,     6,     7,     8,     9,    10,   100,    71,
      72,    11,    12,    13,    43,    81,    44,    82,    14,    15,
     121,   122,   123,   124,    84,    85,    16,    87,    17,    86,
      89,    18,    88,    98,   101,    90,    91,   102,    99,    54,
     104,   111,   114,   126,   120,   128,   119,   107,   106,   142,
     144,   145,   109,   110,   147,   161,   150,   129,   148,   159,
     158,   162,   146,   164,   165,    65,   153,   155,   166,   113,
     149,   141,   156,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93
=======
      63,   128,    95,   136,    45,   137,    59,   138,   166,   139,
      41,    60,    42,   104,   151,   152,   153,   154,    81,    46,
      47,    59,    48,    49,   110,    64,    60,   145,   112,    65,
     114,   116,   118,   168,   169,   170,   171,   172,   173,    46,
      47,    66,    48,    49,   192,    50,   167,    67,    43,   183,
      44,   177,    82,    83,    84,    85,    82,    83,    84,    85,
      59,   142,   143,    68,    59,    60,    59,    59,    59,    60,
      79,    60,    60,    60,    70,    80,   187,   189,   145,    84,
      85,    69,   111,   113,   115,   117,   119,   198,    72,   123,
      54,    55,    56,    57,    58,    73,    86,    59,    75,    76,
      46,    47,    60,    48,    49,    77,    78,   106,   107,   108,
     109,     1,     2,    87,    88,    89,     3,     4,     5,     6,
       7,     8,     9,    10,    91,    90,    92,    11,    12,    13,
     159,    93,    96,   101,    14,    15,    97,    98,   120,   100,
      99,   102,    16,   121,    17,   103,   124,    18,   134,   129,
     125,   140,   144,   150,   149,   127,   156,   130,   132,   158,
     133,   160,   161,   162,   163,   164,   178,   176,   165,   179,
     188,   190,   181,   182,   185,   193,   194,   197,   196,   199,
     200,    71,   180,   201,   122,   175,   184,     0,     0,     0,
       0,   105,   191
>>>>>>> dda9a4c (fix: parser)
};

static const yytype_int16 yycheck[] =
{
<<<<<<< HEAD
       4,    87,    18,    45,    51,     6,    32,     8,    50,    56,
      17,    56,    57,   112,    19,    51,   102,    41,    42,    43,
      44,    45,    46,    47,    48,     7,    49,    50,   114,    52,
      53,    30,    51,   119,    76,    77,    78,    79,    54,    55,
      56,    57,    49,    50,   143,    52,    53,    38,    55,    54,
      55,    56,    57,    51,   140,   141,   142,    49,    50,    51,
      52,    53,     0,     3,   150,     4,     5,    51,    51,    29,
       9,    10,    11,    12,    13,    14,    15,    16,    82,    51,
      51,    20,    21,    22,     6,    32,     8,    19,    27,    28,
      23,    24,    25,    26,    51,    51,    35,    41,    37,    35,
      17,    40,    39,    51,    31,    36,    36,    33,    51,    51,
      51,    19,    17,    19,    30,    17,    41,    51,    53,    34,
       6,    17,    51,    51,    18,   146,    19,    51,    51,    49,
      51,    18,   126,    18,    18,    18,   140,   141,   163,   100,
     129,   116,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75
=======
       4,    99,    63,   113,    17,   115,    54,   117,   141,   119,
       6,    59,     8,    18,    23,    24,    25,    26,    19,    52,
      53,    54,    55,    56,    18,    54,    59,   125,    18,     7,
      18,    18,    18,    41,    42,    43,    44,    45,    46,    52,
      53,    30,    55,    56,   177,    58,   144,    32,     6,   159,
       8,   149,    57,    58,    59,    60,    57,    58,    59,    60,
      54,   122,   123,    54,    54,    59,    54,    54,    54,    59,
      45,    59,    59,    59,    38,    50,   174,   175,   176,    59,
      60,    54,    86,    87,    88,    89,    90,   185,     0,    93,
      47,    48,    49,    50,    51,     3,    17,    54,    54,    54,
      52,    53,    59,    55,    56,    54,    54,    82,    83,    84,
      85,     4,     5,    17,    17,    17,     9,    10,    11,    12,
      13,    14,    15,    16,    29,    17,    32,    20,    21,    22,
     134,    19,    54,    17,    27,    28,    54,    35,    54,    39,
      41,    36,    35,    54,    37,    36,    31,    40,    19,    56,
      33,    19,    17,    30,    41,    54,    19,    54,    54,    17,
      54,    18,    18,    18,    18,    18,     6,    34,    54,    17,
     174,   175,    18,    54,    19,    54,    52,    18,   180,    18,
      18,    18,   156,   198,    93,   146,   165,    -1,    -1,    -1,
      -1,    81,   176
>>>>>>> dda9a4c (fix: parser)
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,     5,     9,    10,    11,    12,    13,    14,    15,
<<<<<<< HEAD
      16,    20,    21,    22,    27,    28,    35,    37,    40,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    78,    81,    82,    83,    84,    95,    96,
      97,     6,     8,     6,     8,    17,    49,    50,    52,    53,
      55,    80,    85,    86,    51,    56,    87,    88,    51,     7,
      30,    32,    51,    51,    38,    61,     0,     3,    98,    51,
      51,    51,    51,    86,    86,    19,    54,    55,    56,    57,
      29,    32,    19,    89,    51,    51,    35,    41,    39,    17,
      36,    36,    18,    85,    86,    86,    86,    86,    51,    51,
      88,    31,    33,    91,    51,    80,    53,    51,    75,    51,
      51,    19,    90,    89,    17,    80,    88,    92,    93,    41,
      30,    23,    24,    25,    26,    77,    19,    74,    17,    51,
      91,    80,    41,    42,    43,    44,    45,    46,    47,    48,
      94,    94,    34,    80,     6,    17,    75,    18,    51,    90,
      19,    79,    80,    88,    80,    88,    92,    91,    51,    49,
      76,    74,    18,    80,    18,    18,    79
=======
      16,    20,    21,    22,    27,    28,    35,    37,    40,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    81,    84,    85,    86,    87,   100,   101,
     102,     6,     8,     6,     8,    17,    52,    53,    55,    56,
      58,    83,    88,    89,    47,    48,    49,    50,    51,    54,
      59,    90,    91,    93,    54,     7,    30,    32,    54,    54,
      38,    64,     0,     3,   103,    54,    54,    54,    54,    89,
      89,    19,    57,    58,    59,    60,    17,    17,    17,    17,
      17,    29,    32,    19,    92,    92,    54,    54,    35,    41,
      39,    17,    36,    36,    18,    88,    89,    89,    89,    89,
      18,    93,    18,    93,    18,    93,    18,    93,    18,    93,
      54,    54,    91,    93,    31,    33,    96,    54,    83,    56,
      54,    78,    54,    54,    19,    94,    94,    94,    94,    94,
      19,    95,    92,    92,    17,    83,    93,    97,    98,    41,
      30,    23,    24,    25,    26,    80,    19,    77,    17,    93,
      18,    18,    18,    18,    18,    54,    96,    83,    41,    42,
      43,    44,    45,    46,    99,    99,    34,    83,     6,    17,
      78,    18,    54,    94,    95,    19,    82,    83,    93,    83,
      93,    97,    96,    54,    52,    79,    77,    18,    83,    18,
      18,    82
>>>>>>> dda9a4c (fix: parser)
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
<<<<<<< HEAD
       0,    59,    60,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    74,    75,    75,    76,
      77,    77,    77,    77,    78,    79,    79,    80,    80,    80,
      80,    81,    82,    83,    84,    85,    85,    86,    86,    86,
      86,    86,    86,    86,    87,    87,    88,    88,    89,    89,
      90,    90,    91,    91,    92,    92,    92,    93,    93,    93,
      93,    94,    94,    94,    94,    94,    94,    94,    94,    95,
      96,    97,    98,    98
=======
       0,    62,    63,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    77,    78,    78,    79,
      80,    80,    80,    80,    81,    82,    82,    83,    83,    83,
      83,    84,    85,    86,    87,    88,    88,    89,    89,    89,
      89,    89,    89,    89,    90,    90,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    92,    92,    92,    93,
      93,    93,    94,    94,    95,    95,    96,    96,    97,    97,
      97,    98,    98,    98,    98,    99,    99,    99,    99,    99,
      99,   100,   101,   102,   103,   103
>>>>>>> dda9a4c (fix: parser)
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       2,     2,     8,     5,     7,     0,     3,     5,     2,     1,
       1,     1,     1,     1,     8,     0,     3,     1,     1,     1,
       1,     4,     7,     6,     2,     1,     3,     3,     3,     3,
<<<<<<< HEAD
       3,     3,     2,     1,     1,     2,     1,     3,     0,     3,
       0,     3,     0,     2,     0,     1,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     7,
       2,     4,     0,     1
=======
       3,     3,     2,     1,     2,     2,     3,     3,     3,     3,
       3,     5,     5,     5,     5,     5,     0,     3,     3,     1,
       3,     1,     0,     3,     0,     3,     0,     2,     0,     1,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     7,     2,     4,     0,     1
>>>>>>> dda9a4c (fix: parser)
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, sql_string, sql_result, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, sql_string, sql_result, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), sql_string, sql_result, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, sql_string, sql_result, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* commands: command_wrapper opt_semicolon  */
<<<<<<< HEAD
#line 178 "yacc_sql.y"
=======
#line 183 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
    sql_result->add_sql_node(std::move(sql_node));
  }
<<<<<<< HEAD
#line 1721 "yacc_sql.cpp"
    break;

  case 23: /* exit_stmt: EXIT  */
#line 208 "yacc_sql.y"
=======
#line 1753 "yacc_sql.cpp"
    break;

  case 23: /* exit_stmt: EXIT  */
#line 213 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
<<<<<<< HEAD
#line 1730 "yacc_sql.cpp"
    break;

  case 24: /* help_stmt: HELP  */
#line 214 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 1738 "yacc_sql.cpp"
    break;

  case 25: /* sync_stmt: SYNC  */
#line 219 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 1746 "yacc_sql.cpp"
    break;

  case 26: /* begin_stmt: TRX_BEGIN  */
#line 225 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 1754 "yacc_sql.cpp"
    break;

  case 27: /* commit_stmt: TRX_COMMIT  */
#line 231 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 1762 "yacc_sql.cpp"
    break;

  case 28: /* rollback_stmt: TRX_ROLLBACK  */
#line 237 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 1770 "yacc_sql.cpp"
    break;

  case 29: /* drop_table_stmt: DROP TABLE ID  */
#line 243 "yacc_sql.y"
=======
#line 1762 "yacc_sql.cpp"
    break;

  case 24: /* help_stmt: HELP  */
#line 219 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 1770 "yacc_sql.cpp"
    break;

  case 25: /* sync_stmt: SYNC  */
#line 224 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 1778 "yacc_sql.cpp"
    break;

  case 26: /* begin_stmt: TRX_BEGIN  */
#line 230 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 1786 "yacc_sql.cpp"
    break;

  case 27: /* commit_stmt: TRX_COMMIT  */
#line 236 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 1794 "yacc_sql.cpp"
    break;

  case 28: /* rollback_stmt: TRX_ROLLBACK  */
#line 242 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 1802 "yacc_sql.cpp"
    break;

  case 29: /* drop_table_stmt: DROP TABLE ID  */
#line 248 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      (yyval.sql_node)->drop_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
<<<<<<< HEAD
#line 1780 "yacc_sql.cpp"
    break;

  case 30: /* show_tables_stmt: SHOW TABLES  */
#line 250 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 1788 "yacc_sql.cpp"
    break;

  case 31: /* desc_table_stmt: DESC ID  */
#line 256 "yacc_sql.y"
=======
#line 1812 "yacc_sql.cpp"
    break;

  case 30: /* show_tables_stmt: SHOW TABLES  */
#line 255 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 1820 "yacc_sql.cpp"
    break;

  case 31: /* desc_table_stmt: DESC ID  */
#line 261 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
             {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      (yyval.sql_node)->desc_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
<<<<<<< HEAD
#line 1798 "yacc_sql.cpp"
    break;

  case 32: /* create_index_stmt: CREATE INDEX ID ON ID LBRACE ID RBRACE  */
#line 265 "yacc_sql.y"
=======
#line 1830 "yacc_sql.cpp"
    break;

  case 32: /* create_index_stmt: CREATE INDEX ID ON ID LBRACE ID RBRACE  */
#line 270 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
      create_index.index_name = (yyvsp[-5].string);
      create_index.relation_name = (yyvsp[-3].string);
      create_index.attribute_name = (yyvsp[-1].string);
      free((yyvsp[-5].string));
      free((yyvsp[-3].string));
      free((yyvsp[-1].string));
    }
<<<<<<< HEAD
#line 1813 "yacc_sql.cpp"
    break;

  case 33: /* drop_index_stmt: DROP INDEX ID ON ID  */
#line 279 "yacc_sql.y"
=======
#line 1845 "yacc_sql.cpp"
    break;

  case 33: /* drop_index_stmt: DROP INDEX ID ON ID  */
#line 284 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      (yyval.sql_node)->drop_index.index_name = (yyvsp[-2].string);
      (yyval.sql_node)->drop_index.relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
<<<<<<< HEAD
#line 1825 "yacc_sql.cpp"
    break;

  case 34: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE  */
#line 289 "yacc_sql.y"
=======
#line 1857 "yacc_sql.cpp"
    break;

  case 34: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE  */
#line 294 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-4].string);
      free((yyvsp[-4].string));

      std::vector<AttrInfoSqlNode> *src_attrs = (yyvsp[-1].attr_infos);

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
      }
      create_table.attr_infos.emplace_back(*(yyvsp[-2].attr_info));
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete (yyvsp[-2].attr_info);
    }
<<<<<<< HEAD
#line 1845 "yacc_sql.cpp"
    break;

  case 35: /* attr_def_list: %empty  */
#line 307 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 1853 "yacc_sql.cpp"
    break;

  case 36: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 311 "yacc_sql.y"
=======
#line 1877 "yacc_sql.cpp"
    break;

  case 35: /* attr_def_list: %empty  */
#line 312 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 1885 "yacc_sql.cpp"
    break;

  case 36: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 316 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(*(yyvsp[-1].attr_info));
      delete (yyvsp[-1].attr_info);
    }
<<<<<<< HEAD
#line 1867 "yacc_sql.cpp"
    break;

  case 37: /* attr_def: ID type LBRACE number RBRACE  */
#line 324 "yacc_sql.y"
=======
#line 1899 "yacc_sql.cpp"
    break;

  case 37: /* attr_def: ID type LBRACE number RBRACE  */
#line 329 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-3].number);
      (yyval.attr_info)->name = (yyvsp[-4].string);
      (yyval.attr_info)->length = (yyvsp[-1].number);
      free((yyvsp[-4].string));
    }
<<<<<<< HEAD
#line 1879 "yacc_sql.cpp"
    break;

  case 38: /* attr_def: ID type  */
#line 332 "yacc_sql.y"
=======
#line 1911 "yacc_sql.cpp"
    break;

  case 38: /* attr_def: ID type  */
#line 337 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[0].number);
      (yyval.attr_info)->name = (yyvsp[-1].string);
      (yyval.attr_info)->length = 4;
      free((yyvsp[-1].string));
    }
<<<<<<< HEAD
#line 1891 "yacc_sql.cpp"
    break;

  case 39: /* number: NUMBER  */
#line 341 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 1897 "yacc_sql.cpp"
    break;

  case 40: /* type: INT_T  */
#line 344 "yacc_sql.y"
               { (yyval.number)=INTS; }
#line 1903 "yacc_sql.cpp"
    break;

  case 41: /* type: STRING_T  */
#line 345 "yacc_sql.y"
               { (yyval.number)=CHARS; }
#line 1909 "yacc_sql.cpp"
    break;

  case 42: /* type: FLOAT_T  */
#line 346 "yacc_sql.y"
               { (yyval.number)=FLOATS; }
#line 1915 "yacc_sql.cpp"
    break;

  case 43: /* type: DATE_T  */
#line 347 "yacc_sql.y"
              { (yyval.number)=DATES; }
#line 1921 "yacc_sql.cpp"
    break;

  case 44: /* insert_stmt: INSERT INTO ID VALUES LBRACE value value_list RBRACE  */
#line 351 "yacc_sql.y"
=======
#line 1923 "yacc_sql.cpp"
    break;

  case 39: /* number: NUMBER  */
#line 346 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 1929 "yacc_sql.cpp"
    break;

  case 40: /* type: INT_T  */
#line 349 "yacc_sql.y"
               { (yyval.number)=INTS; }
#line 1935 "yacc_sql.cpp"
    break;

  case 41: /* type: STRING_T  */
#line 350 "yacc_sql.y"
               { (yyval.number)=CHARS; }
#line 1941 "yacc_sql.cpp"
    break;

  case 42: /* type: FLOAT_T  */
#line 351 "yacc_sql.y"
               { (yyval.number)=FLOATS; }
#line 1947 "yacc_sql.cpp"
    break;

  case 43: /* type: DATE_T  */
#line 352 "yacc_sql.y"
              { (yyval.number)=DATES; }
#line 1953 "yacc_sql.cpp"
    break;

  case 44: /* insert_stmt: INSERT INTO ID VALUES LBRACE value value_list RBRACE  */
#line 356 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_INSERT);
      (yyval.sql_node)->insertion.relation_name = (yyvsp[-5].string);
      if ((yyvsp[-1].value_list) != nullptr) {
        (yyval.sql_node)->insertion.values.swap(*(yyvsp[-1].value_list));
      }
      (yyval.sql_node)->insertion.values.emplace_back(*(yyvsp[-2].value));
      std::reverse((yyval.sql_node)->insertion.values.begin(), (yyval.sql_node)->insertion.values.end());
      delete (yyvsp[-2].value);
      free((yyvsp[-5].string));
    }
<<<<<<< HEAD
#line 1937 "yacc_sql.cpp"
    break;

  case 45: /* value_list: %empty  */
#line 366 "yacc_sql.y"
    {
      (yyval.value_list) = nullptr;
    }
#line 1945 "yacc_sql.cpp"
    break;

  case 46: /* value_list: COMMA value value_list  */
#line 369 "yacc_sql.y"
=======
#line 1969 "yacc_sql.cpp"
    break;

  case 45: /* value_list: %empty  */
#line 371 "yacc_sql.y"
    {
      (yyval.value_list) = nullptr;
    }
#line 1977 "yacc_sql.cpp"
    break;

  case 46: /* value_list: COMMA value value_list  */
#line 374 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
                              { 
      if ((yyvsp[0].value_list) != nullptr) {
        (yyval.value_list) = (yyvsp[0].value_list);
      } else {
        (yyval.value_list) = new std::vector<Value>;
      }
      (yyval.value_list)->emplace_back(*(yyvsp[-1].value));
      delete (yyvsp[-1].value);
    }
<<<<<<< HEAD
#line 1959 "yacc_sql.cpp"
    break;

  case 47: /* value: NUMBER  */
#line 380 "yacc_sql.y"
=======
#line 1991 "yacc_sql.cpp"
    break;

  case 47: /* value: NUMBER  */
#line 385 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
<<<<<<< HEAD
#line 1968 "yacc_sql.cpp"
    break;

  case 48: /* value: FLOAT  */
#line 384 "yacc_sql.y"
=======
#line 2000 "yacc_sql.cpp"
    break;

  case 48: /* value: FLOAT  */
#line 389 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
           {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
<<<<<<< HEAD
#line 1977 "yacc_sql.cpp"
    break;

  case 49: /* value: DATE_STR  */
#line 388 "yacc_sql.y"
=======
#line 2009 "yacc_sql.cpp"
    break;

  case 49: /* value: DATE_STR  */
#line 393 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
              {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.value) = new Value(date(tmp));
      free(tmp);
      (yyloc) = (yylsp[0]);
    }
<<<<<<< HEAD
#line 1988 "yacc_sql.cpp"
    break;

  case 50: /* value: SSS  */
#line 394 "yacc_sql.y"
=======
#line 2020 "yacc_sql.cpp"
    break;

  case 50: /* value: SSS  */
#line 399 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
         {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.value) = new Value(tmp);
      free(tmp);
    }
<<<<<<< HEAD
#line 1998 "yacc_sql.cpp"
    break;

  case 51: /* delete_stmt: DELETE FROM ID where  */
#line 403 "yacc_sql.y"
=======
#line 2030 "yacc_sql.cpp"
    break;

  case 51: /* delete_stmt: DELETE FROM ID where  */
#line 408 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      (yyval.sql_node)->deletion.relation_name = (yyvsp[-1].string);
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->deletion.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-1].string));
    }
<<<<<<< HEAD
#line 2012 "yacc_sql.cpp"
    break;

  case 52: /* update_stmt: UPDATE ID SET ID EQ value where  */
#line 415 "yacc_sql.y"
=======
#line 2044 "yacc_sql.cpp"
    break;

  case 52: /* update_stmt: UPDATE ID SET ID EQ value where  */
#line 420 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_UPDATE);
      (yyval.sql_node)->update.relation_name = (yyvsp[-5].string);
      (yyval.sql_node)->update.attribute_name = (yyvsp[-3].string);
      (yyval.sql_node)->update.value = *(yyvsp[-1].value);
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->update.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-5].string));
      free((yyvsp[-3].string));
    }
<<<<<<< HEAD
#line 2029 "yacc_sql.cpp"
    break;

  case 53: /* select_stmt: SELECT select_attr FROM ID rel_list where  */
#line 430 "yacc_sql.y"
=======
#line 2061 "yacc_sql.cpp"
    break;

  case 53: /* select_stmt: SELECT select_attr FROM ID rel_list where  */
#line 435 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SELECT);
      if ((yyvsp[-4].rel_attr_list) != nullptr) {
        (yyval.sql_node)->selection.attributes.swap(*(yyvsp[-4].rel_attr_list));
        delete (yyvsp[-4].rel_attr_list);
      }
      if ((yyvsp[-1].relation_list) != nullptr) {
        (yyval.sql_node)->selection.relations.swap(*(yyvsp[-1].relation_list));
        delete (yyvsp[-1].relation_list);
      }
      (yyval.sql_node)->selection.relations.push_back((yyvsp[-2].string));
      std::reverse((yyval.sql_node)->selection.relations.begin(), (yyval.sql_node)->selection.relations.end());

      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->selection.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-2].string));
    }
<<<<<<< HEAD
#line 2053 "yacc_sql.cpp"
    break;

  case 54: /* calc_stmt: CALC expression_list  */
#line 452 "yacc_sql.y"
=======
#line 2085 "yacc_sql.cpp"
    break;

  case 54: /* calc_stmt: CALC expression_list  */
#line 457 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      std::reverse((yyvsp[0].expression_list)->begin(), (yyvsp[0].expression_list)->end());
      (yyval.sql_node)->calc.expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
<<<<<<< HEAD
#line 2064 "yacc_sql.cpp"
    break;

  case 55: /* expression_list: expression  */
#line 462 "yacc_sql.y"
=======
#line 2096 "yacc_sql.cpp"
    break;

  case 55: /* expression_list: expression  */
#line 467 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
    {
      (yyval.expression_list) = new std::vector<Expression*>;
      (yyval.expression_list)->emplace_back((yyvsp[0].expression));
    }
<<<<<<< HEAD
#line 2073 "yacc_sql.cpp"
    break;

  case 56: /* expression_list: expression COMMA expression_list  */
#line 467 "yacc_sql.y"
=======
#line 2105 "yacc_sql.cpp"
    break;

  case 56: /* expression_list: expression COMMA expression_list  */
#line 472 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
    {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<Expression *>;
      }
      (yyval.expression_list)->emplace_back((yyvsp[-2].expression));
    }
<<<<<<< HEAD
#line 2086 "yacc_sql.cpp"
    break;

  case 57: /* expression: expression '+' expression  */
#line 477 "yacc_sql.y"
                              {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2094 "yacc_sql.cpp"
    break;

  case 58: /* expression: expression '-' expression  */
#line 480 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2102 "yacc_sql.cpp"
    break;

  case 59: /* expression: expression '*' expression  */
#line 483 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2110 "yacc_sql.cpp"
    break;

  case 60: /* expression: expression '/' expression  */
#line 486 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2118 "yacc_sql.cpp"
    break;

  case 61: /* expression: LBRACE expression RBRACE  */
#line 489 "yacc_sql.y"
=======
#line 2118 "yacc_sql.cpp"
    break;

  case 57: /* expression: expression '+' expression  */
#line 482 "yacc_sql.y"
                              {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2126 "yacc_sql.cpp"
    break;

  case 58: /* expression: expression '-' expression  */
#line 485 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2134 "yacc_sql.cpp"
    break;

  case 59: /* expression: expression '*' expression  */
#line 488 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2142 "yacc_sql.cpp"
    break;

  case 60: /* expression: expression '/' expression  */
#line 491 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2150 "yacc_sql.cpp"
    break;

  case 61: /* expression: LBRACE expression RBRACE  */
#line 494 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
                               {
      (yyval.expression) = (yyvsp[-1].expression);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
<<<<<<< HEAD
#line 2127 "yacc_sql.cpp"
    break;

  case 62: /* expression: '-' expression  */
#line 493 "yacc_sql.y"
                                  {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 2135 "yacc_sql.cpp"
    break;

  case 63: /* expression: value  */
#line 496 "yacc_sql.y"
=======
#line 2159 "yacc_sql.cpp"
    break;

  case 62: /* expression: '-' expression  */
#line 498 "yacc_sql.y"
                                  {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 2167 "yacc_sql.cpp"
    break;

  case 63: /* expression: value  */
#line 501 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
            {
      (yyval.expression) = new ValueExpr(*(yyvsp[0].value));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].value);
    }
<<<<<<< HEAD
#line 2145 "yacc_sql.cpp"
    break;

  case 64: /* select_attr: '*'  */
#line 504 "yacc_sql.y"
        {
      (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      RelAttrSqlNode attr;
      attr.relation_name  = "";
      attr.attribute_name = "*";
      (yyval.rel_attr_list)->emplace_back(attr);
    }
#line 2157 "yacc_sql.cpp"
    break;

  case 65: /* select_attr: rel_attr attr_list  */
#line 511 "yacc_sql.y"
                         {
=======
#line 2177 "yacc_sql.cpp"
    break;

  case 64: /* select_attr: rel_attr aggre_list  */
#line 509 "yacc_sql.y"
                        {
>>>>>>> dda9a4c (fix: parser)
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyval.rel_attr_list)->emplace_back(*(yyvsp[-1].rel_attr));
      delete (yyvsp[-1].rel_attr);
    }
<<<<<<< HEAD
#line 2171 "yacc_sql.cpp"
    break;

  case 66: /* rel_attr: ID  */
#line 523 "yacc_sql.y"
       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      (yyval.rel_attr)->aggre_type = AggreType::NONE;
      free((yyvsp[0].string));
    }
#line 2181 "yacc_sql.cpp"
    break;

  case 67: /* rel_attr: ID DOT ID  */
#line 528 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      (yyval.rel_attr)->aggre_type = AggreType::NONE;
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2193 "yacc_sql.cpp"
    break;

  case 66: /* rel_attr: MAX LBRACE ID RBRACE  */
#line 531 "yacc_sql.y"
                           {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
      (yyval.rel_attr)->aggre_type = AggreType::MAX;
      free((yyvsp[-1].string));
    }
#line 2206 "yacc_sql.cpp"
    break;

  case 67: /* rel_attr: MIN LBRACE ID RBRACE  */
#line 537 "yacc_sql.y"
                           {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
      (yyval.rel_attr)->aggre_type = AggreType::MIN;
      free((yyvsp[-1].string));
    }
#line 2217 "yacc_sql.cpp"
    break;

  case 68: /* rel_attr: CNT LBRACE ID RBRACE  */
#line 543 "yacc_sql.y"
                           {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
      (yyval.rel_attr)->aggre_type = AggreType::CNT;
      free((yyvsp[-1].string));
    }
#line 2228 "yacc_sql.cpp"
    break;

  case 69: /* rel_attr: SUM LBRACE ID RBRACE  */
#line 549 "yacc_sql.y"
                           {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
      (yyval.rel_attr)->aggre_type = AggreType::SUM;
      free((yyvsp[-1].string));
    }
#line 2239 "yacc_sql.cpp"
    break;

  case 70: /* rel_attr: AVG LBRACE ID RBRACE  */
#line 555 "yacc_sql.y"
                           {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[-1].string);
      (yyval.rel_attr)->aggre_type = AggreType::AVG;
      free((yyvsp[-1].string));
    }
#line 2250 "yacc_sql.cpp"
    break;

  case 71: /* rel_attr: MAX LBRACE '*' RBRACE  */
#line 561 "yacc_sql.y"
=======
#line 2191 "yacc_sql.cpp"
    break;

  case 65: /* select_attr: aggre_attr aggre_list  */
#line 518 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
                            {
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyval.rel_attr_list)->insert((yyval.rel_attr_list)->end(), (yyvsp[-1].rel_attr_list)->begin(), (yyvsp[-1].rel_attr_list)->end());
      delete (yyvsp[-1].rel_attr_list);
    }
#line 2205 "yacc_sql.cpp"
    break;

  case 66: /* aggre_attr: MAX LBRACE RBRACE  */
#line 530 "yacc_sql.y"
                      {
      (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
    }
#line 2213 "yacc_sql.cpp"
    break;

  case 67: /* aggre_attr: MIN LBRACE RBRACE  */
#line 533 "yacc_sql.y"
                        {
      (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
    }
#line 2221 "yacc_sql.cpp"
    break;

  case 68: /* aggre_attr: SUM LBRACE RBRACE  */
#line 536 "yacc_sql.y"
                        {
      (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
    }
#line 2229 "yacc_sql.cpp"
    break;

  case 69: /* aggre_attr: CNT LBRACE RBRACE  */
#line 539 "yacc_sql.y"
                        {
      (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
    }
#line 2237 "yacc_sql.cpp"
    break;

  case 70: /* aggre_attr: AVG LBRACE RBRACE  */
#line 542 "yacc_sql.y"
                        {
      (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
    }
#line 2245 "yacc_sql.cpp"
    break;

  case 71: /* aggre_attr: MAX LBRACE rel_attr attr_list RBRACE  */
#line 545 "yacc_sql.y"
                                           {
      if ((yyvsp[-1].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[-1].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyvsp[-2].rel_attr)->aggre_type = AggreType::MAX;
      (yyval.rel_attr_list)->emplace_back(*(yyvsp[-2].rel_attr));
      delete (yyvsp[-2].rel_attr);
    }
#line 2260 "yacc_sql.cpp"
    break;

  case 72: /* aggre_attr: MIN LBRACE rel_attr attr_list RBRACE  */
#line 555 "yacc_sql.y"
                                           {
      if ((yyvsp[-1].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[-1].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyvsp[-2].rel_attr)->aggre_type = AggreType::MIN;
      (yyval.rel_attr_list)->emplace_back(*(yyvsp[-2].rel_attr));
      delete (yyvsp[-2].rel_attr);
    }
#line 2275 "yacc_sql.cpp"
    break;

  case 73: /* aggre_attr: SUM LBRACE rel_attr attr_list RBRACE  */
#line 565 "yacc_sql.y"
                                           {
      if ((yyvsp[-1].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[-1].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyvsp[-2].rel_attr)->aggre_type = AggreType::SUM;
      (yyval.rel_attr_list)->emplace_back(*(yyvsp[-2].rel_attr));
      delete (yyvsp[-2].rel_attr);
    }
#line 2290 "yacc_sql.cpp"
    break;

  case 74: /* aggre_attr: CNT LBRACE rel_attr attr_list RBRACE  */
#line 575 "yacc_sql.y"
                                           {
      if ((yyvsp[-1].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[-1].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyvsp[-2].rel_attr)->aggre_type = AggreType::CNT;
      (yyval.rel_attr_list)->emplace_back(*(yyvsp[-2].rel_attr));
      delete (yyvsp[-2].rel_attr);
    }
#line 2305 "yacc_sql.cpp"
    break;

  case 75: /* aggre_attr: AVG LBRACE rel_attr attr_list RBRACE  */
#line 585 "yacc_sql.y"
                                           {
      if ((yyvsp[-1].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[-1].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyvsp[-2].rel_attr)->aggre_type = AggreType::AVG;
      (yyval.rel_attr_list)->emplace_back(*(yyvsp[-2].rel_attr));
      delete (yyvsp[-2].rel_attr);
    }
#line 2320 "yacc_sql.cpp"
    break;

  case 76: /* aggre_list: %empty  */
#line 599 "yacc_sql.y"
    {
      (yyval.rel_attr_list) = nullptr;
    }
<<<<<<< HEAD
#line 2201 "yacc_sql.cpp"
    break;

  case 69: /* attr_list: COMMA rel_attr attr_list  */
#line 542 "yacc_sql.y"
=======
#line 2328 "yacc_sql.cpp"
    break;

  case 77: /* aggre_list: COMMA aggre_attr aggre_list  */
#line 602 "yacc_sql.y"
                                  {
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyval.rel_attr_list)->insert((yyval.rel_attr_list)->end(), (yyvsp[-1].rel_attr_list)->begin(), (yyvsp[-1].rel_attr_list)->end());
      delete (yyvsp[-1].rel_attr_list);
    }
#line 2342 "yacc_sql.cpp"
    break;

  case 78: /* aggre_list: COMMA rel_attr aggre_list  */
#line 611 "yacc_sql.y"
                                {
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyval.rel_attr_list)->emplace_back(*(yyvsp[-1].rel_attr));
      delete (yyvsp[-1].rel_attr);
    }
#line 2356 "yacc_sql.cpp"
    break;

  case 79: /* rel_attr: ID  */
#line 623 "yacc_sql.y"
       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name = "";
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      (yyval.rel_attr)->aggre_type = AggreType::NONE;
      free((yyvsp[0].string));
    }
#line 2368 "yacc_sql.cpp"
    break;

  case 80: /* rel_attr: ID DOT ID  */
#line 630 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      (yyval.rel_attr)->aggre_type = AggreType::NONE;
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2381 "yacc_sql.cpp"
    break;

  case 81: /* rel_attr: '*'  */
#line 638 "yacc_sql.y"
          {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = "";
      (yyval.rel_attr)->attribute_name = "*";
      (yyval.rel_attr)->aggre_type = AggreType::NONE;
    }
#line 2392 "yacc_sql.cpp"
    break;

  case 82: /* attr_list: %empty  */
#line 648 "yacc_sql.y"
    {
      (yyval.rel_attr_list) = nullptr;
    }
#line 2400 "yacc_sql.cpp"
    break;

  case 83: /* attr_list: COMMA rel_attr attr_list  */
<<<<<<< HEAD
#line 646 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
=======
#line 651 "yacc_sql.y"
>>>>>>> 7bf3cb6 (fix(yacc_sql.y): rel_attr aggre_type to NONE)
                               {
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }

      (yyval.rel_attr_list)->emplace_back(*(yyvsp[-1].rel_attr));
      delete (yyvsp[-1].rel_attr);
    }
<<<<<<< HEAD
<<<<<<< HEAD
#line 2216 "yacc_sql.cpp"
    break;

  case 70: /* rel_list: %empty  */
#line 556 "yacc_sql.y"
    {
      (yyval.relation_list) = nullptr;
    }
#line 2224 "yacc_sql.cpp"
    break;

  case 71: /* rel_list: COMMA ID rel_list  */
#line 559 "yacc_sql.y"
=======
#line 2410 "yacc_sql.cpp"
=======
#line 2415 "yacc_sql.cpp"
>>>>>>> 7bf3cb6 (fix(yacc_sql.y): rel_attr aggre_type to NONE)
    break;

  case 84: /* rel_list: %empty  */
#line 665 "yacc_sql.y"
    {
      (yyval.relation_list) = nullptr;
    }
#line 2423 "yacc_sql.cpp"
    break;

  case 85: /* rel_list: COMMA ID rel_list  */
<<<<<<< HEAD
#line 663 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
=======
#line 668 "yacc_sql.y"
>>>>>>> 7bf3cb6 (fix(yacc_sql.y): rel_attr aggre_type to NONE)
                        {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<std::string>;
      }

      (yyval.relation_list)->push_back((yyvsp[-1].string));
      free((yyvsp[-1].string));
    }
<<<<<<< HEAD
<<<<<<< HEAD
#line 2239 "yacc_sql.cpp"
    break;

  case 72: /* where: %empty  */
#line 572 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2247 "yacc_sql.cpp"
    break;

  case 73: /* where: WHERE condition_list  */
#line 575 "yacc_sql.y"
                           {
      (yyval.condition_list) = (yyvsp[0].condition_list);  
    }
#line 2255 "yacc_sql.cpp"
    break;

  case 74: /* condition_list: %empty  */
#line 581 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2263 "yacc_sql.cpp"
    break;

  case 75: /* condition_list: condition  */
#line 584 "yacc_sql.y"
=======
#line 2433 "yacc_sql.cpp"
=======
#line 2438 "yacc_sql.cpp"
>>>>>>> 7bf3cb6 (fix(yacc_sql.y): rel_attr aggre_type to NONE)
    break;

  case 86: /* where: %empty  */
#line 681 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2446 "yacc_sql.cpp"
    break;

  case 87: /* where: WHERE condition_list  */
#line 684 "yacc_sql.y"
                           {
      (yyval.condition_list) = (yyvsp[0].condition_list);  
    }
#line 2454 "yacc_sql.cpp"
    break;

  case 88: /* condition_list: %empty  */
#line 690 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2462 "yacc_sql.cpp"
    break;

  case 89: /* condition_list: condition  */
<<<<<<< HEAD
#line 688 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
=======
#line 693 "yacc_sql.y"
>>>>>>> 7bf3cb6 (fix(yacc_sql.y): rel_attr aggre_type to NONE)
                {
      (yyval.condition_list) = new std::vector<ConditionSqlNode>;
      (yyval.condition_list)->emplace_back(*(yyvsp[0].condition));
      delete (yyvsp[0].condition);
    }
<<<<<<< HEAD
<<<<<<< HEAD
#line 2273 "yacc_sql.cpp"
    break;

  case 76: /* condition_list: condition AND condition_list  */
#line 589 "yacc_sql.y"
=======
#line 2467 "yacc_sql.cpp"
    break;

  case 90: /* condition_list: condition AND condition_list  */
#line 693 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
=======
#line 2472 "yacc_sql.cpp"
    break;

  case 90: /* condition_list: condition AND condition_list  */
#line 698 "yacc_sql.y"
>>>>>>> 7bf3cb6 (fix(yacc_sql.y): rel_attr aggre_type to NONE)
                                   {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyval.condition_list)->emplace_back(*(yyvsp[-2].condition));
      delete (yyvsp[-2].condition);
    }
<<<<<<< HEAD
<<<<<<< HEAD
#line 2283 "yacc_sql.cpp"
    break;

  case 77: /* condition: rel_attr comp_op value  */
#line 597 "yacc_sql.y"
=======
#line 2477 "yacc_sql.cpp"
    break;

  case 91: /* condition: rel_attr comp_op value  */
#line 701 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
=======
#line 2482 "yacc_sql.cpp"
    break;

  case 91: /* condition: rel_attr comp_op value  */
#line 706 "yacc_sql.y"
>>>>>>> 7bf3cb6 (fix(yacc_sql.y): rel_attr aggre_type to NONE)
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 1;
      (yyval.condition)->left_attr = *(yyvsp[-2].rel_attr);
      (yyval.condition)->right_is_attr = 0;
      (yyval.condition)->right_value = *(yyvsp[0].value);
      (yyval.condition)->comp = (yyvsp[-1].comp);

      delete (yyvsp[-2].rel_attr);
      delete (yyvsp[0].value);
    }
<<<<<<< HEAD
<<<<<<< HEAD
#line 2299 "yacc_sql.cpp"
    break;

  case 78: /* condition: value comp_op value  */
#line 609 "yacc_sql.y"
=======
#line 2493 "yacc_sql.cpp"
    break;

  case 92: /* condition: value comp_op value  */
#line 713 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
=======
#line 2498 "yacc_sql.cpp"
    break;

  case 92: /* condition: value comp_op value  */
#line 718 "yacc_sql.y"
>>>>>>> 7bf3cb6 (fix(yacc_sql.y): rel_attr aggre_type to NONE)
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 0;
      (yyval.condition)->left_value = *(yyvsp[-2].value);
      (yyval.condition)->right_is_attr = 0;
      (yyval.condition)->right_value = *(yyvsp[0].value);
      (yyval.condition)->comp = (yyvsp[-1].comp);

      delete (yyvsp[-2].value);
      delete (yyvsp[0].value);
    }
<<<<<<< HEAD
<<<<<<< HEAD
#line 2315 "yacc_sql.cpp"
    break;

  case 79: /* condition: rel_attr comp_op rel_attr  */
#line 621 "yacc_sql.y"
=======
#line 2509 "yacc_sql.cpp"
    break;

  case 93: /* condition: rel_attr comp_op rel_attr  */
#line 725 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
=======
#line 2514 "yacc_sql.cpp"
    break;

  case 93: /* condition: rel_attr comp_op rel_attr  */
#line 730 "yacc_sql.y"
>>>>>>> 7bf3cb6 (fix(yacc_sql.y): rel_attr aggre_type to NONE)
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 1;
      (yyval.condition)->left_attr = *(yyvsp[-2].rel_attr);
      (yyval.condition)->right_is_attr = 1;
      (yyval.condition)->right_attr = *(yyvsp[0].rel_attr);
      (yyval.condition)->comp = (yyvsp[-1].comp);

      delete (yyvsp[-2].rel_attr);
      delete (yyvsp[0].rel_attr);
    }
<<<<<<< HEAD
<<<<<<< HEAD
#line 2331 "yacc_sql.cpp"
    break;

  case 80: /* condition: value comp_op rel_attr  */
#line 633 "yacc_sql.y"
=======
#line 2525 "yacc_sql.cpp"
    break;

  case 94: /* condition: value comp_op rel_attr  */
#line 737 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
=======
#line 2530 "yacc_sql.cpp"
    break;

  case 94: /* condition: value comp_op rel_attr  */
#line 742 "yacc_sql.y"
>>>>>>> 7bf3cb6 (fix(yacc_sql.y): rel_attr aggre_type to NONE)
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 0;
      (yyval.condition)->left_value = *(yyvsp[-2].value);
      (yyval.condition)->right_is_attr = 1;
      (yyval.condition)->right_attr = *(yyvsp[0].rel_attr);
      (yyval.condition)->comp = (yyvsp[-1].comp);

      delete (yyvsp[-2].value);
      delete (yyvsp[0].rel_attr);
    }
<<<<<<< HEAD
<<<<<<< HEAD
#line 2347 "yacc_sql.cpp"
    break;

  case 81: /* comp_op: EQ  */
#line 647 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 2353 "yacc_sql.cpp"
    break;

  case 82: /* comp_op: LT  */
#line 648 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 2359 "yacc_sql.cpp"
    break;

  case 83: /* comp_op: GT  */
#line 649 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 2365 "yacc_sql.cpp"
    break;

  case 84: /* comp_op: LE  */
#line 650 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 2371 "yacc_sql.cpp"
    break;

  case 85: /* comp_op: GE  */
#line 651 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 2377 "yacc_sql.cpp"
    break;

  case 86: /* comp_op: LK  */
#line 652 "yacc_sql.y"
         { (yyval.comp) = LIKE; }
#line 2383 "yacc_sql.cpp"
    break;

  case 87: /* comp_op: NLK  */
#line 653 "yacc_sql.y"
          { (yyval.comp) = NOT_LIKE; }
#line 2389 "yacc_sql.cpp"
    break;

  case 88: /* comp_op: NE  */
#line 654 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 2395 "yacc_sql.cpp"
    break;

  case 89: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE ID  */
#line 659 "yacc_sql.y"
=======
#line 2541 "yacc_sql.cpp"
=======
#line 2546 "yacc_sql.cpp"
>>>>>>> 7bf3cb6 (fix(yacc_sql.y): rel_attr aggre_type to NONE)
    break;

  case 95: /* comp_op: EQ  */
#line 756 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 2552 "yacc_sql.cpp"
    break;

  case 96: /* comp_op: LT  */
#line 757 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 2558 "yacc_sql.cpp"
    break;

  case 97: /* comp_op: GT  */
#line 758 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 2564 "yacc_sql.cpp"
    break;

  case 98: /* comp_op: LE  */
#line 759 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 2570 "yacc_sql.cpp"
    break;

  case 99: /* comp_op: GE  */
#line 760 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 2576 "yacc_sql.cpp"
    break;

  case 100: /* comp_op: NE  */
#line 761 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 2582 "yacc_sql.cpp"
    break;

  case 101: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE ID  */
<<<<<<< HEAD
#line 761 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
=======
#line 766 "yacc_sql.y"
>>>>>>> 7bf3cb6 (fix(yacc_sql.y): rel_attr aggre_type to NONE)
    {
      char *tmp_file_name = common::substr((yyvsp[-3].string), 1, strlen((yyvsp[-3].string)) - 2);
      
      (yyval.sql_node) = new ParsedSqlNode(SCF_LOAD_DATA);
      (yyval.sql_node)->load_data.relation_name = (yyvsp[0].string);
      (yyval.sql_node)->load_data.file_name = tmp_file_name;
      free((yyvsp[0].string));
      free(tmp_file_name);
    }
<<<<<<< HEAD
<<<<<<< HEAD
#line 2409 "yacc_sql.cpp"
    break;

  case 90: /* explain_stmt: EXPLAIN command_wrapper  */
#line 672 "yacc_sql.y"
=======
#line 2591 "yacc_sql.cpp"
    break;

  case 102: /* explain_stmt: EXPLAIN command_wrapper  */
#line 774 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
=======
#line 2596 "yacc_sql.cpp"
    break;

  case 102: /* explain_stmt: EXPLAIN command_wrapper  */
#line 779 "yacc_sql.y"
>>>>>>> 7bf3cb6 (fix(yacc_sql.y): rel_attr aggre_type to NONE)
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->explain.sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
<<<<<<< HEAD
<<<<<<< HEAD
#line 2418 "yacc_sql.cpp"
    break;

  case 91: /* set_variable_stmt: SET ID EQ value  */
#line 680 "yacc_sql.y"
=======
#line 2600 "yacc_sql.cpp"
    break;

  case 103: /* set_variable_stmt: SET ID EQ value  */
#line 782 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
=======
#line 2605 "yacc_sql.cpp"
    break;

  case 103: /* set_variable_stmt: SET ID EQ value  */
#line 787 "yacc_sql.y"
>>>>>>> 7bf3cb6 (fix(yacc_sql.y): rel_attr aggre_type to NONE)
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name  = (yyvsp[-2].string);
      (yyval.sql_node)->set_variable.value = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
<<<<<<< HEAD
<<<<<<< HEAD
#line 2430 "yacc_sql.cpp"
    break;


#line 2434 "yacc_sql.cpp"
=======
#line 2612 "yacc_sql.cpp"
    break;


#line 2616 "yacc_sql.cpp"
>>>>>>> dda9a4c (fix: parser)
=======
#line 2617 "yacc_sql.cpp"
    break;


#line 2621 "yacc_sql.cpp"
>>>>>>> 7bf3cb6 (fix(yacc_sql.y): rel_attr aggre_type to NONE)

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, sql_string, sql_result, scanner, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, sql_string, sql_result, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, sql_string, sql_result, scanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, sql_string, sql_result, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

<<<<<<< HEAD
<<<<<<< HEAD
#line 692 "yacc_sql.y"
=======
#line 794 "yacc_sql.y"
>>>>>>> dda9a4c (fix: parser)
=======
#line 799 "yacc_sql.y"
>>>>>>> 7bf3cb6 (fix(yacc_sql.y): rel_attr aggre_type to NONE)

//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, ParsedSqlResult *sql_result) {
  yyscan_t scanner;
  yylex_init(&scanner);
  scan_string(s, scanner);
  int result = yyparse(s, sql_result, scanner);
  yylex_destroy(scanner);
  return result;
}
