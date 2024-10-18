#include <stdio.h>
#include <sys/stat.h>

#include "assemb.h"

StkErrors file_transformation(const char* const name, Proc* const processor, StkErrors* err)
{
    assert(name != nullptr);
    assert(processor != nullptr);

    make_init_buf (name, processor, err);
    
    processing_text(processor, err);  

    output_file (processor, err);

    return ALL_RIGHT;
}

StkErrors make_init_buf (const char* const  name, Proc* processor, StkErrors* err)
{
    assert(name != nullptr);
    assert(processor != nullptr);

    FILE* file; 
    file = fopen(name, "r");
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
    
    for(int i = 0; i < init_file_size; i++)
        printf("%c", *(file_buf + i));

    processor->file_buf = file_buf;
    processor->init_file_size = init_file_size;

    return ALL_RIGHT;
}

StkErrors processing_text(Proc* processor, StkErrors* err)
{
    assert(processor != nullptr);

    char* text = 0;
    size_t size = 0;

    size_t* symb_amount_arr = (size_t*)calloc(MAX_INPUT_CMD_AMT, sizeof(size_t));
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
        }
        else if (*ch == ' ')
        {
            *ch = '\0';   //здесь тоже надо, команды ведь как строки будут анализироваться
            i++;
            space_cnt++;
            symb_amount_arr[i] = i;
        }
        else
            i++;
        
        symbol++;
    }

    space_cnt = space_cnt + str_cnt;
    processor->input_file_commands_amount = space_cnt;

    printf("amount of chars\n");
    for(int j = 0; j < i; j++)
        printf("%d", symb_amount_arr[j]); 

    symb_amount_arr = (size_t*)realloc(symb_amount_arr, space_cnt*sizeof(size_t));
    if (symb_amount_arr == nullptr)
    {
        printf("no place\n");
        return NO_PLACE;
    }
     
    processor->file_buf = text;
    processor->symb_amount_arr = symb_amount_arr;
    printf("words amt %d\n", space_cnt);

    return ALL_RIGHT;
}

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
    
    fwrite(new_file_buf, sizeof(int), input_file_commands_amount, output_file);
    
    fclose(output_file);

    processor->new_file_buf = new_file_buf;

    return ALL_RIGHT;
}

StkErrors compile(Proc* const processor, int* const new_file_buf, StkErrors* err)
{
    assert(new_file_buf != nullptr);
    assert(processor != nullptr);

    

    size_t word_num = 0;
    char* str = 0;
    int arg = 0;
    size_t* symb_amount_arr = processor->symb_amount_arr;
    size_t input_file_commands_amount = processor->input_file_commands_amount;
    size_t size = processor->init_file_size;
    char* file_buf = processor->file_buf + symb_amount_arr[word_num];
    

    printf("in compile\n");
    printf("symb amt %d\n", size);
    for(int i = 0; i < size; i++)
        printf("%c", *(file_buf + i)); 

    //показывает, что все хорошо, все на месте

    for (size_t i = 0; i < input_file_commands_amount; i++)
    {
        LabelType label_place = START;
        LabelRes is_label = NOT_LABEL;

        SSCANF(file_buf, "%s", str)        //вот здесь проблема, в строчку ничего не записывается
        //printf("str 1 %s\n", str);

        check_label(processor, file_buf, &word_num, label_place, &is_label, &arg);

        if (is_label == LABEL);
            continue;   //так же норм? не обязательно ставить else?
        
        for (size_t cmd = 0; cmd < CMD_AMT; cmd++)
        {
            if (strcmp(str, bunch_of_commands[cmd].com_str) == 0)
            {
                new_file_buf[i] = bunch_of_commands[cmd].com_num;
                i++;

                for (size_t j = 0; j < bunch_of_commands[cmd].arg_amt; j++) 
                {
                    label_place = ARG;
                    check_label(processor, file_buf, &word_num, label_place, &is_label, &arg);
                    if (is_label = LABEL)
                    {
                        new_file_buf[i] = arg;
                    }
                    else
                    {
                        SSCANF(file_buf, "%d", &arg)
                        new_file_buf[i] = arg;
                    }
                    i++;
                }
            }
            printf("processed\n");
        }
    }
    return ALL_RIGHT;
}

LabelRes check_label(Proc* processor, char* file_buf, size_t* word_num, LabelType label_place, LabelRes* is_label, int* arg)
{ 
    char* str = 0;
    LabelParameters* labels = processor->labels;
    size_t* symb_amount_arr = processor->symb_amount_arr;
    size_t num = *word_num;

    sscanf(file_buf, "%s", &str);  //надо прочитать макросом, чтобы сразу заменить аргумент при случае

    if (str[symb_amount_arr[num + 1] - symb_amount_arr[num] - 1] == ':')
    {
        if (label_place = START)
        {
            for (size_t i = 0; i < LABELS_AMT; i++)
            {
                if (labels[i].j_pointer == -1)
                {
                    labels[i].j_pointer = num;
                    labels[i].label_word = str;

                    *word_num = num;
                    file_buf = file_buf + symb_amount_arr[*word_num];
                    processor->labels = labels;

                    *is_label = LABEL;
                    return LABEL;
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

                if (i == LABELS_AMT)
                {
                    printf("there is no such label\n");
                    return SYN_PROBLEM;
                }
            }
        }
    }
}

void free_bufs (Proc* processor)
{
    assert(processor != 0);

    free(processor->file_buf);
    free(processor->new_file_buf);
    free(processor->symb_amount_arr);
}