#ifndef _INPUT_H_
#define _INPUT_H_
#include "stack_operations.h"

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
    LabelParameters labels[LABELS_AMT];
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
    char* file_buf;
    size_t init_file_size;
    size_t input_ncmd;
    
    int* new_file_buf;

    size_t label_delta;
    struct Labels labels;

    struct RegisterParameters registers[REG_AMT];
    
    size_t ip;
    char** addresses;

    size_t skip_arg;
    int RAM[RAM_AMT];
};

#define LOCATION_CHECK(buf) do                                      \
                            {                                       \
                                if (buf == 0)                       \
                                {                                   \
                                    printf("no place\n");           \
                                    return;                         \
                                }                                   \
                            }while(0);

#define FILE_CHECK(file)    do                                      \
                            {                                       \
                                if (file == nullptr)                \
                                {                                   \
                                    printf("file wasn't opened\n"); \
                                    return;                         \
                                }                                   \
                            }while(0);


void get_name(char** name, char** argv);
void get_file(char* name, Processor* proc);
void proc_buf(Processor* proc);

#endif //_INPUT_H_