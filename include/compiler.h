#ifndef COMPILER_H
#define COMPILER_H

#include <math.h>
#include "stack.h"
#include "hash.h"

const size_t DEF_SIZE = 100;

enum compile_errors
{
    SUCC,
    FILE_OPEN_ERROR,
    ALLOCATE_MEMORY_ERROR
};

enum compile_instruction
{
    PUSH = 1,
    POP = 2,
    ADD = 3,
    SUB = 4,
    DIV = 5,
    MUL = 6,
    SQRT = 7,
    SIN = 8,
    COS = 9,
    DUMP = 10,
    IN = 11,
    OUT = 12,
    JMP = 13,
    JA = 14,
    JAE = 15,
    JB = 16,
    JBE = 17,
    JE = 18,
    JNE = 19,
    PUSHR = 20,
    POPR = 21,
    HLT = -1
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

struct asm_code
{
    Stack stk;
    double * asm_arr;
    size_t asm_size;
    size_t ip;

    double reg[10];
};




int Compile(const char * InFileName, const char * OutFileName);
int CommandToEnum(char * command);
int RunProgram(const char * RunFileName);
void CleanLine(char * line, size_t size);

#endif
