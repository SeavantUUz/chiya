
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
eval_identifier_expression(CHY_Interpreter *inter,LocalEnvironment *env,char *identifier){
    CHY_Value v;
    Variable *variable;

    variable = chy_search_local_variable(env,identifier);
    if (variable != NULL) {
        v = variable->value;
    }else {
        variable = chy_search_global_variable(inter,env,identifier);
        if (variable != NULL) {
            v = variable->value;
        } else{
        DBG_panic(("panic!!!"));
        }
    }
    return v;
}

static CHY_Value
eval_assign_expression(CHY_Interpreter *inter,LocalEnvironment *env,char *identifier,Expression *expression){
    CHY_Value v;
    Variable *left;

    v = chy_eval_expression(inter,env,expression);
    
    left = chy_search_local_variable(env,identifier);
    if(variable == NULL) {
        left = chy_sarch_global_variable(inter,env,identifier);
    }
    if(left != NULL){
        // 这里有一个释放和引用字符串的操作，应该是和字符串池有关，不管
        left->value = v;
    }else{
        if (env != null){
            chy_add_local_variable(&v);
        }
        else{
            chy_add_global_variable(&v);
        }
    return v;
}

Static CHY_Value
eval_binary_expression(CHY_Interpreter *inter,LocalEnvironment *env,ExpressionType type,Expression *left,Expression *right){
    CHY_Value left_val;
    CHY_Value right_val;
    CHY_Value result;

    if (left_val.type == CHY_INT_VALUE
            && right_val.type == CHY_INT_VALUE) {
        //为什么需要记录行数呢？
        eval_binary_int(inter,type,left_value.u.int_value,right_val.u.int_value,&result);
    }
    else if(left_value.type == CHY_DOUBLE_VALUE
            && right_value.type == CHY_DOUBLE_VALUE){
        eval_binary_int(inter,type,left_value.u.double_value,right_value.u.double_value,&result);
    }
    else if(left_value.type == CHY_INT_VALUE
            && right_value.type == CHY_DOUBLE_VALUE){
        left_value.u.double_value = left_val.u.int_value;
        eval_binary_double(inter,type, left_value.u.double_value,right_value.u.double_value,&result);
    }
    else if(left_value.type == CHY_DOUBLE_VALUE
            && right_value.type == CHY_INT_VALUE){
        right_value.u.double_value = right_val.u.int_value;
        eval_binary_double(inter,type,left_value.u.double_value,right_value.u.double_value,&result);
    }
    else if(left_value.type == CHY_BOOLEAN_VALUE
            && right_value.type == CHY_BOOLEAN_VALUE){
        result.type = CHY_BOOLEAN_VALUE;
        result.u.boolean_value
            = eval_binary_boolean(inter,type,left_value.u.boolean_value,right_value.u.boolean_value);
    }
    else if(left_value.type == CHY_STRING_VALUE
            && type == ADD_EXPRESSION) {
        char buf[LINE_BUF_SIZE];
        CHY_String *right_string;

        // 这里英文好奇怪
        if(right_value.type == CHY_INT_VALUE) {
            sprintf(buf,"%d",right_value.u.int_value);
            right_string = chy_create_string(inter,MEM_strdup(buf));
        } else if (right_val.type == CHY_DOUBLE_VALUE) {
            sprintf(buf,"%f",right_val.u.double_value);
            right_string = chy_create_string(inter,MEM_strdup(buf));
        } else if(right_value.type == CHY_BOOLEAN_VALUE){
            if (right_value.u.boolean_value){
            right_string = chy_create_string(inter,MEM_strdup("true"));
            }
            else {
            right_string = chy_create_string(inter,MEM_strdup("false"));
            }
        } else if(right_value.type == CHY_STRING_VALUE){
           right_string = right_value.u.string_value; 
        } 
        // 我还沒弄懂这个内部指针到底是干嘛用的，所以pass
            else if (right_value.type == CHY_NONE_VALUE){
            right_string = chy_create_string(inter,MEM_strdup("none"))
        }
        result.type = CHY_STRING_VALUE;
        result.u.string_value = chain_string(inter,left_val.u.string_value,right_string);
    }else if (left_val.type == CHY_STRING_VALUE
            && right_value.type == CHY_STRING_VALUE){
        result.type = CHY_BOOLEAN_VALUE;
        result.u.boolean_value
            = eval_compare_string(type,&left_val,&right_val);
    } else if (left_val.type == CHY_NONE_VALUE
            || right_value.type == CHY_NONE_VALUE){
        result.type = CHY_BOOLEAN_VALUE;
        result.u.boolean_value
             = eval_compare_none(inter,type,&left_val,&right_val);
    } else{
        DEG_panic(("panic!!!"));
    }
    return result;
}
