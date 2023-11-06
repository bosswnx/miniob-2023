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
  YYSYMBOL_INNER_JOIN = 33,                /* INNER_JOIN  */
  YYSYMBOL_WHERE = 34,                     /* WHERE  */
  YYSYMBOL_HAVING = 35,                    /* HAVING  */
  YYSYMBOL_ORDER_BY = 36,                  /* ORDER_BY  */
  YYSYMBOL_GROUP_BY = 37,                  /* GROUP_BY  */
  YYSYMBOL_ASC = 38,                       /* ASC  */
  YYSYMBOL_IN = 39,                        /* IN  */
  YYSYMBOL_NI = 40,                        /* NI  */
  YYSYMBOL_EXISTS = 41,                    /* EXISTS  */
  YYSYMBOL_NOT_EXISTS = 42,                /* NOT_EXISTS  */
  YYSYMBOL_AND = 43,                       /* AND  */
  YYSYMBOL_OR = 44,                        /* OR  */
  YYSYMBOL_SET = 45,                       /* SET  */
  YYSYMBOL_ON = 46,                        /* ON  */
  YYSYMBOL_LOAD = 47,                      /* LOAD  */
  YYSYMBOL_DATA = 48,                      /* DATA  */
  YYSYMBOL_INFILE = 49,                    /* INFILE  */
  YYSYMBOL_NOT_NULL = 50,                  /* NOT_NULL  */
  YYSYMBOL_EXPLAIN = 51,                   /* EXPLAIN  */
  YYSYMBOL_UNIQUE = 52,                    /* UNIQUE  */
  YYSYMBOL_AS = 53,                        /* AS  */
  YYSYMBOL_EQ = 54,                        /* EQ  */
  YYSYMBOL_LT = 55,                        /* LT  */
  YYSYMBOL_GT = 56,                        /* GT  */
  YYSYMBOL_LE = 57,                        /* LE  */
  YYSYMBOL_GE = 58,                        /* GE  */
  YYSYMBOL_NE = 59,                        /* NE  */
  YYSYMBOL_MAX = 60,                       /* MAX  */
  YYSYMBOL_MIN = 61,                       /* MIN  */
  YYSYMBOL_SUM = 62,                       /* SUM  */
  YYSYMBOL_CNT = 63,                       /* CNT  */
  YYSYMBOL_AVG = 64,                       /* AVG  */
  YYSYMBOL_LK = 65,                        /* LK  */
  YYSYMBOL_NLK = 66,                       /* NLK  */
  YYSYMBOL_IS_NOT = 67,                    /* IS_NOT  */
  YYSYMBOL_IS = 68,                        /* IS  */
  YYSYMBOL_LENGTH = 69,                    /* LENGTH  */
  YYSYMBOL_ROUND = 70,                     /* ROUND  */
  YYSYMBOL_DATE_FORMAT = 71,               /* DATE_FORMAT  */
  YYSYMBOL_P_INT = 72,                     /* P_INT  */
  YYSYMBOL_P_FLOAT = 73,                   /* P_FLOAT  */
  YYSYMBOL_ID = 74,                        /* ID  */
  YYSYMBOL_DATE_STR = 75,                  /* DATE_STR  */
  YYSYMBOL_SSS = 76,                       /* SSS  */
  YYSYMBOL_NULL_T = 77,                    /* NULL_T  */
  YYSYMBOL_78_ = 78,                       /* '+'  */
  YYSYMBOL_79_ = 79,                       /* '-'  */
  YYSYMBOL_80_ = 80,                       /* '*'  */
  YYSYMBOL_81_ = 81,                       /* '/'  */
  YYSYMBOL_UMINUS = 82,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 83,                  /* $accept  */
  YYSYMBOL_commands = 84,                  /* commands  */
  YYSYMBOL_command_wrapper = 85,           /* command_wrapper  */
  YYSYMBOL_exit_stmt = 86,                 /* exit_stmt  */
  YYSYMBOL_help_stmt = 87,                 /* help_stmt  */
  YYSYMBOL_sync_stmt = 88,                 /* sync_stmt  */
  YYSYMBOL_begin_stmt = 89,                /* begin_stmt  */
  YYSYMBOL_commit_stmt = 90,               /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 91,             /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 92,           /* drop_table_stmt  */
  YYSYMBOL_show_tables_stmt = 93,          /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 94,           /* desc_table_stmt  */
  YYSYMBOL_create_index_stmt = 95,         /* create_index_stmt  */
  YYSYMBOL_drop_index_stmt = 96,           /* drop_index_stmt  */
  YYSYMBOL_create_table_stmt = 97,         /* create_table_stmt  */
  YYSYMBOL_ID_list = 98,                   /* ID_list  */
  YYSYMBOL_attr_def_list = 99,             /* attr_def_list  */
  YYSYMBOL_attr_def = 100,                 /* attr_def  */
  YYSYMBOL_opt_null = 101,                 /* opt_null  */
  YYSYMBOL_number = 102,                   /* number  */
  YYSYMBOL_float = 103,                    /* float  */
  YYSYMBOL_type = 104,                     /* type  */
  YYSYMBOL_insert_stmt = 105,              /* insert_stmt  */
  YYSYMBOL_value_list = 106,               /* value_list  */
  YYSYMBOL_value = 107,                    /* value  */
  YYSYMBOL_delete_stmt = 108,              /* delete_stmt  */
  YYSYMBOL_update_stmt = 109,              /* update_stmt  */
  YYSYMBOL_update_value_target = 110,      /* update_value_target  */
  YYSYMBOL_update_value_list = 111,        /* update_value_list  */
  YYSYMBOL_order_by_type = 112,            /* order_by_type  */
  YYSYMBOL_order_by_body = 113,            /* order_by_body  */
  YYSYMBOL_order_by_list = 114,            /* order_by_list  */
  YYSYMBOL_group_by_body = 115,            /* group_by_body  */
  YYSYMBOL_group_by_list = 116,            /* group_by_list  */
  YYSYMBOL_select_stmt = 117,              /* select_stmt  */
  YYSYMBOL_calc_stmt = 118,                /* calc_stmt  */
  YYSYMBOL_expression_list = 119,          /* expression_list  */
  YYSYMBOL_expression = 120,               /* expression  */
  YYSYMBOL_select_attr = 121,              /* select_attr  */
  YYSYMBOL_rel_attr = 122,                 /* rel_attr  */
  YYSYMBOL_relation = 123,                 /* relation  */
  YYSYMBOL_attr_list = 124,                /* attr_list  */
  YYSYMBOL_rel_list = 125,                 /* rel_list  */
  YYSYMBOL_join_list = 126,                /* join_list  */
  YYSYMBOL_sub_select_stmt = 127,          /* sub_select_stmt  */
  YYSYMBOL_where = 128,                    /* where  */
  YYSYMBOL_having = 129,                   /* having  */
  YYSYMBOL_condition_list = 130,           /* condition_list  */
  YYSYMBOL_condition = 131,                /* condition  */
  YYSYMBOL_comp_op = 132,                  /* comp_op  */
  YYSYMBOL_aggre_type = 133,               /* aggre_type  */
  YYSYMBOL_load_data_stmt = 134,           /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 135,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 136,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 137             /* opt_semicolon  */
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
typedef yytype_int16 yy_state_t;

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
#define YYFINAL  80
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   405

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  83
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  163
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  305

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   333


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
       2,     2,    80,    78,     2,    79,     2,    81,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    82
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   231,   231,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   263,   269,   274,   280,   286,   292,
     298,   305,   311,   319,   335,   354,   365,   373,   392,   411,
     432,   435,   449,   452,   465,   474,   483,   493,   505,   508,
     513,   514,   518,   519,   523,   524,   525,   526,   529,   545,
     548,   559,   563,   567,   573,   576,   584,   597,   622,   627,
     635,   638,   653,   656,   659,   666,   669,   684,   687,   702,
     705,   719,   722,   735,   742,   805,   815,   820,   831,   835,
     839,   845,   849,   855,   859,   863,   867,   871,   875,   886,
     890,   894,   900,   906,   912,   915,   918,   921,   924,   928,
     934,  1042,  1046,  1052,  1061,  1066,  1073,  1084,  1087,  1101,
    1104,  1126,  1129,  1174,  1183,  1186,  1193,  1196,  1203,  1206,
    1212,  1218,  1246,  1258,  1273,  1292,  1302,  1321,  1331,  1359,
    1371,  1381,  1399,  1417,  1431,  1432,  1433,  1434,  1435,  1436,
    1437,  1438,  1439,  1440,  1444,  1445,  1446,  1447,  1448,  1452,
    1465,  1473,  1483,  1484
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
  "FLOAT_T", "HELP", "EXIT", "DOT", "INTO", "VALUES", "FROM", "INNER_JOIN",
  "WHERE", "HAVING", "ORDER_BY", "GROUP_BY", "ASC", "IN", "NI", "EXISTS",
  "NOT_EXISTS", "AND", "OR", "SET", "ON", "LOAD", "DATA", "INFILE",
  "NOT_NULL", "EXPLAIN", "UNIQUE", "AS", "EQ", "LT", "GT", "LE", "GE",
  "NE", "MAX", "MIN", "SUM", "CNT", "AVG", "LK", "NLK", "IS_NOT", "IS",
  "LENGTH", "ROUND", "DATE_FORMAT", "P_INT", "P_FLOAT", "ID", "DATE_STR",
  "SSS", "NULL_T", "'+'", "'-'", "'*'", "'/'", "UMINUS", "$accept",
  "commands", "command_wrapper", "exit_stmt", "help_stmt", "sync_stmt",
  "begin_stmt", "commit_stmt", "rollback_stmt", "drop_table_stmt",
  "show_tables_stmt", "desc_table_stmt", "create_index_stmt",
  "drop_index_stmt", "create_table_stmt", "ID_list", "attr_def_list",
  "attr_def", "opt_null", "number", "float", "type", "insert_stmt",
  "value_list", "value", "delete_stmt", "update_stmt",
  "update_value_target", "update_value_list", "order_by_type",
  "order_by_body", "order_by_list", "group_by_body", "group_by_list",
  "select_stmt", "calc_stmt", "expression_list", "expression",
  "select_attr", "rel_attr", "relation", "attr_list", "rel_list",
  "join_list", "sub_select_stmt", "where", "having", "condition_list",
  "condition", "comp_op", "aggre_type", "load_data_stmt", "explain_stmt",
  "set_variable_stmt", "opt_semicolon", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-238)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     326,    21,    17,   227,   227,   -50,    30,  -238,    11,    22,
     -39,    42,  -238,  -238,  -238,  -238,  -238,    10,    69,   326,
      78,   126,  -238,  -238,  -238,  -238,  -238,  -238,  -238,  -238,
    -238,  -238,  -238,  -238,  -238,  -238,  -238,  -238,  -238,  -238,
    -238,  -238,  -238,    57,    62,   135,    72,    73,   227,  -238,
    -238,  -238,  -238,  -238,   128,   136,   140,  -238,  -238,   129,
    -238,  -238,  -238,   227,   130,  -238,    71,   144,    86,   138,
     143,  -238,  -238,    97,   113,   127,   165,   139,   146,  -238,
    -238,  -238,  -238,    -6,   151,   131,  -238,   158,    24,   227,
     227,   227,   -48,  -238,   137,   227,   227,   227,   227,   227,
     185,   142,  -238,   150,   227,  -238,   187,   192,   153,  -238,
     109,   161,   167,    42,   168,   194,   169,  -238,    28,    20,
      99,  -238,  -238,  -238,  -238,   -47,   -47,  -238,  -238,  -238,
     232,  -238,   -38,   244,   143,   234,   159,  -238,   213,  -238,
    -238,  -238,  -238,  -238,    48,  -238,  -238,  -238,   238,   166,
     250,  -238,   253,   203,  -238,  -238,  -238,   227,   227,  -238,
     204,  -238,   150,   246,  -238,   109,    -5,   263,   275,   324,
     217,  -238,    84,    65,  -238,  -238,   287,  -238,  -238,  -238,
    -238,    98,   167,   276,   221,   288,    32,    44,  -238,   244,
     150,   192,   289,     4,  -238,     4,  -238,   297,   298,  -238,
    -238,  -238,  -238,  -238,  -238,  -238,  -238,  -238,  -238,   249,
     249,   159,   159,  -238,   308,  -238,   242,   -51,  -238,  -238,
    -238,   250,     0,   313,   243,  -238,  -238,  -238,   299,   314,
     109,   315,   289,   289,     4,  -238,     4,  -238,   134,  -238,
     134,  -238,  -238,  -238,   260,   192,  -238,   272,   331,  -238,
      42,  -238,   277,   334,   313,   159,   227,   318,   289,  -238,
     338,   339,   289,   289,   304,  -238,   -37,  -238,   313,  -238,
     341,   246,     5,   227,   325,  -238,  -238,  -238,   343,   344,
      65,  -238,  -238,  -238,  -238,  -238,  -238,   346,   347,   159,
    -238,  -238,  -238,   308,   227,  -238,   227,  -238,  -238,  -238,
       5,   346,   346,  -238,  -238
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,    27,    28,    29,    25,    24,     0,     0,     0,
       0,   162,    23,    22,    15,    16,    17,    18,    10,    11,
      12,    13,    14,     9,     6,     8,     7,     4,     3,     5,
      19,    20,    21,     0,     0,     0,     0,     0,     0,   154,
     155,   156,   157,   158,     0,     0,     0,    88,    89,    93,
      90,    92,    91,     0,    96,    85,    86,     0,   111,    83,
     117,    32,    31,     0,     0,     0,     0,     0,     0,   160,
       1,   163,     2,     0,     0,     0,    30,     0,     0,     0,
       0,     0,     0,   109,     0,     0,     0,     0,     0,     0,
       0,     0,   113,     0,     0,   110,     0,   124,     0,   123,
       0,     0,     0,     0,     0,     0,     0,   108,     0,     0,
       0,    94,    95,    97,    87,   104,   105,   106,   107,    99,
       0,   112,   114,   119,   117,     0,   128,    66,     0,    50,
      52,    63,    65,    64,     0,    61,    62,   161,     0,     0,
      42,    36,     0,     0,    35,   100,   101,     0,     0,    98,
       0,   116,     0,   121,   118,     0,     0,     0,     0,     0,
       0,   125,   129,     0,    51,    53,     0,    54,    55,    57,
      56,    45,     0,     0,     0,     0,     0,     0,   115,   119,
       0,   124,    59,     0,   139,     0,   140,     0,     0,   144,
     145,   146,   147,   148,   151,   149,   150,   152,   153,     0,
       0,   128,   128,    68,    70,    69,     0,     0,    48,    49,
      46,    42,    39,    40,     0,   102,   103,   120,     0,    77,
       0,     0,    59,    59,     0,   135,     0,   137,   132,   133,
     138,   143,   130,   131,     0,   124,   159,     0,     0,    43,
       0,    37,     0,     0,    40,   128,     0,    81,    59,    58,
       0,     0,    59,    59,     0,    67,    44,    38,    40,    33,
       0,   121,    72,     0,   126,    60,   141,   142,     0,     0,
       0,    47,    41,    34,   122,    74,    73,    75,    79,   128,
      84,   134,   136,    70,     0,    78,     0,    82,   127,    71,
      72,    75,    75,    80,    76
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -238,  -238,   348,  -238,  -238,  -238,  -238,  -238,  -238,  -238,
    -238,  -238,  -238,  -238,  -238,  -237,   147,   188,   103,   155,
    -238,  -238,  -238,  -214,  -104,  -238,  -238,    94,    82,    76,
    -167,  -238,  -238,  -238,     3,  -238,    19,    -3,  -238,  -100,
    -160,   251,   195,   115,     1,  -182,  -238,  -204,  -238,   218,
    -238,  -238,  -238,  -238,  -238
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,   253,   183,   150,   220,   145,
     146,   181,    34,   231,   213,    35,    36,   214,   245,   287,
     295,   257,   297,   274,    76,    38,    65,    68,    69,    70,
     133,   105,   163,   191,   170,   137,   290,   171,   172,   209,
      67,    40,    41,    42,    82
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      66,    39,   189,    37,   134,     4,   147,   242,   243,   229,
       4,   112,    48,   218,     4,   160,   285,   270,   260,   261,
      39,   139,    37,    46,    71,    47,   121,    43,   247,    44,
     228,   282,   122,    98,    99,    75,   161,    72,   156,   157,
     219,    73,   117,   286,   275,    88,   155,   113,   278,   279,
     225,   271,     4,   250,    74,    49,    50,    51,    52,    53,
      93,   192,   226,   265,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    45,    63,    64,   139,   140,    80,   141,
     142,   143,    11,   144,    77,   298,   118,   119,   120,   232,
      95,   233,    66,   125,   126,   127,   128,    66,    96,    97,
      98,    99,    96,    97,    98,    99,    96,    97,    98,    99,
      96,    97,    98,    99,   124,   217,   151,    78,   158,   130,
     174,   175,    96,    97,    98,    99,   258,   211,   212,    81,
     262,    83,   263,   169,   303,   304,    84,   139,   140,   101,
     141,   142,   143,    85,   144,    89,    86,    87,   218,    96,
      97,    98,    99,    90,   186,   187,   272,    91,    92,    94,
     102,   100,   104,    88,    96,    97,    98,    99,   194,   196,
     103,   106,   108,   288,   215,   219,   166,    96,    97,    98,
      99,   139,   140,   109,   141,   142,   143,   107,   144,   177,
     178,   179,   180,   110,   300,   111,   301,   114,   235,   237,
     167,   168,    48,   129,   116,   115,   238,   240,   169,   169,
     239,   241,    96,    97,    98,    99,   131,   123,   135,    49,
      50,    51,    52,    53,   132,   251,   136,   138,    54,    55,
      56,    57,    58,    59,    60,    61,    62,   148,    63,    64,
     153,   149,   152,   154,    48,    49,    50,    51,    52,    53,
     159,   165,   169,   267,    54,    55,    56,    57,    58,    59,
      60,    61,    62,   162,    63,    64,   166,   173,   176,   182,
     184,   199,   200,   201,   202,   203,   204,   185,   188,   190,
     193,   215,   205,   206,   207,   208,   169,    49,    50,    51,
      52,    53,   195,   216,   222,   223,    54,    55,    56,    57,
      58,    59,    60,    61,    62,   224,    63,    64,   230,    49,
      50,    51,    52,    53,   234,   236,   246,   254,    54,    55,
      56,    57,    58,    59,    60,    61,    62,   244,    63,    64,
       1,     2,   252,   259,   264,     3,     4,     5,     6,     7,
       8,     9,    10,    11,   174,   255,    12,    13,    14,   266,
     256,   268,   269,    15,    16,   273,   276,   277,   280,   283,
     289,   291,   292,   197,   198,   294,   296,    79,   249,   281,
     221,    17,   248,    18,   293,   299,   302,    19,   199,   200,
     201,   202,   203,   204,   227,   164,   284,     0,   210,   205,
     206,   207,   208,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,    97,    98,    99
};

