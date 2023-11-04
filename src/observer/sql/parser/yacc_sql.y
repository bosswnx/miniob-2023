
%{

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

%}

%define api.pure full
%define parse.error verbose
/** 启用位置标识 **/
%locations
%lex-param { yyscan_t scanner }
/** 这些定义了在yyparse函数中的参数 **/
%parse-param { const char * sql_string }
%parse-param { ParsedSqlResult * sql_result }
%parse-param { void * scanner }

//标识tokens
%token  SEMICOLON
        CREATE
        DROP
        TABLE
        TABLES
        INDEX
        CALC
        SELECT
        DESC
        SHOW
        SYNC
        INSERT
        DELETE
        UPDATE
        LBRACE
        RBRACE
        COMMA
        TRX_BEGIN
        TRX_COMMIT
        TRX_ROLLBACK
        INT_T
        STRING_T
        DATE_T
        FLOAT_T
        HELP
        EXIT
        DOT //QUOTE
        INTO
        VALUES
        FROM
        INNER_JOIN
        WHERE
        ORDER_BY
        ASC
        IN
        NI // NOT_IN
        EXISTS
        NOT_EXISTS
        AND
        OR
        SET
        ON
        LOAD
        DATA
        INFILE
        NULLABLE
        NOT_NULL
        EXPLAIN
        UNIQUE
        AS
        EQ
        LT
        GT
        LE
        GE
        NE
        MAX
        MIN
        SUM
        CNT
        AVG
        LK
        NLK // NOT_LIKE
        IS_NOT
        IS

/** union 中定义各种数据类型，真实生成的代码也是union类型，所以不能有非POD类型的数据 **/
%union {
  ParsedSqlNode *                   sql_node;
  ConditionSqlNode *                condition;
  ConditionSqlNode *                join_condition;
  Value *                           value;
  enum CompOp                       comp;
  RelAttrSqlNode *                  rel_attr;
  RelationSqlNode *                 relation;
  std::vector<AttrInfoSqlNode> *    attr_infos;
  AttrInfoSqlNode *                 attr_info;
  UpdateTarget *                      update_target_t;
  Expression *                      expression;
  std::vector<Expression *> *       expression_list;
  std::vector<Value> *              value_list;
  std::vector<ConditionSqlNode> *   condition_list;
  std::vector<RelAttrSqlNode> *     rel_attr_list;
  std::vector<RelationSqlNode> *    relation_list;
  std::vector<RelAttrSqlNode> *     order_by_list;
  std::vector<RelAttrSqlNode> *     order_by_body;
  bool                              order_by_type;
  std::vector<std::string> *        ID_list;
  std::vector<JoinSqlNode> *        join_list;
  UpdateSqlNode *                   update_info;
  char *                            string;
  int                               number;
  float                             floats;
}

%token <number> NUMBER
%token <floats> FLOAT
%token <string> ID
%token <string> DATE_STR
%token <string> SSS
%token <string> NULL_T
//非终结符

/** type 定义了各种解析后的结果输出的是什么类型。类型对应了 union 中的定义的成员变量名称 **/
%type <number>              type
%type <condition>           condition
// %type <join_condition>      join_condition
%type <value>               value
%type <number>              number


%type <comp>                comp_op
%type <rel_attr>            rel_attr
%type <relation>            relation
%type <attr_infos>          attr_def_list
%type <number>              opt_null
%type <attr_info>           attr_def
%type <value_list>          value_list
%type <condition_list>      where
%type <condition_list>      condition_list
%type <rel_attr_list>       select_attr
%type <relation_list>       rel_list
%type <ID_list>             ID_list
%type <rel_attr_list>       aggre_attr
%type <rel_attr_list>       aggre_list
%type <rel_attr_list>       attr_list
%type <order_by_type>       order_by_type
%type <order_by_body>       order_by_body
%type <order_by_list>       order_by_list
%type <join_list>           join_list
%type <expression>          expression
%type <expression_list>     expression_list
%type <update_info>         update_value_list
%type <sql_node>            calc_stmt
%type <sql_node>            select_stmt
%type <sql_node>            sub_select_stmt
%type <update_target_t>     update_value_target
%type <sql_node>            insert_stmt
%type <sql_node>            update_stmt
%type <sql_node>            delete_stmt
%type <sql_node>            create_table_stmt
%type <sql_node>            drop_table_stmt
%type <sql_node>            show_tables_stmt
%type <sql_node>            desc_table_stmt
%type <sql_node>            create_index_stmt
%type <sql_node>            drop_index_stmt
%type <sql_node>            sync_stmt
%type <sql_node>            begin_stmt
%type <sql_node>            commit_stmt
%type <sql_node>            rollback_stmt
%type <sql_node>            load_data_stmt
%type <sql_node>            explain_stmt
%type <sql_node>            set_variable_stmt
%type <sql_node>            help_stmt
%type <sql_node>            exit_stmt
%type <sql_node>            command_wrapper
// commands should be a list but I use a single command instead
%type <sql_node>            commands

