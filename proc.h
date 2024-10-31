/*#ifndef _PROC_H_
#define _PROC_H_

#include "asm.h"

static const size_t JUMP_AMT = 6;
enum ArgType
{
    NULL_ = 0,
    INT = 1,
    REG = 2,
    RAM = 4
};

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

#define TWO_ARGS        stk_pop(stk, &sec_el, err);                 \
                        stk_pop(stk, &first_el, err);

#define ONE_ARG         ip++;                                       \
                        stk_pop(stk, &arg, err);

#define JUMP_INFO       assert(stk != nullptr);                     \
                        assert(processor != nullptr);               \
                        assert(err != nullptr);                     \
                                                                    \
                        size_t ip = processor->ip;                  \
                        int* program = processor->new_file_buf;     \
                        stack_element_t first_el = 0;               \
                        stack_element_t sec_el = 0;                 \
                                                                    \
                        stk_pop(stk, &sec_el, err);                 \
                        stk_pop(stk, &first_el, err);   

typedef DoJump (*comparator_t) (int first_el, int sec_el);

struct JumpParameters 
{
    ResultOfComparing res;
    comparator_t comparator;
};

void proc_code (Stack* const stk, Processor* const proc, Errors* const err);

DoJump comparator_less (int first_el, int sec_el);
DoJump comparator_equal (int first_el, int sec_el);
DoJump comparator_greater (int first_el, int sec_el);
DoJump comparator_not_equal (int first_el, int sec_el);
DoJump comparator_less_and_equal (int first_el, int sec_el);
DoJump comparator_greater_and_equal (int first_el, int sec_el);

static const struct JumpParameters jump_array[JUMP_AMT] = {{.res = LESS, .comparator = comparator_less},
                                                     {.res = EQUAL, .comparator = comparator_equal},
                                                     {.res = GREATER, .comparator = comparator_greater},
                                                     {.res = NOT_EQUAL, .comparator = comparator_not_equal},
                                                     {.res = LESS_AND_EQUAL, .comparator = comparator_less_and_equal},
                                                     {.res = GREATER_AND_EQUAL, .comparator = comparator_greater_and_equal},};

#endif //_PROC_H_*/