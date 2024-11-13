#include <stdio.h>

#include "check.h"
#include "stack.h"
#include "input.h"
#include "assembly.h"
#include "proc.h"

int main (int argc, char** argv)
{
    /*printf("IN MAIN\n");
    printf("==========================================\nREGISTERS\n");  //ПРОБЛЕМА!! АДРЕСА У ВСЕХ РЕГИСТРОВ НУЛЕВЫЕ 00000FFFFF
    for (int i = 0; i <REG_AMT; i++)
    {
        printf("name %s\n", registers[i]->name);
        printf("address %p\n", registers + i);
        printf("value %d\n", registers[i]->value);
        printf("num %d\n", registers[i]->num);
    }

    printf("END START REGS\n");*/
    Text input = {};

    get_name(&input, argv);

    get_file_data(&input);

    remove_carriage(&input);

    Labels labels = {};
    Stack functions = {};  //для ret
    stk_ctor(&functions);
    ctor_labels(&labels);

    Stack new_buf = {};
    stk_ctor(&new_buf);

    assembly(&input, &labels, &new_buf, &functions);
    dtor_labels(&labels);

    Stack stk = {};
    stk_ctor(&stk);

    Processor proc = {};
    proc.new_file_buf = new_buf.data;
    proc.ncmd = new_buf.size;
    proc_file (&stk, &proc, &functions);

    

    input_dtor(&input);
    stk_dtor(&stk);
    stk_dtor(&new_buf);
    stk_dtor(&functions);
}