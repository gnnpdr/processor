#ifndef _RUN_H_
#define _RUN_H_

#include "stack_operations.h"

enum ResultOfComparing
{
    LESS = 0,
    EQUAL = 1,
    GREATER = 2,
    NOT_EQUAL = 3,
    LESS_AND_EQUAL = 4,
    GREATER_AND_EQUAL = 5
};

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


#define JUMP_FUNC       do                                  \
                        {                                   \
                            ip++;                           \
                            size_t new_ip = program[ip];    \
                            processor->ip = new_ip;         \
                            break;                          \
                        }while(0);


void run_prog (Stack* const stk, Proc* const processor, Errors* const err);


ResultOfComparing comparing(int first_el, int sec_el);
void jump (Proc* const processor, Stack* const stk, Errors* const err);


void make_regs(Proc* processor);
void popr(Stack* const stk, Proc* const processor, Errors* const err);
void pushr (Stack* const stk, Proc* const processor, Errors* const err);


#endif //_RUN_H_