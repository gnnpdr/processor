#include <stdio.h>

#include "compile.h"

void make_asm_file(char** name)
{
    FILE* file;
    file = fopen(*name, "r");

    FILE* asm_file;
    asm_file = fopen("asm.txt", "a");  //сделать так, чтобы компилирования, файл закрывался, а при новой компиляции открывался новый. кейсы?

    size_t amount = 0;
    compile(file, asm_file, &amount);
    make_array(asm_file, amount);

    fclose(file);
    fclose(asm_file);
}

void compile (FILE* file, FILE* asm_file, size_t* amount)
{
    size_t args_amount = *amount;

    while (1)
    {
        char cmd[50] = "";              //размер? наверное, стоит что-то придумать, чтобы можно было его менять. или чтобы он увеличивался
        fscanf(file, "%s", cmd);
        int arg = 0;

        if (strcmp(cmd, "push") == 0)
        {
            fprintf (asm_file, "%d ", PUSH);  //fprintf записывает как символы, надо использовать fwrite
            args_amount++;
            fscanf (file, "%d", &arg);          //пока ввод здесь, можно сделать в 
            fprintf(asm_file, "%d\n", arg);
            args_amount++;
        }

        else if (strcmp(cmd, "add") == 0)
        {
            fprintf (asm_file, "%d\n", ADD);
            args_amount++;
        }

        else if (strcmp(cmd, "sub") == 0)
        {
            fprintf (asm_file, "%d\n", SUB);
            args_amount++;
        }

        else if (strcmp(cmd, "mul") == 0)
        {
            fprintf (asm_file, "%d\n", MUL);
            args_amount++;
        }

        else if (strcmp(cmd, "div") == 0)
        {
            fprintf (asm_file, "%d\n", DIV);
            args_amount++;
        }

        else if (strcmp(cmd, "sqrt") == 0)
        {
            fprintf (asm_file, "%d ", SQRT);
            args_amount++;
            fscanf (file, "%d", &arg);
            fprintf(asm_file, "%d\n", arg);
            args_amount++;
        }

        else if (strcmp(cmd, "sin") == 0)
        {
            fprintf (asm_file, "%d ", SIN);
            args_amount++;
            fscanf (file, "%d", &arg);
            fprintf(asm_file, "%d\n", arg);
            args_amount++;
        }

        else if (strcmp(cmd, "cos") == 0)
        {
            fprintf (asm_file, "%d ", COS);
            args_amount++;
            fscanf (file, "%d", &arg);
            fprintf(asm_file, "%d\n", arg);
            args_amount++;
        }

        else if (strcmp(cmd, "dump") == 0)
        {
            fprintf (asm_file, "%d\n", DUMP);
            args_amount++;
        }

        else if(strcmp(cmd, "hlt") == 0)
        {
            fprintf (asm_file, "%d\n", HLT);
            args_amount++;
            break;
        }
    }
    *amount = args_amount;
}

void make_array(FILE* asm_file, size_t amount)
{
    int* program = (int*)calloc(amount, sizeof(int));
    fread(program, sizeof(int), amount, asm_file);

    for (int i = 0; i < amount; i++)
    {
        printf("%d ", program[i]);
    }
}