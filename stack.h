#ifndef _CTOR_H_
#define _CTOR_H_

#include <stdlib.h>
#include <string.h>
#include "check.h"

static const int stk_amount = 4;
static const int delta = 2;
static const int double_delta = 4;

Errors stk_ctor(Stack* stk, const char* file, const char* func, const int code_str, Errors* err);

void stk_dtor(Stack* stk);

#endif //_CTOR_H_