static const yytype_int16 yycheck[] =
{
       3,     0,   162,     0,   104,    10,   110,   211,   212,   191,
      10,    17,    17,    50,    10,    53,    11,   254,   232,   233,
      19,    72,    19,     6,    74,     8,    74,     6,    79,     8,
     190,   268,    80,    80,    81,    74,    74,     7,    18,    19,
      77,    30,    18,    38,   258,    48,    18,    53,   262,   263,
      18,   255,    10,    53,    32,    60,    61,    62,    63,    64,
      63,   165,    18,   245,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    52,    79,    80,    72,    73,     0,    75,
      76,    77,    17,    79,    74,   289,    89,    90,    91,   193,
      19,   195,    95,    96,    97,    98,    99,   100,    78,    79,
      80,    81,    78,    79,    80,    81,    78,    79,    80,    81,
      78,    79,    80,    81,    95,    17,   113,    48,    19,   100,
      72,    73,    78,    79,    80,    81,   230,    43,    44,     3,
     234,    74,   236,   136,   301,   302,    74,    72,    73,    53,
      75,    76,    77,     8,    79,    17,    74,    74,    50,    78,
      79,    80,    81,    17,   157,   158,   256,    17,    29,    29,
      74,    17,    19,   166,    78,    79,    80,    81,   167,   168,
      32,    74,    45,   273,   173,    77,    17,    78,    79,    80,
      81,    72,    73,    18,    75,    76,    77,    74,    79,    23,
      24,    25,    26,    54,   294,    49,   296,    46,   197,   198,
      41,    42,    17,    18,    46,    74,   209,   210,   211,   212,
     209,   210,    78,    79,    80,    81,    74,    80,    31,    60,
      61,    62,    63,    64,    74,   222,    34,    74,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    76,    79,    80,
      46,    74,    74,    74,    17,    60,    61,    62,    63,    64,
      18,    17,   255,   250,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    19,    79,    80,    17,    54,    30,    19,
      17,    54,    55,    56,    57,    58,    59,    74,    74,    33,
      17,   280,    65,    66,    67,    68,   289,    60,    61,    62,
      63,    64,    17,     6,    18,    74,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    17,    79,    80,    19,    60,
      61,    62,    63,    64,    17,    17,    74,    74,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    19,    79,    80,
       4,     5,    19,    18,    74,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    72,    46,    20,    21,    22,    18,
      36,    74,    18,    27,    28,    37,    18,    18,    54,    18,
      35,    18,    18,    39,    40,    19,    19,    19,   221,   266,
     182,    45,   217,    47,   280,   293,   300,    51,    54,    55,
      56,    57,    58,    59,   189,   134,   271,    -1,   170,    65,
      66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    80,    81
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,     5,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    20,    21,    22,    27,    28,    45,    47,    51,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,   105,   108,   109,   117,   118,   127,
     134,   135,   136,     6,     8,    52,     6,     8,    17,    60,
      61,    62,    63,    64,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    79,    80,   119,   120,   133,   120,   121,
     122,    74,     7,    30,    32,    74,   117,    74,    48,    85,
       0,     3,   137,    74,    74,     8,    74,    74,   120,    17,
      17,    17,    29,   120,    29,    19,    78,    79,    80,    81,
      17,    53,    74,    32,    19,   124,    74,    74,    45,    18,
      54,    49,    17,    53,    46,    74,    46,    18,   120,   120,
     120,    74,    80,    80,   119,   120,   120,   120,   120,    18,
     119,    74,    74,   123,   122,    31,    34,   128,    74,    72,
      73,    75,    76,    77,    79,   102,   103,   107,    76,    74,
     100,   117,    74,    46,    74,    18,    18,    19,    19,    18,
      53,    74,    19,   125,   124,    17,    17,    41,    42,   120,
     127,   130,   131,    54,    72,    73,    30,    23,    24,    25,
      26,   104,    19,    99,    17,    74,   120,   120,    74,   123,
      33,   126,   107,    17,   127,    17,   127,    39,    40,    54,
      55,    56,    57,    58,    59,    65,    66,    67,    68,   132,
     132,    43,    44,   107,   110,   127,     6,    17,    50,    77,
     101,   100,    18,    74,    17,    18,    18,   125,   123,   128,
      19,   106,   107,   107,    17,   127,    17,   127,   120,   127,
     120,   127,   130,   130,    19,   111,    74,    79,   102,    99,
      53,   117,    19,    98,    74,    46,    36,   114,   107,    18,
     106,   106,   107,   107,    74,   128,    18,   117,    74,    18,
      98,   130,   122,    37,   116,   106,    18,    18,   106,   106,
      54,   101,    98,    18,   126,    11,    38,   112,   122,    35,
     129,    18,    18,   110,    19,   113,    19,   115,   130,   111,
     122,   122,   112,   113,   113
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    83,    84,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    95,    96,    97,    97,    97,    97,
      98,    98,    99,    99,   100,   100,   100,   100,   101,   101,
     102,   102,   103,   103,   104,   104,   104,   104,   105,   106,
     106,   107,   107,   107,   107,   107,   108,   109,   110,   110,
     111,   111,   112,   112,   112,   113,   113,   114,   114,   115,
     115,   116,   116,   117,   117,   118,   119,   119,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     121,   122,   122,   122,   123,   123,   123,   124,   124,   125,
     125,   126,   126,   127,   128,   128,   129,   129,   130,   130,
     130,   130,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   133,   133,   133,   133,   133,   134,
     135,   136,   137,   137
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     2,     2,     9,    10,     5,     5,     8,     9,     7,
       0,     3,     0,     3,     5,     2,     3,     6,     1,     1,
       1,     2,     1,     2,     1,     1,     1,     1,     8,     0,
       3,     1,     1,     1,     1,     1,     4,     8,     1,     1,
       0,     5,     0,     1,     1,     0,     4,     0,     4,     0,
       3,     0,     3,     2,    10,     2,     1,     3,     1,     1,
       1,     1,     1,     1,     3,     3,     1,     3,     4,     3,
       4,     4,     6,     6,     3,     3,     3,     3,     3,     2,
       2,     1,     3,     2,     1,     3,     2,     0,     3,     0,
       3,     0,     5,     3,     0,     2,     0,     2,     0,     1,
       3,     3,     3,     3,     6,     3,     6,     3,     3,     2,
       2,     5,     5,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     7,
       2,     4,     0,     1
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
#line 232 "yacc_sql.y"
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
    sql_result->add_sql_node(std::move(sql_node));
  }
