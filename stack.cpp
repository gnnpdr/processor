#include <stdio.h>

#include "stack.h"

void ctor (Stack* stk)
{
    stk->size = 0;
    stk->capacity = stk_amount;
    size_t capacity = stk->capacity;
    stack_element_t* data = stk->data;

    data = (stack_element_t*)calloc(capacity, sizeof(stack_element_t));
    if (data == nullptr)
    {
        printf("no place for arrays\n");
    }
    
    for (size_t i = 0; i < capacity; i++)
        data[i] = poison;

    stk->data = data;
}

void dtor(Stack* stk)
{
    free(stk->data);
}