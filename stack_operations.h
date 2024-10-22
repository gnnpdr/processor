
#include "asm.h"


#ifndef _PUSH_H_
#define _PUSH_H_

#include "stack.h"

Errors stk_push(Stack* stk, stack_element_t element, Errors* err);

Errors stk_pop(Stack* stk, stack_element_t* elem, Errors* err);
Errors change_capacity(Stack* stk, size_t new_capacity, size_t capacity, Errors* err);

#endif //_PUSH_H_