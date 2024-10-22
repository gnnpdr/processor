#include <stdio.h>
#include <sys/stat.h>

#include "asm.h"

Errors proc (const char* const name, Proc* const processor, Errors* err)
{
    assert(name != nullptr);
    assert(processor != nullptr);

    init_buf(name, processor, err);
    
    processing_text(processor);  
    filling_addresses(processor);

    output_file(processor, err);

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

    //print_init_buf (file_buf, init_file_size);

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

    //print_addresses (addresses, processor->input_file_commands_amount);

    processor->addresses = addresses;

    return ALL_RIGHT;
}

Errors output_file(Proc* const processor, Errors* err)
{
    assert(processor != nullptr);

    int* new_buf = processor->new_file_buf;
    size_t input_file_commands_amount = processor->input_file_commands_amount;

    new_buf = (int*)calloc(input_file_commands_amount, sizeof(int));
    CAPACITY_CHECK(new_buf)

    processor->new_file_buf = new_buf;

    make_labels(processor);
    assemb(processor, err);
    //assemb(processor, err);  

    new_buf = processor->new_file_buf;

    FILE* output_file;
    output_file = fopen("out.txt", "a");
    FILE_CHECK(output_file)

    for (size_t cmd_num = 0; cmd_num < input_file_commands_amount; cmd_num++)
        fprintf(output_file, "%d ", new_buf[cmd_num]);
    
    fclose(output_file);

    return ALL_RIGHT;
}

Errors assemb (Proc* const processor, Errors* err)
{
    assert(processor != nullptr);
    
    char str [80] = "";
    int arg = 0;
    int* new_buf = processor->new_file_buf;

    size_t input_file_commands_amount = processor->input_file_commands_amount;
    size_t size = processor->init_file_size;

    for (size_t cmd_num = 0; cmd_num < input_file_commands_amount; cmd_num++)
    {
        processor->labels.is_label = false;
        processor->labels.label_type = DEFINE;
        char* file_buf = processor->addresses[cmd_num];

        sscanf(file_buf, "%s", str);
        print_scanf_res(file_buf, str);
        
        check_label(processor, str, cmd_num, &arg);
        if (processor->labels.is_label == true)
        {
            new_buf = (int*)realloc(new_buf, (input_file_commands_amount - 1)*sizeof(int));
            CAPACITY_CHECK(new_buf)
            
            continue;
        }
            
        
        for (size_t cmd = 0; cmd < CMD_AMT; cmd++)
        {
            if (strcmp(str, bunch_of_commands[cmd].cmd_str) == 0)
            {
                new_buf[cmd_num] = bunch_of_commands[cmd].cmd_num;
                //print_output_buf(new_buf, input_file_commands_amount, cmd_num, str);
                
                for (size_t arg_num = 0; arg_num < bunch_of_commands[cmd].arg_amt; arg_num++) 
                {
                    cmd_num++;
                    file_buf = processor->addresses[cmd_num];

                    processor->labels.label_type = ARG;

                    sscanf(file_buf, "%s", str);

                    check_label(processor, str, cmd_num, &arg);

                    if (processor->labels.is_label == true)
                    {
                        new_buf[cmd_num] = arg;
                        //print_output_buf(new_buf, input_file_commands_amount, cmd_num, str);
                    }
                        
                    else
                    {
                        sscanf(file_buf, "%d", &arg);
                        new_buf[cmd_num] = arg;
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
    }

    processor->new_file_buf = new_buf;

    return ALL_RIGHT;
}

void make_labels(Proc* processor)
{
    assert(processor != nullptr);

    LabelParameters label[LABELS_AMT];

    for (size_t i = 0; i < LABELS_AMT; i++)
    {
        //printf("i in make labels %d\n", i);

        label[i].target = -1;
        //
    }

    for (int i = 0; i < LABELS_AMT; i++)
    {
        printf("value %d\n", label[i].target);
        processor->labels.labels[i].target = label[i].target;
    }

    printf("in make labels\n");
    print_labels (processor->labels.labels);
}

Errors check_label(Proc* processor, char* str, size_t cmd_num, int* arg)
{ 
    bool is_label = false;

    LabelParameters* label = processor->labels.labels;
    
    is_label = find_label_mark(str);
    
    //printf("in check labels\n");
    //print_labels (label);
    //printf("go in\n");

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
    printf("str in flm %s\n", str);

    while (ch != '\0')
    {
        ch = str[i];
        printf("%c", ch);
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
        //printf("name %d: %s\n", i, labels[i].name);
    }
}