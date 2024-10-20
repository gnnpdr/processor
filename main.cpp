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

    LabelParameters labels[LABELS_AMT];
    
    for (size_t i = 0; i < LABELS_AMT; i++)
        labels[i].j_pointer = -1;

    processor.labels = labels;

    char* name = (char*)calloc(strlen(argv[1]) + 1, sizeof(char));
    if (name == nullptr)
    {
        printf("no place\n");
        return NO_PLACE;
    }

    strcpy(name, argv[1]);

    file_transformation(name, &processor, &err);

    //Stack stk = {};

    //run_prog(&stk, &processor, &err);

    free(name);
    free_bufs(&processor);

}

