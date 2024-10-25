#include <stdio.h>

#include "stack.h"

Errors stk_ctor(Stack* stk, const char* file, const char* func, const int code_str, Errors* err)
{
    #ifdef DEBUG
    stk->origin_file = file;
    stk->origin_str = code_str;
    stk->origin_func = func;
    #endif

    stk->size = LEFT_CANARY_ADD;
    stk->capacity = stk_amount;
    size_t capacity = stk->capacity;
    stack_element_t* data = stk->data;
    printf("cap %d\n", stk->capacity);

    data = (stack_element_t*)calloc(capacity CANARY_CAPACITY_ADD, sizeof(stack_element_t));
    if (data == nullptr)
    {
        printf("no place for arrays\n");
        return NO_PLACE;
    }
    
    for (size_t i = 0; i < capacity; i++)
        data[i LEFT_CANARY_ADD] = POISON;

    stk->data = data;

    #ifdef DEBUG
    data[0] = left_canary_value;
    data[capacity LEFT_CANARY_ADD] = right_canary_value;
    #endif

    CHECK_STK(stk, err)
    return *err;
}

void stk_dtor(Stack* stk)
{
    free(stk->data);
}