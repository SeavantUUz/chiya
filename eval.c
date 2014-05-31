
CHY_Value 
chy_eval_expression(CHY_Interpreter *inter,LocalEnvironment *env,Expression *expression){
    return eval_expression(inter,env,expression);    
}

static CHY_Value
eval_expression(CHY_Interpreter *inter,LocalEnvironment *env,Expression *expression){
    CHY_Value v;
    switch (expression ->type) {
    case BOOLEAN_EXPRESSION:
        v = eval_boolean_expression(expression->u.boolean_value);
        break;
    case INT_EXPRESSION:
        v = eval_int_expression(expression->u.int_value);
        break;
    case DOUBLE_EXPRESSION:
        v = eval_double_expression(expression->u.double_value);
        break;
    case STRING_EXPRESSION:
        v = eval_string_expression(inter,expression->u.string_value);
        break;
    case IDENTIFIER_EXPRESSION:
        v = eval_identifier_expression(inter,env,expression->u.identifier);
        break;
    case ASSIGN_EXPRESSION:
        v = eval_assign_expression(inter,env,expression->u.assign_expression.variable,expression->u.assign_expression.expression);
        break;
    case ADD_EXPRESSION:
    case SUB_EXPRESSION:
    case MUL_EXPRESSION:
    case DIV_EXPRESSION:
    case MOD_EXPRESSION:
    case EQ_EXPRESSION:
    case NE_EXPRESSION:
    case GT_EXPRESSION:
    case GE_EXPRESSION:
    case LT_EXPRESSION:
    case LE_EXPRESSION:
        v = eval_binary_expression(inter,env,expression->type,expression->left,expression->right);
        break;
    case LOGICAL_AND_EXPRESSION:
    case LOGICAL_OR_EXPRESSION:
        v = eval_logical_expression(inter,env,expression->type,expression->left,expression->right);
        break;
    case CALL_EXPRESSION:
        v = eval_call_expression(inter,env,expression->identifier,expression->argument);
        break;
    case NONE_EXPRESSION:
        v = eval_none_expression();
        break;
    default:
        DBG_panic(("panic!!!"));
    }
    return v;
}

static CHY_Value
eval_boolean_expression(CHY_Boolean boolean_value){
    CHY_Value v;
    v.type = CHY_BOOLEAN_VALUE;
    v.u.boolean_value = boolean_value;
    return v;
}

static CHY_Value
eval_int_expression(int int_value){
    CHY_Value v;
    v.type = CHY_INT_VALUE;
    v.u.int_value = int_value;
    return v;
}

static CHY_Value
eval_double_expression(double double_value){
    CHY_Value v;
    v.type = CHY_DOUBLE_VALUE;
    v.u.double_value = double_value;
    return v;
}

static CHY_Value
eval_string_expression(CHY_Interpreter *inter,CHY_String *string_value){
    CHY_Value v;
    v.type = CHY_STRING_VALUE;
    v.u.string_value = chy_literal_to_chy_string(inter,string_value);
    return v;
}

static CHY_Value
eval_identifier_expression(CHY_Interpreter *inter,LocalEnvironment *env,Expression *expression){
    CHY_value v;
    Variable *variable;

    variable = chy_search_local_variable(env,expression->u.identifier);
    if (variable != NULL) {
        v = variable->value;
    }else {
        variable = chy_search_global_variable(inter,env,expression->u.identifier);
        if (variable != NULL) {
            v = variable->value;
        } else{
        DBG_panic(("panic!!!"));
        }
    }
    return v;
}
