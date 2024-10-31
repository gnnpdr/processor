#include <stdio.h>

#include "check.h"
#include "stack.h"
#include "input.h"
#include "assembly.h"
#include "proc.h"

int main (int argc, char** argv)
{
    //Errors err = ALL_RIGHT;
    //char* name;
    Text input = {};

    Stack new_buf = {};
    stk_ctor(&new_buf);
    
    Labels lables = {};


    get_name(&input, argv);
    get_file(&input);
    remove_carriage(&input);
    //Stack stk1 = {};
    //stk_ctor(&stk1);
    
    Processor proc = {};

    assembly(&input, &proc);
    //Stack stk = {};
    //stk_ctor(&stk);

    //proc_code(&stk, &proc, &err);
    /*stk_push(&stk1, 1);
    stk_push(&stk1, 1);
    stk_push(&stk1, 3);
    stk_push(&stk1, 1);
    stk_push(&stk1, 1);
    stk_push(&stk1, 5);
    stk_push(&stk1, 2);
    stk_push(&stk1, 9);
    stk_push(&stk1, 10);
    printf("\n\n");
    for (int i = 0; i < stk1.capacity; i++)
        printf("%d ", stk1.data[i]);*/


    input_dtor(&input);
    //printf("dtor\n");
    stk_dtor(&(proc.new_file_buf));
    //printf("dtor!!\n");
}