#ifndef _ASM_H_
#define _ASM_H_

#include "input.h"

static const int CMD_AMT = 12;
static const int START_VALUE = -1;
static const int OUT_AMT = 100;

static const size_t LABELS_AMT = 10;
static const size_t REG_AMT = 4;
static const size_t RAM_AMT = 50;

static const char LABEL_MARK = ':';
static const char COMMENT_MARK = ';';

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
};

struct RegisterParameters
{
    int value;
    char* name;
};

struct RegisterParameters ax = {.value = START_VALUE, .name = "ax"};
struct RegisterParameters bx = {.value = START_VALUE, .name = "bx"};
struct RegisterParameters cx = {.value = START_VALUE, .name = "cx"};
struct RegisterParameters ex = {.value = START_VALUE, .name = "ex"};

struct RegisterParameters registers [REG_AMT] = { ax,
                                                  bx,
                                                  cx,
                                                  ex };


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

static const CommandParameters bunch_of_commands [CMD_AMT]  =   {PushStr,
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

void assembly (Text* input, Labels* labels, Stack* new_buf);
void make_file (Stack* new_buf);
#endif //_ASM_H_