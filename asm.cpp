#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "asm.h"

static void get_output_file(Processor* proc);
static size_t assembly_file(Processor* proc);
static void args (Processor* proc, char* file_buf, int* new_buf, size_t* out_cmd_num, int* cmd_num, size_t cmd, size_t* out_cmd_amt);

static void fill_labels(Processor* proc);
static void check_label(Processor* proc, char* str, int cmd_num, int* arg);
static bool find_label_mark (const char* const str);

static void fill_regs(Processor* proc);
static void push_pop_case (Processor* proc, int* cmd_num, size_t* out_cmd_num, int* new_buf, size_t* out_cmd_amt);
static int arg_analysis (Processor* proc, int* arg1, int* arg2, int* cmd_num);
static int plus_case(Processor* proc, int* cmd_num, int* arg_value);
static int find_register (Processor* proc, char* str);

static void fill_RAM (Processor* proc);

void assembly_code (char* name, Processor* proc)
{
    get_file(name, proc);

    proc_buf(proc);
 
    get_output_file(proc);
}

void get_output_file(Processor* proc)
{
    //size_t input_ncmd = proc->input_ncmd;
    //size_t label_delta = proc->label_delta;

    int* new_buf = (int*)calloc(OUT_AMT, sizeof(int));
    LOCATION_CHECK(new_buf)

    proc->new_file_buf = new_buf;
    

    fill_labels(proc);
    fill_regs(proc);
    fill_RAM(proc);
    size_t size = assembly_file(proc);


    new_buf = proc->new_file_buf;


    FILE* output_file;
    output_file = fopen("out.txt", "a");
    FILE_CHECK(output_file)

    for (int cmd_num = 0; cmd_num < size; cmd_num++)
        fprintf(output_file, "%d ", new_buf[cmd_num]);

    fclose(output_file);
}

void fill_regs(Processor* proc)
{
    RegisterParameters reg[REG_AMT] = {};
    char* reg_names[REG_AMT] = {"ax", "bx", "cx", "dx", "ex"};

    for (size_t i = 0; i < REG_AMT; i++)
    {
        reg[i].name = reg_names[i];
        reg[i].value = START_VALUE;
    }
 
    for (size_t i = 0; i < REG_AMT; i++)
    {
        proc->registers[i].name = reg[i].name;
        proc->registers[i].value = reg[i].value;
    }
}

void fill_labels(Processor* proc)
{
    LabelParameters label[LABELS_AMT] = {};

    for (size_t i = 0; i < LABELS_AMT; i++)
        label[i].target = START_VALUE;

    for (size_t i = 0; i < LABELS_AMT; i++)
        proc->labels.labels[i].target = label[i].target;

}

void fill_RAM (Processor* proc)
{
    char array[RAM_AMT] = {};

    for (int i = 0; i < RAM_AMT; i++)
        array[i] = START_VALUE;

    for(int i = 0; i < RAM_AMT; i++)
        proc->RAM[i] = array[i];

}

size_t assembly_file(Processor* proc)
{
    size_t out_cmd_amt = OUT_AMT;
    size_t out_cmd_num = 0;

    char str [80] = "";
    int arg = 0;

    int* new_buf = proc->new_file_buf;
    size_t input_ncmd = proc->input_ncmd;

    size_t lable_delta = 0;

    for (int cmd_num = 0; cmd_num < input_ncmd; cmd_num++)
    {
        proc->labels.is_label = false;
        proc->labels.label_type = LABEL_DEF;

        char* file_buf = proc->addresses[cmd_num];

        sscanf(file_buf, "%s", str);

        check_label(proc, str, cmd_num, &arg);
        if (proc->labels.is_label == true)
        {
            lable_delta++;
            continue;
        }

        for (size_t cmd = 0; cmd < CMD_AMT; cmd++)
        {
            if (strcmp(str, bunch_of_commands[cmd].cmd_str) == 0)
            {
                CAP_CHECK(out_cmd_num, out_cmd_amt)
                new_buf[out_cmd_num] = bunch_of_commands[cmd].cmd_num;
                if(strcmp("push", str) == 0 || strcmp("pop", str) == 0)
                {
                    push_pop_case(proc, &cmd_num, &out_cmd_num, new_buf, &out_cmd_amt);
                    out_cmd_num--;
                    break;
                }

                CAP_CHECK(out_cmd_num, out_cmd_amt)
                new_buf[out_cmd_num] = bunch_of_commands[cmd].cmd_num;

                args(proc, file_buf, new_buf, &out_cmd_num, &cmd_num, cmd, &out_cmd_amt);
                
                break;
            }
            else if (cmd > CMD_AMT - 1)
            {
                printf("there is no such command\n");
            }
        }
        out_cmd_num++;
    }
    proc->input_ncmd = out_cmd_num;

    proc->label_delta = lable_delta;
    proc->new_file_buf = new_buf;

    return out_cmd_num;
}

