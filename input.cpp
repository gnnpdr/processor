#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "input.h"


Errors get_name(Text *const input, char **const argv)
{
    assert(input);
    assert(argv);

    char* name = (char*)calloc(strlen(argv[1]), sizeof(char));
    ALLOCATION_CHECK(*name)
    
    strncpy(name, argv[1], strlen(argv[1]) + 1);
    input->name = name;

    return ALL_RIGHT;
}


Errors get_file_data(Text *const input)  //может, можно унифицировать эту функцию?
{
    assert(input);

    FILE* file; 
    file = fopen(input->name, "rb");
    FILE_CHECK(file)
    //printf("wow 1\n");
    size_t size = find_file_size(input->name);

    char* file_buf = (char*)calloc(size, sizeof(char));
    ALLOCATION_CHECK(file_buf)
    //printf("wow 2\n");
    size_t read_result = fread(file_buf, sizeof(char), size, file);
    if (read_result != size)
    {
        fputs("Ошибка чтения", stderr);
        return READ_ERROR;
    }
    //printf("wow 3\n");
    if(fclose(file) != 0)
        return CLOSE_ERROR;

    //printf("wow 4\n");
    input->file_buf = file_buf;
    input->init_file_size = size;

    return ALL_RIGHT;
}


size_t find_file_size (const char *const name)
{
    assert(name);

    struct stat file_info;

    if (stat(name, &file_info) == -1)
    {
        printf("stat problem\n");
        return 1;
    }

    return file_info.st_size;
}


void remove_carriage(Text *const input)
{
    size_t symb_num = 0;
    size_t word_cnt = 0;

    size_t size = input->init_file_size;
    char* buf = input->file_buf;
    char** addresses = input->addresses;

    addresses[word_cnt] = buf + symb_num;

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

        else if (*ch == COMMENT_MARK)
        {
            do
                symb_num++;
            while (*ch != '\n');
        }
        
        else if (*ch == '\n')
        {
            *ch = '\0';
            word_cnt++;
            addresses[word_cnt] = buf + symb_num + 1;
        }

        symb_num++;
    }

    for (size_t i = 0; i < word_cnt; i++)
        input->addresses[i] = addresses[i]; 

    input->file_buf = buf;
    input->cmd_amt = word_cnt;
}

void input_dtor (Text* input)
{
    assert(input);

    free(input->name);
    free(input->file_buf);
}