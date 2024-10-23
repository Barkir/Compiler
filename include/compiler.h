#ifndef COMPILER_H
#define COMPILER_H

#include <math.h>
#include "stack.h"
#include "hash.h"

const size_t DEF_SIZE = 100;
const size_t HEADER_SIZE = 2;
const size_t RAM_SIZE = 4096;
const size_t DEF_STK_SIZE = 16;

enum compile_errors
{
    SUCC,
    FILE_OPEN_ERROR,
    ALLOCATE_MEMORY_ERROR,
    SIZE_ERROR,
    COMPILE_ERROR,
    RUN_ERROR
};

enum cmd_options
{
    CMD_COMPILE,
    CMD_DEF_COMPILE,
    CMD_RUN,
    CMD_DEFAULT
};

enum compile_instruction
{
    CMD_PUSH = 1,
    CMD_POP = 2,
    CMD_ADD = 3,
    CMD_SUB = 4,
    CMD_DIV = 5,
    CMD_MUL = 6,
    CMD_SQRT = 7,
    CMD_SIN = 8,
    CMD_COS = 9,
    CMD_DUMP = 10,
    CMD_IN = 11,
    CMD_OUT = 12,
    CMD_JMP = 13,
    CMD_JA = 14,
    CMD_JAE = 15,
    CMD_JB = 16,
    CMD_JBE = 17,
    CMD_JE = 18,
    CMD_JNE = 19,
    CMD_PUSHR = 20,
    CMD_POPR = 21,
    CMD_HLT = -1
};

enum reg
{
    AX = 1,
    BX = 2,
    CX = 3,
    DX = 4,
    EX = 5,
    FX = 6,
    GX = 7,
    RX = 8
};

struct spu
{
    Stack stk;
    double * array;
    size_t size; // void *
    size_t ip;

    double * RAM;
    double reg[10];

};




int Compile(const char * InFileName, const char * OutFileName);
int RunProgram(const char * RunFileName);

void AsmDump(spu * code);

#endif