%left '+' '-'
%left '*' '/'
%nonassoc UMINUS
%%

commands: command_wrapper opt_semicolon  //commands or sqls. parser starts here.
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>($1);
    sql_result->add_sql_node(std::move(sql_node));
  }
  ;

command_wrapper:
    calc_stmt
  | select_stmt
  | sub_select_stmt
  | insert_stmt
  | update_stmt
  | delete_stmt
  | create_table_stmt
  | drop_table_stmt
  | show_tables_stmt
  | desc_table_stmt
  | create_index_stmt
  | drop_index_stmt
  | sync_stmt
  | begin_stmt
  | commit_stmt
  | rollback_stmt
  | load_data_stmt
  | explain_stmt
  | set_variable_stmt
  | help_stmt
  | exit_stmt
    ;

exit_stmt:      
    EXIT {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      $$ = new ParsedSqlNode(SCF_EXIT);
    };

help_stmt:
    HELP {
      $$ = new ParsedSqlNode(SCF_HELP);
    };

sync_stmt:
    SYNC {
      $$ = new ParsedSqlNode(SCF_SYNC);
    }
    ;

begin_stmt:
    TRX_BEGIN  {
      $$ = new ParsedSqlNode(SCF_BEGIN);
    }
    ;

commit_stmt:
    TRX_COMMIT {
      $$ = new ParsedSqlNode(SCF_COMMIT);
    }
    ;

rollback_stmt:
    TRX_ROLLBACK  {
      $$ = new ParsedSqlNode(SCF_ROLLBACK);
    }
    ;

drop_table_stmt:    /*drop table 语句的语法解析树*/
    DROP TABLE ID {
      $$ = new ParsedSqlNode(SCF_DROP_TABLE);
      $$->drop_table.relation_name = $3;
      free($3);
    };

show_tables_stmt:
    SHOW TABLES {
      $$ = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
    ;

desc_table_stmt:
    DESC ID  {
      $$ = new ParsedSqlNode(SCF_DESC_TABLE);
      $$->desc_table.relation_name = $2;
      free($2);
    }
    ;

create_index_stmt:    /*create index 语句的语法解析树*/
    CREATE INDEX ID ON ID LBRACE ID ID_list RBRACE
    {
      $$ = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = $$->create_index;
      if ($8 != nullptr) {
        create_index.attribute_names.swap(*$8);
      }
      create_index.is_unique = false;
      create_index.index_name = $3;
      create_index.relation_name = $5;
      create_index.attribute_names.push_back($7);
      std::reverse(create_index.attribute_names.begin(), create_index.attribute_names.end());
      free($3);
      free($5);
      free($7);
    } 
    | CREATE UNIQUE INDEX ID ON ID LBRACE ID ID_list RBRACE
    {
      $$ = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = $$->create_index;
      if ($9 != nullptr) {
        create_index.attribute_names.swap(*$9);
      }
      create_index.is_unique = true;
      create_index.index_name = $4;
      create_index.relation_name = $6;
      create_index.attribute_names.push_back($8);
      std::reverse(create_index.attribute_names.begin(), create_index.attribute_names.end());
      free($4);
      free($6);
      free($8);
    }
    ;

drop_index_stmt:      /*drop index 语句的语法解析树*/
    DROP INDEX ID ON ID
    {
      $$ = new ParsedSqlNode(SCF_DROP_INDEX);
      $$->drop_index.index_name = $3;
      $$->drop_index.relation_name = $5;
      free($3);
      free($5);
    }
    ;
create_table_stmt:    /*create table 语句的语法解析树*/
    CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE
    {
      $$ = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = $$->create_table;
      create_table.relation_name = $3;
      create_table.sub_select = nullptr;
      free($3);

      std::vector<AttrInfoSqlNode> *src_attrs = $6;

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
      }
      create_table.attr_infos.emplace_back(*$5);
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete $5;
    }
    //as
    | CREATE TABLE ID AS select_stmt
    {
      $$ = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = $$->create_table;
      create_table.relation_name = $3;
      free($3);
      create_table.sub_select = $5;
    }
    // as with attr
    | CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE AS select_stmt
    {
      $$ = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = $$->create_table;
      create_table.relation_name = $3;
      free($3);

      std::vector<AttrInfoSqlNode> *src_attrs = $6;

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
      }
      create_table.attr_infos.emplace_back(*$5);
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete $5;

      create_table.sub_select = $9;
    }
    ;

