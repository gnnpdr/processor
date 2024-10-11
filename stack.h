#ifndef _CTOR_H_
#define _CTOR_H_

#include <stdlib.h>
#include <string.h>

typedef int stack_element_t;
static const stack_element_t poison = 13;

static const int stk_amount = 4;
static const int delta = 2;
static const int double_delta = 4;

struct Stack
{
    stack_element_t* data;
    size_t size;
    size_t capacity;
};

enum StkErrors
{
    ALL_RIGHT,
    NO_PLACE,
    PROBLEM,
    BUFFER_OVERFLOW,
    HASH_PROBLEM,
    VALUE_PROBLEM,
    UNKNOWN
};


void ctor (Stack* stk);

void dtor(Stack* stk);

#endif //_CTOR_H_