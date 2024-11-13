#ifndef _CTOR_H_
#define _CTOR_H_

#include <stdlib.h>
#include <string.h>
#include "check.h"


static const int delta = 2;
static const int double_delta = 4;


void stk_push(Stack* stk, stack_element_t element);

void stk_pop(Stack* stk, stack_element_t* elem);
void change_capacity(Stack* stk);

void stk_ctor(Stack* stk);

void stk_dtor(Stack* stk);

#endif //_CTOR_H_