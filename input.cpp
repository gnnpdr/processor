#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "input.h"


static size_t find_file_size (char* name);

static void make_str(Processor* proc);
static void get_addr(Processor* proc);

void get_name(char** name, char** argv)
{
    *name = (char*)calloc(strlen(argv[1]), sizeof(char));
    LOCATION_CHECK(*name)

    strcpy(*name, argv[1]);

    return;
}

void get_file(char* name, Processor* proc)
{
    FILE* file; 
    file = fopen(name, "rb");
    FILE_CHECK(file)

    size_t size = find_file_size(name);

    char* file_buf = (char*)calloc(size, sizeof(char));
    LOCATION_CHECK(file_buf)

    fread(file_buf, sizeof(char), size, file);
    LOCATION_CHECK(file_buf)

    fclose(file);

    proc->file_buf = file_buf;
    proc->init_file_size = size;

    return;
}

size_t find_file_size (char* name)
{
    struct stat file_info;

    if (stat(name, &file_info) == -1)
    {
        printf("stat problem\n");
        return 1;
    }

    return file_info.st_size;
}

void proc_buf(Processor* proc)
{
    make_str(proc);
    get_addr(proc);
}

void make_str(Processor* proc)
{
    size_t symb_num = 0;
    size_t word_cnt = 0;

    size_t size = proc->init_file_size;
    char* buf = proc->file_buf;

    while (symb_num < size)
    {
        char* ch = buf + symb_num;

        if (*ch == ' ')
        {
            *ch = '\0';
            word_cnt++;
        }

        else if (*ch == '\r')
            *ch = '\0';
        
        else if (*ch == '\n')
        {
            *ch = '\0';
            word_cnt++;
        }

        symb_num++;
    }

    proc->input_ncmd = word_cnt;
    proc->file_buf = buf;
}

void get_addr(Processor* proc)
{
    size_t symb_num = 0;
    int string_index = 0;
    char* ch = 0;

    char* buf = proc->file_buf;
    size_t size = proc->init_file_size;

    char** addresses = (char**)calloc(proc->input_ncmd, sizeof(char*));
    LOCATION_CHECK(addresses)
    
    addresses[string_index] = buf + symb_num;

    while (symb_num < size)
    {
        ch = buf + symb_num;

        if (*ch == '\0')
        {
            string_index++;
            symb_num++;
            ch = buf + symb_num;

            if (*ch == '\0') //на случай \r
                symb_num++;

            addresses[string_index] = buf + symb_num;
        }

        symb_num++;
    }

    proc->addresses = addresses;
}