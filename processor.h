#ifndef _RUN_H_
#define _RUN_H_

#include "stack_operations.h"

#define JUMP_INFO       assert(stk != nullptr);         \
                        assert(processor != nullptr);   \
                        assert(err != nullptr);         \
                                                        \
                        size_t ip = 0;                  \
                        ip = processor->ip;             \
                        stack_element_t first_el = 0;   \
                        stack_element_t sec_el = 0;     \
                                                        \
                        stk_pop(stk, &sec_el, err);     \
                        stk_pop(stk, &first_el, err);   


void run_prog (Stack* const stk, Proc* const processor, Errors* const err);

void ja(Proc* const processor, Stack* const stk, Errors* const err);
void jae (Proc* const processor, Stack* const stk, Errors* const err);
void je (Proc* const processor, Stack* const stk, Errors* const err);
void jne (Proc* const processor, Stack* const stk, Errors* const err);

void popr(Stack* const stk, Proc* const processor, size_t reg_ind, Errors* const err);
void pushr (Stack* const stk, Proc* const processor, size_t reg_ind, Errors* const err);


#endif //_RUN_H_