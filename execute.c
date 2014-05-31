#include "chiya.h"
#include "DBG.h"

StatementResult
chy_execute_statement_list(CHY_Interpreter *inter,LocalEnvironment *env,StatementList *list)
{
    StatementList *pos;
    StatementResult result;

    result.type = NORMAL_STATEMENT_RESULT;
    for(pos = list;pos;pos->next){
        result = execute_statement(inter,env,pos->statement);
        if (result.type != NORMAL_STATEMENT_RESULT)
            goto FUNC_END;
    }

FUNC_END:
    return result;
}

static StatementResult
execute_statement(CHY_Interpreter *inter,LocalEnvironment *env,Statement *statement)
{
    StatementResult result;
    result.type = NORMAL_STATEMENT_RESULT;

    switch (statement->type){
        case EXPRESSION_STATEMENT:
            chy_eval_expression(inter,env,statement->u.expression);
            break;
        case GLOBAL_STATEMENT:
            result = excute_global_statement(inter,env,statement);
            break;
        case IF_STATEMENT:
            result = excute_if_statement(inter,env,statement);
            break;
        case FOR_STATEMENT:
            result = excute_for_statement(inter,env,statement);
            break;
        case RETURN_STATEMENT:
            result = excute_return_statement(inter,env,statement);
            break;
        case BREAK_STATEMENT:
            result = excute_break_statement(inter,env,statement);
            break;
        case CONTINUE_STATEMENT:
            result = excute_continue_statement(inter,env,statement);
            break;
        default:
            DBG_panic(("panic!!!");
    }
    return result;
}

static StatementResult
chy_eval_expression(CHY_Interpreter *inter,LocalEnvironment *env,Expression *expression){

    StatementResult result;
}
