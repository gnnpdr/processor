#ifndef _COMPILE_H_
#define _COMPILE_H_

#include "stack.h"

static const size_t CMD_AMT           = 18;
static const size_t MAX_INPUT_CMD_AMT = 60;
static const size_t LABELS_AMT        = 10;

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
    const char*  com_str;
    CommandsNums com_num;
    size_t       arg_amt;
};

struct LabelParameters
{
    size_t j_pointer;
    char* label_word;
};

const struct CommandParameters PushStr  =  {.com_str = "push" , .com_num = PUSH , .arg_amt = 1};
const struct CommandParameters AddStr   =  {.com_str = "add"  , .com_num = ADD  , .arg_amt = 0};
const struct CommandParameters SubStr   =  {.com_str = "sub"  , .com_num = SUB  , .arg_amt = 0};
const struct CommandParameters MulStr   =  {.com_str = "mul"  , .com_num = MUL  , .arg_amt = 0};
const struct CommandParameters DivStr   =  {.com_str = "div"  , .com_num = DIV  , .arg_amt = 0};
const struct CommandParameters SqrtStr  =  {.com_str = "sqrt" , .com_num = SQRT , .arg_amt = 1};
const struct CommandParameters SinStr   =  {.com_str = "sin"  , .com_num = SIN  , .arg_amt = 1};
const struct CommandParameters CosStr   =  {.com_str = "cos"  , .com_num = COS  , .arg_amt = 1};
const struct CommandParameters DumpStr  =  {.com_str = "dump" , .com_num = DUMP , .arg_amt = 0};
const struct CommandParameters HltStr   =  {.com_str = "hlt"  , .com_num = HLT  , .arg_amt = 0};
const struct CommandParameters OutStr   =  {.com_str = "out"  , .com_num = OUT  , .arg_amt = 1};
const struct CommandParameters JaStr    =  {.com_str = "ja"   , .com_num = JA   , .arg_amt = 2};
const struct CommandParameters JaeStr   =  {.com_str = "jae"  , .com_num = JAE  , .arg_amt = 2};
const struct CommandParameters JeStr    =  {.com_str = "je"   , .com_num = JE   , .arg_amt = 2};
const struct CommandParameters JneStr   =  {.com_str = "jne"  , .com_num = JNE  , .arg_amt = 2};
const struct CommandParameters PopStr   =  {.com_str = "pop"  , .com_num = POP  , .arg_amt = 1};
const struct CommandParameters PushrStr =  {.com_str = "pushr", .com_num = PUSHR, .arg_amt = 0};
const struct CommandParameters PoprStr  =  {.com_str = "popr" , .com_num = POPR , .arg_amt = 0};

static const struct CommandParameters bunch_of_commands [CMD_AMT]  =    {PushStr,
                                                                        AddStr   ,
                                                                        SubStr   ,
                                                                        MulStr   ,
                                                                        DivStr   ,
                                                                        SqrtStr  ,
                                                                        SinStr   ,
                                                                        CosStr   ,
                                                                        DumpStr  ,
                                                                        HltStr   ,
                                                                        OutStr   ,
                                                                        JaStr    ,
                                                                        JaeStr   ,
                                                                        JeStr    ,
                                                                        PopStr   ,
                                                                        PushrStr ,
                                                                        PoprStr };

struct Proc
{
    char* file_buf;
    int* new_file_buf;
    size_t input_file_commands_amount;
    size_t init_file_size;
    
    int* registers;

    size_t ip;
    size_t* symb_amount_arr;
    char** addresses;
    LabelParameters* labels;

};

enum LabelType
{
    START,
    ARG
};

enum LabelRes
{
    LABEL        ,
    NOT_LABEL    ,
    PLACE_PROBLEM,
    SYN_PROBLEM
};

#define SSCANF(file_buf, ...)   do                                                   \
                                {                                                    \
                                    sscanf (file_buf, __VA_ARGS__);                  \
                                    i++;                                      \
                                    file_buf = processor->addresses[i]; \
                                }while(0);

StkErrors file_transformation(const char* const name, Proc* const processor, StkErrors* err);
StkErrors make_init_buf (const char* const  name, Proc* const processor, StkErrors* err);
void processing_text(Proc* processor);
void filling_addresses(Proc* processor);
StkErrors output_file(Proc* const processor, StkErrors* err);
StkErrors compile(Proc* const  processor, int* const new_file_buf, StkErrors* err);
LabelRes check_label(Proc* processor, char* file_buf, size_t* word_num, LabelType label_place, LabelRes* is_label, int* arg);
void free_bufs (Proc* processor);

#endif //_COMPILE_H_