ID_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA ID ID_list
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<std::string>;
      }
      $$->emplace_back($2);
      delete $2;
    }
    ;

attr_def_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA attr_def attr_def_list
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<AttrInfoSqlNode>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;
    
attr_def:
    ID type LBRACE number RBRACE 
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = $4;
      $$->is_null = false;
      free($1);
    }
    | ID type
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = 4;
      $$->is_null = false;
      free($1);
    }
    | ID type opt_null
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = 4;
      $$->is_null = $3;
      free($1);
      
    }
    | ID type LBRACE number RBRACE opt_null
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = $4;
      $$->is_null = $6;
      free($1);
      
    }
    ;
opt_null:
	NOT_NULL {
		$$ = false;
	}
	| NULLABLE {
		$$ = true;
	}
	; 
number:
    NUMBER {$$ = $1;}
    ;
type:
    INT_T      { $$=INTS; }
    | STRING_T { $$=CHARS; }
    | FLOAT_T  { $$=FLOATS; }
    | DATE_T  { $$=DATES; }
    ;
insert_stmt:        /*insert   语句的语法解析树*/
    INSERT INTO ID VALUES LBRACE value value_list RBRACE 
    {
      $$ = new ParsedSqlNode(SCF_INSERT);
      $$->insertion.relation_name = $3;
      if ($7 != nullptr) {
        $$->insertion.values.swap(*$7);
      }
      $$->insertion.values.emplace_back(*$6);
      std::reverse($$->insertion.values.begin(), $$->insertion.values.end());
      delete $6;
      free($3);
    }
    ;

