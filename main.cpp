#include <stdio.h>

#include "check.h"
#include "stack.h"
#include "stack_operations.h"
#include "input.h"
#include "asm.h"
#include "proc.h"

int main (int argc, char** argv)
{
    Errors err = ALL_RIGHT;
    char* name;
    get_name(&name, argv);

    Processor proc = {};

    assembly (name, &proc);
    Stack stk = {};
    stk_ctor(&stk, __FILE__, __func__, __LINE__, &err);

    processor(&stk, &proc, &err);

    proc_dtor(&proc, name);
    stk_dtor(&stk);
}