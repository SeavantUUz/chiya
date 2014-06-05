
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

static CHY_Value
eval_logical_expression(CHY_Interpreter *inter,LocalEnvironment *env,char *identifier,ExpressionType operator,Expression *right){
    CHY_Value left_val;
    CHY_Value right_val;
    CHY_Value result;
    int flag = -1;
   
    result.type = CHY_BOOLEAN_VALUE;
    left_val = eval_expression(inter,env,left);
    if (left_val.type != CHY_BOOLEAN_VALUE){
        if(left_val.type == CHY_NONE_VALUE){
            flag = 0;
        }
        else{
            flag = 1;
        }

    }
    if (operator == LOGICAL_AND_EXPRESSION){
        if (flag == 0 || !left_val.u.boolean_value){
            result.u.boolean_value = CHY_FALSE;
            return result;
        }
    } else if (operator == LOGICAL_OR_EXPRESSION){
        if (flag == 1 || left_val.u.boolean_value){
            result.u.boolean_value = CHY_TRUE;
            return result;
        }
    } else {
        DBG_panic(("bad operator"!!));
    }

    right_val = eval_expression(inter,env,right);
    if (right_val.type != CHY_BOOLEAN_VALUE){
        if (right_vale.type == CHY_NONE_VALUE){
            result.u.boolean_value = CHY_FALSE;
            return result;
        } else {
            result.u.boolean_value = CHY_TRUE;
            return result;
        }

        result.u.boolean_value = right_value.u.boolean_value;
        return result;
    }
}

static CHY_Value
eval_call_expression(CHY_Interpreter *inter,LocalEnvironment *env,Expression *expression){
    CHY_Value value;
    FunctionDefination *function;

    char *identifier = expression -> u.call_expression.identifier;

    function = search_function(identifier);
    if (function == NULL) {
        chy_runtime_error("could not find function !!!");
    }
    switch(function->type){
        case CHIYA_FUNCTION_DEFINETION:
            value = call_chiya_function(inter,env,expression,function);
            break;
        case NATIVE_FUNCTION_DEFINETION:
            // 其实我不知道这里的proc是干嘛的……
            // proc是一个回调函数
            value = call_native_function(inter,env,expression,function->u.native_f.proc);
            break;
        default:
            DBG_panic(("panic!!!"));
    }

    return value;
}

static CHY_Value
eval_native_function(CHY_Interpreter *inter,LocalEnvironment *env,Expression *expression,CHY_NativeFunctionProc *proc){
    CHY_Value value;
    int arg_count;
    ArgumentList *arg_p;
    CHY_Value *args;
    int i;

    for(arg_count = 0,arg_p = expression->u.call_expression.argument;arg_p;arg_p = arg_p->next){
        arg_count ++;
    }

    args = MEM_malloc(sizeof(CHY_Value) * arg_count);

    for (arg_p = expression->u.call_expression.argument,i=0;arg_p;arg_p = arg_p->next){
        args[i] = eval_expression(inter,env,arg_p->expression);
    }
    value = proc(inter,env,args);
    MEM_free(args);
    return value;
}

static CHY_Value
call_chiya_function(CHY_Interpreter *inter,LocalEnvironment *env,
        Expression *expression,FunctionDefination *func){
    CHY_Value value;
    StatementResult result;
    ArgumentList *arg_p;
    ParameterList *param_p;
    LocalEnvironment *local_env;

    local_env = alloc_local_env();
    for (arg_p = expression->call_expression.argument,
            param_p = func->chiya_f.parameter;
            arg_p;
            arg_p = arg_p -> next,param_p = param_p -> next){
        CHY_Value arg_val;

        arg_val = eval_expression(inter,env,arg_p -> expression);
        chy_add_local_variable(local_env,param_p->identifier,&arg_val);
    }

    result = chy_execute_statement_list(inter,local_env,func->chiya_f.block->statement_list);

    if(result.type = RETURN_STATEMENT_RESULT){
        value = result.u.value;
    }else{
        value.type = CHY_NONE_VALUE;
    }

    dispose_local_environment(local_env);
    return value;
}
