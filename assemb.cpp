#include <stdio.h>
#include <sys/stat.h>

#include "assemb.h"

StkErrors file_transformation(const char* const name, Proc* const processor, StkErrors* err)
{
    assert(name != nullptr);
    assert(processor != nullptr);

    make_init_buf (name, processor, err);
    
    processing_text(processor);  
    filling_addresses(processor);

    output_file (processor, err);

    return ALL_RIGHT;
}

StkErrors make_init_buf (const char* const  name, Proc* processor, StkErrors* err)
{
    assert(name != nullptr);
    assert(processor != nullptr);

    FILE* file; 
    file = fopen(name, "rb");
    if (file == nullptr)
    {
        printf("file wasnt opened\n");
        return FILE_PROBLEM;
    }

    struct stat buf;
    stat(name, &buf);
    size_t init_file_size = buf.st_size;

    char* file_buf = (char*)calloc(init_file_size, sizeof(char));
    if (file_buf == nullptr)
    {
        printf("no place\n");
        return NO_PLACE;
    }

    fread(file_buf, sizeof(char), init_file_size, file);
    fclose(file);

    processor->file_buf = file_buf;
    processor->init_file_size = init_file_size;

    printf("size file %d\n", processor->init_file_size);

    printf("before processing\n");
    for(int i = 0; i < processor->init_file_size; i++)
    {
        printf("%c", *(processor->file_buf + i));
        if (*(processor->file_buf + i) == '\0')
            printf("! !");
    }
    return ALL_RIGHT;
}

void processing_text(Proc* processor)
{
    size_t symbol = 0;
    size_t cnt = 0;

    while (symbol < processor->init_file_size)
    {
        char* ch = processor->file_buf + symbol;

        if (*ch == ' ')
        {
            *ch = '\0';
            cnt++;
        }
        else if (*ch == '\r')
        {
            *ch = '\0';
        }
        else if (*ch == '\n')
        {
            *ch = '\0';
            cnt++;
        }
        symbol++;
    }

    printf("after processing\n");
    for(int i = 0; i < processor->init_file_size; i++)
    {
        printf("%c", *(processor->file_buf + i));
        if (*(processor->file_buf + i) == '\0')
            printf("! !");
    }

    processor->input_file_commands_amount = cnt;
    printf("amount %d\n", cnt);
}

void filling_addresses(Proc* processor)
{ 
    processor->addresses = (char**)calloc(processor->input_file_commands_amount, sizeof(char*));
    
    size_t element = 0;
    int index_of_first_element_in_string = 0;

    processor->addresses[index_of_first_element_in_string] = processor->file_buf + element;

    while (element < processor->init_file_size)
    {
        char* ch = processor->file_buf + element;

        if (*ch == '\0')
        {
            index_of_first_element_in_string++;
            
            element++;
            ch = processor->file_buf + element;

            if (*ch == '\0') //на случай \r
                element++;

            processor->addresses[index_of_first_element_in_string] = processor->file_buf + element;
            
            printf("add %d\n pointer %p\n first symb %c\n", index_of_first_element_in_string, processor->addresses[index_of_first_element_in_string], *(processor->file_buf + element));
        }
        element++;
    }
}
/*
StkErrors processing_text(Proc* processor, StkErrors* err)
{
    assert(processor != nullptr);

    char* text = 0;
    size_t size = 0;

    size_t* symb_amount_arr = (size_t*)calloc(10 MAX_INPUT_CMD_AMT, sizeof(size_t));
    if (symb_amount_arr == nullptr)
    {
        printf("no place\n");
        return NO_PLACE;
    }

    text = processor->file_buf;
    size = processor->init_file_size;

    size_t symbol = 0;
    size_t space_cnt = 0;
    size_t str_cnt = 0;

    char* ch = text + symbol;
    size_t i = 0;
    symb_amount_arr[i] = i;

    while (symbol < size)
    {
        ch = text + symbol;

        //if (ch == '\r')
        //    ch = '\0';

        if (*ch == '\n')
        {
            *ch = '\0';
            i++;
            str_cnt++;

            symb_amount_arr[i] = i;
            printf("write %d arr %d \n", i, symb_amount_arr[i]);

            
        }
        else if (*ch == ' ')
        {
            *ch = '\0';   //здесь тоже надо, команды ведь как строки будут анализироваться
            i++;
            space_cnt++;


            symb_amount_arr[i] = i;
            printf("write %d arr %d \n", i, symb_amount_arr[i]);
            
        }
        else
            i++;
        
        symbol++;
    }

    space_cnt = space_cnt + str_cnt;
    processor->input_file_commands_amount = space_cnt;

    

    symb_amount_arr = (size_t*)realloc(symb_amount_arr, space_cnt*sizeof(size_t));
    if (symb_amount_arr == nullptr)
    {
        printf("no place\n");
        return NO_PLACE;
    }

    printf("amount of chars\n");
    for(size_t j = 0; j < space_cnt; j++)
        printf("%d\n", symb_amount_arr[j]); 


    processor->file_buf = text;
    processor->symb_amount_arr = symb_amount_arr;
    printf("words amt %d\n", space_cnt);

    return ALL_RIGHT;
}
*/
StkErrors output_file(Proc* const processor, StkErrors* err)
{
    assert(processor != nullptr);

    int* new_file_buf = processor->new_file_buf;
    size_t input_file_commands_amount = processor->input_file_commands_amount;

    new_file_buf = (int*)calloc(input_file_commands_amount, sizeof(int));
    if (new_file_buf == nullptr)
    {
        printf("no place\n");
        return NO_PLACE;
    }

    compile(processor, new_file_buf, err);

    FILE* output_file;
    output_file = fopen("out.txt", "a");
    if (output_file == nullptr)
    {
        printf("file wasnt opened\n");
        return FILE_PROBLEM;
    }
    
    for (int i = 0; i < input_file_commands_amount; i++)
        fprintf(output_file, "%d ", new_file_buf[i]);

    //fwrite(, sizeof(int), , );
    
    fclose(output_file);

    processor->new_file_buf = new_file_buf;

    return ALL_RIGHT;
}

