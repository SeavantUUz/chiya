#include "MEM.h"
#include "chiya.h"

void
CHY_interpret(CHY_Interpreter *interpreter)
{
    interpreter->execute_storage = MEM_open_storage(0);
    chy_add_std_fp(interpreter);
    chy_execute_statement_list(interpreter,NULL,interpreter->statement_list);
}
