#include <stdio.h>

#include "check.h"
#include "stack.h"
#include "input.h"
#include "assembly.h"
#include "proc.h"

int main (int argc, char** argv)
{
    Text input = {};

    get_name(&input, argv);

    get_file_data(&input);

    remove_carriage(&input);

    Labels labels = {};
    ctor_labels(&labels);

    Stack new_buf = {};
    stk_ctor(&new_buf);

    assembly(&input, &labels, &new_buf);
    dtor_labels(&labels);

    //make_binary_file(&new_buf);


    Stack stk = {};
    stk_ctor(&stk);

    Processor proc = {};
    proc.new_file_buf = new_buf.data;
    proc.ncmd = new_buf.size;
    proc_file (&stk, &proc);

    

    input_dtor(&input);
    stk_dtor(&stk);
    stk_dtor(&new_buf);
}