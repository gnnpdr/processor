#ifndef _RUN_H_
#define _RUN_H_

#include "stack_operations.h"

void run_prog (Stack* const stk, Proc* const processor, StkErrors* const err);

void ja(Proc* const processor, Stack* const stk, StkErrors* const err);
void jae (Proc* const processor, Stack* const stk, StkErrors* const err);
void je (Proc* const processor, Stack* const stk, StkErrors* const err);
void jne (Proc* const processor, Stack* const stk, StkErrors* const err);

void popr(Stack* const stk, Proc* const processor, size_t reg_ind, StkErrors* const err);
void pushr (Stack* const stk, Proc* const processor, size_t reg_ind, StkErrors* const err);


#endif //_RUN_H_