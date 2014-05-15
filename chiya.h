#ifndef PRIVATE_CHIYA_H_INCLUDED
#define PRIVATE_CHIYA_H_INCLUDED
#include <stdio.h>

typedef Expression_tag Expression;

typedef struct ArgumentList_tag{
    Expression *expression;
    struct ArgumentList_tag *next;
} ArgumentList;

typedef struct ParameterList_tag{
    char *identifier;
    struct ParameterList_tag *next;
} ParameterList;

typedef struct {
    char *variable;
    Expression *expression;
} AssignExpression;

typedef struct {
    Expression *left;
    Expression *right;
} BinaryExpression;

typedef struct {
    char *identifier;
    ArgumentList *argument;
} FunctionCallExpression;

typedef Statement_tag Statement;

typedef struct StatementList_tag {
    Statement *statement;
    struct StatementList_tag *next;
} StatementList;

typedef struct {
    *StatementList statement_list;
} Block;

#endif
