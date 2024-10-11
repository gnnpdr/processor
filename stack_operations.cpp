#include <stdio.h>

#include "stack_operations.h"

void push(Stack* stk, stack_element_t element)
{

    stack_element_t* data      = stk->data;
    size_t           size      = stk->size;
    size_t           capacity  = stk->capacity;

    if (size >= capacity)
    {
        size_t new_capacity = capacity * delta;
        change_capacity(stk, new_capacity);
    }

    data[size] = element;

    size++;
    stk->size = size;
}

void change_capacity(Stack* stk, size_t new_capacity)
{
    stack_element_t* data = stk->data;
    size_t capacity = stk->capacity;

    data = (stack_element_t*)realloc(data, (new_capacity)*sizeof(stack_element_t));
    #ifdef DEBUG
    if (data == nullptr)
    {
        printf("no place\n");
        return NO_PLACE;
    }
    #endif 

    if (new_capacity > capacity)
    {
        for (size_t i = 0; i < capacity; i++)
            data[capacity + i] = poison;
    }
}

stack_element_t pop(Stack* stk)
{
    size_t size = stk->size;
    size_t capacity = stk->capacity;
    stack_element_t* data = stk->data;

    if (size == (capacity / double_delta))
    {
        size_t new_capacity = capacity / delta;
        change_capacity(stk, new_capacity);
    }
    
    size--;   //по идее сначала заполнение пойзонами, а потом изменение размера, разный вывод при дебаге и недебаге, ничего не понимаю
    stack_element_t arg = data[size];
    data[size] = poison;
    
    stk->size = size;
    return arg;
}