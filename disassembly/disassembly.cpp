#include <stdio.h>

#include "disassembly.h"

static Dig* dig_list_ctor(size_t size, ErrList *const list);
static void make_dig_list(Dig *const digits, size_t size, int *const code, ErrList *const list);
static void dig_list_dtor(Dig *const digits);

static LabelFunc* labels_funcs_ctor(Dig *const digits, size_t size, ErrList *const list);
static void label_funcs_dtor(LabelFunc *const jmp_addresses);
static void add_label_func(size_t ip, LabelFunc *const jmp_addresses, char *const text, ErrList *const list);

static size_t find_com(int dig, ErrList *const list);
static size_t find_jmp(LabelFunc *const jmp_addresses, int value, ErrList *const list);
static void add_args(Dig *const digits, LabelFunc *const jmp_addresses, char *const text, size_t *const ip, size_t com_num, ErrList *const list);

void disassembly(int *const code, char *const text, size_t dig_amt, ErrList *const list)
{
    assert(code);
    assert(text);
    assert(list);

    Dig* digits = dig_list_ctor(dig_amt, list);
    make_dig_list(digits, dig_amt, code, list);

    for (int i = 0; i < dig_amt; i++)
        printf("VALUE %d TYPE %d\n", digits[i].index, digits[i].type);
    printf("\n-----------\n");

    LabelFunc* jmp_addresses = labels_funcs_ctor(digits, dig_amt,  list);

    for (int i = 0; i < dig_amt; i++)
        printf("VALUE %d TYPE %d\n", jmp_addresses[i].target_ip, jmp_addresses[i].type);
    printf("\n-----------\n");

    size_t ip = 0;
    int sprintf_res = 0;

    while(ip < dig_amt)
    {
        add_label_func(ip, jmp_addresses, text, list);
        RETURN_VOID

        size_t com_num = find_com(code[ip], list);
        RETURN_VOID

        sprintf_res = sprintf_s(text, MAX_FILE_SIZE, "%s%s", text, bunch_of_commands[com_num].cmd_str);
        SPRINTF_CHECK_VOID

        add_args(digits, jmp_addresses, text, &ip, com_num, list);

        ip++;
    }

    printf("ENTER NAME FOR YOUR NEW ASM FILE\n");
    char asm_file_name[MAX_STR_LEN] = {};
    scanf("%s", asm_file_name);

    fill_input_file(asm_file_name, text, list);
    free(text);

    label_funcs_dtor(jmp_addresses);
    dig_list_dtor(digits);
}

Dig* dig_list_ctor(size_t size, ErrList *const list)
{
    assert(list);

    Dig* digits = (Dig*)calloc(size, sizeof(Dig));
    ALLOCATION_CHECK_PTR(digits)

    return digits;
}

void dig_list_dtor(Dig *const digits)
{
    free(digits);
}

void make_dig_list(Dig *const digits, size_t size, int *const code, ErrList *const list)
{
    assert(digits);
    assert(code);
    assert(list);

    size_t ip = 0;
    
    while (ip < size)
    {
        size_t com_num = find_com(code[ip], list);
        RETURN_VOID

        digits[ip].index = code[ip];
        digits[ip].type = COM;

        for (size_t arg = 0; arg < bunch_of_commands[com_num].arg_amt; arg++)
        {
            ip++;
            if (code[ip - 1] != CALL_A)
            {
                digits[ip].index = code[ip];
                digits[ip].type = ARG_TYPE;
                ip++;
            }

            digits[ip].index = code[ip];
            digits[ip].type = THE_ARG;
        }

        ip++;
    }
}