#line 1887 "yacc_sql.cpp"
    break;

  case 24: /* exit_stmt: EXIT  */
#line 263 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 1896 "yacc_sql.cpp"
    break;

  case 25: /* help_stmt: HELP  */
#line 269 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 1904 "yacc_sql.cpp"
    break;

  case 26: /* sync_stmt: SYNC  */
#line 274 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 1912 "yacc_sql.cpp"
    break;

  case 27: /* begin_stmt: TRX_BEGIN  */
#line 280 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 1920 "yacc_sql.cpp"
    break;

  case 28: /* commit_stmt: TRX_COMMIT  */
#line 286 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 1928 "yacc_sql.cpp"
    break;

  case 29: /* rollback_stmt: TRX_ROLLBACK  */
#line 292 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 1936 "yacc_sql.cpp"
    break;

  case 30: /* drop_table_stmt: DROP TABLE ID  */
#line 298 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      (yyval.sql_node)->drop_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1946 "yacc_sql.cpp"
    break;

  case 31: /* show_tables_stmt: SHOW TABLES  */
#line 305 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 1954 "yacc_sql.cpp"
    break;

  case 32: /* desc_table_stmt: DESC ID  */
#line 311 "yacc_sql.y"
             {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      (yyval.sql_node)->desc_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1964 "yacc_sql.cpp"
    break;

  case 33: /* create_index_stmt: CREATE INDEX ID ON ID LBRACE ID ID_list RBRACE  */
#line 320 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
      if ((yyvsp[-1].ID_list) != nullptr) {
        create_index.attribute_names.swap(*(yyvsp[-1].ID_list));
      }
      create_index.is_unique = false;
      create_index.index_name = (yyvsp[-6].string);
      create_index.relation_name = (yyvsp[-4].string);
      create_index.attribute_names.push_back((yyvsp[-2].string));
      std::reverse(create_index.attribute_names.begin(), create_index.attribute_names.end());
      free((yyvsp[-6].string));
      free((yyvsp[-4].string));
      free((yyvsp[-2].string));
    }
