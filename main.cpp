#include <stdio.h>
#include <math.h>

#include "check.h"
#include "stack.h"
#include "check_proc.h"
#include "asm.h"
#include "stack_operations.h"
#include "processor.h"

int main (int argc, char** argv)
{
    Errors err = ALL_RIGHT;
    Proc processor = {};

    make_regs(&processor);

    char* name = (char*)calloc(strlen(argv[1]) + 1, sizeof(char));
    CAPACITY_CHECK(name)
    strcpy(name, argv[1]);

    proc (name, &processor, &err);

    Stack stk = {};
    stk_ctor(&stk, __FILE__, __func__, __LINE__, &err);
    printf("here! 1\n");

    run_prog(&stk, &processor, &err);
    printf("here! 2\n");

    print_stk_elements(stk.data, processor.input_file_commands_amount, processor.input_file_commands_amount);
    
    free(name);
    proc_dtor(&processor);

}

