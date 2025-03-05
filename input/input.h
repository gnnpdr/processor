#ifndef _INPUT_H_
#define _INPUT_H_

#include <ctype.h>

#include "get_database.h"

enum WordType
{
    CMD, 
    ARG,
    LABEL_ARG,
    LABEL_CMD
};

struct Word
{
    char* word_start;
    size_t len;
    size_t str_num; 
    WordType type;
};

static const size_t FILE_CMD_AMT = 300;

Word* word_list_ctor(ErrList *const list);
void word_list_dtor(Word *const words);

void get_code(Input *const asm_text, Word *const words, ErrList *const list);
void get_bin_code(Input *const base_text, int* bin_code, size_t *const size, ErrList *const list);
void fill_input_file(const char* const  input_file_name, const char* const input_file_data, ErrList *const list);

#endif //_INPUT_H_