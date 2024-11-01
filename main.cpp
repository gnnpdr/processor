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
    get_file(&input);
    remove_carriage(&input);

    Labels labels = {};
    Stack new_buf = {};
    stk_ctor(&new_buf);

    assembly(&input, &labels, &new_buf);

    make_file(&new_buf);

    Processor proc = {};

    input_dtor(&input);
}