#ifndef _PROC_H_
#define _PROC_H_

#include "assembly.h"

struct Processor
{
    int* new_file_buf;

    size_t ncmd;
    
    size_t ip;

    int RAM[RAM_AMT];
};

static const size_t JUMP_AMT = 6;

enum ResultOfComparing
{
    LESS = 0,
    EQUAL = 1,
    GREATER = 2,
    NOT_EQUAL = 3,
    LESS_AND_EQUAL = 4,
    GREATER_AND_EQUAL = 5
};

enum DoJump
{
    DO_JUMP,
    NO_JUMP
};

#define TWO_ARGS        stk_pop(stk, &sec_el);                      \
                        stk_pop(stk, &first_el);

#define ONE_ARG         ip++;                                       \
                        stk_pop(stk, &arg);

#define JUMP_INFO       assert(stk != nullptr);                     \
                        assert(processor != nullptr);               \
                        assert(err != nullptr);                     \
                                                                    \
                        size_t ip = processor->ip;                  \
                        int* program = processor->new_file_buf;     \
                        stack_element_t first_el = 0;               \
                        stack_element_t sec_el = 0;                 \
                                                                    \
                        stk_pop(stk, &sec_el);                      \
                        stk_pop(stk, &first_el);   

typedef DoJump (*comparator_t) (int first_el, int sec_el);

struct JumpParameters 
{
    ResultOfComparing res;
    comparator_t comparator;
};

void proc_file (Stack* const stk, Processor* const proc);

#endif //_PROC_H_