void push_pop_case (Processor* proc, int* cmd_num, size_t* out_cmd_num, int* new_buf, size_t* out_cmd_amt)
{
    int arg1 = POISON;
    int arg2 = POISON;

    int arg_value = arg_analysis(proc, &arg1, &arg2, cmd_num);
    (*cmd_num)--;

    (*out_cmd_num)++;

    CAP_CHECK(*out_cmd_num, *out_cmd_amt)
    new_buf[(*out_cmd_num)++] = arg_value;

    if (arg1 != POISON)
    {
        CAP_CHECK(*out_cmd_num, *out_cmd_amt)
        new_buf[(*out_cmd_num)++] = arg1;
    }
    if (arg2 != POISON)
    {
        CAP_CHECK(*out_cmd_num, *out_cmd_amt)
        new_buf[(*out_cmd_num)++] = arg2;
    }

    return;
}


int arg_analysis (Processor* proc, int* arg1, int* arg2, int* cmd_num)
{
    int arg_value = 0;
    char* file_buf = proc->addresses[*cmd_num];
    char str[80] = "";

    (*cmd_num)++;

    file_buf = proc->addresses[*cmd_num];
    int ind = sscanf (file_buf, "%s", str); 

    if (strcmp(str, "[") == 0)
    {
        arg_value += 4;

        (*cmd_num)++;
        file_buf = proc->addresses[*cmd_num];
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

            (*cmd_num)++;
            file_buf = proc->addresses[*cmd_num];
            sscanf (file_buf, "%s", str);

            if (strcmp(str, "+") == 0)
                *arg2 = plus_case(proc, cmd_num, &arg_value);
        }

        (*cmd_num)++;
        file_buf = proc->addresses[*cmd_num];
        sscanf (file_buf, "%s", str); 
        (*cmd_num)++;
    }
    else if (str[1] == 'x')
    {
        *arg1 = find_register(proc, str);
        arg_value += 2;
        (*cmd_num)++;
    }
    else if (ind == 1)
    {
        sscanf(file_buf, "%d", arg1);
        arg_value += 1;

        (*cmd_num)++;
        
        file_buf = proc->addresses[*cmd_num];
        sscanf (file_buf, "%s", str); 

        if (strcmp(str, "+") == 0)
            *arg2 = plus_case(proc, cmd_num, &arg_value);

    }
    else if (ind == 0)   // распространение на случай pop 
    {
        arg_value += 0;
        (*cmd_num)++;
    }

    return arg_value;
}

int plus_case(Processor* proc, int* cmd_num, int* arg_value)
{
    int arg2 = 0;
    char str[80] = "";
    char* file_buf = proc->addresses[*cmd_num];

    *arg_value += 2;

    (*cmd_num)++;
    file_buf = proc->addresses[*cmd_num];
    sscanf (file_buf, "%s", str);
    arg2 = find_register(proc, str);

    return arg2;
}

int find_register (Processor* proc, char* str)
{
    for (int i = 0; i < REG_AMT; i++)
    {
        if (strcmp(proc->registers[i].name, str) == 0)
            return i + 1;
    }

    printf("there is no such register\n");
    return POISON;
}

void args (Processor* proc, char* file_buf, int* new_buf, size_t* out_cmd_num, int* cmd_num, size_t cmd, size_t* out_cmd_amt)
{
    int arg = 0;
    char str[80] = "";

    for (size_t arg_num = 0; arg_num < bunch_of_commands[cmd].arg_amt; arg_num++) 
    {
        proc->labels.label_type = ARG;

        (*cmd_num)++;
        (*out_cmd_num)++;
                    
        file_buf = proc->addresses[*cmd_num];
        sscanf(file_buf, "%s", str);

        check_label(proc, str, *cmd_num, &arg);

        if (proc->labels.is_label == true)
        {
            CAP_CHECK(*out_cmd_num, *out_cmd_amt)
            new_buf[*out_cmd_num] = arg;
        }
                        
        else
        {
            sscanf(file_buf, "%d", &arg);
            CAP_CHECK(*out_cmd_num, *out_cmd_amt)
            new_buf[*out_cmd_num] = arg;
        }
    }

    return;
}

void check_label(Processor* proc, char* str, int cmd_num, int* arg)
{ 
    bool is_label = false;

    LabelParameters* label = proc->labels.labels;
    
    is_label = find_label_mark(str);

    if (is_label == true)
    {
        printf("label str %s\n", str);
        if (proc->labels.label_type == LABEL_DEF)
        {
            for (size_t i = 0; i < LABELS_AMT; i++)
            {
                if (label[i].target == -1 && label[i].name == nullptr)
                {
                    printf("IN DEFINE %s\n", str);
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
        }

        else if (proc->labels.label_type == ARG)
        {
            printf("in args\n");
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
                    return;
                }

                break;
            }
        }

        printf("will write %s\n", label[0].name);
        for (size_t i = 0; i < 2; i++)
        {
            printf("target in proc%d, not %d\n", proc->labels.labels[i].target, label[i].target);
            printf("name in proc \"%s\", not \"%s\"\n", proc->labels.labels[i].name, label[i].name);
        }

        for (size_t i = 0; i < LABELS_AMT; i++)
        {
            proc->labels.labels[i].target = label[i].target;
            proc->labels.labels[i].name = label[i].name;
        }

        proc->labels.is_label = true;
    }

    return;
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
            is_label = true;
        
        i++;
    }

    return is_label;
}

void proc_dtor (Processor* proc, char* name)
{
    free(name);
    free(proc->file_buf);
    free(proc->new_file_buf);
    free(proc->addresses);
}