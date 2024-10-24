#ifndef _COMPILE_H_
#define _COMPILE_H_

#include "check_proc.h"


static const size_t RAM_AMT           = 25;
static const size_t CMD_AMT           = 15;
static const size_t MAX_INPUT_CMD_AMT = 60;
static const size_t LABELS_AMT        = 10;
static const size_t REG_AMT           =  5;

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
    JUMP  = 12,
    POP   = 13,
    POPR  = 14,
    PUSHR = 15
};

struct CommandParameters 
{
    const char*  cmd_str;
    CommandsNums cmd_num;
    size_t       arg_amt;
};


const struct CommandParameters PushStr  =  {.cmd_str = "push" , .cmd_num = PUSH , .arg_amt = 3};  //максимальное значение количества агрументов
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
const struct CommandParameters JumpStr  =  {.cmd_str = "jump" , .cmd_num = JUMP , .arg_amt = 2};
const struct CommandParameters PopStr   =  {.cmd_str = "pop"  , .cmd_num = POP  , .arg_amt = 1};
const struct CommandParameters PushrStr =  {.cmd_str = "pushr", .cmd_num = PUSHR, .arg_amt = 1};
const struct CommandParameters PoprStr  =  {.cmd_str = "popr" , .cmd_num = POPR , .arg_amt = 1};

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
                                                                        JumpStr  ,
                                                                        PopStr   ,
                                                                        PushrStr ,
                                                                        PoprStr };

struct RegisterParameters
{
    int value;
    char* name;
};

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
    LabelParameters labels[LABELS_AMT];
    LabelType label_type;
    bool is_label;
    size_t label_delta;
};

struct Proc
{
    char* file_buf;
    int* new_file_buf;
    size_t input_file_commands_amount;
    size_t init_file_size;
    
    struct RegisterParameters* registers;

    size_t ip;
    size_t* symb_amount_arr;
    char** addresses;
    struct Labels labels;

    char RAM[RAM_AMT];
    size_t skip_arg;
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
                                    *(cmd_num)++;                                    \
                                    file_buf = processor->addresses[*cmd_num];       \
                                    sscanf (file_buf, __VA_ARGS__);                  \
                                }while(0);


Errors proc (const char* const name, Proc* const processor, Errors* err);

Errors init_buf (const char* const  name, Proc* processor, Errors* err);
size_t find_file_size (const char* const name);

void processing_text (Proc* const processor);
Errors filling_addresses(Proc* processor);

Errors output_file(Proc* processor, Errors* err);
Errors assembly (Proc* const processor, Errors* err);

void arg_analysis (Proc* processor, int* arg1, int* arg2, size_t* arg_value, size_t* cmd_num);
void plus_case(Proc* processor, int* arg2, size_t* cmd_num, size_t* arg_value);

void make_labels(Proc* processor);
void make_regs(Proc* processor);
Errors check_label(Proc* processor, char* str, size_t cmd_num, int* arg);
bool find_label_mark (const char* const str);

void proc_dtor (Proc* processor);

void print_labels (LabelParameters* labels);

void RAM_filling (Proc* processor);
void draw_RAM (Proc* processor);

#endif //_COMPILE_H_