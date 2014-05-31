#ifndef PRIVATE_CHIYA_H_INCLUDED
#define PRIVATE_CHIYA_H_INCLUDED
#include <stdio.h>

typedef struct ParameterList_tag{
    char *identifier;
    struct ParameterList_tag *next;
} ParameterList;

typedef struct ArgumentList_tag{
    Expression *expression;
    struct ArgumentList_tag *next;
} ArgumentList;

typedef struct IdentifierList_tag{
    char *identifier;
    struct IdentifierList_tag *next;
} IdentifierList;

enum ExpressionType {
    BOOLEAN_EXPRESSION = 1;
    INT_EXPRESSION;
    DOUBLE_EXPRESSION;
    STRING_EXPRESSION;
    IDENTIFIER_EXPRESSION;
    ASSIGN_EXPRESSION;
    ADD_EXPRESSION;
    SUB_EXPRESSION;
    MUL_EXPRESSION;
    DIV_EXPRESSION;
    EQ_EXPRESSION;
    NE_EXPRESSION;
    GT_EXPRESSION;
    GE_EXPRESSION;
    LT_EXPRESSION;
    LE_EXPRESSION;
    LOGICAL_AND_EXPRESSION;
    LOGICAL_OR_EXPRESSION;
    CALL_EXPRESSION;
    NONE_EXPRESSION;
};

struct Expression_tag{
    ExpressionType *type;
    int line_number;
    union {
        CHY_BOOLEAN boolean;
        int int_value;
        double double_value;
        char *string_value;
        char *identifier;
        AssignExpression assign_expression;
        BinaryExpression binary_expression;
        CallExpression call_expression;
    } u;
} ;

typedef Expression_tag Expression;


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
} CallExpression;

typedef enum {
    EXPRESSION_STATEMENT = 1;
    GLOBAL_STATEMENT;
    IF_STATEMENT;
    FOR_STATEMENT;
    RETURN_STATEMENT;
    BREAK_STATEMENT;
    CONTINUE_STATEMENT;
} StatementType;

typedef struct Statement_tag {
    StatementType type;
    int line_number;
    union {
        Expression *expression;
        GlobalStatement *global_s;
        IfStatement *if_s;
        ForStatement *for_s;
        ReturnStatement *return_s;
    } u;
};

typedef Statement_tag Statement;

typedef struct StatementList_tag {
    Statement *statement;
    struct StatementList_tag *next;
} StatementList;

typedef struct Elif_tag {
    Expression *expression;
    Block *block;
    struct Elif_tag *next;
} Elif;

typedef struct {
    StatementList *statement_list;
} Block;

typedef struct {
    IdentifierList *identifier_list;
} GlobalStatement;

typedef struct {
    Expression *condition;
    Block *if_block;
    Elif *elif_list;
    Block *else_block;
} IfStatement;

typedef struct {
    Expression *init;
    Expression *codition;
    Expression *end;
    Block * block;
} ForStatement;

typedef struct {
    Expression *value;
} ReturnStatement;

enum {
    CHIYA_FUNCTION_DEFINATION = 1;
    NATIVE_FUNCTION_DEFINATION;
} FunctionDefinationType;

typedef struct FunctionDefinition_tag {
    char *function_name;
    FunctionDefinitionType type;
    union {
        struct {
        ParamentList *paramenter;
        Block *block;
        } chiya_f;
        struct {
        CHY_NativeFunctionProc *proc;
        } native_f;
    } u;
    struct FunctionDefinition_tag *next;
} FunctionDefination;

struct CHY_Interpreter_tag {
    MEM_Storage     interpreter_storage;
    MEM_Storage     execute_storage;
    Variable        *variable;
    FunctionDefinition *function_list;
    StatementList   *statement_list;
    int     current_line_number;
};

// global variable storage
typedef struct Variable_tag {
    char *name;
    CHY_value value;
    struct Variable_tag *next;
} Variable;

typedef struct GlobalVariableRef_tag {
    Variable *variable;
    struct GlobalVariableRef_tag *next;
} GlobalVariableRef;

typedef struct {
    Variable *variable;
    GlobalVariableRef *global_variable;
} LocalEnvironment;

typedef enum {
    NO_RETURN_STATEMENT_RESULT = 1;
    RETURN_STATEMENT_RESULT;
    BREAK_STATEMENT_RESULT;
    CONTINUE_STATEMENT_RESULT;
} StatementResultType;

typedef struct {
    StatementResultType type;
    union {
        CHY_Value value; 
    }
} StatementResult;

struct CHY_String_tag {
    int ref_count;
    char *string;
    CHY_Boolean is_literal;
};

typedef struct {
    CHY_String *strings;
} StringPool;

/* execute.c */


#endif