value_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA value value_list  { 
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<Value>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;
value:
    NUMBER {
      $$ = new Value((int)$1);
      @$ = @1;
    }
    |FLOAT {
      $$ = new Value((float)$1);
      @$ = @1;
    }
    |DATE_STR {
      char *tmp = common::substr($1,1,strlen($1)-2);
      $$ = new Value(date(tmp));
      free(tmp);
      @$ = @1;
    }
    |SSS {
      char *tmp = common::substr($1,1,strlen($1)-2);
      $$ = new Value(tmp);
      free(tmp);
    }
    |NULLABLE {
      $$ = new Value(true, true);
    }
    ;
    
delete_stmt:    /*  delete 语句的语法解析树*/
    DELETE FROM ID where 
    {
      $$ = new ParsedSqlNode(SCF_DELETE);
      $$->deletion.relation_name = $3;
      if ($4 != nullptr) {
        $$->deletion.conditions.swap(*$4);
        delete $4;
      }
      free($3);
    }
    ;

update_stmt:      /*  update 语句的语法解析树*/
    UPDATE ID SET ID EQ update_value_target update_value_list where 
    {
      $$ = new ParsedSqlNode(SCF_UPDATE);
      if ($7 != nullptr) {
        $$->update.attributes_name.swap($7->attributes_name);
        $$->update.targets.swap($7->targets);
        delete $7;
      }
      $$->update.relation_name = $2;
      $$->update.attributes_name.emplace_back($4);
      $$->update.targets.emplace_back(*$6);
      // reverse
      std::reverse($$->update.targets.begin(), $$->update.targets.end());
      std::reverse($$->update.attributes_name.begin(), $$->update.attributes_name.end());
      if ($8 != nullptr) {
        $$->update.conditions.swap(*$8);
        //reverse
        std::reverse($$->update.conditions.begin(), $$->update.conditions.end());
        delete $8;
      }
      free($2);
      free($4);
    };

update_value_target:
    value {
      $$ = new UpdateTarget;
      $$->value = *$1;
      $$->is_value = 1;
    }
    | sub_select_stmt {
      $$ = new UpdateTarget;
      $$->sub_select = $1;
      $$->is_value = 0;
    };

update_value_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA ID EQ update_value_target update_value_list
    {
      if ($5 != nullptr) {
        $$ = $5;
      } else {
        $$ = new UpdateSqlNode;
      }
      $$->attributes_name.emplace_back($2);
      $$->targets.emplace_back(*$4);
      free($2);
      delete $4;
    };

order_by_type:
    /* empty */
    {
      $$ = 1;
    }
    | ASC {
      $$ = 1;
    }
    | DESC {
      $$ = 0;
    }
    ;

order_by_body:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA rel_attr order_by_type order_by_body
    {
      if ($4 != nullptr) {
        $$ = $4;
      } else {
        $$ = new std::vector<RelAttrSqlNode>;
      }
      $2->is_asc = $3;
      $$->emplace_back(*$2);
      delete $2;
    }
    ;

order_by_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | ORDER_BY rel_attr order_by_type order_by_body
    {
      if ($4 != nullptr) {
        $$ = $4;
      } else {
        $$ = new std::vector<RelAttrSqlNode>;
      }
      $2->is_asc = $3;
      $$->emplace_back(*$2);
      delete $2;
    }
    ;

select_stmt:        /*  select 语句的语法解析树。这里为什么 rel_list 前还要加一个 ID 呢？因为要保证至少有一个表。*/
    SELECT select_attr FROM relation rel_list join_list where order_by_list
    {
      $$ = new ParsedSqlNode(SCF_SELECT);
      if ($2 != nullptr) {
        $$->selection.attributes.swap(*$2);
        delete $2;
      }
      if ($5 != nullptr) {
        $$->selection.relations.swap(*$5);
        delete $5;
      }
      $$->selection.relations.push_back(*$4);
      std::reverse($$->selection.relations.begin(), $$->selection.relations.end());
      if ($6 != nullptr) {
        /* $$->selection.joins.swap(*$6); */
        /* 通过遍历 将$6中的数据取出来*/
        /* 先 reverse 一下 */
        std::reverse($6->begin(), $6->end());
        for (auto &join : *$6) {
          $$->selection.relations.push_back(join.relation);
          for (auto &condition : join.conditions) {
            $$->selection.conditions.emplace_back(condition);
          }
        }
      }

      if ($7 != nullptr) {
        /*$$->selection.conditions.swap(*$7);*/
        for (auto &condition : *$7) {
          $$->selection.conditions.emplace_back(condition);
        }
        // reverse
        std::reverse($$->selection.conditions.begin(), $$->selection.conditions.end());
        /* delete $7; */
      }
      delete $4;

      if ($8 != nullptr) {
        $$->selection.order_attrs.swap(*$8);
        // reverse
        std::reverse($$->selection.order_attrs.begin(), $$->selection.order_attrs.end());
        delete $8;
      }
    }
    ;
calc_stmt:
    CALC expression_list
    {
      $$ = new ParsedSqlNode(SCF_CALC);
      std::reverse($2->begin(), $2->end());
      $$->calc.expressions.swap(*$2);
      delete $2;
    }
    ;

expression_list:
    expression
    {
      $$ = new std::vector<Expression*>;
      $$->emplace_back($1);
    }
    | expression COMMA expression_list
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<Expression *>;
      }
      $$->emplace_back($1);
    }
    ;
expression:
    expression '+' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::ADD, $1, $3, sql_string, &@$);
    }
    | expression '-' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::SUB, $1, $3, sql_string, &@$);
    }
    | expression '*' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::MUL, $1, $3, sql_string, &@$);
    }
    | expression '/' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::DIV, $1, $3, sql_string, &@$);
    }
    | LBRACE expression RBRACE {
      $$ = $2;
      $$->set_name(token_name(sql_string, &@$));
    }
    | '-' expression %prec UMINUS {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, $2, nullptr, sql_string, &@$);
    }
    | value {
      $$ = new ValueExpr(*$1);
      $$->set_name(token_name(sql_string, &@$));
      delete $1;
    }
    ;

