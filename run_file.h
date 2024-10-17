#ifndef _RUN_H_
#define _RUN_H_

#include "stack_operations.h"

void run_prog (Stack* stk, Proc* processor, StkErrors* err);

void ja(Proc* processor, Stack* stk);
void jae(Proc* processor, Stack* stk);
void je(Proc* processor, Stack* stk);
void jne(Proc* processor, Stack* stk);

#endif //_RUN_H_