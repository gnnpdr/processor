#ifndef _ASM_H_
#define _ASM_H_

#include <stdint.h>
#include "input.h"

static const int BITS_IN_BYTES = 8;
static const int INT_BYTE_SIZE = sizeof(int)*BITS_IN_BYTES;

static const size_t CMD_AMT     =  18;
static const int    START_VALUE =  -1;
static const size_t OUT_AMT     = 100;
static const size_t MAX_STR_LEN =  80;
static const char   START_CHAR  = 'a';

static const size_t LABELS_AMT = 10;
static const size_t REG_AMT = 4;
static const size_t RAM_AMT = 50;

static const char LABEL_MARK = ':';

static const char* POP_STR = "pop";
static const char* PUSH_STR = "push";

static const char* JA_STR = "ja";
static const char* JE_STR = "je";
static const char* JB_STR = "jb";

static const char* JAE_STR = "jae";
static const char* JNE_STR = "jne";
static const char* JBE_STR = "jbe";

static const size_t MK_ARGS_STRS = 6;

static const char* MK_ARGS[6] =   {
                                        JA_STR,
                                        JAE_STR,
                                        JB_STR,
                                        JBE_STR,
                                        JE_STR,
                                        JNE_STR
                                    };

static const size_t COMPL_ARG_AMT = 2;

static const char* COMPL_ARG[COMPL_ARG_AMT] =   {
                                                    POP_STR,
                                                    PUSH_STR
                                                };

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

enum Registers
{
    AX = 1,
    BX = 2,
    CX = 3,
    DX = 4
};


struct RegisterParameters
{
    int value;
    const char* name;
    size_t num;
};

static const char* AX_STR = "ax";
static const char* BX_STR = "bx";
static const char* CX_STR = "cx";
static const char* DX_STR = "dx";

const struct RegisterParameters ax = {.value = START_VALUE, .name = AX_STR, .num = AX};
const struct RegisterParameters bx = {.value = START_VALUE, .name = BX_STR, .num = BX};
const struct RegisterParameters cx = {.value = START_VALUE, .name = CX_STR, .num = CX};
const struct RegisterParameters dx = {.value = START_VALUE, .name = DX_STR, .num = DX};


static struct RegisterParameters registers [REG_AMT] = { ax,
                                                        bx,
                                                        cx,
                                                        dx };


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
    POP   = 11,
    JA    = 12,
    JAE   = 13,
    JB    = 14,
    JBE   = 15,
    JE    = 16,
    JNE   = 17,
    JMP   = 18
};

struct CommandParameters 
{
    const char*  cmd_str;
    CommandsNums cmd_num;
    size_t       arg_amt;
};

enum ArgType
{
    INT = 1 << (sizeof(int)*BITS_IN_BYTES - 1),
    REG = 1 << (sizeof(int)*BITS_IN_BYTES - 2),
    RAM = 1 << (sizeof(int)*BITS_IN_BYTES - 3),
    FREE = ~(7 << (sizeof(int)*BITS_IN_BYTES - 3)),  //снимает маску
};


const struct CommandParameters PushStr  =  {.cmd_str = "push" , .cmd_num = PUSH , .arg_amt = 2};
const struct CommandParameters AddStr   =  {.cmd_str = "add"  , .cmd_num = ADD  , .arg_amt = 0};
const struct CommandParameters SubStr   =  {.cmd_str = "sub"  , .cmd_num = SUB  , .arg_amt = 0};
const struct CommandParameters MulStr   =  {.cmd_str = "mul"  , .cmd_num = MUL  , .arg_amt = 0};
const struct CommandParameters DivStr   =  {.cmd_str = "div"  , .cmd_num = DIV  , .arg_amt = 0};
const struct CommandParameters SqrtStr  =  {.cmd_str = "sqrt" , .cmd_num = SQRT , .arg_amt = 1};
const struct CommandParameters SinStr   =  {.cmd_str = "sin"  , .cmd_num = SIN  , .arg_amt = 1};
const struct CommandParameters CosStr   =  {.cmd_str = "cos"  , .cmd_num = COS  , .arg_amt = 1};
const struct CommandParameters HltStr   =  {.cmd_str = "hlt"  , .cmd_num = HLT  , .arg_amt = 0};
const struct CommandParameters OutStr   =  {.cmd_str = "out"  , .cmd_num = OUT  , .arg_amt = 0};
const struct CommandParameters PopStr   =  {.cmd_str = "pop"  , .cmd_num = POP  , .arg_amt = 2};
const struct CommandParameters JaStr    =  {.cmd_str = "ja"   , .cmd_num = JA   , .arg_amt = 1};
const struct CommandParameters JaeStr   =  {.cmd_str = "jae"  , .cmd_num = JAE  , .arg_amt = 1};
const struct CommandParameters JbStr    =  {.cmd_str = "jb"   , .cmd_num = JB   , .arg_amt = 1};
const struct CommandParameters JbeStr   =  {.cmd_str = "jbe"  , .cmd_num = JBE  , .arg_amt = 1};
const struct CommandParameters JeStr    =  {.cmd_str = "je"   , .cmd_num = JE   , .arg_amt = 1};
const struct CommandParameters JneStr   =  {.cmd_str = "jne"  , .cmd_num = JNE  , .arg_amt = 1};
const struct CommandParameters JmpStr   =  {.cmd_str = "jmp"  , .cmd_num = JMP  , .arg_amt = 1};

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
                                                                PopStr   ,
                                                                JaStr    ,
                                                                JaeStr   ,
                                                                JbStr    ,
                                                                JbeStr   ,
                                                                JeStr    ,
                                                                JneStr   ,
                                                                JmpStr   };

#define SSCANF_CHECK   do                                  \
                        {                                  \
                            if (sscanf_check == -1)        \
                            {                              \
                                return SSCANF_ERROR;       \
                            }                              \
                        }while(0);

void assembly (Text *const input, Labels *const labels, Stack* new_buf);
Errors ctor_labels(Labels* labels);
void dtor_labels(Labels* labels);
Errors make_binary_file (Stack *const new_buf);

#endif //_ASM_H_