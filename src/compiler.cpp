#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>

#include "compiler.h"
// intercept-build


int Compile(const char * InFileName, const char * OutFileName)
{
    struct stat In;

    FILE * InFile = fopen(InFileName, "rb");
    if (ferror(InFile)) return FILE_OPEN_ERROR;

    FILE * OutFile = fopen(OutFileName, "wb");
    if (ferror(OutFile)) return FILE_OPEN_ERROR;

    stat(InFileName, &In);
    size_t InSize = In.st_size;

    char * InBuf = (char*) calloc(InSize, sizeof(char));
    if (!InBuf) return ALLOCATE_MEMORY_ERROR;
    fread(InBuf, sizeof(char), InSize, InFile);

    char * ptr = InBuf;

    asm_code code = {};
    code.asm_arr = (double*) calloc(InSize + 2, sizeof(double));
    code.asm_arr += 2;
    code.asm_size = InSize;

    if (!code.asm_arr) return ALLOCATE_MEMORY_ERROR;

    while (1)
    {
        char command[DEF_SIZE] = "";
        char func[DEF_SIZE] = "";
        char reg[DEF_SIZE] = "";
        double val = pzn;

        if (sscanf(ptr, "%[^\n]s", command) == -1) break;
        ptr += strlen(command) + 1;

        sscanf(command, "%s", func);

        switch (CommandToEnum(func))
        {
            case PUSH:
                sscanf(command, "%s %lg", func, &val);
                if (val == pzn)
                {
                    sscanf(command, "%s %s", func, reg);
                    code.asm_arr[code.ip++] = PUSHR;
                    code.asm_arr[code.ip++] = CommandToEnum(reg);
                    break;
                }
                code.asm_arr[code.ip++] = PUSH;
                code.asm_arr[code.ip++] = val;
                CleanLine(reg, DEF_SIZE);
                break;
            case POP:
                sscanf(command, "%s %s", func, reg);
                if (reg[0] != 0)
                {
                    code.asm_arr[code.ip++] = POPR;
                    code.asm_arr[code.ip++] = CommandToEnum(reg);
                    break;
                }
                code.asm_arr[code.ip++] = POP;
                CleanLine(reg, DEF_SIZE);
                break;
            case JMP:
                sscanf(command, "%s %lg", func, &val);
                code.asm_arr[code.ip++] = JMP;
                code.asm_arr[code.ip++] = val;
                break;
            case JA:
                sscanf(command, "%s %lg", func, &val);
                code.asm_arr[code.ip++] = JA;
                code.asm_arr[code.ip++] = val;
                break;
            case JB:
                sscanf(command, "%s %lg", func, &val);
                code.asm_arr[code.ip++] = JB;
                code.asm_arr[code.ip++] = val;
                break;
            case JAE:
                sscanf(command, "%s %lg", func, &val);
                code.asm_arr[code.ip++] = JAE;
                code.asm_arr[code.ip++] = val;
                break;
            case JBE:
                sscanf(command, "%s %lg", func, &val);
                code.asm_arr[code.ip++] = JBE;
                code.asm_arr[code.ip++] = val;
                break;
            case JE:
                sscanf(command, "%s %lg", func, &val);
                code.asm_arr[code.ip++] = JE;
                code.asm_arr[code.ip++] = val;
                break;
            case JNE:
                sscanf(command, "%s %lg", func, &val);
                code.asm_arr[code.ip++] = JNE;
                code.asm_arr[code.ip++] = val;
                break;
            case ADD:
                code.asm_arr[code.ip++] = ADD;
                break;
            case SUB:
                code.asm_arr[code.ip++] = SUB;
                break;
            case DIV:
                code.asm_arr[code.ip++] = DIV;
                break;
            case MUL:
                code.asm_arr[code.ip++] = MUL;
                break;
            case SQRT:
                code.asm_arr[code.ip++] = SQRT;
                break;
            case SIN:
                code.asm_arr[code.ip++] = SIN;
                break;
            case COS:
                code.asm_arr[code.ip++] = COS;
                break;
            case DUMP:
                code.asm_arr[code.ip++] = DUMP;
                break;
            case IN:
                code.asm_arr[code.ip++] = IN;
                break;
            case OUT:
                code.asm_arr[code.ip++] = OUT;
                break;
            case HLT:
                code.asm_arr[code.ip++] = HLT;
                break;
            default:
                fprintf(stderr, "wrong code %s\n", func);
                break;
        }
    }

    code.asm_size = code.ip;
    code.ip = 0;

    code.asm_arr -= 2;
    code.asm_arr[0] = 0xACAB;
    code.asm_arr[1] = code.asm_size;
    fwrite(code.asm_arr, code.asm_size + 2, sizeof(double), OutFile);

    free(InBuf);
    free(code.asm_arr);
    fclose(InFile);
    fclose(OutFile);

    return SUCCESS;
}

