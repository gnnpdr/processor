#include <stdio.h>
#include <sys/stat.h>

#include "asm.h"

Errors proc (const char* const name, Proc* const processor, Errors* err)
{
    assert(name != nullptr);
    assert(processor != nullptr);

    //printf("hello 1\n");
    init_buf(name, processor, err);
    //printf("hello 2\n");
    processing_text(processor);  
    //printf("hello 3\n");
    filling_addresses(processor);
    //printf("hello 4\n");
    output_file(processor, err);
    //printf("hello 5\n");

    return ALL_RIGHT;
}

Errors init_buf (const char* const  name, Proc* processor, Errors* err)
{
    assert(name != nullptr);
    assert(processor != nullptr);

    FILE* file; 
    file = fopen(name, "rb");
    FILE_CHECK(file)

    size_t init_file_size = find_file_size(name);

    char* file_buf = (char*)calloc(init_file_size, sizeof(char));
    CAPACITY_CHECK(file_buf)

    fread(file_buf, sizeof(char), init_file_size, file);
    CAPACITY_CHECK(file_buf)

    fclose(file);

    processor->file_buf = file_buf;
    processor->init_file_size = init_file_size;

    return ALL_RIGHT;
}

size_t find_file_size (const char* const name)
{
    struct stat file_info;
    stat(name, &file_info);

    return file_info.st_size;
}

void processing_text (Proc* const processor)
{
    size_t symb_num = 0;
    size_t word_cnt = 0;
    size_t str_cnt = 0;
    size_t size = processor->init_file_size;
    char* buf = processor->file_buf;

    while (symb_num < size)
    {
        char* ch = buf + symb_num;

        if (*ch == ' ')
        {
            *ch = '\0';
            word_cnt++;
        }
        else if (*ch == '\r')
        {
            *ch = '\0';
        }
        else if (*ch == '\n')
        {
            *ch = '\0';
            word_cnt++;
            str_cnt++;
        }

        symb_num++;
    }

    processor->input_file_commands_amount = word_cnt;
    processor->file_buf = buf;
}

Errors filling_addresses(Proc* processor)
{ 

    size_t symb_num = 0;
    int index_of_first_element_in_string = 0;
    char* ch = 0;
    char* buf = processor->file_buf;
    size_t size = processor->init_file_size;

    char** addresses = (char**)calloc(processor->input_file_commands_amount, sizeof(char*));
    CAPACITY_CHECK(addresses)
    
    addresses[index_of_first_element_in_string] = buf + symb_num;

    while (symb_num < size)
    {
        ch = buf + symb_num;

        if (*ch == '\0')
        {
            index_of_first_element_in_string++;
            symb_num++;
            ch = buf + symb_num;

            if (*ch == '\0') //на случай \r
                symb_num++;

            addresses[index_of_first_element_in_string] = buf + symb_num;
            }
        symb_num++;
    }
    /*printf("in addresses\n");
    for (int i = 0; i < processor->input_file_commands_amount; i++)
    {
        printf("%s\n", addresses[i]);
    }*/

    processor->addresses = addresses;

    return ALL_RIGHT;
}

Errors output_file(Proc* processor, Errors* err)
{
    assert(processor != nullptr);
    //printf("oh no 7\n");
    size_t label_delta = processor->labels.label_delta;
    //printf("oh no 8\n");
    int* new_buf;
    //printf("oh no 9\n");
    size_t input_file_commands_amount = processor->input_file_commands_amount;
    //printf("oh no 10\n");
    
    new_buf = (int*)calloc(input_file_commands_amount, sizeof(int));
    CAPACITY_CHECK(new_buf)
    //printf("oh no 12\n");
    processor->new_file_buf = new_buf;

    //printf("oh no 1\n");
    make_labels(processor);
    make_regs(processor);
    //printf("oh no 2\n");
    assembly(processor, err);
    //assembly(processor, err);  
    //printf("oh no 3\n");
    new_buf = processor->new_file_buf;

    FILE* output_file;
    output_file = fopen("out.txt", "a");
    FILE_CHECK(output_file)
    //printf("oh no 4\n");

    for (size_t cmd_num = 0; cmd_num < input_file_commands_amount - label_delta; cmd_num++)
        fprintf(output_file, "%d ", new_buf[cmd_num]);
    
    //printf("oh no 5\n");
    fclose(output_file);

    return ALL_RIGHT;
}