#line 1984 "yacc_sql.cpp"
    break;

  case 34: /* create_index_stmt: CREATE UNIQUE INDEX ID ON ID LBRACE ID ID_list RBRACE  */
#line 336 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
      if ((yyvsp[-1].ID_list) != nullptr) {
        create_index.attribute_names.swap(*(yyvsp[-1].ID_list));
      }
      create_index.is_unique = true;
      create_index.index_name = (yyvsp[-6].string);
      create_index.relation_name = (yyvsp[-4].string);
      create_index.attribute_names.push_back((yyvsp[-2].string));
      std::reverse(create_index.attribute_names.begin(), create_index.attribute_names.end());
      free((yyvsp[-6].string));
      free((yyvsp[-4].string));
      free((yyvsp[-2].string));
    }
#line 2004 "yacc_sql.cpp"
    break;

  case 35: /* drop_index_stmt: DROP INDEX ID ON ID  */
#line 355 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      (yyval.sql_node)->drop_index.index_name = (yyvsp[-2].string);
      (yyval.sql_node)->drop_index.relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2016 "yacc_sql.cpp"
    break;

  case 36: /* create_table_stmt: CREATE TABLE ID AS select_stmt  */
#line 366 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-2].string);
      free((yyvsp[-2].string));
      create_table.sub_select = (yyvsp[0].sql_node);
    }
#line 2028 "yacc_sql.cpp"
    break;

  case 37: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE select_stmt  */
#line 374 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-5].string);
      free((yyvsp[-5].string));

      std::vector<AttrInfoSqlNode> *src_attrs = (yyvsp[-2].attr_infos);

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
      }
      create_table.attr_infos.emplace_back(*(yyvsp[-3].attr_info));
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete (yyvsp[-3].attr_info);

      create_table.sub_select = (yyvsp[0].sql_node);
    }
#line 2050 "yacc_sql.cpp"
    break;

  case 38: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE AS select_stmt  */
#line 393 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-6].string);
      free((yyvsp[-6].string));

      std::vector<AttrInfoSqlNode> *src_attrs = (yyvsp[-3].attr_infos);

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
      }
      create_table.attr_infos.emplace_back(*(yyvsp[-4].attr_info));
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete (yyvsp[-4].attr_info);

      create_table.sub_select = (yyvsp[0].sql_node);
    }
#line 2072 "yacc_sql.cpp"
    break;

  case 39: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE  */
#line 412 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-4].string);
      create_table.sub_select = nullptr;
      free((yyvsp[-4].string));

      std::vector<AttrInfoSqlNode> *src_attrs = (yyvsp[-1].attr_infos);

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
      }
      create_table.attr_infos.emplace_back(*(yyvsp[-2].attr_info));
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete (yyvsp[-2].attr_info);
    }
#line 2093 "yacc_sql.cpp"
    break;

  case 40: /* ID_list: %empty  */
#line 432 "yacc_sql.y"
    {
      (yyval.ID_list) = nullptr;
    }
#line 2101 "yacc_sql.cpp"
    break;

  case 41: /* ID_list: COMMA ID ID_list  */
#line 436 "yacc_sql.y"
    {
      if ((yyvsp[0].ID_list) != nullptr) {
        (yyval.ID_list) = (yyvsp[0].ID_list);
      } else {
        (yyval.ID_list) = new std::vector<std::string>;
      }
      (yyval.ID_list)->emplace_back((yyvsp[-1].string));
      delete (yyvsp[-1].string);
    }
#line 2115 "yacc_sql.cpp"
    break;

  case 42: /* attr_def_list: %empty  */
#line 449 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 2123 "yacc_sql.cpp"
    break;

  case 43: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 453 "yacc_sql.y"
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(*(yyvsp[-1].attr_info));
      delete (yyvsp[-1].attr_info);
    }
#line 2137 "yacc_sql.cpp"
    break;

  case 44: /* attr_def: ID type LBRACE number RBRACE  */
#line 466 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-3].number);
      (yyval.attr_info)->name = (yyvsp[-4].string);
      (yyval.attr_info)->length = (yyvsp[-1].number);
      (yyval.attr_info)->is_null = false;
      free((yyvsp[-4].string));
    }
#line 2150 "yacc_sql.cpp"
    break;

  case 45: /* attr_def: ID type  */
#line 475 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[0].number);
      (yyval.attr_info)->name = (yyvsp[-1].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->is_null = false;
      free((yyvsp[-1].string));
    }
#line 2163 "yacc_sql.cpp"
    break;

  case 46: /* attr_def: ID type opt_null  */
#line 484 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->is_null = (yyvsp[0].number);
      free((yyvsp[-2].string));
      
    }
#line 2177 "yacc_sql.cpp"
    break;

  case 47: /* attr_def: ID type LBRACE number RBRACE opt_null  */
#line 494 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-4].number);
      (yyval.attr_info)->name = (yyvsp[-5].string);
      (yyval.attr_info)->length = (yyvsp[-2].number);
      (yyval.attr_info)->is_null = (yyvsp[0].number);
      free((yyvsp[-5].string));
      
    }
#line 2191 "yacc_sql.cpp"
    break;

  case 48: /* opt_null: NOT_NULL  */
#line 505 "yacc_sql.y"
                 {
		(yyval.number) = false;
	}
#line 2199 "yacc_sql.cpp"
    break;

  case 49: /* opt_null: NULL_T  */
#line 508 "yacc_sql.y"
                 {
		(yyval.number) = true;
	}
#line 2207 "yacc_sql.cpp"
    break;

  case 50: /* number: P_INT  */
#line 513 "yacc_sql.y"
                {(yyval.number) = (yyvsp[0].number);}
#line 2213 "yacc_sql.cpp"
    break;

  case 51: /* number: '-' P_INT  */
#line 514 "yacc_sql.y"
                  {(yyval.number) = -(yyvsp[0].number);}
#line 2219 "yacc_sql.cpp"
    break;

  case 52: /* float: P_FLOAT  */
#line 518 "yacc_sql.y"
                 {(yyval.floats) = (yyvsp[0].floats);}
#line 2225 "yacc_sql.cpp"
    break;

  case 53: /* float: '-' P_FLOAT  */
#line 519 "yacc_sql.y"
                   {(yyval.floats) = -(yyvsp[0].floats);}
#line 2231 "yacc_sql.cpp"
    break;

  case 54: /* type: INT_T  */
#line 523 "yacc_sql.y"
               { (yyval.number)=INTS; }
#line 2237 "yacc_sql.cpp"
    break;

  case 55: /* type: STRING_T  */
#line 524 "yacc_sql.y"
               { (yyval.number)=CHARS; }
#line 2243 "yacc_sql.cpp"
    break;

  case 56: /* type: FLOAT_T  */
#line 525 "yacc_sql.y"
               { (yyval.number)=FLOATS; }
#line 2249 "yacc_sql.cpp"
    break;

  case 57: /* type: DATE_T  */
#line 526 "yacc_sql.y"
               { (yyval.number)=DATES; }
#line 2255 "yacc_sql.cpp"
    break;

  case 58: /* insert_stmt: INSERT INTO ID VALUES LBRACE value value_list RBRACE  */
#line 530 "yacc_sql.y"
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
#line 2271 "yacc_sql.cpp"
    break;

  case 59: /* value_list: %empty  */
#line 545 "yacc_sql.y"
    {
      (yyval.value_list) = nullptr;
    }
#line 2279 "yacc_sql.cpp"
    break;

  case 60: /* value_list: COMMA value value_list  */
#line 548 "yacc_sql.y"
                              { 
      if ((yyvsp[0].value_list) != nullptr) {
        (yyval.value_list) = (yyvsp[0].value_list);
      } else {
        (yyval.value_list) = new std::vector<Value>;
      }
      (yyval.value_list)->emplace_back(*(yyvsp[-1].value));
      delete (yyvsp[-1].value);
    }
#line 2293 "yacc_sql.cpp"
    break;

  case 61: /* value: number  */
#line 559 "yacc_sql.y"
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2302 "yacc_sql.cpp"
    break;

  case 62: /* value: float  */
#line 563 "yacc_sql.y"
            {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2311 "yacc_sql.cpp"
    break;

  case 63: /* value: DATE_STR  */
#line 567 "yacc_sql.y"
              {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.value) = new Value(date(tmp));
      free(tmp);
      (yyloc) = (yylsp[0]);
    }
#line 2322 "yacc_sql.cpp"
    break;

  case 64: /* value: NULL_T  */
#line 573 "yacc_sql.y"
             {
      (yyval.value) = new Value(true, true);
    }
#line 2330 "yacc_sql.cpp"
    break;

  case 65: /* value: SSS  */
#line 576 "yacc_sql.y"
         {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.value) = new Value(tmp);
      free(tmp);
    }
