#ifndef _ASM_H_
#define _ASM_H_

#include "input.h"

static const int CMD_AMT = 12;
static const int START_VALUE = -1;
static const int OUT_AMT = 100;

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

#define SSCANF(file_buf, ...)   do                                                   \
                                {                                                    \
                                    cmd_num++;                                       \
                                    file_buf = proc->addresses[*cmd_num];            \
                                    sscanf (file_buf, __VA_ARGS__);                  \
                                }while(0);

#define CAP_CHECK(out_cmd_num, out_cmd_amt)     do                                                                  \
                                                {                                                                   \
                                                    if (out_cmd_num == out_cmd_amt - 1)                             \
                                                    {                                                               \
                                                        out_cmd_amt += OUT_AMT;                                     \
                                                        new_buf = (int*)realloc(new_buf, out_cmd_amt*sizeof(int));  \
                                                    }                                                               \
                                                }while(0);


void assembly_code (char* name, Processor* proc);

void proc_dtor (Processor* proc, char* name);

#endif //_ASM_H_