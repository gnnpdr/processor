#include <stdio.h>

#include "check_proc.h"

void print_init_buf (char* file_buf, size_t init_file_size)
{
    printf("size: %d\n", init_file_size);
    printf("address: %p\n", file_buf);
    printf("text:\n");

    for (size_t i = 0; i < init_file_size; i++)
        printf("%c", file_buf[i]);
        
    printf("\n");
}

void print_addresses (char** addresses, size_t size)
{
    printf("size: %d\n", size);
    printf("addresses:\n");

    for (size_t i = 0; i < size; i++)
        printf("%p\n", addresses[i]);

}

void print_output_buf (int* file_buf, size_t size, size_t cmd_num, char* str)
{
    printf("size: %d\n", size);
    printf("address: %p\n", file_buf);
    printf("current str: %s\n", str);
    printf("current cmd: %d\n", cmd_num);
    printf("text:\n");

    for (size_t i = 0; i < size; i++)
        printf("%d", file_buf[i]);

    printf("\n");
}

void print_scanf_res (char* file_buf, char* str)
{
    printf("str address: %p\n", file_buf);
    printf("str: %s\n", str);
}