#line 2340 "yacc_sql.cpp"
    break;

  case 66: /* delete_stmt: DELETE FROM ID where  */
#line 585 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      (yyval.sql_node)->deletion.relation_name = (yyvsp[-1].string);
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->deletion.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-1].string));
    }
#line 2354 "yacc_sql.cpp"
    break;

  case 67: /* update_stmt: UPDATE ID SET ID EQ update_value_target update_value_list where  */
#line 598 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_UPDATE);
      if ((yyvsp[-1].update_info) != nullptr) {
        (yyval.sql_node)->update.attributes_name.swap((yyvsp[-1].update_info)->attributes_name);
        (yyval.sql_node)->update.targets.swap((yyvsp[-1].update_info)->targets);
        delete (yyvsp[-1].update_info);
      }
      (yyval.sql_node)->update.relation_name = (yyvsp[-6].string);
      (yyval.sql_node)->update.attributes_name.emplace_back((yyvsp[-4].string));
      (yyval.sql_node)->update.targets.emplace_back(*(yyvsp[-2].update_target_t));
      // reverse
      std::reverse((yyval.sql_node)->update.targets.begin(), (yyval.sql_node)->update.targets.end());
      std::reverse((yyval.sql_node)->update.attributes_name.begin(), (yyval.sql_node)->update.attributes_name.end());
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->update.conditions.swap(*(yyvsp[0].condition_list));
        //reverse
        std::reverse((yyval.sql_node)->update.conditions.begin(), (yyval.sql_node)->update.conditions.end());
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-6].string));
      free((yyvsp[-4].string));
    }
#line 2381 "yacc_sql.cpp"
    break;

  case 68: /* update_value_target: value  */
#line 622 "yacc_sql.y"
          {
      (yyval.update_target_t) = new UpdateTarget;
      (yyval.update_target_t)->value = *(yyvsp[0].value);
      (yyval.update_target_t)->is_value = 1;
    }
#line 2391 "yacc_sql.cpp"
    break;

  case 69: /* update_value_target: sub_select_stmt  */
#line 627 "yacc_sql.y"
                      {
      (yyval.update_target_t) = new UpdateTarget;
      (yyval.update_target_t)->sub_select = (yyvsp[0].sql_node);
      (yyval.update_target_t)->is_value = 0;
    }
#line 2401 "yacc_sql.cpp"
    break;

  case 70: /* update_value_list: %empty  */
#line 635 "yacc_sql.y"
    {
      (yyval.update_info) = nullptr;
    }
#line 2409 "yacc_sql.cpp"
    break;

  case 71: /* update_value_list: COMMA ID EQ update_value_target update_value_list  */
#line 639 "yacc_sql.y"
    {
      if ((yyvsp[0].update_info) != nullptr) {
        (yyval.update_info) = (yyvsp[0].update_info);
      } else {
        (yyval.update_info) = new UpdateSqlNode;
      }
      (yyval.update_info)->attributes_name.emplace_back((yyvsp[-3].string));
      (yyval.update_info)->targets.emplace_back(*(yyvsp[-1].update_target_t));
      free((yyvsp[-3].string));
      delete (yyvsp[-1].update_target_t);
    }
#line 2425 "yacc_sql.cpp"
    break;

  case 72: /* order_by_type: %empty  */
#line 653 "yacc_sql.y"
    {
      (yyval.order_by_type) = 1;
    }
#line 2433 "yacc_sql.cpp"
    break;

  case 73: /* order_by_type: ASC  */
#line 656 "yacc_sql.y"
          {
      (yyval.order_by_type) = 1;
    }
#line 2441 "yacc_sql.cpp"
    break;

  case 74: /* order_by_type: DESC  */
#line 659 "yacc_sql.y"
           {
      (yyval.order_by_type) = 0;
    }
#line 2449 "yacc_sql.cpp"
    break;

  case 75: /* order_by_body: %empty  */
#line 666 "yacc_sql.y"
    {
      (yyval.order_by_body) = nullptr;
    }
#line 2457 "yacc_sql.cpp"
    break;

  case 76: /* order_by_body: COMMA rel_attr order_by_type order_by_body  */
#line 670 "yacc_sql.y"
    {
      if ((yyvsp[0].order_by_body) != nullptr) {
        (yyval.order_by_body) = (yyvsp[0].order_by_body);
      } else {
        (yyval.order_by_body) = new std::vector<RelAttrSqlNode>;
      }
      (yyvsp[-2].rel_attr)->is_asc = (yyvsp[-1].order_by_type);
      (yyval.order_by_body)->emplace_back(*(yyvsp[-2].rel_attr));
      delete (yyvsp[-2].rel_attr);
    }
#line 2472 "yacc_sql.cpp"
    break;

  case 77: /* order_by_list: %empty  */
#line 684 "yacc_sql.y"
    {
      (yyval.order_by_list) = nullptr;
    }
#line 2480 "yacc_sql.cpp"
    break;

  case 78: /* order_by_list: ORDER_BY rel_attr order_by_type order_by_body  */
#line 688 "yacc_sql.y"
    {
      if ((yyvsp[0].order_by_body) != nullptr) {
        (yyval.order_by_list) = (yyvsp[0].order_by_body);
      } else {
        (yyval.order_by_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyvsp[-2].rel_attr)->is_asc = (yyvsp[-1].order_by_type);
      (yyval.order_by_list)->emplace_back(*(yyvsp[-2].rel_attr));
      delete (yyvsp[-2].rel_attr);
    }
#line 2495 "yacc_sql.cpp"
    break;

  case 79: /* group_by_body: %empty  */
#line 702 "yacc_sql.y"
    {
      (yyval.group_by_body) = nullptr;
    }
#line 2503 "yacc_sql.cpp"
    break;

  case 80: /* group_by_body: COMMA rel_attr order_by_body  */
#line 706 "yacc_sql.y"
    {
      if ((yyvsp[0].order_by_body) != nullptr) {
        (yyval.group_by_body) = (yyvsp[0].order_by_body);
      } else {
        (yyval.group_by_body) = new std::vector<RelAttrSqlNode>;
      }
      (yyval.group_by_body)->emplace_back(*(yyvsp[-1].rel_attr));
      delete (yyvsp[-1].rel_attr);
    }
#line 2517 "yacc_sql.cpp"
    break;

  case 81: /* group_by_list: %empty  */
#line 719 "yacc_sql.y"
    {
      (yyval.group_by_list) = nullptr;
    }
#line 2525 "yacc_sql.cpp"
    break;

  case 82: /* group_by_list: GROUP_BY rel_attr group_by_body  */
#line 723 "yacc_sql.y"
    {
      if ((yyvsp[0].group_by_body) != nullptr) {
        (yyval.group_by_list) = (yyvsp[0].group_by_body);
      } else {
        (yyval.group_by_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyval.group_by_list)->emplace_back(*(yyvsp[-1].rel_attr));
      delete (yyvsp[-1].rel_attr);
    }
#line 2539 "yacc_sql.cpp"
    break;

  case 83: /* select_stmt: SELECT select_attr  */
#line 735 "yacc_sql.y"
                       {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SELECT);
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.sql_node)->selection.attributes.swap(*(yyvsp[0].rel_attr_list));
        delete (yyvsp[0].rel_attr_list);
      }
    }
#line 2551 "yacc_sql.cpp"
    break;

  case 84: /* select_stmt: SELECT select_attr FROM relation rel_list join_list where order_by_list group_by_list having  */
#line 743 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SELECT);
      if ((yyvsp[-8].rel_attr_list) != nullptr) {
        (yyval.sql_node)->selection.attributes.swap(*(yyvsp[-8].rel_attr_list));
        delete (yyvsp[-8].rel_attr_list);
      }
      if ((yyvsp[-5].relation_list) != nullptr) {
        (yyval.sql_node)->selection.relations.swap(*(yyvsp[-5].relation_list));
        delete (yyvsp[-5].relation_list);
      }
      (yyval.sql_node)->selection.relations.push_back(*(yyvsp[-6].relation));
      std::reverse((yyval.sql_node)->selection.relations.begin(), (yyval.sql_node)->selection.relations.end());
      if ((yyvsp[-4].join_list) != nullptr) {
        /* $$->selection.joins.swap(*$6); */
        /* 通过遍历 将$6中的数据取出来*/
        /* 先 reverse 一下 */
        std::reverse((yyvsp[-4].join_list)->begin(), (yyvsp[-4].join_list)->end());
        for (auto &join : *(yyvsp[-4].join_list)) {
          (yyval.sql_node)->selection.relations.push_back(join.relation);
          for (auto &condition : join.conditions) {
            (yyval.sql_node)->selection.conditions.emplace_back(condition);
          }
        }
      }

      if ((yyvsp[-3].condition_list) != nullptr) {
        /*$$->selection.conditions.swap(*$7);*/
        for (auto &condition : *(yyvsp[-3].condition_list)) {
          (yyval.sql_node)->selection.conditions.emplace_back(condition);
        }
        // reverse
        std::reverse((yyval.sql_node)->selection.conditions.begin(), (yyval.sql_node)->selection.conditions.end());
        /* delete $7; */
      }
      delete (yyvsp[-6].relation);

      if ((yyvsp[-2].order_by_list) != nullptr) {
        (yyval.sql_node)->selection.order_attrs.swap(*(yyvsp[-2].order_by_list));
        // reverse
        std::reverse((yyval.sql_node)->selection.order_attrs.begin(), (yyval.sql_node)->selection.order_attrs.end());
        delete (yyvsp[-2].order_by_list);
      }

      if ((yyvsp[-1].group_by_list) != nullptr) {
        (yyval.sql_node)->selection.group_attrs.swap(*(yyvsp[-1].group_by_list));
        // reverse
        std::reverse((yyval.sql_node)->selection.group_attrs.begin(), (yyval.sql_node)->selection.group_attrs.end());
        delete (yyvsp[-1].group_by_list);
      }

      if ((yyvsp[0].condition_list) != nullptr) {
        /*$$->selection.havings.swap(*$10);*/
        for (auto &condition : *(yyvsp[0].condition_list)) {
          (yyval.sql_node)->selection.havings.emplace_back(condition);
        }
        // reverse
        std::reverse((yyval.sql_node)->selection.havings.begin(), (yyval.sql_node)->selection.havings.end());
        delete (yyvsp[0].condition_list);
      }
    }