select_attr:
    rel_attr aggre_list {
      if ($2 != nullptr) {
        $$ = $2;
      } else {
        $$ = new std::vector<RelAttrSqlNode>;
      }
      $$->emplace_back(*$1);
      delete $1;
    }
    | aggre_attr aggre_list {
      if ($2 != nullptr) {
        $$ = $2;
      } else {
        $$ = new std::vector<RelAttrSqlNode>;
      }
      $$->insert($$->end(), $1->begin(), $1->end());
      delete $1;
    }
    ;

aggre_attr:
    MAX LBRACE RBRACE {
      $$ = new std::vector<RelAttrSqlNode>;
    }
    | MIN LBRACE RBRACE {
      $$ = new std::vector<RelAttrSqlNode>;
    }
    | SUM LBRACE RBRACE {
      $$ = new std::vector<RelAttrSqlNode>;
    }
    | CNT LBRACE RBRACE {
      $$ = new std::vector<RelAttrSqlNode>;
    }
    | AVG LBRACE RBRACE {
      $$ = new std::vector<RelAttrSqlNode>;
    }
    | MAX LBRACE rel_attr attr_list RBRACE {
      if ($4 != nullptr) {
        $$ = $4;
      } else {
        $$ = new std::vector<RelAttrSqlNode>;
      }
      $3->aggre_type = AggreType::MAX;
      $$->emplace_back(*$3);
      delete $3;
    }
    | MIN LBRACE rel_attr attr_list RBRACE {
      if ($4 != nullptr) {
        $$ = $4;
      } else {
        $$ = new std::vector<RelAttrSqlNode>;
      }
      $3->aggre_type = AggreType::MIN;
      $$->emplace_back(*$3);
      delete $3;
    }
    | SUM LBRACE rel_attr attr_list RBRACE {
      if ($4 != nullptr) {
        $$ = $4;
      } else {
        $$ = new std::vector<RelAttrSqlNode>;
      }
      $3->aggre_type = AggreType::SUM;
      $$->emplace_back(*$3);
      delete $3;
    }
    | CNT LBRACE rel_attr attr_list RBRACE {
      if ($4 != nullptr) {
        $$ = $4;
      } else {
        $$ = new std::vector<RelAttrSqlNode>;
      }
      $3->aggre_type = AggreType::CNT;
      $$->emplace_back(*$3);
      delete $3;
    }
    | AVG LBRACE rel_attr attr_list RBRACE {
      if ($4 != nullptr) {
        $$ = $4;
      } else {
        $$ = new std::vector<RelAttrSqlNode>;
      }
      $3->aggre_type = AggreType::AVG;
      $$->emplace_back(*$3);
      delete $3;
    }
    ;

aggre_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA aggre_attr aggre_list {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<RelAttrSqlNode>;
      }
      $$->insert($$->end(), $2->begin(), $2->end());
      delete $2;
    }
    | COMMA rel_attr aggre_list {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<RelAttrSqlNode>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;

rel_attr:
    ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name = "";
      $$->attribute_name = $1;
      $$->aggre_type = AggreType::NONE;
      free($1);
    }
    | ID AS ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name = "";
      $$->attribute_name = $1;
      $$->alias = $3;
      $$->aggre_type = AggreType::NONE;
      free($1);
      free($3);
    }
    | ID ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name = "";
      $$->attribute_name = $1;
      $$->alias = $2;
      $$->aggre_type = AggreType::NONE;
      free($1);
      free($2);
    }
    | ID DOT ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = $3;
      $$->aggre_type = AggreType::NONE;
      free($1);
      free($3);
    }
    | ID DOT ID AS ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = $3;
      $$->alias = $5;
      $$->aggre_type = AggreType::NONE;
      free($1);
      free($3);
      free($5);
    }
    | ID DOT ID ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = $3;
      $$->alias = $4;
      $$->aggre_type = AggreType::NONE;
      free($1);
      free($3);
      free($4);
    }
    | '*' {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = "";
      $$->attribute_name = "*";
      $$->aggre_type = AggreType::NONE;
    }
    | '*' AS ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = "";
      $$->attribute_name = "*";
      $$->alias = $3;
      $$->aggre_type = AggreType::NONE;
      free($3);
    }
    | '*' ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = "";
      $$->attribute_name = "*";
      $$->alias = $2;
      $$->aggre_type = AggreType::NONE;
      free($2);
    }
    ;