int RunProgram(const char * RunFileName)
{
    struct stat Run;

    FILE * RunFile = fopen(RunFileName, "rb");
    asm_code code = {};

    stat(RunFileName, &Run);

    code.asm_size = Run.st_size;
    code.asm_arr = (double*) calloc(code.asm_size + 2, sizeof(double));
    fread(code.asm_arr, code.asm_size, sizeof(double), RunFile);
    code.asm_arr += 2;

    double val1, val2, val3 = pzn;
    double * cell;

    Stack stk = code.stk;
    BEGIN_CHECK
    STACK_CTOR(&stk, sizeof(double), 16);

    while (code.asm_arr[code.ip] != HLT)
    {
        switch((int) code.asm_arr[code.ip])
        {
            case PUSH:
                STACK_PUSH(&stk, &(code.asm_arr[code.ip + 1]));
                code.ip += 2;
                break;

            case PUSHR:
                cell = (double*)((size_t) code.reg + (size_t) *(code.asm_arr + code.ip + 1));
                val1 = *cell;
                STACK_PUSH(&stk, &val1);
                code.ip += 2;
                break;

            case POP:
                STACK_POP(&stk);
                code.ip++;
                break;

            case POPR:
                cell = (double*)((size_t) code.reg + (size_t) *(code.asm_arr + code.ip + 1));
                val1 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                *cell = val1;
                code.ip += 2;
                break;

            case JMP:
                code.ip = code.asm_arr[code.ip + 1];
                break;
            case JA:
                val1 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                val2 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                if (val2 > val1)
                    code.ip = code.asm_arr[code.ip + 1];
                else
                {
                    STACK_PUSH(&stk, &val2);
                    STACK_PUSH(&stk, &val1);
                    code.ip++;
                }
                break;
            case JB:
                val1 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                val2 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                if (val2 < val1)
                    code.ip = code.asm_arr[code.ip + 1];
                else
                {
                    STACK_PUSH(&stk, &val2);
                    STACK_PUSH(&stk, &val1);
                    code.ip++;
                }
                break;
            case JAE:
                val1 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                val2 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                if (val2 >= val1)
                    code.ip = code.asm_arr[code.ip + 1];
                else
                {
                    STACK_PUSH(&stk, &val2);
                    STACK_PUSH(&stk, &val1);
                    code.ip++;
                }
                break;
            case JBE:
                val1 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                val2 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                if (val2 <= val1)
                    code.ip = code.asm_arr[code.ip + 1];
                else
                {
                    STACK_PUSH(&stk, &val2);
                    STACK_PUSH(&stk, &val1);
                    code.ip++;
                }
                break;
            case JE:
                val1 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                val2 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                if (val2 == val1)
                    code.ip = code.asm_arr[code.ip + 1];
                else
                {
                    STACK_PUSH(&stk, &val2);
                    STACK_PUSH(&stk, &val1);
                    code.ip++;
                }
                break;
            case JNE:
                val1 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                val2 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                if (val2 != val1)
                    code.ip = code.asm_arr[code.ip + 1];
                else
                {
                    STACK_PUSH(&stk, &val2);
                    STACK_PUSH(&stk, &val1);
                    code.ip++;
                }
                break;
            case ADD:
                val1 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                val2 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                val3 = val2 + val1;
                STACK_PUSH(&stk, &val3);
                code.ip++;
                break;
            case SUB:
                val1 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                val2 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                val3 = val2 - val1;
                STACK_PUSH(&stk, &val3);
                code.ip++;
                break;
            case MUL:
                val1 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                val2 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                val3 = val2 * val1;
                STACK_PUSH(&stk, &val3);
                code.ip++;
                break;
            case DIV:
                val1 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                val2 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                val3 = val2 / val1;
                STACK_PUSH(&stk, &val3);
                code.ip++;
                break;
            case SQRT:
                val1 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                val3 = sqrt(val1);
                STACK_PUSH(&stk, &val3);
                code.ip++;
                break;
            case SIN:
                val1 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                val3 = sin(val1);
                STACK_PUSH(&stk, &val3);
                code.ip++;
                break;
            case COS:
                val1 = *(double*) (STACK_TOP(&stk));
                STACK_POP(&stk);
                val3 = cos(val1);
                STACK_PUSH(&stk, &val3);
                code.ip++;
                break;
            case OUT:
                fprintf(stdout, "%lg\n", *(double*) STACK_TOP(&stk));
                STACK_POP(&stk);
                code.ip++;
                break;
            case HLT:
                code.ip++;
                break;
            default:
                code.ip++;
                break;
        }

    }

    code.asm_arr -= 2;
    free(code.asm_arr);
    fclose(RunFile);
}



int CommandToEnum(char * command)
{
    if (strcmp(command, "push") == 0)       return PUSH;
    if (strcmp(command, "pop")  == 0)       return POP;
    if (strcmp(command, "jmp")  == 0)       return JMP;
    if (strcmp(command, "ja")   == 0)       return JA;
    if (strcmp(command, "jb")   == 0)       return JB;
    if (strcmp(command, "jae")  == 0)       return JAE;
    if (strcmp(command, "jbe")  == 0)       return JBE;
    if (strcmp(command, "je")   == 0)       return JE;
    if (strcmp(command, "jne")  == 0)       return JNE;
    if (strcmp(command, "add")  == 0)       return ADD;
    if (strcmp(command, "sub")  == 0)       return SUB;
    if (strcmp(command, "div")  == 0)       return DIV;
    if (strcmp(command, "mul")  == 0)       return MUL;
    if (strcmp(command, "sqrt") == 0)       return SQRT;
    if (strcmp(command, "sin")  == 0)       return SIN;
    if (strcmp(command, "cos")  == 0)       return COS;
    if (strcmp(command, "dump") == 0)       return DUMP;
    if (strcmp(command, "in")   == 0)       return IN;
    if (strcmp(command, "out")  == 0)       return OUT;
    if (strcmp(command, "hlt")  == 0)       return HLT;

    if (strcmp(command, "AX")   == 0)       return AX;
    if (strcmp(command, "BX")   == 0)       return BX;
    if (strcmp(command, "CX")   == 0)       return CX;
    if (strcmp(command, "DX")   == 0)       return DX;
    if (strcmp(command, "EX")   == 0)       return EX;
    if (strcmp(command, "FX")   == 0)       return FX;
    if (strcmp(command, "GX")   == 0)       return GX;
    if (strcmp(command, "RX")   == 0)       return RX;
    return 0;

}

void CleanLine(char * line, size_t size)
{
    for (int i = 0; line[i] != 0; i++)
        line[i] = 0;
}


