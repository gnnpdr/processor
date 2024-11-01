#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "input.h"

void get_name(Text* input, char** argv)
{
    char* name = (char*)calloc(strlen(argv[1]), sizeof(char));
    ALOCATION_CHECK(*name)
    
    strcpy(name, argv[1]);
    input->name = name;

    return;
}

void get_file_data(Text* input)  //может, можно унифицировать эту функцию?
{
    FILE* file; 
    file = fopen(input->name, "rb");
    FILE_CHECK(file)

    size_t size = find_file_size(input->name);

    char* file_buf = (char*)calloc(size, sizeof(char));
    ALOCATION_CHECK(file_buf)

    fread(file_buf, sizeof(char), size, file);
    ALOCATION_CHECK(file_buf)

    fclose(file);

    input->file_buf = file_buf;
    input->init_file_size = size;

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

void remove_carriage(Text* input)
{
    size_t symb_num = 0;
    size_t word_cnt = 0;

    size_t size = input->init_file_size;
    char* buf = input->file_buf;
    char** addresses = input->addresses;

    addresses[word_cnt] = buf + symb_num;

    int str = 0;

    while (symb_num < size)
    {
        char* ch = buf + symb_num;

        if (*ch == ' ')
        {
            *ch = '\0';
            word_cnt++;
            addresses[word_cnt] = buf + symb_num + 1;
        }

        else if (*ch == '\r')
            *ch = '\0';
        
        else if (*ch == '\n')
        {
            *ch = '\0';
            word_cnt++;
            addresses[word_cnt] = buf + symb_num + 1;
        }

        symb_num++;
    }

    for (int i = 0; i < word_cnt; i++)
        input->addresses[i] = addresses[i]; 

    input->file_buf = buf;
    input->cmd_amt = word_cnt;
}

void input_dtor (Text* input)
{
    free(input->name);
    free(input->file_buf);
}