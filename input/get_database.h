#ifndef _BASE_H_
#define _BASE_H_ 

#include "..\errors\errors.h"

#include <sys/stat.h>
#include <string.h>

static const size_t MAX_FILE_SIZE = 10000;
static const size_t MAX_STR_LEN   = 100;

struct Input 
{
    char* name;
    char* text;
    size_t size;
};

void input_ctor (Input *const base_text, ErrList *const list);
void input_dtor(Input* base_text);
void fill_input(Input *const base_text, char *const input_name, ErrList *const list);

void count_file_size(const char *const name, size_t* size, ErrList *const list);

#endif //_BASE_H_