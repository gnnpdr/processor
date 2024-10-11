#ifndef _PUSH_H_
#define _PUSH_H_

#include "stack.h"

void push(Stack* stk, stack_element_t element);

stack_element_t pop(Stack* stk);
void change_capacity(Stack* stk, size_t new_capacity);

#endif //_PUSH_H_