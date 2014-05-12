%{
#include <stdio.h>
#include "chiya.h"
#define YYDEBUG 1
%}
%union {
    char        *identifier;
    ParameterList *parameter_list;
    ArguementList *arguement_list;
    Expression  *expression;
    Statement *statement;
    StatementList *statement_list;
    Block *Block;
}
%token <identifier> IDENTIFIER;
%token <expression> INT_LITERAL;
%token <expression> DOUBLE_LITERAL;
%token <expression> STRING_LITERAL;
%token DEF IF ELIF ELSE GLOBAL FOR RETURN NONE TRUE FALSE BREAK CONTINUE SEMICOLON COMMA LP RP LC RC NOT RC NOT AND OR ASSIGN EQ GT GE LT LE ADD SUB MUL DIV;
%token <parameter_list> parameter_list;
%token <arguement_list> argument_list;
%token <expression> expression logical_and_expression logical_or_expression logical_not_expression equality_expression; 
%token <statement> statement if_statement elif_statement for_statement return_statement break_statement continue_statement global_statement;
%token <statement_list> statement_list;
%token <block> block;
%%
program
    : defination_or_statement_list
    ;

defination_or_statement_list
    : defination_or_statement
    | defination_or_statement_list defination_or_statement 
    ;

defination_or_statement
    : function_defination
    | statement
    {
        CHY_interpreter *inter = chy_get_current_interpreter();
        inter->statement_list = chy_chain_statement_list(inter->statement_list,$1);
    }
    ;

function_defination
    : DEF IDENTIFIER LP parameter_list RP block
    {
        chy_function_define($2,$4,$6);
    }
    | DEF IDENTIFIER LP RP block
    {
        chy_function_define($2,NULL,$5);
    }
    ;

parameter_list
    : IDENTIFIER
    {
        $$ = chy_create_parameter($1);
    }
    | paramter COMMA IDENTIFIER
    {
        $$ = chy_chain_parameter_list($1,$3);
    }
    ;

argument_list
    : expression
    {
        $$ = chy_create_argument_list($1);
    }
    | argument_list COMMA expression
    {
        $$ = chy_chain_argument_list($1,$3);
    }
    ;

statement_list
    : statement
    {
        $$ = chy_create_statement_list($1);
    }
    | statement_list statement
    {
        $$ = chy_chain_statement_list($1,$2);
    }
    ;

expression
    : logical_or_expression
    | IDENTIFIER ASSIGN expression
    {
        $$ = chy_create_assign_expression($1,$3);
    }
    ;

logical_or_expression
    : logical_and_expression

statement
    : expression SEMICOLON
    {
        $$ = chy_create_expression_statement($1);
    }
    | global_statement
    | if_statement
    | for_statement
    | return_statement
    | break_statement
    | continue_statement
    ;

