#include <stdio.h>

#include "assembly.h"

static void ctor_labels(Labels* labels);
static void dtor_labels(Labels* labels);

static void fill_labels(Labels* labels, Text* input);
static bool find_label_mark (const char* const str);
static bool find_jump (char* str);
static int find_label(Labels* labels, char* str);

static void handle_commands(Labels* labels, Text* input, Stack* new_buf);
static void args (Labels* labels, Text* input, size_t cmd, Stack* new_buf);
static void complicated_arg_case (Stack* new_buf, Text* input, size_t cmd);

static int arg_analysis (Text* input, int* arg1, int* arg2);
static void RAM_case (int* arg1, int* arg2, Text* input);
static void plus_case(size_t* cmd_num, Text* input, int* arg2);

static bool find_register (char* str, int* arg);


void assembly (Text* input, Labels* labels, Stack* new_buf)
{
    ctor_labels(labels);
    fill_labels(labels, input);

    handle_commands(labels, input, new_buf);

    dtor_labels(labels);
}

void ctor_labels(Labels* labels)
{
    LabelParameters* label = (LabelParameters*)calloc(LABELS_AMT, sizeof(LabelParameters));

    for (size_t i = 0; i < LABELS_AMT; i++)
        label[i].target = START_VALUE;

    labels->labels = label;
}

void fill_labels(Labels* labels, Text* input)
{
    char** buf = input->addresses;
    bool is_label = false;
    bool is_jump  = false;
    int label_ind = 0;
    char str[80] = "";

    for (int word = 0; word < input->cmd_amt; word++)
    {
        strcpy(str, buf[word]);  //вроде так должно работать, иначе можно сделать через sscanf

        is_label = find_label_mark(str);

        if (is_label == true)
        {
            label_ind = find_label(labels, str);

            is_jump = find_jump(buf[word - 1]);  //ничего получше придумать не могу

            if (is_jump != true && label_ind != START_VALUE)  //только надо проверить, что не агрумент, только difine записываем иначе может быть неверный номер
            {
                labels->labels[label_ind].target = word;
                labels->labels[label_ind].name = buf[word];
            }

            is_label = false;  //чтобы зашел еще раз потом
        }
    }
}

bool find_label_mark (const char* const str)
{
    bool is_label = false;

    char ch = 'a';
    int   i =  0;

    while (ch != '\0')
    {
        ch = str[i];

        if (ch == LABEL_MARK)
            is_label = true;
        
        i++;
    }

    return is_label;
}

bool find_jump (char* str)
{
    if (strcmp(str, "ja") == 0  || strcmp(str, "jae") == 0 || strcmp(str, "jb") == 0 \
     || strcmp(str, "jbe") == 0 || strcmp(str, "je") == 0  || strcmp(str, "jne") == 0)
        return true;
    else
        return false;
}

int find_label(Labels* labels, char* str)
{
    for (int i = 0; i < LABELS_AMT; i++)
    {
        if (labels->labels[i].name == nullptr || strcmp(labels->labels[i].name, str) == 0)
            return i;
    }

    return -1; //можно ошибку добавить
}





void handle_commands(Labels* labels, Text* input, Stack* new_buf)
{
    size_t cmd_amt = input->cmd_amt;
    char str [80] = "";
    char* file_buf;
    bool is_label = false;

    for (int cmd_num = 0; cmd_num < cmd_amt; cmd_num++)
    {
        labels->label_type = LABEL_DEF;
        is_label = false;
        
        file_buf = input->addresses[cmd_num];
        sscanf(file_buf, "%s", str);  // вот не знаю,что лучше sscanf или strcpy

        is_label = find_label_mark(str);
        if (is_label == true)
            continue;

        for (size_t cmd = 0; cmd < CMD_AMT; cmd++)
        {
            if (strcmp(str, bunch_of_commands[cmd].cmd_str) == 0)
            {
                stk_push(new_buf, bunch_of_commands[cmd].cmd_num);
            
                if(strcmp("push", str) == 0 || strcmp("pop", str) == 0) //строковые константы, отдельная функция
                {
                    complicated_arg_case(new_buf, input, cmd);
                    break;
                }

                args(labels, input, cmd, new_buf);
                break;
            }
            else if (cmd > CMD_AMT - 1)
            {
                printf("there is no such command\n");
            }
        }
    }
    
    return; 
}

