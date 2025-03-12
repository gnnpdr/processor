#ifndef _DATA_H_
#define _DATA_H_

//-----------------------COMMANDS---------------------------------------------------------------------------

static const size_t CMD_AMT  =  18;
static const size_t COMPL_ARG_AMT = 2;

enum ArgType
{
    ARG_NUM, 
    ARG_LAB,
    ARG_RAM,
    ARG_FUNC,
    ARG_REG
};

enum CommandsNums
{
    PUSH_A  = 1 ,
    ADD_A   = 2 ,
    SUB_A   = 3 ,
    MUL_A   = 4 ,
    DIV_A   = 5 ,
    OUT_A   = 6 ,
    HLT_A   = 7 ,
    POP_A   = 8 ,
    JA_A    = 9 ,
    JAE_A   = 10,
    JB_A    = 11,
    JBE_A   = 12,
    JE_A    = 13,
    JNE_A   = 14,
    JMP_A   = 15,
    CALL_A  = 16,
    RET_A   = 17,
    SQRT_A  = 18
};

static const char* POP_STR  =  "pop";
static const char* PUSH_STR = "push";

static const char* JA_STR = "ja";
static const char* JE_STR = "je";
static const char* JB_STR = "jb";

static const char* JAE_STR = "jae";
static const char* JNE_STR = "jne";
static const char* JBE_STR = "jbe";
static const char* JMP_STR = "jmp";

static const char* RET_STR = "ret";
static const char* HLT_STR = "hlt";

static const char* CALL_STR = "call";

static const char* SQRT_STR = "sqrt";

struct CommandParameters 
{
    const char*  cmd_str;
    CommandsNums cmd_num;
    bool         has_args;
};

const struct CommandParameters PushStr  =  {"push" , PUSH_A , true};
const struct CommandParameters AddStr   =  {"add"  , ADD_A  , false};
const struct CommandParameters SubStr   =  {"sub"  , SUB_A  , false};
const struct CommandParameters MulStr   =  {"mul"  , MUL_A  , false};
const struct CommandParameters DivStr   =  {"div"  , DIV_A  , false};
const struct CommandParameters HltStr   =  {"hlt"  , HLT_A  , false};
const struct CommandParameters OutStr   =  {"out"  , OUT_A  , false};
const struct CommandParameters PopStr   =  {"pop"  , POP_A  , true};
const struct CommandParameters JaStr    =  {"ja"   , JA_A   , true};
const struct CommandParameters JaeStr   =  {"jae"  , JAE_A  , true};
const struct CommandParameters JbStr    =  {"jb"   , JB_A   , true};
const struct CommandParameters JbeStr   =  {"jbe"  , JBE_A  , true};
const struct CommandParameters JeStr    =  {"je"   , JE_A   , true};
const struct CommandParameters JneStr   =  {"jne"  , JNE_A  , true};
const struct CommandParameters JmpStr   =  {"jmp"  , JMP_A  , true};
const struct CommandParameters CallStr  =  {"call" , CALL_A , true};
const struct CommandParameters RetStr   =  {"ret"  , RET_A  , false};
const struct CommandParameters SqrtStr  =  {"sqrt" , SQRT_A , false};

static const CommandParameters bunch_of_commands [CMD_AMT]  =   {PushStr,
                                                                AddStr   ,
                                                                SubStr   ,
                                                                MulStr   ,
                                                                DivStr   ,
                                                                HltStr   ,
                                                                OutStr   ,
                                                                PopStr   ,
                                                                JaStr    ,
                                                                JaeStr   ,
                                                                JbStr    ,
                                                                JbeStr   ,
                                                                JeStr    ,
                                                                JneStr   ,
                                                                JmpStr   ,
                                                                CallStr  ,
                                                                RetStr   ,
                                                                SqrtStr};


                  
static const CommandsNums compl_cmds[COMPL_ARG_AMT] = {PUSH_A,
                                                       POP_A};

//----------------------------------LABELS---------------------------------------------------------------------

static const char LABEL_MARK = ':';

static const size_t LABELS_AMT    = 10;
static const size_t LABEL_ARG_AMT = 5;

struct LabelParameters
{
    char* start_word;
    size_t len;

    size_t cmd_target;
    size_t* arg_target;
};

//--------------------------------FUNCS-------------------------------------

static const size_t FUNCS_AMT = 10;

struct FuncParameters
{
    char* start_word;
    size_t len;

    size_t cmd_target;
    size_t call_target;

    size_t ret_word;
};

//--------------------------------REGISTERS---------------------------------------------------------------

static const size_t REG_AMT = 4;

enum Registers
{
    AX = 0,
    BX = 1,
    CX = 2,
    DX = 3
};

struct RegisterParameters
{
    const char* name;
    Registers reg_num;
};

static const char* AX_STR = "ax";
static const char* BX_STR = "bx";
static const char* CX_STR = "cx";
static const char* DX_STR = "dx";

static struct RegisterParameters ax = {AX_STR, AX};
static struct RegisterParameters bx = {BX_STR, BX};
static struct RegisterParameters cx = {CX_STR, CX};
static struct RegisterParameters dx = {DX_STR, DX};

static struct RegisterParameters* registers[REG_AMT] = {&ax,
                                                        &bx,
                                                        &cx,
                                                        &dx};


//--------------------------------ARGS----------------------------------------------------------------

static const char* PLUS    = "+";
static const char RAM_MARK = '[';

#endif //_DATA_H_