#ifndef _ASM_H_
#define _ASM_H_

#include "input.h"

static const int CMD_AMT = 12;
static const int START_VALUE = -1;
static const int OUT_AMT = 100;

static const size_t LABELS_AMT = 10;
static const size_t REG_AMT = 5;
static const size_t RAM_AMT = 50;

enum LabelType
{
    LABEL_DEF,
    ARG
};

struct LabelParameters
{
    int target;
    char* name;
};

struct Labels
{
    LabelParameters* labels;
    LabelType label_type;
    bool is_label;
};

struct RegisterParameters
{
    int value;
    char* name;
};

struct Processor
{
    Stack new_file_buf;

    RegisterParameters registers[REG_AMT];
    
    size_t ip;

    int RAM[RAM_AMT];
};

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
    OUT   = 9 ,
    HLT   = 10,
    JUMP  = 11,
    POP   = 12
};

struct CommandParameters 
{
    const char*  cmd_str;
    CommandsNums cmd_num;
    size_t       arg_amt;
};


const struct CommandParameters PushStr  =  {.cmd_str = "push" , .cmd_num = PUSH , .arg_amt = 3};
const struct CommandParameters AddStr   =  {.cmd_str = "add"  , .cmd_num = ADD  , .arg_amt = 0};
const struct CommandParameters SubStr   =  {.cmd_str = "sub"  , .cmd_num = SUB  , .arg_amt = 0};
const struct CommandParameters MulStr   =  {.cmd_str = "mul"  , .cmd_num = MUL  , .arg_amt = 0};
const struct CommandParameters DivStr   =  {.cmd_str = "div"  , .cmd_num = DIV  , .arg_amt = 0};
const struct CommandParameters SqrtStr  =  {.cmd_str = "sqrt" , .cmd_num = SQRT , .arg_amt = 1};
const struct CommandParameters SinStr   =  {.cmd_str = "sin"  , .cmd_num = SIN  , .arg_amt = 1};
const struct CommandParameters CosStr   =  {.cmd_str = "cos"  , .cmd_num = COS  , .arg_amt = 1};
const struct CommandParameters HltStr   =  {.cmd_str = "hlt"  , .cmd_num = HLT  , .arg_amt = 0};
const struct CommandParameters OutStr   =  {.cmd_str = "out"  , .cmd_num = OUT  , .arg_amt = 1};
const struct CommandParameters JumpStr  =  {.cmd_str = "jump" , .cmd_num = JUMP , .arg_amt = 2};
const struct CommandParameters PopStr   =  {.cmd_str = "pop"  , .cmd_num = POP  , .arg_amt = 3};

static const struct CommandParameters bunch_of_commands [CMD_AMT]  =    {PushStr,
                                                                        AddStr   ,
                                                                        SubStr   ,
                                                                        MulStr   ,
                                                                        DivStr   ,
                                                                        SqrtStr  ,
                                                                        SinStr   ,
                                                                        CosStr   ,
                                                                        HltStr   ,
                                                                        OutStr   ,
                                                                        JumpStr  ,
                                                                        PopStr   };

void assembly (Text* input, Processor* proc);
void fill_labels(Labels* labels);
void handle_commands(Labels* labels, Processor* proc, Text* input);
void cmds(Labels* labels, Text* input, Stack* new_buf, Processor* proc);
void args (Labels* labels, Text* input, size_t cmd, Stack* new_buf);
void label_define(Labels* labels, size_t cmd_num, char* str);
void label_arg(Labels* labels, size_t cmd_num, int* arg, char* str);
bool find_label_mark (const char* const str);
void push_pop_case (Stack* new_buf, Text* input, Processor* proc, size_t cmd);
int arg_analysis (Text* input, int* arg1, int* arg2, Processor* proc);
int RAM_case (Processor* proc, int* arg1, int* arg2, Text* input);
int plus_case(Processor* proc, size_t* cmd_num, int* arg_value, Text* input);
int find_register (Processor* proc, char* str);
void labels_dtor(Labels* labels);

#endif //_ASM_H_