#ifndef _COMPILE_H_
#define _COMPILE_H_

#include "check_proc.h"

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
    const char*  cmd_str;
    CommandsNums cmd_num;
    size_t       arg_amt;
};


const struct CommandParameters PushStr  =  {.cmd_str = "push" , .cmd_num = PUSH , .arg_amt = 1};
const struct CommandParameters AddStr   =  {.cmd_str = "add"  , .cmd_num = ADD  , .arg_amt = 0};
const struct CommandParameters SubStr   =  {.cmd_str = "sub"  , .cmd_num = SUB  , .arg_amt = 0};
const struct CommandParameters MulStr   =  {.cmd_str = "mul"  , .cmd_num = MUL  , .arg_amt = 0};
const struct CommandParameters DivStr   =  {.cmd_str = "div"  , .cmd_num = DIV  , .arg_amt = 0};
const struct CommandParameters SqrtStr  =  {.cmd_str = "sqrt" , .cmd_num = SQRT , .arg_amt = 1};
const struct CommandParameters SinStr   =  {.cmd_str = "sin"  , .cmd_num = SIN  , .arg_amt = 1};
const struct CommandParameters CosStr   =  {.cmd_str = "cos"  , .cmd_num = COS  , .arg_amt = 1};
const struct CommandParameters DumpStr  =  {.cmd_str = "dump" , .cmd_num = DUMP , .arg_amt = 0};
const struct CommandParameters HltStr   =  {.cmd_str = "hlt"  , .cmd_num = HLT  , .arg_amt = 0};
const struct CommandParameters OutStr   =  {.cmd_str = "out"  , .cmd_num = OUT  , .arg_amt = 1};
const struct CommandParameters JaStr    =  {.cmd_str = "ja"   , .cmd_num = JA   , .arg_amt = 2};
const struct CommandParameters JaeStr   =  {.cmd_str = "jae"  , .cmd_num = JAE  , .arg_amt = 2};
const struct CommandParameters JeStr    =  {.cmd_str = "je"   , .cmd_num = JE   , .arg_amt = 2};
const struct CommandParameters JneStr   =  {.cmd_str = "jne"  , .cmd_num = JNE  , .arg_amt = 2};
const struct CommandParameters PopStr   =  {.cmd_str = "pop"  , .cmd_num = POP  , .arg_amt = 1};
const struct CommandParameters PushrStr =  {.cmd_str = "pushr", .cmd_num = PUSHR, .arg_amt = 0};
const struct CommandParameters PoprStr  =  {.cmd_str = "popr" , .cmd_num = POPR , .arg_amt = 0};

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


enum LabelType
{
    DEFINE,
    ARG
};


struct LabelParameters
{
    size_t target;
    char* name;
};

struct Labels
{
    LabelParameters* labels;
    LabelType label_type;
    bool is_label;
};

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
    struct Labels labels;
};






#define FILE_CHECK(file)    do                                      \
                            {                                       \
                                if (file == nullptr)                \
                                {                                   \
                                    printf("file wasnt opened\n");  \
                                    return FILE_PROBLEM;            \
                                }                                   \
                            }while(0);

#define CAPACITY_CHECK(buf) do                                      \
                            {                                       \
                                if (buf == nullptr)                 \
                                {                                   \
                                    printf("no place\n");           \
                                    return LOCATION_ERROR;          \
                                }                                   \
                            }while(0);



#define SSCANF(file_buf, ...)   do                                                   \
                                {                                                    \
                                    sscanf (file_buf, __VA_ARGS__);                  \
                                    cmd_num++;                                       \
                                    file_buf = processor->addresses[cmd_num];        \
                                }while(0);

Errors proc (const char* const name, Proc* const processor, Errors* err);

Errors init_buf (const char* const  name, Proc* processor, Errors* err);
size_t find_file_size (const char* const name);

void processing_text (Proc* const processor);
Errors filling_addresses(Proc* processor);

Errors output_file(Proc* const processor, Errors* err);
Errors assemb (Proc* const processor, Errors* err);

void make_labels(Proc* processor);
Errors check_label(Proc* processor, char* str, size_t cmd_num, int* arg);
bool find_label_mark (const char* const str);

void proc_dtor (Proc* processor);

#endif //_COMPILE_H_