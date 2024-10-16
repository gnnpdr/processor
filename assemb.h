#ifndef _COMPILE_H_
#define _COMPILE_H_

#include "stack.h"

enum CommandsNums
{
    PUSH  = 1 ,
    ADD   = 2 ,
    SUB   = 3 ,
    MUL   = 4 ,
    DIV   = 5 ,
    SQRT  = 6 ,
    SIN   = 7 ,
    COS   = 8 ,
    DUMP  = 9 ,
    OUT   = 10,
    HLT   = 11,
    JA    = 12,
    JAE   = 13,
    JE    = 14,
    JNE   = 15,
    POP   = 16,
    POPR  = 17,
    PUSHR = 18
};

struct CommandParameters 
{
    char* com_str;
    CommandsNums com_num;
};

static const size_t commands_amount = 18;

struct Proc
{
    char* file_buf;
    int* new_file_buf;
    size_t input_file_commands_amount;
    size_t init_file_size;
    struct CommandParameters  bunch_of_commands [commands_amount]  =    {{"push", PUSH},
                                                                        {"add"  , ADD} ,
                                                                        {"sub"  , SUB} ,
                                                                        {"mul"  , MUL} ,
                                                                        {"div"  , DIV} ,
                                                                        {"sqrt" , SQRT},
                                                                        {"sin"  , SIN} ,
                                                                        {"cos"  , COS} ,
                                                                        {"dump" , DUMP},
                                                                        {"hlt"  , HLT} ,
                                                                        {"out"  , OUT} ,
                                                                        {"ja"   , JA}  ,
                                                                        {"jae"  , JAE} ,
                                                                        {"je"   , JE}  ,
                                                                        {"jne"  , JNE} ,
                                                                        {"pop"  , POP} ,
                                                                        {"popr" , POPR},
                                                                        {"pushr", PUSHR}};
    int* registers;
    size_t ip;
};

void file_transformation(const char* const name, Proc* const processor);
void make_init_buf (const char* const  name, Proc* const processor);
void processing_text(Proc* const processor);
void output_file(Proc* const processor);
void cool_compile(int* const new_file_buf, Proc* const  processor);

#endif //_COMPILE_H_