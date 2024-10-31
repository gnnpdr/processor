#include <stdio.h>

#include "assembly.h"

void assembly (Text* input, Labels* labels, Stack* new_buf)
{
    ctor_labels(labels);
    fill_labels(labels, input);

    handle_commands(labels, input, new_buf);
  
    labels_dtor(labels);
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
        strcpy(str, buf[word]);

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
    for (int i = 0; i < LABELS_AMT; i++) //лаги-баги, это константа...
    {
        if (labels->labels[i].name == nullptr || strcmp(labels->labels[i].name, str) == 0)
            return i;
    }

    return -1; //можно ошибку добавить
}





void handle_commands(Labels* labels, Text* input, Stack* new_buf)
{
    asm_cmds(labels, input, new_buf); 


    proc->new_file_buf = new_buf;
    int k = 0;

    for (int i = 0; i < proc->new_file_buf.capacity; i++)
        printf("%d ", proc->new_file_buf.data[i]);

    /*while (proc->new_file_buf.data[k] != POISON)
    {
        printf("%d ", proc->new_file_buf.data[k]);
        k++;
    }*/



    stk_dtor(&new_buf);
}

void asm_cmds(Labels* labels, Text* input, Stack* new_buf)
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
        sscanf(file_buf, "%s", str);

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
                    //printf("push\n");
                    complicated_arg_case(new_buf, input, proc, cmd);
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

    size_t cmd_num = input->cmd_num;

    for (size_t arg_num = 0; arg_num < bunch_of_commands[cmd].arg_amt; arg_num++) 
    {
        cmd_num++; 
        char* file_buf = input->addresses[cmd_num];
        sscanf(file_buf, "%s", str);
        

        find_label_mark(str);
        if (labels->is_label == true)
        {
            label_arg(labels, cmd_num, &arg, str);
            stk_push(new_buf, arg);
            return;
        }         
        else
        {
            sscanf(file_buf, "%d", &arg);
            stk_push(new_buf, arg);
        }
    }

    return;
}

void label_define(Labels* labels, size_t cmd_num, char* str)
{
    LabelParameters* label = labels->labels;

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
            return;
        }
                
        break;
    }

    labels->labels = label;

    return;
}

void label_arg(Labels* labels, size_t cmd_num, int* arg, char* str)
{
    LabelParameters* label = labels->labels;

    for (size_t i = 0; i < LABELS_AMT; i++)
    {
        if (label[i].target != START_VALUE && strcmp(label[i].name, str) == 0)  //отдельно strcmp
        {
            *arg = label[i].target;
        }
                    
        else if (label[i].target == START_VALUE && strcmp(label[i].name, str) == 0)
        {
            label[i].target = cmd_num;
            *arg = label[i].target;
        }

        if (i >= LABELS_AMT - 1 && label[i].name != nullptr)
        {
            printf("there is no such label\n");
            return;
        }

        break;
    }
    labels->labels = label;

    return;
}



void push_pop_case (Stack* new_buf, Text* input, Processor* proc, size_t cmd)
{
    printf("this case\n");
    int arg1 = POISON;
    int arg2 = POISON;

    int arg_value = arg_analysis(input, &arg1, &arg2, proc);
    printf("arg value %d\n", arg_value);
    printf("arg1 %d\n", arg1);
    printf("arg2 %d\n", arg2);

    stk_push(new_buf, arg_value);

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


int arg_analysis (Text* input, int* arg1, int* arg2, Processor* proc)
{
    int arg_value = 0;
    size_t cmd_num = input->cmd_num;
    char str[80] = "";

    cmd_num++;
    char* file_buf = input->addresses[cmd_num];
    
    int ind = sscanf (file_buf, "%s", str); 
    
    if (strcmp(str, "[") == 0)
    {
        RAM_case(proc, arg1, arg2, input);
    }
    else if (str[1] == 'x')     //функция проверки на регистр
    {
        *arg1 = find_register(proc, str);
        arg_value += 2;
        cmd_num++;
    }
    else if (ind == 1)
    {
        sscanf(file_buf, "%d", arg1); //записать тип в число побитовыми сдвигами
        //printf("buf %s\n", file_buf);
        arg_value += 1;
        printf("arg 1 in an %d\n", *arg1);

        cmd_num++;
        
        file_buf = input->addresses[cmd_num];
        sscanf (file_buf, "%s", str); 

        if (strcmp(str, "+") == 0)
            *arg2 = plus_case(proc, &cmd_num, &arg_value, input);
    }
    else if (ind == 0)   // распространение на случай pop 
    {
        arg_value += 0;
        cmd_num++;
    }

    cmd_num--;
    input->cmd_num = cmd_num;

    return arg_value;
}

int RAM_case (Processor* proc, int* arg1, int* arg2, Text* input)
{
    int arg_value = 0;
    arg_value += 4;
    char str[80] = "";

    size_t cmd_num = input->cmd_num;

    cmd_num++;
    char* file_buf = input->addresses[cmd_num];
    sscanf (file_buf, "%s", str); 

    if (str[1] == 'x' && strlen(str) == 2)
    {
        *arg1 = find_register(proc, str);
        arg_value += 2;
    }
    else
    {
        sscanf(file_buf, "%d", arg1);
        arg_value += 1;

        cmd_num++;
        file_buf = input->addresses[cmd_num];
        sscanf (file_buf, "%s", str);

        if (strcmp(str, "+") == 0)
        *arg2 = plus_case(proc, &cmd_num, &arg_value, input);
    }

    cmd_num++;

    file_buf = input->addresses[cmd_num];
    sscanf (file_buf, "%s", str); 
    cmd_num++;

    input->cmd_num = cmd_num;

    return arg_value;
}

int plus_case(Processor* proc, size_t* cmd_num, int* arg_value, Text* input)
{
    int arg2 = 0;
    char str[80] = "";
    char* file_buf = input->addresses[*cmd_num];

    *arg_value += 2;

    (*cmd_num)++;
    file_buf = input->addresses[*cmd_num];
    sscanf (file_buf, "%s", str);
    arg2 = find_register(proc, str);

    return arg2;
}

int find_register (Processor* proc, char* str)  //статический массив, вынести из процессора
{
    for (int i = 0; i < REG_AMT; i++)
    {
        if (strcmp(proc->registers[i].name, str) == 0)
            return i + 1;
    }

    printf("there is no such register\n");
    return POISON;
}

void labels_dtor(Labels* labels)
{
    free(labels->labels);
}