#include <stdio.h>

#include "proc.h"

static int get_arg (Processor* proc);
static int* get_arg_addr (Processor* proc, int* arg);

static ResultOfComparing comparing(int first_el, int sec_el);

//надо попробовать унифицировать функцию для получения имени

void proc_file (Stack* const stk, Processor* const proc)
{
    get_file(proc);
    int size = proc->ncmd;
    
    int ip = proc->ip;
    int* program = proc->new_file_buf;

    stack_element_t elem = 0;
    int first_el = 0;
    int sec_el = 0;
    int arg = 0;

    while(ip < size)
    {
        switch(program[ip])
        {
            case PUSH:
                proc->ip = ip;
                stk_push (stk, get_arg(proc));
                ip = proc->ip;
                break;

            case ADD:
                TWO_ARGS
                stk_push (stk, first_el + sec_el);
                break;

            case SUB:
                TWO_ARGS
                stk_push (stk, first_el - sec_el);
                break;

            case MUL:
                TWO_ARGS
                stk_push (stk, first_el * sec_el);
                break;

            case DIV:
                TWO_ARGS
                stk_push (stk, first_el / sec_el);
                break;

            case SQRT:
                ONE_ARG
                stk_push (stk, pow(arg, 0.5));
                break;
                
            case SIN:
                ONE_ARG
                stk_push (stk, sin(arg));
                break;

            case COS:
                ONE_ARG
                stk_push (stk, cos(arg));
                break;
            
            case JA:
                ip++;
                proc->ip = ip;
                ja(proc, stk);
                break;

            case JAE:
                ip++;
                jae(proc, stk);
                break;

            case JE:
                ip++;
                je(proc, stk);
                break;

            case JNE:
                ip++;
                jne(proc, stk);
                break;

            case JMP:
                ip++;
                ip = proc->new_file_buf[ip];
                break;

            case POP:
                proc->ip = ip;
                stk_pop (stk, get_arg_addr(proc));
                ip = proc->ip;
                break;

            case HLT:
                break;

            default:
                printf("something went wrong :(\n");
                break;
        }   
        ip++;
    }
}

Errors get_file(Processor* proc)
{
    char name[80] = {};
    printf("enter input file name\n");  //можно взять функцию из input, тогда получится как бы вторая программа
    scanf ("%s", name);

    FILE* input_file;

    input_file = fopen(name, "rb"); //так же, наверное, бинарноe чтение
    FILE_CHECK(input_file)

    int file_size = find_file_size(name);

    int* buf = (int*)calloc(file_size, sizeof(int));
    ALLOCATION_CHECK(buf)

    fread(buf, sizeof(int), file_size, input_file);
    proc->new_file_buf = buf;

    proc->ncmd = file_size;
}

int get_arg (Processor* proc)
{
    size_t ip = proc->ip;
    int* program = proc->new_file_buf;

    ip++;   //переход на аргументы
    int arg = program[ip];
    int res_arg = 0;

    bool is_int = false;
    bool is_reg = false;
    bool is_ram = false;

    //сначала проверится оп память. Потом возможные аргументы - инт, инт и регистр или регистр. В случае инта будет проверка след аргумента

    if (arg & RAM == RAM)
        is_ram = true;

    if (arg & INT == INT)
    {
        is_int = true;
        res_arg += arg & FREE;
        ip++;
        arg = program[ip];
    }

    if (arg & REG == REG)
    {
        is_reg = true;
        res_arg += registers[arg & FREE].value;
    }
    else 
        ip--;

    if (is_ram == true)
        res_arg = proc->RAM[res_arg];

    proc->ip = ip;
    
    return res_arg;
}

int* get_arg_addr (Processor* proc)
{
    size_t ip = proc->ip;
    int* program = proc->new_file_buf;

    ip++;   //переход на аргументы
    int* arg = &program[ip];

    int* res_addr = 0;
    int res_arg = 0;

    int for_stk_pop = 0;

    bool is_reg = false;
    bool is_ram = false;

    if ((*arg) & RAM == RAM)
        is_ram = true;

    if ((*arg) & INT == INT)  //проверка на инт не нужна, тк адрес нужен только для pop, а она не принимает просто инт без оп
    {
        res_arg += (*arg) & FREE;
        ip++;
        *arg = program[ip];
    }

    if ((*arg) & REG == REG)
    {
        is_reg = true;

        if (is_ram == true)
            res_arg += registers[(*arg) & FREE].value;

        else
            res_addr = &registers[(*arg) & FREE].value;
    }

    if (is_reg == false && is_ram == false)
    {
        ip--;
        res_addr = &for_stk_pop;  //типа фиктивная переменная, чтобы указатель на аргумент был
    }
        
    if (is_ram == true)
        res_addr = &proc->RAM[res_arg];

    proc->ip = ip;
    
    return res_addr;
}

void ja(Processor* proc, Stack* stk)
{
    size_t ip = proc->ip;
    int* program = proc->new_file_buf;
    int arg = program[ip];

    stack_element_t first_el = 0;
    stack_element_t sec_el = 0;

    stk_pop(stk, &sec_el);
    stk_pop(stk, &first_el);

    DoJump do_jump = NO_JUMP;

    ResultOfComparing res = comparing(first_el, sec_el);
    if (res == GREATER)
        do_jump = DO_JUMP;

    if(do_jump = DO_JUMP)
        ip = arg;

    proc->ip = ip;
}

void jae (Processor* proc, Stack* stk)
{
    size_t ip = proc->ip;
    int* program = proc->new_file_buf;
    int arg = program[ip];
    
    stack_element_t first_el = 0;
    stack_element_t sec_el = 0;

    stk_pop(stk, &sec_el);
    stk_pop(stk, &first_el);

    DoJump do_jump = NO_JUMP;

    ResultOfComparing res = comparing(first_el, sec_el);
    if (res == GREATER || res == EQUAL)
        do_jump = DO_JUMP;

    if(do_jump = DO_JUMP)
        ip = arg;

    proc->ip = ip;
}

void je (Processor* proc, Stack* stk)
{
    size_t ip = proc->ip;
    int* program = proc->new_file_buf;
    int arg = program[ip];
    
    stack_element_t first_el = 0;
    stack_element_t sec_el = 0;

    stk_pop(stk, &sec_el);
    stk_pop(stk, &first_el);

    DoJump do_jump = NO_JUMP;

    ResultOfComparing res = comparing(first_el, sec_el);
    if (res == EQUAL)
        do_jump = DO_JUMP;

    if(do_jump = DO_JUMP)
        ip = arg;

    proc->ip = ip;
}

void jne (Processor* proc, Stack* stk)
{
    size_t ip = proc->ip;
    int* program = proc->new_file_buf;
    int arg = program[ip];
    
    stack_element_t first_el = 0;
    stack_element_t sec_el = 0;

    stk_pop(stk, &sec_el);
    stk_pop(stk, &first_el);

    DoJump do_jump = NO_JUMP;

    ResultOfComparing res = comparing(first_el, sec_el);
    if (res == GREATER || res == LESS)
        do_jump = DO_JUMP;

    if(do_jump = DO_JUMP)
        ip = arg;

    proc->ip = ip;
}

ResultOfComparing comparing(int first_el, int sec_el)
{
    if (first_el > sec_el)
        return GREATER;

    else if (first_el == sec_el)
        return EQUAL;

    else 
        return LESS;
}