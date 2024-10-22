#ifndef _DUMP_H_
#define _DUMP_H_

#include "stack.h"

void print_init_buf (char* file_buf, size_t init_file_size);

void print_addresses (char** addresses, size_t size);

void print_output_buf (int* file_buf, size_t size, size_t cmd_num, char* str);

void print_scanf_res (char* file_buf, char* str);



#endif //_DUMP_H_