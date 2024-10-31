#ifndef _INPUT_H_
#define _INPUT_H_
#include "stack.h"

static const size_t MAX_CMD_AMT = 100;

struct Text 
{
    char* name;
    char* file_buf;
    size_t init_file_size;
    char* addresses[MAX_CMD_AMT];
    size_t cmd_amt;

    size_t cmd_num;
};

#define ALOCATION_CHECK(buf) do                                      \
                            {                                       \
                                if (&buf == 0)                       \
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


void get_name(Text* input, char** argv);
void get_file(Text* input);
size_t find_file_size (char* name);
void remove_carriage(Text* input);
void input_dtor (Text* input);

#endif //_INPUT_H_