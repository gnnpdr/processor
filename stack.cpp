#include <stdio.h>

#include "stack.h"

void stk_ctor(Stack* stk)
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
    //printf("cap %lld\n", stk->capacity);

    data = (stack_element_t*)calloc(capacity CANARY_CAPACITY_ADD, sizeof(stack_element_t));
    if (data == nullptr)
    {
        printf("no place for arrays\n");
        //return NO_PLACE;
    }
    
    for (size_t i = 0; i < capacity; i++)
        data[i LEFT_CANARY_ADD] = POISON;

    stk->data = data;

    #ifdef DEBUG
    data[0] = left_canary_value;
    data[capacity LEFT_CANARY_ADD] = right_canary_value;
    #endif

    //CHECK_STK(stk, err)
    return;
}

void stk_dtor(Stack* stk)
{
    free(stk->data);
}

void stk_push(Stack* stk, stack_element_t element)
{
    size_t size      = stk->size;
    size_t capacity  = stk->capacity;

    if (size >= capacity)
        change_capacity(stk);
    
    int* data = stk->data;

    data[size] = element;

    size++;
    stk->size = size;

    /*printf("--------------------------------\nSTACK PUSH\n");
    printf("\n\n");
    for (size_t i = 0; i < stk->capacity; i++)
            printf("%d ", stk->data[i]);
    printf("\n");*/

    stk->data = data; 
    return;
}

void change_capacity(Stack* stk)  //теперь эта функция не подходит для попа
{
    size_t size = stk->size;
    size_t capacity = stk->capacity;
    int* data = stk->data;

    size_t new_capacity = 0;

    if (size >= capacity)
        new_capacity = capacity * delta;
    else
        new_capacity = capacity / delta;

    data = (int*)realloc(data, new_capacity * sizeof(int));
    
    for (size_t i = 0; i <= capacity LEFT_CANARY_ADD; i++)
        data[capacity LEFT_CANARY_ADD + i] = POISON;

    stk->capacity = new_capacity;
    stk->data = data; 

    /*for (int i = 0; i < new_capacity; i++)
        printf("%d ", data[i]);

    printf("\n");*/
    return;
}

void stk_pop(Stack* stk, stack_element_t* elem)
{
    size_t size = stk->size;
    size_t capacity = stk->capacity;
    int* data = stk->data;

    if (size == (capacity / double_delta) && size > 3)  //3 сделать константой
        change_capacity(stk);
    
    size--; 
    *elem = data[size];
    data[size] = POISON;
    
    stk->size = size;

    stk->data = data;

    /*printf("--------------------------------\nSTACK POP\n");
    printf("\n\n");
    for (size_t i = 0; i < stk->capacity; i++)
            printf("%d ", stk->data[i]);
    printf("\n");*/

    //CHECK_STK(stk, err)
    //return;
}