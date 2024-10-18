#include <stdio.h>

#include "run_file.h"

void run_prog (Stack* const stk, Proc* const processor, StkErrors* const err)
{
    assert(stk != nullptr);
    assert(processor != nullptr);
    assert(err != nullptr);

    size_t ip = processor->ip;
    int* program = processor->new_file_buf;

    stack_element_t elem = 0;
    int first_el = 0;
    int sec_el = 0;
    int arg = 0;
    size_t amount = processor->input_file_commands_amount;

    while(ip < amount)
    {
        switch(program[ip])
        {
            case PUSH:
                stk_push (stk, program[ip], err);
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

            case DUMP:
                //print_stk_elements(stk);    сделать другой дамп
                break;

            case OUT:
                printf("%d\n", stk->data[0]);
                break; 
            
            case JA:
                ip++;
                processor->ip = ip;
                ja(processor, stk, err);
                break;

            case JAE:
                ip++;
                jae(processor, stk, err);
                break;

            case JE:
                ip++;
                je(processor, stk, err);
                break;

            case JNE:
                ip++;
                jne(processor, stk, err);
                break;

            case POP:
                stk_pop (stk, &elem, err);
                break;

            case POPR:
                ip++;
                stk_pop(stk, &arg, err);
                popr(stk, processor, arg, err);
                break;

            case PUSHR:
                ip++;
                stk_pop(stk, &arg, err);
                pushr(stk, processor, arg, err);
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

void ja(Proc* const processor, Stack* const stk, StkErrors* const err)  //не уверена, настолько ли важно хранить айпи в стэке, а не как просто переностимую переменную
{
    assert(stk != nullptr);
    assert(processor != nullptr);
    assert(err != nullptr);

    size_t ip = 0;
    ip = processor->ip;
    stack_element_t first_el = 0;
    stack_element_t sec_el = 0;

    stk_pop(stk, &sec_el, err);
    stk_pop(stk, &first_el, err);

    if(first_el > sec_el && processor->registers[0] < 10)
    {
        ip = processor->new_file_buf[ip];
        processor->registers[0]++;
    }

    processor->ip = ip;
}

void jae (Proc* const processor, Stack* const stk, StkErrors* const err)
{
    assert(stk != nullptr);
    assert(processor != nullptr);
    assert(err != nullptr);

    size_t ip = 0;
    ip = processor->ip;
    stack_element_t first_el = 0;
    stack_element_t sec_el = 0;
    
    stk_pop(stk, &sec_el, err);
    stk_pop(stk, &first_el, err);

    if(first_el >= sec_el && processor->registers[0] < 10)
    {
        ip = processor->new_file_buf[ip];
        processor->registers[0]++;
    }

    processor->ip = ip;
}

void je (Proc* const processor, Stack* const stk, StkErrors* const err)
{
    assert(stk != nullptr);
    assert(processor != nullptr);
    assert(err != nullptr);

    size_t ip = 0;
    ip = processor->ip;
    stack_element_t first_el = 0;
    stack_element_t sec_el = 0;
    
    stk_pop(stk, &sec_el, err);
    stk_pop(stk, &first_el, err);

    if(first_el == sec_el && processor->registers[0] < 10)
    {
        ip = processor->new_file_buf[ip];
        processor->registers[0]++;
    }

    processor->ip = ip;
}

void jne (Proc* const processor, Stack* const stk, StkErrors* const err)
{
    assert(stk != nullptr);
    assert(processor != nullptr);
    assert(err != nullptr);

    size_t ip = 0;
    ip = processor->ip;
    stack_element_t first_el = 0;
    stack_element_t sec_el = 0;
    
    stk_pop(stk, &sec_el, err);
    stk_pop(stk, &first_el, err);

    if(first_el != sec_el && processor->registers[0] < 10)
    {
        ip = processor->new_file_buf[ip];
        processor->registers[0]++;
    }

    processor->ip = ip;
}

void popr(Stack* const stk, Proc* const processor, size_t reg_ind, StkErrors* const err)
{
    assert(stk != nullptr);
    assert(processor != nullptr);
    assert(err != nullptr);

    int elem = 0;
    stk_pop(stk, &elem, err);
    processor->registers[reg_ind] = elem;
}

void pushr (Stack* const stk, Proc* const processor, size_t reg_ind, StkErrors* const err)
{
    assert(stk != nullptr);
    assert(processor != nullptr);
    assert(err != nullptr);

    int elem = 0;
    elem = processor->registers[reg_ind];
    stk_push(stk, elem, err);
}