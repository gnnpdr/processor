#include <stdio.h>
#include <math.h>

#include "check.h"
#include "stack.h"
#include "assemb.h"
#include "stack_operations.h"
#include "run_file.h"

int main (int argc, char** argv)
{
    StkErrors err = ALL_RIGHT;
    Proc processor = {};
    
    char* name = (char*)calloc(strlen(argv[1]) + 1, sizeof(char));  //серьезно надо сделать такую функцию?
    if (name == nullptr)
    {
        printf("no place\n");
        return NO_PLACE;
    }

    strcpy(name, argv[1]);

    file_transformation(name, &processor);

    Stack stk = {};

    run_prog(&stk, &processor, err);
}

