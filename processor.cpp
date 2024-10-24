#include <stdio.h>

#include "processor.h"

void run_prog (Stack* const stk, Proc* const processor, Errors* const err)
{
    assert(stk != nullptr);
    assert(processor != nullptr);
    assert(err != nullptr);

    size_t ip = 0;
    int* program = processor->new_file_buf;
    size_t label_delta = processor->labels.label_delta;

    stack_element_t elem = 0;
    int first_el = 0;
    int sec_el = 0;
    int arg = 0;
    size_t amount = processor->input_file_commands_amount - label_delta;

    while(ip < amount)
    {
        print_stk_elements(stk->data, stk->capacity, stk->size);
        switch(program[ip])
        {
            case PUSH:
                stk_push (stk, get_arg(processor), err);
                break;

            case ADD:
                stk_pop(stk, &sec_el, err);
                stk_pop(stk, &first_el, err);
                printf("first %d sec %d, sum %d\n", first_el, sec_el, first_el + sec_el);
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

            case DUMP:
                //print_stk_elements(stk);    сделать другой дамп
                break;

            case OUT:
                printf("%d\n", stk->data[0]);
                break; 
            
            case JUMP:
                processor->ip = ip;
                jump(processor, stk, err);
                ip = processor->ip;
                break;

            case POP:
                stk_pop (stk, &elem, err);
                break;

            case POPR:   //забирает из стэка и пушит в регистр
                processor->ip = ip;
                popr(stk, processor, err);
                ip = processor->ip;
                break;

            case PUSHR:   //забирает из регистра и пушит в стэк
                processor->ip = ip;
                pushr(stk, processor, err);
                ip = processor->ip;
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

ResultOfComparing comparing(int first_el, int sec_el)
{
    if (first_el > sec_el)
        return GREATER;

    else if (first_el == sec_el)
        return EQUAL;

    else 
        return LESS;
}

void jump (Proc* const processor, Stack* const stk, Errors* const err)
{
    JUMP_INFO
    ip++;
    int expected_res = program[ip];
    ResultOfComparing comp_res = comparing(first_el, sec_el);

    switch (expected_res)
    {
        case(GREATER):
            if (comp_res == GREATER)
                JUMP_FUNC

        case(GREATER_AND_EQUAL):
            if (comp_res == GREATER || comp_res == EQUAL)
                JUMP_FUNC

        case(LESS):
            if (comp_res == LESS)
                JUMP_FUNC

        case(LESS_AND_EQUAL):
            if (comp_res == LESS || comp_res == EQUAL)
                JUMP_FUNC

        case(EQUAL):
            if (comp_res == EQUAL)
                JUMP_FUNC

        case(NOT_EQUAL):
            if (comp_res != EQUAL)
                JUMP_FUNC
    }
}




int get_arg (Proc* processor)
{
    size_t ip = processor->ip;
    int* program = processor->new_file_buf;
    ip++;  //перемещение на тип аргумента

    int result = 0;
    int arg_type = program[ip++];

    if (arg_type & INT)
        result += program[ip++];
    
    if (arg_type & REG)
        result += processor->registers[ip++].value;

    if (arg_type & RAM)   //к определенной ячейке оперативной памяти, загружаемой как аргумент
        result += processor->RAM[result];
    
    ip--;  //чтобы не проскочил след команду из-за прибавления ip в цикле
    processor->ip = ip;

    return result;
}

void popr(Stack* const stk, Proc* const processor, Errors* const err)
{
    assert(stk != nullptr);
    assert(processor != nullptr);
    assert(err != nullptr);


    int elem = 0;
    stk_pop(stk, &elem, err);
    
    size_t ip = processor->ip;
    ip++;
    int* program = processor->new_file_buf;
    int reg_ind = program[ip];

    processor->registers[reg_ind].value = elem;
    processor->ip = ip;
}

void pushr (Stack* const stk, Proc* const processor, Errors* const err)
{

    assert(stk != nullptr);
    assert(processor != nullptr);
    assert(err != nullptr);

    size_t ip = processor->ip;
    ip++;
    int* program = processor->new_file_buf;
    int reg_ind = program[ip];

    int elem = processor->registers[reg_ind].value;

    stk_push(stk, elem, err);
    processor->ip = ip;
}