#include <stdio.h>

#include "check.h"

#ifdef DEBUG
void stk_dump(Stack* stk, const char* file, const char* func, const int code_str)
{
    stack_element_t* data = stk->data;
    size_t size = stk->size;
    size_t capacity = stk->capacity;

    printf("Stack [%x]\n", (size_t)stk);

    printf("called from %s: %d (%s)\n", file, code_str, func);
    printf("name stk born at %s: %d (%s)\n", stk->origin_file, stk->origin_str, stk->origin_func);

    printf("\nleft canary = %#x\n", (size_t)data[0]);
    printf("right canary = %#x\n\n", (size_t)data[capacity LEFT_CANARY_ADD]);

    printf("array data address %p\n", data);
    printf("capacity = %d\n", capacity);
    printf("size = %d\n", size);
    printf("array data [%x]\n{\n", (size_t)data);
    print_stk_elements(data, capacity, size);
}


Errors check(Stack* stk)
{
    assert(stk != nullptr);
    //assert(stk->data != nullptr);

    //size_t size = stk->size;
    size_t capacity = stk->capacity;
    //stack_element_t* data = stk->data;

    if (stk == nullptr)
    {
        printf("no place for arrays\n");
        return NO_PLACE;
    }

    if (stk->size > stk->capacity)
    {
        printf("program crossed the line\n");
        return BUFFER_OVERFLOW;
    }
        

    //if(data[capacity LEFT_CANARY_ADD] != right_canary_value)
    //{
    //    printf("problem in right canary\n");
    //    return PROBLEM;
    //}

    if (stk->hash != stk_hash(stk))
    {
        printf("strange rearrangement in the array, hash is incorrect\n");
        return HASH_PROBLEM;
    }

    return ALL_RIGHT;
}


unsigned long long stk_hash(Stack* stk)
{
    unsigned long long hash = start_hash;
    stack_element_t* data = stk->data;
    size_t size = stk->size;
    if (data[size] == poison)
        size--;

    size_t elem_num = LEFT_CANARY_ADD;

    while (elem_num < size)
    {
        hash = hash * 33  + (unsigned long long)data[elem_num];
        elem_num++;
    }
    return hash;
}
#endif

void print_stk_elements(Stack* stk)
{
    assert (stk != nullptr);

    int* data = stk->data;
    int size = stk->size;
    int capacity = stk->capacity;
    
    for (size_t i = LEFT_CANARY_ADD; i < capacity; i++)
    {
        if(i < size)
            printf(" * ");
        else
            printf("   ");
        if (data[i] == POISON)
            printf("[%d] = %d (POISON)\n", i, POISON);
        else
            printf("[%d] = %d\n", i, data[i]);
    }
}