void args (Labels* labels, Text* input, size_t cmd, Stack* new_buf)
{
    int arg = 0;
    char str[80] = "";
    bool is_label = false;
    int ind = 0;

    size_t cmd_num = input->cmd_num;

    for (size_t arg_num = 0; arg_num < bunch_of_commands[cmd].arg_amt; arg_num++) 
    {
        is_label = false;
        cmd_num++; 
        char* file_buf = input->addresses[cmd_num];
        sscanf(file_buf, "%s", str);

        is_label = find_label_mark(str);
        if (is_label == true)
        {
            ind = find_label(labels, str);              //сделать на джампы 1 аргумент
            stk_push(new_buf, labels->labels[ind].target);
        }         
        else
        {
            sscanf(file_buf, "%d", &arg);
            stk_push(new_buf, arg);
        }
    }

    return;
}

void complicated_arg_case (Stack* new_buf, Text* input, size_t cmd)  //надо еще посмотреть, что тут с cmd_num
{
    int arg1 = POISON;
    int arg2 = POISON;

    arg_analysis(input, &arg1, &arg2);


    if (arg1 != POISON)
    {
        stk_push(new_buf, arg1);
    }
    if (arg2 != POISON)
    {
        stk_push(new_buf, arg2);
    }

    return;
}


int arg_analysis (Text* input, int* arg1, int* arg2)
{
    size_t cmd_num = input->cmd_num;
    char str[80] = "";
    bool is_register = find_register(str, arg1);

    cmd_num++;
    char* file_buf = input->addresses[cmd_num];
    int ind = sscanf (file_buf, "%s", str); 
    
    if (strcmp(str, "[") == 0)
    {
        RAM_case(arg1, arg2, input);
    }
    else if (is_register == true)
    {
        *arg1 = (*arg1 | (1 >> sizeof(int)*8 - 2));
        cmd_num++;
    }
    else if (ind == 1)
    {
        sscanf(file_buf, "%d", arg1);

        cmd_num++;
        file_buf = input->addresses[cmd_num];
        sscanf (file_buf, "%s", str); 

        if (strcmp(str, "+") == 0)
             plus_case(&cmd_num, input, arg2);
    }
    else if (ind == 0)
    {
        cmd_num++;
    }

    cmd_num--;
    input->cmd_num = cmd_num;
}

void RAM_case (int* arg1, int* arg2, Text* input)
{
    *arg1 = (*arg1 | (1 >> sizeof(int)*8 - 3));
    

    size_t cmd_num = input->cmd_num;
    char str[80] = "";
    bool is_register = find_register(str, arg1);

    cmd_num++;
    char* file_buf = input->addresses[cmd_num];
    sscanf (file_buf, "%s", str); 

    if (is_register == true)
        *arg1 = (*arg1 | (1 >> sizeof(int)*8 - 2));

    else
    {
        sscanf(file_buf, "%d", arg1);
        *arg1 = (*arg1 | (1 >> sizeof(int)*8 - 1));

        cmd_num++;
        file_buf = input->addresses[cmd_num];
        sscanf (file_buf, "%s", str);

        if (strcmp(str, "+") == 0)
        {
            plus_case(&cmd_num, input, arg2);
            *arg2 = (*arg2 | (1 >> sizeof(int)*8 - 3));
        }
    }

    cmd_num++;

    file_buf = input->addresses[cmd_num];
    sscanf (file_buf, "%s", str); 
    cmd_num++;

    input->cmd_num = cmd_num;
}

void plus_case(size_t* cmd_num, Text* input, int* arg2)
{
    char str[80] = "";
    char* file_buf = input->addresses[*cmd_num];


    (*cmd_num)++;
    file_buf = input->addresses[*cmd_num];
    sscanf (file_buf, "%s", str);

    find_register(str, arg2);
    *arg2 = (*arg2 | (1 >> sizeof(int)*8 - 2));
}

bool find_register (char* str, int* arg)  //статический массив, вынести из процессора
{
    bool is_register = false;

    for (int i = 0; i < REG_AMT; i++)
    {
        if (strcmp(registers[i].name, str) == 0)
        {
            is_register = true;
            *arg = i + 1;
            return is_register;
        } 
    }

    printf("there is no such register\n");

    return is_register;
}

void dtor_labels(Labels* labels)
{
    free(labels->labels);
}


void make_file (Stack* new_buf)
{
    int* stack_buf = new_buf->data;
    int* file_buf = (int*)calloc(new_buf->capacity, sizeof(int));
    int file_size = 0;

    while(stack_buf[file_size] != POISON)
    {
        file_buf[file_size] = stack_buf[file_size];
        file_size++;
    }

    file_buf = (int*)realloc(file_buf, sizeof(int)*file_size);

    FILE* output_file;
    output_file = fopen("out.txt", "a");
    FILE_CHECK(output_file)

    for (int cmd_num = 0; cmd_num < file_size; cmd_num++)
        fprintf(output_file, "%d ", file_buf[cmd_num]);

    fclose(output_file);

    free(file_buf);
}