%{
#include <stdio.h>
#include "y.tab.h"
#include "chiya.h"

int
yywrap(void){
    return 1;
}

static void
line_increment(void){
    chy_get_current_interpeter() -> current_line_number ++; 
}

%}
%start COMMENT STRING_LITERAL_STATE
%%
<INITIAL> "def"         return DEF;
<INITIAL> "if"          return IF;
<INITIAL> "elif"        return ELIF;
<INITIAL> "else"        return ELSE;
<INITIAL> "global"      return GLOBAL;
<INITIAL> "for"         return FOR;
<INITIAL> "return"      return RETURN;
<INITIAL> "none"        return NONE;
<INITIAL> "true"        return TRUE;
<INITIAL> "false"       return FALSE;
<INITIAL> "break"       return BREAK;
<INITIAL> "continue"    return CONTINUE;

<INITIAL> ";"           return SEMICOLON;
<INITIAL> ","           return COMMA;

<INITIAL> "("           return LP;
<INITIAL> ")"           return RP;
<INITIAL> "{"           return LC;
<INITIAL> "}"           return RC;
<INITIAL> "not"         return NOT;
<INITIAL> "and"         return AND;
<INITIAL> "or"          return OR;
<INITIAL> "="           return ASSIGN;
<INITIAL> "=="          return EQ;
<INITIAL> ">"           return GT;
<INITIAL> ">="          return GE;
<INITIAL> "<"           return LT;
<INITIAL> "<="          return LE;

<INITIAL> "+"           return ADD;
<INITIAL> "-"           return SUB;
<INITIAL> "*"           return MUL;
<INITIAL> "/"           return DIV;

<INITIAL>[A-Za-z_][A-Za-z_0-9]* {
    yylval.identifier = chy_create_identifier(yytext);
    return IDENTIFIER;
}

<INITIAL>([1-9][0-9]*)|0 {
    Expression *expression = chy_create_expression(INT_EXPRESSION);
    sscanf(yytext,"%d",expression->u.int_value);
    yylval.expression = expression;
    return INT_LITERAL;
}

<INITIAL>[0-9]+|0\.[0-9]+ {
    Expression *expression = chy_create_expression(DOUBLE_EXPRESSION);
    sscanf(yytext,"%lf",expression->u.double_value);
    yylval.expression = expression;
    return DOUBLE_LITERAL;
}

<INITIAL>\" {
    chy_begin_string_literal();
    BEGIN STRING_LITERAL_STATE;
}

<INITAL>[ \t];

<INITAL>\n {
    line_increment();
}

<INITIAL># {
    BEGIN COMMENT;
}

<INITAL>. {
    chy_compiler_error(CHARACTER_INVAILD_ERROR);    
}

<COMMENT>\n {
    line_increment();        
    BEGIN INITIAL;
}

<COMMENT>. ;

<STRING_LITERAL_STATE>\" {
    Expression *expression = chy_create_expression(STRING_EXPRESSION);
    expression->u.string_value = chy_end_string_literal();
    yylval.expression = expression;
    BEGIN INITIAL;
    return STRING_LITERAL;
}

<STRING_LITERAL_STATE>\n {
    chy_add_string_char('\n');
    line_increment();
}

<STRING_LITERAL_STATE>\\\" {
    chy_add_string_char('"');
}

<STRING_LITERAL_STATE>\\\n {
    chy_add_string_char('\n');
}

<STRING_LITERAL_STATE>\\\t {
    chy_add_string_char('\t');
}

<STRING_LITERAL_STATE>\\\\ {
    chy_add_string_char('\\');
}

<STRING_LITERAL_STATE>. {
    chy_add_string_char(yytext[0]);
}
%%