#line 2616 "yacc_sql.cpp"
    break;

  case 85: /* calc_stmt: CALC expression_list  */
#line 806 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      std::reverse((yyvsp[0].expression_list)->begin(), (yyvsp[0].expression_list)->end());
      (yyval.sql_node)->calc.expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2627 "yacc_sql.cpp"
    break;

  case 86: /* expression_list: expression  */
#line 816 "yacc_sql.y"
    {
      (yyval.expression_list) = new std::vector<Expression*>;
      (yyval.expression_list)->emplace_back((yyvsp[0].expression));
    }
#line 2636 "yacc_sql.cpp"
    break;

  case 87: /* expression_list: expression COMMA expression_list  */
#line 821 "yacc_sql.y"
    {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<Expression *>;
      }
      (yyval.expression_list)->emplace_back((yyvsp[-2].expression));
    }
#line 2649 "yacc_sql.cpp"
    break;

  case 88: /* expression: P_INT  */
#line 831 "yacc_sql.y"
          {
      (yyval.expression) = new ValueExpr(Value((yyvsp[0].number)));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2658 "yacc_sql.cpp"
    break;

  case 89: /* expression: P_FLOAT  */
#line 835 "yacc_sql.y"
              {
      (yyval.expression) = new ValueExpr(Value((yyvsp[0].floats)));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2667 "yacc_sql.cpp"
    break;

  case 90: /* expression: DATE_STR  */
#line 839 "yacc_sql.y"
               {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.expression) = new ValueExpr(Value(date(tmp)));
      free(tmp);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2678 "yacc_sql.cpp"
    break;

  case 91: /* expression: NULL_T  */
#line 845 "yacc_sql.y"
             {
      (yyval.expression) = new ValueExpr(Value(true, true));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2687 "yacc_sql.cpp"
    break;

  case 92: /* expression: SSS  */
#line 849 "yacc_sql.y"
          {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.expression) = new ValueExpr(Value(tmp));
      free(tmp);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2698 "yacc_sql.cpp"
    break;

  case 93: /* expression: ID  */
#line 855 "yacc_sql.y"
         {
      (yyval.expression) = new RelAttrExpr("", (yyvsp[0].string));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2707 "yacc_sql.cpp"
    break;

  case 94: /* expression: ID DOT ID  */
#line 859 "yacc_sql.y"
                {
      (yyval.expression) = new RelAttrExpr((yyvsp[-2].string), (yyvsp[0].string));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2716 "yacc_sql.cpp"
    break;

  case 95: /* expression: ID DOT '*'  */
#line 863 "yacc_sql.y"
                 {
      (yyval.expression) = new RelAttrExpr((yyvsp[-2].string), "*");
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2725 "yacc_sql.cpp"
    break;

  case 96: /* expression: '*'  */
#line 867 "yacc_sql.y"
          {
      (yyval.expression) = new RelAttrExpr("", "*");
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2734 "yacc_sql.cpp"
    break;

  case 97: /* expression: '*' DOT '*'  */
#line 871 "yacc_sql.y"
                  {
      (yyval.expression) = new RelAttrExpr("*", "*");
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2743 "yacc_sql.cpp"
    break;

  case 98: /* expression: aggre_type LBRACE expression_list RBRACE  */
#line 875 "yacc_sql.y"
                                               {
      if ((yyvsp[-1].expression_list) == nullptr || (yyvsp[-1].expression_list)->size() > 1) {
        (yyval.expression) = new AggreExpr((yyvsp[-3].aggre_type), nullptr);
      } else {
        (yyval.expression) = new AggreExpr((yyvsp[-3].aggre_type), (*(yyvsp[-1].expression_list))[0]);
      }
      if ((yyvsp[-1].expression_list) != nullptr) {
        delete (yyvsp[-1].expression_list);
      }
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2759 "yacc_sql.cpp"
    break;

  case 99: /* expression: aggre_type LBRACE RBRACE  */
#line 886 "yacc_sql.y"
                               {
      (yyval.expression) = new AggreExpr((yyvsp[-2].aggre_type), nullptr);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2768 "yacc_sql.cpp"
    break;

  case 100: /* expression: LENGTH LBRACE expression RBRACE  */
#line 890 "yacc_sql.y"
                                      {
      (yyval.expression) = new FuncExpr(FuncType::LENGTH, (yyvsp[-1].expression));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2777 "yacc_sql.cpp"
    break;

  case 101: /* expression: ROUND LBRACE expression RBRACE  */
#line 894 "yacc_sql.y"
                                     {
      FuncExpr *tmp = new FuncExpr(FuncType::ROUND, (yyvsp[-1].expression));
      tmp->set_name(token_name(sql_string, &(yyloc)));
      tmp->set_param(new ValueExpr(Value(0)));
      (yyval.expression) = tmp;
    }
#line 2788 "yacc_sql.cpp"
    break;

  case 102: /* expression: ROUND LBRACE expression COMMA expression RBRACE  */
#line 900 "yacc_sql.y"
                                                      {
      FuncExpr *tmp = new FuncExpr(FuncType::ROUND, (yyvsp[-3].expression));
      tmp->set_name(token_name(sql_string, &(yyloc)));
      tmp->set_param((yyvsp[-1].expression));
      (yyval.expression) = tmp;
    }
#line 2799 "yacc_sql.cpp"
    break;

  case 103: /* expression: DATE_FORMAT LBRACE expression COMMA expression RBRACE  */
#line 906 "yacc_sql.y"
                                                            {
      FuncExpr *tmp = new FuncExpr(FuncType::DATE_FORMAT, (yyvsp[-3].expression));
      tmp->set_name(token_name(sql_string, &(yyloc)));
      tmp->set_param((yyvsp[-1].expression));
      (yyval.expression) = tmp;
    }
#line 2810 "yacc_sql.cpp"
    break;

  case 104: /* expression: expression '+' expression  */
#line 912 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2818 "yacc_sql.cpp"
    break;

  case 105: /* expression: expression '-' expression  */
#line 915 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2826 "yacc_sql.cpp"
    break;

  case 106: /* expression: expression '*' expression  */
#line 918 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2834 "yacc_sql.cpp"
    break;

  case 107: /* expression: expression '/' expression  */
#line 921 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2842 "yacc_sql.cpp"
    break;

  case 108: /* expression: LBRACE expression RBRACE  */
#line 924 "yacc_sql.y"
                               {
      (yyval.expression) = (yyvsp[-1].expression);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2851 "yacc_sql.cpp"
    break;

  case 109: /* expression: '-' expression  */
#line 928 "yacc_sql.y"
                                  {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 2859 "yacc_sql.cpp"
    break;

  case 110: /* select_attr: rel_attr attr_list  */
#line 934 "yacc_sql.y"
                       {
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyval.rel_attr_list)->emplace_back(*(yyvsp[-1].rel_attr));
      delete (yyvsp[-1].rel_attr);
    }
#line 2873 "yacc_sql.cpp"
    break;

  case 111: /* rel_attr: expression  */
#line 1042 "yacc_sql.y"
               {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->expr = (yyvsp[0].expression);
    }
#line 2882 "yacc_sql.cpp"
    break;

  case 112: /* rel_attr: expression AS ID  */
#line 1046 "yacc_sql.y"
                       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->expr = (yyvsp[-2].expression);
      (yyval.rel_attr)->alias = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2893 "yacc_sql.cpp"
    break;

  case 113: /* rel_attr: expression ID  */
#line 1052 "yacc_sql.y"
                    {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->expr = (yyvsp[-1].expression);
      (yyval.rel_attr)->alias = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2904 "yacc_sql.cpp"
    break;

  case 114: /* relation: ID  */
#line 1061 "yacc_sql.y"
       {
      (yyval.relation) = new RelationSqlNode;
      (yyval.relation)->name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2914 "yacc_sql.cpp"
    break;

  case 115: /* relation: ID AS ID  */
#line 1066 "yacc_sql.y"
               {
      (yyval.relation) = new RelationSqlNode;
      (yyval.relation)->name = (yyvsp[-2].string);
      (yyval.relation)->alias = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2926 "yacc_sql.cpp"
    break;

  case 116: /* relation: ID ID  */
#line 1073 "yacc_sql.y"
            {
      (yyval.relation) = new RelationSqlNode;
      (yyval.relation)->name = (yyvsp[-1].string);
      (yyval.relation)->alias = (yyvsp[0].string);
      free((yyvsp[-1].string));
      free((yyvsp[0].string));
    }
#line 2938 "yacc_sql.cpp"
    break;

  case 117: /* attr_list: %empty  */
#line 1084 "yacc_sql.y"
    {
      (yyval.rel_attr_list) = nullptr;
    }
#line 2946 "yacc_sql.cpp"
    break;

  case 118: /* attr_list: COMMA rel_attr attr_list  */
#line 1087 "yacc_sql.y"
                               {
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }

      (yyval.rel_attr_list)->emplace_back(*(yyvsp[-1].rel_attr));
      delete (yyvsp[-1].rel_attr);
    }
#line 2961 "yacc_sql.cpp"
    break;

  case 119: /* rel_list: %empty  */
#line 1101 "yacc_sql.y"
    {
      (yyval.relation_list) = nullptr;
    }
#line 2969 "yacc_sql.cpp"
    break;

  case 120: /* rel_list: COMMA relation rel_list  */
#line 1104 "yacc_sql.y"
                              {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<RelationSqlNode>;
      }

      (yyval.relation_list)->emplace_back(*(yyvsp[-1].relation));
      free((yyvsp[-1].relation));
    }
#line 2984 "yacc_sql.cpp"
    break;

  case 121: /* join_list: %empty  */
#line 1126 "yacc_sql.y"
    {
      (yyval.join_list) = nullptr;
    }
#line 2992 "yacc_sql.cpp"
    break;

  case 122: /* join_list: INNER_JOIN relation ON condition_list join_list  */
#line 1129 "yacc_sql.y"
                                                      {
      if ((yyvsp[0].join_list) != nullptr) {
        (yyval.join_list) = (yyvsp[0].join_list);
      } else {
        (yyval.join_list) = new std::vector<JoinSqlNode>;
      }

      JoinSqlNode join1;
      join1.relation = *(yyvsp[-3].relation);
      delete (yyvsp[-3].relation);
      /*join1.conditions.swap(*$4); */
      /*join1.conditions.emplace_back(*$4); */
      // reverse
      std::reverse((yyvsp[-1].condition_list)->begin(), (yyvsp[-1].condition_list)->end());
      for (auto &condition : *(yyvsp[-1].condition_list)) {
        join1.conditions.emplace_back(condition);
      }
      (yyval.join_list)->emplace_back(join1);
      /*delete $4;
      free($2);*/
    }
#line 3018 "yacc_sql.cpp"
    break;

  case 123: /* sub_select_stmt: LBRACE select_stmt RBRACE  */
#line 1175 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[-1].sql_node);
    }
#line 3026 "yacc_sql.cpp"
    break;

  case 124: /* where: %empty  */
#line 1183 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3034 "yacc_sql.cpp"
    break;

  case 125: /* where: WHERE condition_list  */
#line 1186 "yacc_sql.y"
                           {
      (yyval.condition_list) = (yyvsp[0].condition_list);  
    }
#line 3042 "yacc_sql.cpp"
    break;

  case 126: /* having: %empty  */
#line 1193 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3050 "yacc_sql.cpp"
    break;

  case 127: /* having: HAVING condition_list  */
#line 1196 "yacc_sql.y"
                            {
      (yyval.condition_list) = (yyvsp[0].condition_list);
    }
#line 3058 "yacc_sql.cpp"
    break;

  case 128: /* condition_list: %empty  */
#line 1203 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3066 "yacc_sql.cpp"
    break;

  case 129: /* condition_list: condition  */
#line 1206 "yacc_sql.y"
                {
      (yyval.condition_list) = new std::vector<ConditionSqlNode>;
      (yyvsp[0].condition)->conjunction_type = 0;
      (yyval.condition_list)->emplace_back(*(yyvsp[0].condition));
      delete (yyvsp[0].condition);
    }
#line 3077 "yacc_sql.cpp"
    break;

  case 130: /* condition_list: condition AND condition_list  */
#line 1212 "yacc_sql.y"
                                   {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyvsp[-2].condition)->conjunction_type = 1;
      (yyval.condition_list)->emplace_back(*(yyvsp[-2].condition));
      delete (yyvsp[-2].condition);
    }
#line 3088 "yacc_sql.cpp"
    break;

  case 131: /* condition_list: condition OR condition_list  */
#line 1218 "yacc_sql.y"
                                  {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyvsp[-2].condition)->conjunction_type = 2;
      (yyval.condition_list)->emplace_back(*(yyvsp[-2].condition));
      delete (yyvsp[-2].condition);
    }
#line 3099 "yacc_sql.cpp"
    break;

  case 132: /* condition: expression comp_op expression  */
#line 1247 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      // $$->left_is_attr = 0;
      (yyval.condition)->left_expr = (yyvsp[-2].expression);
      (yyval.condition)->left_is_expr = true;
      // $$->right_is_attr = 0;
      (yyval.condition)->right_expr = (yyvsp[0].expression);
      (yyval.condition)->right_is_expr = true;
      (yyval.condition)->comp = (yyvsp[-1].comp);
      (yyval.condition)->sub_select = 0;
    }
#line 3115 "yacc_sql.cpp"
    break;

  case 133: /* condition: expression comp_op sub_select_stmt  */
#line 1259 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      // $$->left_is_attr = 0;
      (yyval.condition)->left_expr = (yyvsp[-2].expression);
      (yyval.condition)->left_is_expr = true;
      // $$->right_is_attr = 0;
      //$$->right_value = nullptr;
      //$$->right_sub_select = std::unique_ptr<ParsedSqlNode>($3);
      (yyval.condition)->right_sub_select = (yyvsp[0].sql_node);
      (yyval.condition)->right_is_expr = false;
      //  char            sub_select;      // 0: not sub select, 1: left sub select, 2: right sub select
      (yyval.condition)->sub_select = 2;
      (yyval.condition)->comp = (yyvsp[-1].comp);
    }
#line 3134 "yacc_sql.cpp"
    break;

  case 134: /* condition: expression IN LBRACE value value_list RBRACE  */
#line 1273 "yacc_sql.y"
                                                   {
      // IN/NOT IN
      (yyval.condition) = new ConditionSqlNode;
      // $$->left_is_attr = 1;
      (yyval.condition)->left_expr = (yyvsp[-5].expression);
      (yyval.condition)->left_is_expr = true;
      // $$->right_is_attr = 0;
      (yyval.condition)->comp = IN_;
      (yyval.condition)->sub_select = 2;
      (yyval.condition)->right_sub_select = new ParsedSqlNode(SCF_SOME_VALUES);
      (yyval.condition)->right_is_expr = false;
      if ((yyvsp[-1].value_list) != nullptr) {
        (yyval.condition)->right_sub_select->some_values.values.swap(*(yyvsp[-1].value_list));
        delete (yyvsp[-1].value_list);
      }
      (yyval.condition)->right_sub_select->some_values.values.emplace_back(*(yyvsp[-2].value));
      std::reverse((yyval.condition)->right_sub_select->some_values.values.begin(), (yyval.condition)->right_sub_select->some_values.values.end());
      delete (yyvsp[-2].value);
    }
#line 3158 "yacc_sql.cpp"
    break;

  case 135: /* condition: expression IN sub_select_stmt  */
#line 1292 "yacc_sql.y"
                                    {
      // IN/NOT IN
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = (yyvsp[-2].expression);
      (yyval.condition)->left_is_expr = true;
      (yyval.condition)->comp = IN_;
      (yyval.condition)->sub_select = 2;
      (yyval.condition)->right_sub_select = (yyvsp[0].sql_node);
      (yyval.condition)->right_is_expr = false;
    }
#line 3173 "yacc_sql.cpp"
    break;

  case 136: /* condition: expression NI LBRACE value value_list RBRACE  */
#line 1302 "yacc_sql.y"
                                                   {
      // IN/NOT IN
      (yyval.condition) = new ConditionSqlNode;
      // $$->left_is_attr = 1;
      (yyval.condition)->left_expr = (yyvsp[-5].expression);
      (yyval.condition)->left_is_expr = true;
      // $$->right_is_attr = 0;
      (yyval.condition)->comp = NOT_IN;
      (yyval.condition)->sub_select = 2;
      (yyval.condition)->right_sub_select = new ParsedSqlNode(SCF_SOME_VALUES);
      (yyval.condition)->right_is_expr = false;
      if ((yyvsp[-1].value_list) != nullptr) {
        (yyval.condition)->right_sub_select->some_values.values.swap(*(yyvsp[-1].value_list));
        delete (yyvsp[-1].value_list);
      }
      (yyval.condition)->right_sub_select->some_values.values.emplace_back(*(yyvsp[-2].value));
      std::reverse((yyval.condition)->right_sub_select->some_values.values.begin(), (yyval.condition)->right_sub_select->some_values.values.end());
      delete (yyvsp[-2].value);
    }
#line 3197 "yacc_sql.cpp"
    break;

  case 137: /* condition: expression NI sub_select_stmt  */
#line 1321 "yacc_sql.y"
                                    {
      // IN/NOT IN
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = (yyvsp[-2].expression);
      (yyval.condition)->left_is_expr = true;
      (yyval.condition)->comp = NOT_IN;
      (yyval.condition)->sub_select = 2;
      (yyval.condition)->right_sub_select = (yyvsp[0].sql_node);
      (yyval.condition)->right_is_expr = false;
    }
#line 3212 "yacc_sql.cpp"
    break;

  case 138: /* condition: sub_select_stmt comp_op expression  */
#line 1332 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      // $$->left_is_attr = 0;
      //$$->left_value = nullptr;
      //$$->left_sub_select = std::unique_ptr<ParsedSqlNode>($1);
      (yyval.condition)->left_sub_select = (yyvsp[-2].sql_node);
      (yyval.condition)->left_is_expr = false;
      // $$->right_is_attr = 0;
      (yyval.condition)->right_expr = (yyvsp[0].expression);
      (yyval.condition)->right_is_expr = true;
      (yyval.condition)->sub_select = 1;
      (yyval.condition)->comp = (yyvsp[-1].comp);
    }
#line 3230 "yacc_sql.cpp"
    break;

  case 139: /* condition: EXISTS sub_select_stmt  */
#line 1360 "yacc_sql.y"
    {
      // EXISTS/NOT EXISTS
      (yyval.condition) = new ConditionSqlNode;
      // $$->left_is_attr = 0;
      // $$->right_is_attr = 0;
      (yyval.condition)->comp = EXISTS_;
      (yyval.condition)->right_sub_select = (yyvsp[0].sql_node);
      (yyval.condition)->left_is_expr = false;
      (yyval.condition)->right_is_expr = false;
      (yyval.condition)->sub_select = 2;
    }
#line 3246 "yacc_sql.cpp"
    break;

  case 140: /* condition: NOT_EXISTS sub_select_stmt  */
#line 1372 "yacc_sql.y"
    {
      // EXISTS/NOT EXISTS
      (yyval.condition) = new ConditionSqlNode;
      // $$->left_is_attr = 0;
      // $$->right_is_attr = 0;
      (yyval.condition)->comp = NOT_EXISTS_;
      (yyval.condition)->right_sub_select = (yyvsp[0].sql_node);
      (yyval.condition)->sub_select = 2;
    }
#line 3260 "yacc_sql.cpp"
    break;

  case 141: /* condition: EXISTS LBRACE value value_list RBRACE  */
#line 1381 "yacc_sql.y"
                                            {
      // EXISTS/NOT EXISTS
      (yyval.condition) = new ConditionSqlNode;
      // $$->left_is_attr = 0;
      // $$->right_is_attr = 0;
      (yyval.condition)->comp = EXISTS_;
      (yyval.condition)->left_is_expr = false;
      (yyval.condition)->right_is_expr = false;
      (yyval.condition)->right_sub_select = new ParsedSqlNode(SCF_SOME_VALUES);
      if ((yyvsp[-1].value_list) != nullptr) {
        (yyval.condition)->right_sub_select->some_values.values.swap(*(yyvsp[-1].value_list));
        delete (yyvsp[-1].value_list);
      }
      (yyval.condition)->right_sub_select->some_values.values.emplace_back(*(yyvsp[-2].value));
      std::reverse((yyval.condition)->right_sub_select->some_values.values.begin(), (yyval.condition)->right_sub_select->some_values.values.end());
      delete (yyvsp[-2].value);
      (yyval.condition)->sub_select = 2;
    }
#line 3283 "yacc_sql.cpp"
    break;

  case 142: /* condition: NOT_EXISTS LBRACE value value_list RBRACE  */
#line 1399 "yacc_sql.y"
                                                {
      // EXISTS/NOT EXISTS
      (yyval.condition) = new ConditionSqlNode;
      // $$->left_is_attr = 0;
      // $$->right_is_attr = 0;
      (yyval.condition)->comp = NOT_EXISTS_;
      (yyval.condition)->left_is_expr = false;
      (yyval.condition)->right_is_expr = false;
      (yyval.condition)->right_sub_select = new ParsedSqlNode(SCF_SOME_VALUES);
      if ((yyvsp[-1].value_list) != nullptr) {
        (yyval.condition)->right_sub_select->some_values.values.swap(*(yyvsp[-1].value_list));
        delete (yyvsp[-1].value_list);
      }
      (yyval.condition)->right_sub_select->some_values.values.emplace_back(*(yyvsp[-2].value));
      std::reverse((yyval.condition)->right_sub_select->some_values.values.begin(), (yyval.condition)->right_sub_select->some_values.values.end());
      delete (yyvsp[-2].value);
      (yyval.condition)->sub_select = 2;
    }
#line 3306 "yacc_sql.cpp"
    break;

  case 143: /* condition: sub_select_stmt comp_op sub_select_stmt  */
#line 1418 "yacc_sql.y"
    {
      // EXISTS/NOT EXISTS
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_expr = 0;
      (yyval.condition)->right_is_expr = 0;
      (yyval.condition)->comp = (yyvsp[-1].comp);
      (yyval.condition)->left_sub_select = (yyvsp[-2].sql_node);
      (yyval.condition)->right_sub_select = (yyvsp[0].sql_node);
      (yyval.condition)->sub_select = 3;
    }
#line 3321 "yacc_sql.cpp"
    break;

  case 144: /* comp_op: EQ  */
#line 1431 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 3327 "yacc_sql.cpp"
    break;

  case 145: /* comp_op: LT  */
#line 1432 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 3333 "yacc_sql.cpp"
    break;

  case 146: /* comp_op: GT  */
#line 1433 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 3339 "yacc_sql.cpp"
    break;

  case 147: /* comp_op: LE  */
#line 1434 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 3345 "yacc_sql.cpp"
    break;

  case 148: /* comp_op: GE  */
#line 1435 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 3351 "yacc_sql.cpp"
    break;

  case 149: /* comp_op: LK  */
#line 1436 "yacc_sql.y"
         { (yyval.comp) = LIKE; }
#line 3357 "yacc_sql.cpp"
    break;

  case 150: /* comp_op: NLK  */
#line 1437 "yacc_sql.y"
          { (yyval.comp) = NOT_LIKE; }
#line 3363 "yacc_sql.cpp"
    break;

  case 151: /* comp_op: NE  */
#line 1438 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 3369 "yacc_sql.cpp"
    break;

  case 152: /* comp_op: IS_NOT  */
#line 1439 "yacc_sql.y"
             { (yyval.comp) = IS_NOT_; }
#line 3375 "yacc_sql.cpp"
    break;

  case 153: /* comp_op: IS  */
#line 1440 "yacc_sql.y"
         { (yyval.comp) = IS_; }
#line 3381 "yacc_sql.cpp"
    break;

  case 154: /* aggre_type: MAX  */
#line 1444 "yacc_sql.y"
          { (yyval.aggre_type) = AggreType::MAX; }
#line 3387 "yacc_sql.cpp"
    break;

  case 155: /* aggre_type: MIN  */
#line 1445 "yacc_sql.y"
          { (yyval.aggre_type) = AggreType::MIN; }
#line 3393 "yacc_sql.cpp"
    break;

  case 156: /* aggre_type: SUM  */
#line 1446 "yacc_sql.y"
          { (yyval.aggre_type) = AggreType::SUM; }
#line 3399 "yacc_sql.cpp"
    break;

  case 157: /* aggre_type: CNT  */
#line 1447 "yacc_sql.y"
          { (yyval.aggre_type) = AggreType::CNT; }
#line 3405 "yacc_sql.cpp"
    break;

  case 158: /* aggre_type: AVG  */
#line 1448 "yacc_sql.y"
          { (yyval.aggre_type) = AggreType::AVG; }
#line 3411 "yacc_sql.cpp"
    break;

  case 159: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE ID  */
#line 1453 "yacc_sql.y"
    {
      char *tmp_file_name = common::substr((yyvsp[-3].string), 1, strlen((yyvsp[-3].string)) - 2);
      
      (yyval.sql_node) = new ParsedSqlNode(SCF_LOAD_DATA);
      (yyval.sql_node)->load_data.relation_name = (yyvsp[0].string);
      (yyval.sql_node)->load_data.file_name = tmp_file_name;
      free((yyvsp[0].string));
      free(tmp_file_name);
    }
#line 3425 "yacc_sql.cpp"
    break;

  case 160: /* explain_stmt: EXPLAIN command_wrapper  */
#line 1466 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->explain.sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 3434 "yacc_sql.cpp"
    break;

  case 161: /* set_variable_stmt: SET ID EQ value  */
#line 1474 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name  = (yyvsp[-2].string);
      (yyval.sql_node)->set_variable.value = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 3446 "yacc_sql.cpp"
    break;


#line 3450 "yacc_sql.cpp"

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

#line 1486 "yacc_sql.y"

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