LabelFunc* labels_funcs_ctor(Dig *const digits, size_t size, ErrList *const list)
{
    assert(digits);
    assert(list);

    size_t ip = 0;
    size_t jmp_cnt = 0;

    LabelFunc* jmp_addresses = (LabelFunc*)calloc(MAX_JMPS, sizeof(LabelFunc));
    ALLOCATION_CHECK_PTR(jmp_addresses)

    while (ip < size)
    {
        bool is_jmp = false;

        for (size_t i = 0; i < JMP_CMD_AMT; i++)
        {
            if (digits[ip].index == jumps_cmds[i] && digits[ip].type == COM)
                is_jmp = true;
        }
        
        if (is_jmp)
        {
            if (digits[ip].index == CALL_A)
            {   
                ip++;
                jmp_addresses[jmp_cnt].target_ip = digits[ip].index;
                jmp_addresses[jmp_cnt].type = FUNC_JMP;
            } 
            else
            {
                ip += 2;
                jmp_addresses[jmp_cnt].target_ip = digits[ip].index;
                jmp_addresses[jmp_cnt].type = LABEL_JMP;
            }
                
            jmp_cnt++;
        }
        else
        {
            ip++;
        }
    }

    return jmp_addresses;
}


void label_funcs_dtor(LabelFunc *const jmp_addresses)
{
    free(jmp_addresses);
}

void add_label_func(size_t ip, LabelFunc *const jmp_addresses, char *const text, ErrList *const list)
{
    assert(jmp_addresses);
    assert(text);
    assert(list);

    bool is_label_func = false;
    size_t jmp_num = find_jmp(jmp_addresses, ip, list);
    if (jmp_num != ERROR_VALUE_SIZE_T);

    if (is_label_func)
    {
        int sprintf_res = 0;

        if (jmp_addresses[jmp_num].type == FUNC_JMP)
            sprintf_res = sprintf_s(text, MAX_FILE_SIZE, "%sFUNC%d\n", text, jmp_num);
        else
            sprintf_res = sprintf_s(text, MAX_FILE_SIZE, "%sLABEL%d:\n", text, jmp_num);
        SPRINTF_CHECK_VOID
    }
    else
        return;
}

size_t find_com(int dig, ErrList *const list)
{
    assert(list);

    for (size_t com = 0; com < CMD_AMT; com++)
    {
        if (dig == bunch_of_commands[com].cmd_num)
        return com;
    }

    ERROR(COM_ERROR)
    return ERROR_VALUE_SIZE_T;
}

void add_args(Dig *const digits, LabelFunc *const jmp_addresses, char *const text, size_t *const ip, size_t com_num, ErrList *const list)
{
    assert(digits);
    assert(jmp_addresses);
    assert(text);
    assert(ip);
    assert(list);

    (*ip)++;

    int sprintf_res = 0;

    if (digits[*ip].type == ARG_TYPE)
    {
        (*ip)++;
        switch(digits[*ip].index)
        {
            case ARG_NUM:
                sprintf_res = sprintf_s(text, MAX_FILE_SIZE, "%s %d\n", text, digits[*ip].index);
                break;
            case ARG_LAB:
            {
                size_t jmp_num = find_jmp(jmp_addresses, *ip, list);
                sprintf_res = sprintf_s(text, MAX_FILE_SIZE, "%s LABEL%d:\n", text, jmp_num);
                break;
            }    
            case ARG_RAM:
                sprintf_res = sprintf_s(text, MAX_FILE_SIZE, "%s [%d]\n", text, digits[*ip].index);
                break;
        }
        
    }
    else if (digits[*ip].type == ARG)
    {
        size_t jmp_num = find_jmp(jmp_addresses, *ip, list);
        sprintf_res = sprintf_s(text, MAX_FILE_SIZE, "%s FUNC%d:\n", text, jmp_num);
    }
    else 
    {
        (*ip)--;
        sprintf_res = sprintf_s(text, MAX_FILE_SIZE, "%s\n", text);
    }
    SPRINTF_CHECK_VOID
}

size_t find_jmp(LabelFunc *const jmp_addresses, int value, ErrList *const list)
{
    assert(jmp_addresses);
    assert(list);

    size_t jmp_num = ERROR_VALUE_SIZE_T;

    for (size_t i = 0; i < MAX_JMPS; i++)
    {
        if (value == jmp_addresses[i].target_ip)
            jmp_num = i;
    }

    return jmp_num;
}