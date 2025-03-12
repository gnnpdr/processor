#include <stdio.h>

#include "disassembly.h"

static Dig* dig_list_ctor(size_t size, ErrList *const list);
static void make_dig_list(Dig *const digits, size_t size, int *const code, ErrList *const list);
static void dig_list_dtor(Dig *const digits);

static LabelFunc* labels_funcs_ctor(Dig *const digits, size_t size, ErrList *const list);
static void label_funcs_dtor(LabelFunc *const jmp_addresses);
static void add_label_func(size_t ip, LabelFunc *const jmp_addresses, char *const text, ErrList *const list);

static size_t find_com(int dig, ErrList *const list);
static size_t find_jmp(LabelFunc *const jmp_addresses, int cur_ip, ErrList *const list);
static void add_args(Dig *const digits, LabelFunc *const jmp_addresses, char *const text, size_t *const ip, size_t com_num, ErrList *const list);

//-------------------------COMMON----------------------

void disassembly(int *const code, char *const text, size_t dig_amt, ErrList *const list)
{
    assert(code);
    assert(text);
    assert(list);

    Dig* digits = dig_list_ctor(dig_amt, list);
    make_dig_list(digits, dig_amt, code, list);

    LabelFunc* jmp_addresses = labels_funcs_ctor(digits, dig_amt,  list);

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

    printf("ENTER NAME FOR YOUR NEW ASM FILE(DISASM)\n");
    char asm_file_name[MAX_STR_LEN] = {};
    scanf("%s", asm_file_name);

    fill_input_file(asm_file_name, text, list);
    free(text);

    label_funcs_dtor(jmp_addresses);
    dig_list_dtor(digits);
}

//--------------------DIG-LIST------------------------------------

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

        if (bunch_of_commands[com_num].has_args)
        {
            ip++;

            digits[ip].index = code[ip];
            digits[ip].type = ARG_TYPE;
            ip++;

            digits[ip].index = code[ip];
            digits[ip].type = THE_ARG;
        }

        ip++;
    }
}

//---------------------------LABELS/FUNCS-MARX(ENGELS)------------------------------------

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
                jmp_addresses[jmp_cnt].type = FUNC_JMP;
            else
                jmp_addresses[jmp_cnt].type = LABEL_JMP;

            ip += 2;
            jmp_addresses[jmp_cnt].target_ip = digits[ip].index;
                
            jmp_cnt++;

        }
        else
            ip++;
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

    if (ip == 0)
        return;

    bool is_label_func = false;
    size_t jmp_num = find_jmp(jmp_addresses, ip - 1, list);
    
    if (jmp_num != ERROR_VALUE_SIZE_T)
        is_label_func = true;
        
    if (!is_label_func)
    {
        jmp_num = find_jmp(jmp_addresses, ip, list);
        if (jmp_num != ERROR_VALUE_SIZE_T && jmp_addresses[jmp_num].type == FUNC_JMP)
            is_label_func = true;
    }
        
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

//--------------------------ARGS------------------------------------------------

void add_args(Dig *const digits, LabelFunc *const jmp_addresses, char *const text, size_t *const ip, size_t com_num, ErrList *const list)
{
    assert(digits);
    assert(jmp_addresses);
    assert(text);
    assert(ip);
    assert(list);

    (*ip) += 2;

    int sprintf_res = 0;
    if (digits[(*ip) - 1].type == COM)
    {
        (*ip) -= 2;
        sprintf_res = sprintf_s(text, MAX_FILE_SIZE, "%s\n", text);
        SPRINTF_CHECK_VOID
        return;
    }

    switch(digits[(*ip) - 1].index)
    {
        case ARG_NUM:
            sprintf_res = sprintf_s(text, MAX_FILE_SIZE, "%s %d\n", text, digits[*ip].index);
            break;
        case ARG_LAB:
        {   
            size_t jmp_num = find_jmp(jmp_addresses, digits[*ip].index, list);
            sprintf_res = sprintf_s(text, MAX_FILE_SIZE, "%s LABEL%d:\n", text, jmp_num);
            break;
        }    
        case ARG_RAM:
            sprintf_res = sprintf_s(text, MAX_FILE_SIZE, "%s [%d]\n", text, digits[*ip].index);
            break;
        case ARG_FUNC:
        {
            size_t jmp_num = find_jmp(jmp_addresses, digits[*ip].index, list);
            sprintf_res = sprintf_s(text, MAX_FILE_SIZE, "%s FUNC%d\n", text, jmp_num);
            break;
        }
    }
}