void make_regs(Proc* processor)
{
    assert(processor != nullptr);

    RegisterParameters reg[REG_AMT];
    char* reg_names[REG_AMT] = {"ax", "bx", "cx", "dx", "ex"};

    for (size_t i = 0; i < REG_AMT; i++)
    {
        reg[i].name = reg_names[i];
        reg[i].value = -1;
    }
        
    for (size_t i = 0; i < REG_AMT; i++)
        processor->registers[i] = reg[i];
    
}

Errors assembly (Proc* const processor, Errors* err)
{
    assert(processor != nullptr);
    
    char str [80] = "";
    int arg = 0;
    int* new_buf = processor->new_file_buf;
    size_t out_cmd_num = 0;
    size_t lable_delta = 0;
    size_t arg_value = 0;

    size_t input_file_commands_amount = processor->input_file_commands_amount;
    size_t size = processor->init_file_size;

    for (size_t cmd_num = 0; cmd_num < input_file_commands_amount; cmd_num++)
    {
        arg_value = 0;
        processor->labels.is_label = false;
        processor->labels.label_type = DEFINE;
        char* file_buf = processor->addresses[cmd_num];

        sscanf(file_buf, "%s", str);
        //printf("str %s\n", str);
        
        check_label(processor, str, cmd_num, &arg);
        if (processor->labels.is_label == true)
        {
            lable_delta++;
            continue;
        }
            
        for (size_t cmd = 0; cmd < CMD_AMT; cmd++)
        {
            //printf("cmd %s\n", bunch_of_commands[cmd].cmd_str);
            if (strcmp(str, bunch_of_commands[cmd].cmd_str) == 0)
            {
                new_buf[out_cmd_num] = bunch_of_commands[cmd].cmd_num;

                if(strcmp("push", str) == 0)    //очень выбивается из остальных, не придумала пока, как вписать в остальной код
                {
                    int arg1 = -13;
                    int arg2 = -13;
                    arg_analysis(processor, &arg1, &arg2, &arg_value, &cmd_num);

                    out_cmd_num++;
                    new_buf[out_cmd_num] = arg_value;

                    if (arg1 != -13)
                    {
                        out_cmd_num++;
                        new_buf[out_cmd_num] = arg1;
                    }
                    if (arg2 != -13)
                    {
                        out_cmd_num++;
                        new_buf[out_cmd_num] = arg1;
                    }

                    out_cmd_num++;
                    continue;
                }
                //printf("num %d\n");
                //print_output_buf(new_buf, input_file_commands_amount, cmd_num, str);
                
                for (size_t arg_num = 0; arg_num < bunch_of_commands[cmd].arg_amt; arg_num++) 
                {
                    cmd_num++;
                    out_cmd_num++;
                    file_buf = processor->addresses[cmd_num];

                    processor->labels.label_type = ARG;

                    sscanf(file_buf, "%s", str);

                    check_label(processor, str, cmd_num, &arg);

                    if (processor->labels.is_label == true)
                    {
                        new_buf[out_cmd_num] = arg;
                        //print_output_buf(new_buf, input_file_commands_amount, cmd_num, str);
                    }
                        
                    else
                    {
                        sscanf(file_buf, "%d", &arg);
                        new_buf[out_cmd_num] = arg;
                        //print_output_buf(new_buf, input_file_commands_amount, cmd_num, str);
                    }
                }
                break;
            }
            else if (cmd > CMD_AMT - 1)
            {
                printf("there is no such command\n");
                return SYN_PROBLEM;
            }
        }
        out_cmd_num++;
    }

    processor->labels.label_delta = lable_delta;
    processor->new_file_buf = new_buf;

    return ALL_RIGHT;
}

void arg_analysis (Proc* processor, int* arg1, int* arg2, size_t* arg_value, size_t* cmd_num)
{
    char* file_buf = processor->addresses[*cmd_num];
    char str[80] = "";

    SSCANF(file_buf, "%s", str)

    if (strcmp(str, "[") == 0)
    {
        *arg_value += 4;
        SSCANF(file_buf, "%s", str)

        if (str[1] == 'x')
        {
            *arg1 = find_register(processor, str);
            *arg_value += 2;
        }
        else
        {
            sscanf(file_buf, "%d", arg1);
            *arg_value += 1;

            SSCANF(file_buf, "%s", str)

            if (strcmp(str, "+") == 0)
                plus_case(processor, arg2, cmd_num, arg_value);
            else 
                return;
        }

        SSCANF(file_buf, "%s", str) //чтобы пропустить закрывающую скобочку
        return;
    }
    else if (str[1] == 'x')
    {
        *arg1 = find_register(processor, str);
        *arg_value += 2;
        return;
    }
    else 
    {
        sscanf(file_buf, "%d", *arg1);
        *arg_value += 1;

        SSCANF(file_buf, "%s", str)

        if (strcmp(str, "+") == 0)
            plus_case(processor, arg2, cmd_num, arg_value);
        else 
            return;
    }
}

