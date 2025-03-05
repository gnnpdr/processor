#ifndef DIS_H
#define DIS_H

#include "..\input\input.h"
#include "..\assembly\asm_struct.h"

enum JmpType
{
    LABEL_JMP,
    FUNC_JMP
};

struct LabelFunc
{
    size_t target_ip;
    JmpType type;
};

enum DigType
{
    COM,
    ARG_TYPE,
    THE_ARG
};

struct Dig
{
    int index;
    DigType type;
};

static const size_t JMP_CMD_AMT = 8;

static const CommandsNums jumps_cmds[JMP_CMD_AMT] = {JA_A,
                                                    JAE_A,
                                                    JB_A,
                                                    JBE_A,
                                                    JE_A,
                                                    JNE_A,
                                                    JMP_A,
                                                    CALL_A};

static const size_t MAX_JMPS = 50;

void disassembly(int *const code, char *const text, size_t dig_amt, ErrList *const list);

#endif //DIS_H