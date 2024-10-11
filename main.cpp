#include <stdio.h>
#include <math.h>

#include "stack.h"
#include "compile.h"
#include "stack_operations.h"

void print_stk_elements(Stack* stk);
void run (Stack* stk, int* program, size_t amount);

int main (int argc, char** argv)
{
    char* name[50] = {argv[1]};
    size_t amount = 0;
    int* program = 0;

    Stack stk = {};

    ctor(&stk);
    make_asm_file(name, &amount);
    make_array(amount, program);

    run(&stk, program, amount);

    dtor(&stk);
}

void print_stk_elements(Stack* stk)    //вывод в ком строку, в файле неудобно читать
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

void run (Stack* stk, int* program, size_t amount)
{
    size_t ip = 0;
    int a = 0;
    int b = 0;
    int arg = 0;

    while(ip < amount)
    {
        switch(program[ip])
        {
            case PUSH:
                ip++;
                push (stk, program[ip]);
                break;

            case ADD:
                a = pop(stk);
                b = pop(stk);
                push(stk, b + a);
                break;

            case SUB:
                a = pop(stk);
                b = pop(stk);
                push(stk, b - a);
                break;

            case MUL:
                a = pop(stk);
                b = pop(stk);
                push(stk, b * a);
                break;

            case DIV:
                a = pop(stk);
                b = pop(stk);
                push(stk, b / a);
                break;

            case SQRT:
                ip++;
                arg = pop(stk);
                push(stk, pow(arg, 0.5));
                break;
                
            case SIN:
                ip++;
                arg = pop(stk);
                push(stk, sin(arg));
                break;

            case COS:
                ip++;
                arg = pop(stk);
                push(stk, cos(arg));
                break;

            case DUMP:
                print_stk_elements(stk);
                ip++;

            case HLT:
                break;
            default:
                printf("something went wrong :(\n");
                break;

            
        }   
        ip++;
    }
}

