#ifndef _CHECK_H_
#define _CHECK_H_

#include <assert.h>
#include <math.h>

//#define DEBUG

typedef int stack_element_t;
static const stack_element_t POISON = -13;

#define POSITION , __FILE__, __func__, __LINE__

static const int stk_amount = 6;

struct Stack
{
    #ifdef DEBUG
    unsigned long long left_canary;
    const char* origin_file;
    int origin_str;
    const char* origin_func;
    unsigned long long hash;
    unsigned long long right_canary;
    #endif

    stack_element_t* data;
    size_t size;
    size_t capacity = stk_amount;
};

enum Errors
{
    ALL_RIGHT,
    NO_PLACE,
    ALLOCATION_ERROR,
    PROBLEM,
    BUFFER_OVERFLOW,
    HASH_PROBLEM,
    VALUE_PROBLEM,
    UNKNOWN,
    FILE_PROBLEM,
    PLACE_PROBLEM,
    SYN_ERROR,
    READ_ERROR,
    CLOSE_ERROR,
    SSCANF_ERROR,
    WRITE_ERROR
};

#ifdef DEBUG
    
    #define RETURN(err)  if (err != ALL_RIGHT) return PROBLEM;
    
    static const unsigned long long left_canary_value = 0xC001;
    static const unsigned long long right_canary_value = 0xC001;
    static const unsigned long long start_hash = 5381;

    #define CANARY_CAPACITY_ADD + 2
    #define LEFT_CANARY_ADD + 1


    #define CHECK_STK(stk, err)     do                              \
                                    {                               \
                                        if(check(stk) != ALL_RIGHT) \
                                        {                           \
                                            dump(stk POSITION);     \
                                            *err = PROBLEM;         \
                                        }                           \
                                    } while(0);

#else
    #define CHECK_STK(stk, err) 
    #define RETURN(err) 
    #define CHECK_FUNC(func)
    #define CANARY_CAPACITY_ADD + 0
    #define LEFT_CANARY_ADD + 0
    
#endif

void stk_dump(Stack* stk, const char* file, const char* func, const int code_str);
void print_stk_elements(Stack* stk);
Errors check(Stack* stk);

unsigned long long stk_hash(Stack* stk);

#endif //_CHECK_H_