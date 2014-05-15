%{
#include <stdio.h>
#include "chiya.h"
#define YYDEBUG 1
%}
%union {
    char          *identifier;
    ParameterList *parameter_list;
    ArgumentList *argument_list;
    Expression    *expression;
    Statement     *statement;
    StatementList *statement_list;
    Block         *block;
    Elif *elif    *elif;
    IdentifierList *identifier_list;
}
%token <identifier> IDENTIFIER;
%token <expression> INT_LITERAL;
%token <expression> DOUBLE_LITERAL;
%token <expression> STRING_LITERAL;
%token DEF IF ELIF ELSE GLOBAL FOR RETURN NONE TRUE FALSE BREAK CONTINUE SEMICOLON COMMA LP RP LC RC NOT AND OR ASSIGN EQ NE GT GE LT LE ADD SUB MUL DIV;
%type <parameter_list> parameter_list;
%type <arguement_list> argument_list;
%type <expression> primary_expression expression logical_and_expression logical_or_expression equality_expression relational_expression call_expression arithmetic_expression;
%type <statement> statement if_statement elif_statement for_statement return_statement break_statement continue_statement global_statement;
%type <statement_list> statement_list;
%type <elif> elif_list elif;
%type <block> block;
%type <identifier_list> identifier_list;
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
    | IDENTIFIER ASSIGN logical_or_expression
    {
        $$ = chy_create_assign_expression($1,$3);
    }

logical_or_expression
    : logical_and_expression
    | logical_or_expression OR logical_and_expression
    {
        $$ = chy_create_binary_expression(LOGICAL_OR_EXPRESSION,$1,$3);
    }

logical_and_expression
    : equality_expression
    | logical_and_expression AND equality_expression
    {
        $$ = chy_create_binary_expression(LOGICAL_AND_EXPRESSION,$1,$3);
    }

equality_expression
    : relational_expression
    | equality_expression EQ relational_expression
    {
        $$ = chy_create_binary_expression(EQ_EXPRESSION,$1,$3);
    }
    | equality_expression NE relational_expression
    {
        $$ = chy_create_binary_expression(NE_EXPRESSION,$1,$3);
    }
    ;

relational_expression
    : arithmetic_expression
    | relational_expression GT arithmetic_expression
    {
        $$ = chy_create_binary_expression(GT_EXPRESSION,$1,$3);
    }
    | relational_expression GE arithmetic_expression
    {
        $$ = chy_create_binary_expression(GE_EXPRESSION,$1,$3);
    }
    | relational_expression LT arithmetic_expression
    {
        $$ = chy_create_binary_expression(LT_EXPRESSION,$1,$3);
    }
    | relational_expression LE arithmetic_expression
    {
        $$ = chy_create_binary_expression(LE_EXPRESSION,$1,$3);
    }
    ;

%left ADD SUB
%left MUL DIV
arithmetic_expression
    : call_expression
    | arithmetic_expression ADD call_expression
    {
        $$ = chy_create_binary_expression(ADD_EXPRESSION,$1,$3);
    }
    | arithmetic_expression SUB call_expression
    {
        $$ = chy_create_binary_expression(SUB_EXPRESSION,$1,$3);
    }
    | arithmetic_expression MUL call_expression
    {
        $$ = chy_create_binary_expression(MUL_EXPRESSION,$1,$3);
    }
    | arithmetic_expression DIV call_expression
    {
        $$ = chy_create_binary_expression(DIV_EXPRESSION,$1,$3);
    }
    ;

call_expression
    : IDENTIFIER LP argument_list RP
    {
        $$ = chy_create_function_call_expression($1,$3);
    }
    | IDENTIFIER LP RP
    {
        $$ = chy_create_function_call_expression($1,NULL);
    }
    | LP expression RP
    {
        $$ = $2;
    }
    | IDENTIFIER
    {
        $$ =  chy_create_identifier_expression($1);
    }
    | INT_LITERAL
    | DOUBLE_LITERAL
    | STRING_LITERAL
    | TRUE
    {
        $$ = chy_create_boolen_expression(CHY_TRUE); 
    }
    | FALSE
    {
        $$ = chy_create_boolen_expression(CHY_FALSE);
    }
    | NONE
    {
        $$ = chy_create_none_expression(CHY_NONE);
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

global_statement
    : GLOBAL identifier SEMICOLON
    {
        $$ = chy_create_global_statement($2);
    }
    ;

if_statement
    : IF LP expression RP block
    {
        $$ = chy_create_if_statement($3,$5,NULL,NULL);
    }
    | IF LP expression RP block ELSE block
    {
        $$ = chy_create_if_statement($3,$5,NULL,$7);
    }
    | IF LP expression RP block elif_list
    {
        $$ = chy_create_if_statement($3,$5,$6,NULL);
    }
    | IF LP expression RP block elif_list ELSE block
    {
        $$ = chy_create_if_statement($3,$5,$6,$8);
    }
    ;

elif_list
    : elif
    | elif_list elif
    {
        $$ = chy_chain_elif_statement($1,$2);
    }
    ;

elif
    : ELIF LP expression RP block
    {
        $$ = chy_create_elif_statement($3,$5);
    }
    ;

for_statement
    : FOR LP primary_expression SEMICOLON primary_expression SEMICOLON primary_expression RP block
    {
        $$ = chy_create_for_statement($3,$5,$7,$9);
    }
    ;

return_statement
    : RETURN primary_expression SEMICOLON
    {
        $$ = chy_create_return_statement($2);
    }
    ;

primary_expression
    :
    {
        $$ = NULL;
    }
    | expression
    ;

continue_statement
    : CONTINUE SEMICOLON
    {
        $$ = chy_create_continue_statement();
    }
    ;

break_statement
    : BREAK SEMICOLON
    {
        $$ = chy_create_break_statement();
    }
    ;

block
    : LC statement_list RC
    {
        $$ = chy_create_block_statement($2);
    }
    | LC RC
    {
        $$ = chy_create_block_statement(NULL);
    }
    ;
