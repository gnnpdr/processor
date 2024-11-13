#ifndef _INPUT_H_
#define _INPUT_H_
#include "stack.h"
#include <assert.h>

static const size_t MAX_CMD_AMT = 500;

static const char COMMENT_MARK = ';';

struct Text 
{
    char* name;

    char* file_buf;

    size_t init_file_size;

    char* addresses[MAX_CMD_AMT];

    size_t cmd_amt;
    size_t cmd_num;
};

#define ALLOCATION_CHECK(buf) do                                    \
                            {                                       \
                                if (&buf == 0)                      \
                                {                                   \
                                    printf("no place\n");           \
                                    return ALLOCATION_ERROR;        \
                                }                                   \
                            }while(0);

#define FILE_CHECK(file)    do                                      \
                            {                                       \
                                if (file == nullptr)                \
                                {                                   \
                                    printf("file wasn't opened\n"); \
                                    return FILE_PROBLEM;            \
                                }                                   \
                            }while(0);


Errors get_name(Text *const input, char **const argv);
Errors get_file_data(Text *const input);
size_t find_file_size (const char *const name);
void remove_carriage(Text *const input);
void input_dtor (Text* input);

#endif //_INPUT_H_