void plus_case(Proc* processor, int* arg2, size_t* cmd_num, size_t* arg_value)
{
    char str[80] = "";
    char* file_buf = processor->addresses[*cmd_num];

    *arg_value += 2;

    SSCANF(file_buf, "%s", str)
    *arg2 = find_register(processor, str);
}

int find_register (Proc* processor, char*str)
{
    for (int i = 0; i < REG_AMT; i++)
    {
        if (strcmp(processor->registers[i].name, str) == 0)
            return i + 1;
    }
}

void make_labels(Proc* processor)
{
    assert(processor != nullptr);

    LabelParameters label[LABELS_AMT];

    for (size_t i = 0; i < LABELS_AMT; i++)
        label[i].target = -1;

    for (int i = 0; i < LABELS_AMT; i++)
        processor->labels.labels[i].target = label[i].target;

    //print_labels (processor->labels.labels);
}

Errors check_label(Proc* processor, char* str, size_t cmd_num, int* arg)
{ 
    bool is_label = false;

    LabelParameters* label = processor->labels.labels;
    
    is_label = find_label_mark(str);

    if (is_label == true)
    {
        if (processor->labels.label_type == DEFINE)
        {
            for (size_t i = 0; i < LABELS_AMT; i++)
            {
                if (label[i].target == -1 && label[i].name == nullptr)
                {
                    label[i].target = cmd_num;
                    label[i].name = str;
                }

                else if (label[i].target == -1 && strcmp(label[i].name, str) == 0)
                {
                    label[i].target = cmd_num;
                }

                if (i >= LABELS_AMT - 1 && label[i].target != -1)
                {
                    printf ("no place for new label\n");
                    return PLACE_PROBLEM;
                }
                
                break;
            }
        }

        else if (processor->labels.label_type == ARG)
        {
            for (size_t i = 0; i < LABELS_AMT; i++)
            {
                if (label[i].target != -1 && strcmp(label[i].name, str) == 0)
                {
                    *arg = label[i].target;
                }
                    
                
                else if (label[i].target == -1 && strcmp(label[i].name, str) == 0)
                {
                    label[i].target = cmd_num;
                    *arg = label[i].target;
                }

                if (i >= LABELS_AMT - 1 && label[i].name != nullptr)
                {
                    printf("there is no such label\n");
                    return SYN_PROBLEM;
                }

                break;
            }
        }

        for (int i = 0; i < LABELS_AMT; i++)
        {
            processor->labels.labels[i].target = label[i].target;
        }

        processor->labels.is_label = true;
    }

    return ALL_RIGHT;
}

bool find_label_mark (const char* const str)
{
    bool is_label = false;
    char ch = 'a';
    int i = 0;

    while (ch != '\0')
    {
        ch = str[i];
        if (ch == ':')
        {
            
            is_label = true;
        }   
        i++;
    }

    return is_label;
}

void proc_dtor (Proc* processor)
{
    assert(processor != 0);

    free(processor->file_buf);
    free(processor->new_file_buf);
    free(processor->symb_amount_arr);
}

void print_labels (LabelParameters* labels)
{
    printf("labels:\n");
    for (size_t i = 0; i < LABELS_AMT; i++)
    {
        printf("target %d: %d\n", i, labels[i].target);
    }
}

void RAM_filling (Proc* processor)
{
    char array[RAM_AMT];
    for (int i = 0; i < RAM_AMT; i++)
        array[i] = '.';
    
    array[2] = '*';
    array[6] = '*';
    array[8] = '*';
    array[10] = '*';
    array[14] = '*';
    array[16] = '*';
    array[18] = '*';
    array[22] = '*';

    for(int i = 0; i < RAM_AMT; i++)
        processor->RAM[i] = array[i];

}

void draw_RAM (Proc* processor)
{
    char* array = processor->RAM;

    for (int i = 0; i < RAM_AMT; i++)
    {
        putchar(array[i]);
        printf(" ");

        if ((i + 1)%5 == 0)
            printf("\n");
    }
}