#include <stdio.h>
#include <math.h>

#include "stack.h"
#include "stack_operations.h"

void run (Stack* stk);
void print_stk_elements(Stack* stk);

int main ()
{
    Stack stk = {};

    ctor(&stk);
    run(&stk);
    dtor(&stk);
}

void run (Stack* stk)
{
    while (1)
    {
        char cmd[50] = "";   //размер? наверное, стоит что-то придумать, чтобы можно было его менять. или чтобы он увеличивался
        scanf("%s", cmd);
        int arg = 0;

        if (strcmp(cmd, "push") == 0)
        {
            scanf ("%d", &arg);
            push (stk, arg);
            printf("pushed\n");
        }

        else if (strcmp(cmd, "add") == 0)
        {
            int a = pop(stk);
            int b = pop(stk);
            push(stk, b + a);
        }

        else if (strcmp(cmd, "sub") == 0)
        {
            int a = pop(stk);
            int b = pop(stk);
            push(stk, b - a);
        }

        else if (strcmp(cmd, "mul") == 0)
        {
            int a = pop(stk);
            int b = pop(stk);
            push(stk, b * a);
        }

        else if (strcmp(cmd, "div") == 0)
        {
            int a = pop(stk);
            int b = pop(stk);
            push(stk, b / a);   //целочисленное, хотите другое? приведение типов, смена типа элементов стэка
        }

        else if (strcmp(cmd, "sqrt") == 0)
        {
            scanf ("%d", &arg);
            push (stk, pow(arg, 0.5));
        }

        else if (strcmp(cmd, "sin") == 0)
        {
            scanf ("%d", &arg);
            push (stk, sin(arg));
        }

        else if (strcmp(cmd, "cos") == 0)
        {
            scanf ("%d", &arg);
            push (stk, cos(arg));
        }

        else if (strcmp(cmd, "dump") == 0)
        {
            print_stk_elements(stk);
        }

        else if(strcmp(cmd, "hlt") == 0)
        {
            break;
        }
    }
}

void print_stk_elements(Stack* stk)
{
    size_t size = stk->size;
    size_t capacity = stk->capacity;
    stack_element_t* data = stk->data;

    for (size_t i = 0; i < capacity; i++)
    {
        if(i < size)
            printf(" * ");
        else
            printf("   ");
        if (data[i] == poison)
            printf("[%d] = %d (POISON)\n", i, poison);
        else
            printf("[%d] = %d\n", i, data[i]);
    }
    printf(" }\n}");
}