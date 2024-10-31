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
    printf("cap %d\n", stk->capacity);

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
    //CHECK_STK(stk, err)

    stack_element_t* data = stk->data;
    size_t           size      = stk->size;
    size_t           capacity  = stk->capacity;
    //printf("not right\n data %p\n size %d\n capacity %d\n", data, size, capacity);


    if (size >= capacity)
    {
        /*printf("START\n");
        for (int i = 0; i < stk->capacity; i++)
            printf("%d ", stk->data[i]);

        printf("\n");*/
        size_t new_capacity = capacity * delta;
        change_capacity(stk, new_capacity, capacity);

        /*printf("END\n");
        for (int i = 0; i < stk->capacity; i++)
            printf("%d ", stk->data[i]);

        printf("\n");*/
        stk->data = data; 
        //RETURN(*err)
    }

    //printf("addr 1 %p\n", data + size);
    /*printf("-----------------------------------------------------------------------------------------");
    printf("ELEMENT %d\n", element);
    
    printf("\n\n");
    for (int i = 0; i < stk->capacity; i++)
    {
        printf("%d\n", i); 
        printf("%d\n", stk->data[i]);
        printf("%p\n", stk->data + i);

        
    }
    printf("\n\n");
    for (int i = 0; i < stk->capacity; i++)
            printf("%d ", stk->data[i]);
    printf("\n\n");

    printf("ELEMENT %d\n", element);
    printf("ADDRESS %p\n", data + size);*/
    data[size] = element;
    //printf("data 2 %d\n", data[size]);
    size++;
    stk->size = size;
    //printf("its all right in stack\n");
    
    #ifdef DEBUG
    stk->hash = stk_hash(stk);
    #endif

    printf("\n\n");
    for (int i = 0; i < stk->capacity; i++)
    {
        printf("%d\n", i); 
        printf("%d\n", stk->data[i]);
        printf("%p\n", stk->data + i);
    }
    printf("\n\n");

    printf("\n\n");
    for (int i = 0; i < stk->capacity; i++)
            printf("%d ", stk->data[i]);

    //CHECK_STK(stk, err)
    return;
}

void change_capacity(Stack* stk, size_t new_capacity, size_t capacity)
{
    //CHECK_STK(stk, err)

    stack_element_t* data = stk->data;

    data = (stack_element_t*)realloc(data, (new_capacity CANARY_CAPACITY_ADD)*sizeof(stack_element_t));
    #ifdef DEBUG
    if (data == nullptr)
    {
        printf("no place\n");
        return NO_PLACE;
    }
    #endif 

    if (new_capacity > capacity)
    {
        for (size_t i = 0; i <= capacity LEFT_CANARY_ADD; i++)
            data[capacity LEFT_CANARY_ADD + i] = POISON;
    }
    printf("capacity %d\n", new_capacity);

    stk->capacity = new_capacity;
    stk->data = data;

    #ifdef DEBUG
    data[new_capacity LEFT_CANARY_ADD] = right_canary_value;
    
    #endif

    //CHECK_STK(stk, err)
    return;
}

void stk_pop(Stack* stk, stack_element_t* elem)
{
    //CHECK_STK(stk, err)

    size_t size = stk->size;
    size_t capacity = stk->capacity;
    stack_element_t* data = stk->data;

    if (size == (capacity / double_delta))
    {
        size_t new_capacity = capacity / delta;
        change_capacity(stk, new_capacity, capacity);
        //RETURN(*err)
    }

    //printf("in pop\n");
    //print_stk_elements(stk->data, 6, 6);
    //printf("sizeelem %d\n", data[size]);

    size--; 
    *elem = data[size];
    data[size] = POISON;
    
    stk->size = size;

    stk->data = data;

    #ifdef DEBUG
    stk->hash = stk_hash(stk);
    #endif

    CHECK_STK(stk, err)
    return;
}