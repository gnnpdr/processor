#include <stdio.h>

#include "proc.h"

static int get_arg (Processor* proc);
static int* get_arg_addr (Processor* proc, int* arg);

static void jump (Processor* const proc, Stack* const stk, Errors* const err);
static ResultOfComparing comparing(int first_el, int sec_el);

void processor (Stack* const stk, Processor* const proc, Errors* const err)
{
    size_t ip = 0;
    int* program = proc->new_file_buf;

    stack_element_t elem = 0;
    int first_el = 0;
    int sec_el = 0;
    int arg = 0;
    size_t size = proc->input_ncmd;
    printf("cool 1\n");
    while(ip < size)
    {
        printf("cool 2\n");
        print_stk_elements(stk->data, stk->capacity, stk->size);
        switch(program[ip])
        {
            case PUSH:
                proc->ip = ip;
                stk_push (stk, get_arg(proc), err);
                ip = proc->ip;
                break;

            case ADD:
                stk_pop(stk, &sec_el, err);
                stk_pop(stk, &first_el, err);
                stk_push (stk, first_el + sec_el, err);
                break;

            case SUB:
                stk_pop(stk, &sec_el, err);
                stk_pop(stk, &first_el, err);
                stk_push (stk, first_el - sec_el, err);
                break;

            case MUL:
                stk_pop(stk, &sec_el, err);
                stk_pop(stk, &first_el, err);
                stk_push (stk, first_el * sec_el, err);
                break;

            case DIV:
                stk_pop(stk, &sec_el, err);
                stk_pop(stk, &first_el, err);
                stk_push (stk, first_el / sec_el, err);
                break;

            case SQRT:
                ip++;
                stk_pop(stk, &arg, err);
                stk_push (stk, pow(arg, 0.5), err);
                break;
                
            case SIN:
                ip++;
                stk_pop(stk, &arg, err);
                stk_push (stk, sin(arg), err);
                break;

            case COS:
                ip++;
                stk_pop(stk, &arg, err);
                stk_push (stk, cos(arg), err);
                break;

            case OUT:
                printf("%d\n", stk->data[0]);
                break;
            
            case JUMP:
                proc->ip = ip;
                jump(proc, stk, err);
                ip = proc->ip;
                break;

            case POP:
                proc->ip = ip;
                stk_pop (stk, get_arg_addr(proc, &arg), err);
                ip = proc->ip;
                break;

            case HLT:
                break;

            default:
                printf("something went wrong :(\n");
                break;
        }   
        ip++;
    }
}

int get_arg (Processor* proc)
{
    size_t ip = proc->ip;
    int* program = proc->new_file_buf;
    ip++;  //перемещение на тип аргумента

    int result = 0;
    int arg_type = program[ip++];

    if (arg_type & INT)
        result += program[ip++];
    
    if (arg_type & REG)
        result += proc->registers[ip++].value;

    if (arg_type & RAM)   //к определенной ячейке оперативной памяти, загружаемой как аргумент
        result = proc->RAM[result];
    
    ip--;  //чтобы не проскочил след команду из-за прибавления ip в цикле
    proc->ip = ip;

    return result;
}

int* get_arg_addr (Processor* proc, int* arg)
{
    size_t ip = proc->ip;
    int* program = proc->new_file_buf;
    ip++;  //перемещение на тип аргумента

    int* result = 0;
    int arg_type = program[ip++];
    printf("arg_type %d\n", arg_type);

    if (arg_type == 0)    //почему не работает NULL_
    {
        printf("here\n");
        result = arg;
    }
        

    if (arg_type & INT)
        *result += program[ip++];
    
    if (arg_type & REG)
    {
        result = &(proc->registers[ip - 1].value);   //почему не записывает значение? адрес ведь передается в stk_pop
        ip++;
    }

    if (arg_type & REG && arg_type & INT)
    {
        *result += program[ip++];
        *result = proc->registers[ip - 1].value;
        ip++;
    }

    if (arg_type & RAM)   //к определенной ячейке оперативной памяти, загружаемой как аргумент
        result = &(proc->RAM[*result]);
    
    ip--;  //чтобы не проскочил след команду из-за прибавления ip в цикле
    proc->ip = ip;

    return result;
}

ResultOfComparing comparing(int first_el, int sec_el)
{
    if (first_el > sec_el)
        return GREATER;

    else if (first_el == sec_el)
        return EQUAL;

    else 
        return LESS;
}

DoJump comparator_less (int first_el, int sec_el)
{
    ResultOfComparing comp_res = comparing(first_el, sec_el);
    if (comp_res == LESS)
    {
        return DO_JUMP;
    }

    return NO_JUMP;
}

DoJump comparator_equal (int first_el, int sec_el)
{
    ResultOfComparing comp_res = comparing(first_el, sec_el);
    if (comp_res == EQUAL)
    {
        return DO_JUMP;
    }

    return NO_JUMP;
}

DoJump comparator_greater (int first_el, int sec_el)
{
    ResultOfComparing comp_res = comparing(first_el, sec_el);
    if (comp_res == GREATER)
    {
        return DO_JUMP;
    }

    return NO_JUMP;
}

DoJump comparator_not_equal (int first_el, int sec_el)
{
    ResultOfComparing comp_res = comparing(first_el, sec_el);
    if (comp_res != EQUAL)
    {
        return DO_JUMP;
    }

    return NO_JUMP;
}

DoJump comparator_less_and_equal (int first_el, int sec_el)
{
    ResultOfComparing comp_res = comparing(first_el, sec_el);
    if (comp_res == LESS || comp_res == EQUAL)
    {
        return DO_JUMP;
    }

    return NO_JUMP;
}

DoJump comparator_greater_and_equal (int first_el, int sec_el)
{
    ResultOfComparing comp_res = comparing(first_el, sec_el);
    if (comp_res == GREATER || comp_res == EQUAL)
    {
        return DO_JUMP;
    }

    return NO_JUMP;
}

void jump (Processor* const proc, Stack* const stk, Errors* const err)
{
    assert(stk != nullptr);                     
    assert(proc != nullptr);               
    assert(err != nullptr);                    
                                                                    
    size_t ip = proc->ip;                  
    int* program = proc->new_file_buf;     
    stack_element_t first_el = 0;               
    stack_element_t sec_el = 0;                 
                                                                    
    stk_pop(stk, &sec_el, err);                 
    stk_pop(stk, &first_el, err); 

    comparator_t func = 0;

    ip++;
    int exp_res = program[ip++];

    for (size_t i = 0; i < JUMP_AMT; i++)
    {
        if (exp_res == jump_array[i].res)
            func = jump_array[i].comparator;
    }

    DoJump allow = func(first_el, sec_el);

    if (allow == DO_JUMP)
    {
        ip++;
        int new_ip = program[ip++];
        ip--;  //чтобы потом в цикле ip++, поэтому else тоже не нужен
        proc->ip = new_ip;
    }
}