relation:
    ID {
      $$ = new RelationSqlNode;
      $$->name = $1;
      free($1);
    }
    | ID AS ID {
      $$ = new RelationSqlNode;
      $$->name = $1;
      $$->alias = $3;
      free($1);
      free($3);
    }
    | ID ID {
      $$ = new RelationSqlNode;
      $$->name = $1;
      $$->alias = $2;
      free($1);
      free($2);
    }
    ;

attr_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA rel_attr attr_list {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<RelAttrSqlNode>;
      }

      $$->emplace_back(*$2);
      delete $2;
    }
    ;

rel_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA relation rel_list {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<RelationSqlNode>;
      }

      $$->emplace_back(*$2);
      free($2);
    }
    ;

/*
struct JoinSqlNode
{
  std::string relation_name;  ///< Relation to join with
  std::vector<ConditionSqlNode> conditions;
};
*/

join_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | INNER_JOIN relation ON condition_list join_list {
      if ($5 != nullptr) {
        $$ = $5;
      } else {
        $$ = new std::vector<JoinSqlNode>;
      }

      JoinSqlNode join1;
      join1.relation = *$2;
      delete $2;
      /*join1.conditions.swap(*$4); */
      /*join1.conditions.emplace_back(*$4); */
      // reverse
      std::reverse($4->begin(), $4->end());
      for (auto &condition : *$4) {
        join1.conditions.emplace_back(condition);
      }
      $$->emplace_back(join1);
      /*delete $4;
      free($2);*/
    }
    ;

/*
sub_select_stmt:
    LBRACE SELECT select_attr FROM ID rel_list where RBRACE
    {
      // in parse_defs.h:
      // 其实就是 SelectSqlNode
      // typedef struct SelectSqlNode SubSelectSqlNode;
      $$ = new SubSelectSqlNode;
      // $$->relations.swap(*$4);
      $$->relations.swap(*$6);
      $$->relations.push_back($5);
      std::reverse($$->relations.begin(), $$->relations.end());
      $$->attributes.swap(*$3);
      $$->conditions.swap(*$5);
      delete $3;
      delete $5;
      free($5); 
    }
    ;
*/

sub_select_stmt:
    LBRACE select_stmt RBRACE
    {
      $$ = $2;
    }
    | LBRACE value value_list RBRACE
    {
      // select * from xx in (1, 2, 3) 的情况
      $$ = new ParsedSqlNode(SCF_SOME_VALUES);
      if ($3 != nullptr) {
        $$->some_values.values.swap(*$3);
        delete $3;
      }
      $$->some_values.values.emplace_back(*$2);
      std::reverse($$->some_values.values.begin(), $$->some_values.values.end());
      delete $2;
    }
    ;


where:
    /* empty */
    {
      $$ = nullptr;
    }
    | WHERE condition_list {
      $$ = $2;  
    }
    ;
condition_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | condition {
      $$ = new std::vector<ConditionSqlNode>;
      $1->conjunction_type = 0;
      $$->emplace_back(*$1);
      delete $1;
    }
    | condition AND condition_list {
      $$ = $3;
      $1->conjunction_type = 1;
      $$->emplace_back(*$1);
      delete $1;
    }
    | condition OR condition_list {
      $$ = $3;
      $1->conjunction_type = 2;
      $$->emplace_back(*$1);
      delete $1;
    }
    ;

/*
We no longer need this again.
直接使用 condition_list
join_condition:
    rel_attr comp_op rel_attr
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 1;
      $$->left_attr = *$1;
      $$->right_is_attr = 1;
      $$->right_attr = *$3;
      $$->comp = $2;

      delete $1;
      delete $3;
    }
    ;
*/