StkErrors compile(Proc* const processor, int* const new_file_buf, StkErrors* err)
{
    assert(new_file_buf != nullptr);
    assert(processor != nullptr);

    size_t word_num = 0;
    char str [80] = "";
    int arg = 0;
    //size_t* symb_amount_arr = processor->symb_amount_arr;
    size_t input_file_commands_amount = processor->input_file_commands_amount;
    size_t size = processor->init_file_size;
    int i = 0;
    char* file_buf = processor->addresses[i];
    

    /*printf("in compile\n");
    printf("symb amt %d\n", size);
    for(int i = 0; i < size; i++)
        printf("%c", *(file_buf + i)); 
    */
    //показывает, что все хорошо, все на месте

    for (size_t i = 0; i < input_file_commands_amount; i++)
    {
        LabelType label_place = START;
        LabelRes is_label = NOT_LABEL;

        file_buf = processor->addresses[i];
        //printf("i %d\n", i);
        printf("file buf %p\n", file_buf);
        //check_label(processor, file_buf, &word_num, label_place, &is_label, &arg);
        sscanf(file_buf, "%s", str);
        printf("str  %s\n", str);


        //if (is_label == LABEL);
        //    continue;  
            
        for (size_t cmd = 0; cmd < CMD_AMT; cmd++)
        {
            if (strcmp(str, bunch_of_commands[cmd].com_str) == 0)
            {
                printf("cmd %s\n", bunch_of_commands[cmd].com_str);
                printf("cmd num %d\n", bunch_of_commands[cmd].com_num);
                new_file_buf[i] = bunch_of_commands[cmd].com_num;
                printf("i %d\n", i);
                
                for (size_t j = 0; j < bunch_of_commands[cmd].arg_amt; j++) 
                {
                    /*label_place = ARG;
                    check_label(processor, file_buf, &word_num, label_place, &is_label, &arg);
                    if (is_label = LABEL)
                    {
                        new_file_buf[i] = arg;
                    }
                    else
                    {
                        SSCANF(file_buf, "%d", &arg)
                        new_file_buf[i] = arg;
                    }*/
                    i++;
                    file_buf = processor->addresses[i];
                    printf("file buf %p\n", file_buf);

                    sscanf(file_buf, "%d", &arg);
                    printf("arg %d\n", arg);

                    new_file_buf[i] = arg;
                }
                break;
            }
        }
    }

    printf("our new buffer\n");
    for (int m = 0; m < processor->input_file_commands_amount; m++)
        printf("%d ", new_file_buf[m]);

    return ALL_RIGHT;
}

LabelRes check_label(Proc* processor, char* file_buf, size_t* word_num, LabelType label_place, LabelRes* is_label, int* arg)
{ 
    char* str = 0;
    LabelParameters* labels = processor->labels;
    size_t* symb_amount_arr = processor->symb_amount_arr;
    size_t num = *word_num;

    sscanf(file_buf, "%s", str);  //надо прочитать макросом, чтобы сразу заменить аргумент при случае

    bool word_check = false;

    printf("pivo 1\n");
    char ch = 0;
    int i = 0;
    while (ch != '\0')
    {
        ch = str[i];
        if (ch == ':')
        {
            word_check = true;
            break;
        }   
        i++;
    }

    if (word_check == true)
    {
        printf("pivo 2\n");
        if (label_place = START)
        {
            for (size_t i = 0; i < LABELS_AMT; i++)
            {
                if (labels[i].j_pointer == -1 && labels[i].label_word == 0)
                {
                    labels[i].j_pointer = num;
                    labels[i].label_word = str;

                    *word_num = num;
                    file_buf = file_buf + symb_amount_arr[*word_num];
                    processor->labels = labels;

                    *is_label = LABEL;
                    return LABEL;
                }

                else if (labels[i].j_pointer == -1 && labels[i].label_word != 0)
                {
                    if (strcmp(labels[i].label_word, str) == 0)
                    {
                        labels[i].j_pointer = num;

                        *word_num = num;
                        *is_label = LABEL;
                        return LABEL;
                    }
                }

                if (i == LABELS_AMT - 1)
                {
                    printf ("no place for new label :(\n");
                    return PLACE_PROBLEM;
                }
            }
        }

        else if (label_place = ARG)
        {
            for (size_t i = 0; i < LABELS_AMT; i++)
            {
                if (strcmp(labels[i].label_word, str) == 0)
                {
                    *arg = labels[i].j_pointer;

                    *word_num = num;
                    *is_label = LABEL;
                    return LABEL;
                }
                else if (labels[i].j_pointer == -1)
                {
                    labels[i].label_word = str;
                }

                if (i == LABELS_AMT)
                {
                    printf("there is no such label\n");
                    return SYN_PROBLEM;
                }
            }
        }
    }
    else
    {
        printf("pivo 3\n");
        return NOT_LABEL;
    }
}

void free_bufs (Proc* processor)
{
    assert(processor != 0);

    free(processor->file_buf);
    free(processor->new_file_buf);
    free(processor->symb_amount_arr);
}



