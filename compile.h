#ifndef _COMPILE_H_
#define _COMPILE_H_

#include "stack.h"

enum Commands
{
    PUSH = 1,
    ADD = 2,
    SUB = 3,
    MUL = 4,
    DIV = 5,
    SQRT = 6,
    SIN = 7,
    COS = 8,
    DUMP = 9,
    HLT = 10
};

void make_asm_file(char** name, size_t* amount);

void compile (FILE* file, FILE* asm_file, size_t* amount);

void make_array(size_t amount, int* program);

#endif //_COMPILE_H_