condition:
    rel_attr comp_op value
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 1;
      $$->left_attr = *$1;
      $$->right_is_attr = 0;
      $$->right_value = *$3;
      $$->comp = $2;
      $$->sub_select = 0;

      delete $1;
      delete $3;
    }
    | value comp_op value 
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 0;
      $$->left_value = *$1;
      $$->right_is_attr = 0;
      $$->right_value = *$3;
      $$->comp = $2;
      $$->sub_select = 0;

      delete $1;
      delete $3;
    }
    | rel_attr comp_op rel_attr
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 1;
      $$->left_attr = *$1;
      $$->right_is_attr = 1;
      $$->right_attr = *$3;
      $$->comp = $2;
      $$->sub_select = 0;

      delete $1;
      delete $3;
    }
    | value comp_op rel_attr
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 0;
      $$->left_value = *$1;
      $$->right_is_attr = 1;
      $$->right_attr = *$3;
      $$->comp = $2;
      $$->sub_select = 0;

      delete $1;
      delete $3;
    }
    | value comp_op sub_select_stmt
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 0;
      $$->left_value = *$1;
      $$->right_is_attr = 0;
      //$$->right_value = nullptr;
      //$$->right_sub_select = std::unique_ptr<ParsedSqlNode>($3);
      $$->right_sub_select = $3;
      //  char            sub_select;      // 0: not sub select, 1: left sub select, 2: right sub select
      $$->sub_select = 2;
      $$->comp = $2;
    }
    | rel_attr comp_op sub_select_stmt
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 1;
      $$->left_attr = *$1;
      $$->right_is_attr = 0;
      //$$->right_value = nullptr;
      //$$->right_sub_select = std::unique_ptr<ParsedSqlNode>($3);
      $$->right_sub_select = $3;
      $$->sub_select = 2;
      $$->comp = $2;
    }
    | sub_select_stmt comp_op value
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 0;
      //$$->left_value = nullptr;
      //$$->left_sub_select = std::unique_ptr<ParsedSqlNode>($1);
      $$->left_sub_select = $1;
      $$->right_is_attr = 0;
      $$->right_value = *$3;
      $$->sub_select = 1;
      $$->comp = $2;
    }
    | sub_select_stmt comp_op rel_attr
    {
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 0;
      //$$->left_value = nullptr;
      //$$->left_sub_select = std::unique_ptr<ParsedSqlNode>($1);
      $$->left_sub_select = $1;
      $$->right_is_attr = 1;
      $$->right_attr = *$3;
      $$->sub_select = 1;
      $$->comp = $2;
    }
    | comp_op sub_select_stmt
    {
      // EXISTS/NOT EXISTS
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 0;
      $$->right_is_attr = 0;
      $$->comp = $1;
      $$->right_sub_select = $2;
      $$->sub_select = 2;
    }
    | sub_select_stmt comp_op sub_select_stmt
    {
      // EXISTS/NOT EXISTS
      $$ = new ConditionSqlNode;
      $$->left_is_attr = 0;
      $$->right_is_attr = 0;
      $$->comp = $2;
      $$->left_sub_select = $1;
      $$->right_sub_select = $3;
      $$->sub_select = 3;
    }
    ;

comp_op:
      EQ { $$ = EQUAL_TO; }
    | LT { $$ = LESS_THAN; }
    | GT { $$ = GREAT_THAN; }
    | LE { $$ = LESS_EQUAL; }
    | GE { $$ = GREAT_EQUAL; }
    | LK { $$ = LIKE; }
    | NLK { $$ = NOT_LIKE; }
    | NE { $$ = NOT_EQUAL; }
    | IN { $$ = IN_; }
    | NI { $$ = NOT_IN; }
    | EXISTS { $$ = EXISTS_; }
    | NOT_EXISTS { $$ = NOT_EXISTS_; }
    | IS_NOT { $$ = IS_NOT_; }
    | IS { $$ = IS_; }
    ;

load_data_stmt:
    LOAD DATA INFILE SSS INTO TABLE ID 
    {
      char *tmp_file_name = common::substr($4, 1, strlen($4) - 2);
      
      $$ = new ParsedSqlNode(SCF_LOAD_DATA);
      $$->load_data.relation_name = $7;
      $$->load_data.file_name = tmp_file_name;
      free($7);
      free(tmp_file_name);
    }
    ;

explain_stmt:
    EXPLAIN command_wrapper
    {
      $$ = new ParsedSqlNode(SCF_EXPLAIN);
      $$->explain.sql_node = std::unique_ptr<ParsedSqlNode>($2);
    }
    ;

set_variable_stmt:
    SET ID EQ value
    {
      $$ = new ParsedSqlNode(SCF_SET_VARIABLE);
      $$->set_variable.name  = $2;
      $$->set_variable.value = *$4;
      free($2);
      delete $4;
    }
    ;

opt_semicolon: /*empty*/
    | SEMICOLON
    ;
%%
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
