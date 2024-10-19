#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>

#include "compiler.h"
// intercept-build
// pointer to functions

int Compile(const char * InFileName, const char * OutFileName)
{
    struct stat In;

    FILE * InFile = fopen(InFileName, "rb");
    if (ferror(InFile)) return FILE_OPEN_ERROR;

    FILE * OutFile = fopen(OutFileName, "wb");
    if (ferror(OutFile)) return FILE_OPEN_ERROR;

    stat(InFileName, &In);
    size_t InSize = In.st_size + In.st_size / 2;

    char * InBuf = (char*) calloc(InSize, sizeof(char));
    if (!InBuf) return ALLOCATE_MEMORY_ERROR;
    fread(InBuf, sizeof(char), InSize, InFile);

    char * ptr = InBuf;

    spu code = {};
    code.array = (double*) calloc(InSize + 2, sizeof(double));
    code.array += HEADER_SIZE;
    code.size = InSize;

    if (!code.array) return ALLOCATE_MEMORY_ERROR;

    while (1)
    {
        char command[DEF_SIZE] = "";
        char func[DEF_SIZE] = "";
        char reg[DEF_SIZE] = "";
        double val = pzn;

        if (sscanf(ptr, "%[^\n]s", command) == -1) break;
        ptr += strlen(command) + 1;

        sscanf(command, "%s", func);

        int EnumFunc = CommandToEnum(func);

        switch (EnumFunc)
        {
            case CMD_PUSH:
                PushAnalyzeCompiler(&code, command);
                break;
            case CMD_POP:
                sscanf(command, "%s %s", func, reg);
                if (reg[0] != 0)
                {
                    code.array[code.ip++] = CMD_POPR;
                    code.array[code.ip++] = CommandToEnum(reg);
                    break;
                }
                code.array[code.ip++] = CMD_POP;
                CleanLine(reg, DEF_SIZE);
                break;

            case CMD_JMP:
                JmpAnalyzeCompiler(&code, command);
                break;
            case CMD_JA:
                JmpAnalyzeCompiler(&code, command);
                break;
            case CMD_JB:
                JmpAnalyzeCompiler(&code, command);
                break;
            case CMD_JAE:
                JmpAnalyzeCompiler(&code, command);
                break;
            case CMD_JBE:
                JmpAnalyzeCompiler(&code, command);
                break;
            case CMD_JE:
                JmpAnalyzeCompiler(&code, command);
                break;
            case CMD_JNE:
                JmpAnalyzeCompiler(&code, command);
                break;

            case CMD_ADD:
                code.array[code.ip++] = CMD_ADD;
                break;
            case CMD_SUB:
                code.array[code.ip++] = CMD_SUB;
                break;
            case CMD_DIV:
                code.array[code.ip++] = CMD_DIV;
                break;
            case CMD_MUL:
                code.array[code.ip++] = CMD_MUL;
                break;
            case CMD_SQRT:
                code.array[code.ip++] = CMD_SQRT;
                break;
            case CMD_SIN:
                code.array[code.ip++] = CMD_SIN;
                break;
            case CMD_COS:
                code.array[code.ip++] = CMD_COS;
                break;

            case CMD_IN:
                code.array[code.ip++] = CMD_IN;
                break;
            case CMD_OUT:
                code.array[code.ip++] = CMD_OUT;
                break;
            case CMD_DUMP:
                code.array[code.ip++] = CMD_DUMP;
                break;
            case CMD_HLT:
                code.array[code.ip++] = CMD_HLT;
                break;

            default:
                fprintf(stderr, "wrong code %s\n", func);
                break;
        }
    }

    code.size = code.ip;
    code.ip = 0;

    code.array -= HEADER_SIZE;
    code.array[0] = 0xACAB;
    code.array[1] = code.size;
    fwrite(code.array, code.size + HEADER_SIZE, sizeof(double), OutFile);

    free(InBuf);
    free(code.array);
    fclose(InFile);
    fclose(OutFile);

    return SUCCESS;
}

int RunProgram(const char * RunFileName)
{
    struct stat Run;
    FILE * RunFile = fopen(RunFileName, "rb");
    spu code = {};

    stat(RunFileName, &Run);

    code.size = Run.st_size;
    code.array = (double*) calloc(code.size, 1);
    code.RAM = (double*) calloc(RAM_SIZE, sizeof(double));
    fread(code.array, code.size, sizeof(double), RunFile);
    code.array += HEADER_SIZE;

    double val1, val2, val3 = pzn;
    double * cell;

    BEGIN_CHECK
    STACK_CTOR(&code.stk, sizeof(double), 16);

    while (code.array[code.ip] != CMD_HLT)
    {
        switch((int) code.array[code.ip])
        {
            case CMD_PUSH:
                PushAnalyzeRun(&code);
                break;

            case CMD_PUSHR:
                cell = (double*)((size_t) code.reg + (size_t) *(code.array + code.ip + 1));
                val1 = *cell;
                STACK_PUSH(&code.stk, &val1);
                code.ip += 2;
                break;

            case CMD_POP:
                STACK_POP(&code.stk);
                code.ip++;
                break;

            case CMD_POPR:
                cell = (double*)((size_t) code.reg + (size_t) *(code.array + code.ip + 1));
                val1 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                *cell = val1;
                code.ip += 2;
                break;

            case CMD_JMP:
                code.ip = code.array[code.ip + 1];
                break;

            case CMD_JA:
                val1 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                val2 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                if (val2 > val1)
                    code.ip = code.array[code.ip + 1];
                else
                {
                    STACK_PUSH(&code.stk, &val2);
                    STACK_PUSH(&code.stk, &val1);
                    code.ip++;
                }
                break;
            case CMD_JB:
                val1 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                val2 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                if (val2 < val1)
                    code.ip = code.array[code.ip + 1];
                else
                {
                    STACK_PUSH(&code.stk, &val2);
                    STACK_PUSH(&code.stk, &val1);
                    code.ip++;
                }
                break;
            case CMD_JAE:
                val1 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                val2 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                if (val2 >= val1)
                    code.ip = code.array[code.ip + 1];
                else
                {
                    STACK_PUSH(&code.stk, &val2);
                    STACK_PUSH(&code.stk, &val1);
                    code.ip++;
                }
                break;
            case CMD_JBE:
                val1 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                val2 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                if (val2 <= val1)
                    code.ip = code.array[code.ip + 1];
                else
                {
                    STACK_PUSH(&code.stk, &val2);
                    STACK_PUSH(&code.stk, &val1);
                    code.ip++;
                }
                break;
            case CMD_JE:
                val1 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                val2 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                if (val2 == val1)
                    code.ip = code.array[code.ip + 1];
                else
                {
                    STACK_PUSH(&code.stk, &val2);
                    STACK_PUSH(&code.stk, &val1);
                    code.ip++;
                }
                break;
            case CMD_JNE:
                val1 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                val2 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                if (val2 != val1)
                    code.ip = code.array[code.ip + 1];
                else
                {
                    STACK_PUSH(&code.stk, &val2);
                    STACK_PUSH(&code.stk, &val1);
                    code.ip++;
                }
                break;
            case CMD_ADD:
                val1 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                val2 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                val3 = val2 + val1;
                STACK_PUSH(&code.stk, &val3);
                code.ip++;
                break;
            case CMD_SUB:
                val1 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                val2 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                val3 = val2 - val1;
                STACK_PUSH(&code.stk, &val3);
                code.ip++;
                break;
            case CMD_MUL:
                val1 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                val2 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                val3 = val2 * val1;
                STACK_PUSH(&code.stk, &val3);
                code.ip++;
                break;
            case CMD_DIV:
                val1 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                val2 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                val3 = val2 / val1;
                STACK_PUSH(&code.stk, &val3);
                code.ip++;
                break;
            case CMD_SQRT:
                val1 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                val3 = sqrt(val1);
                STACK_PUSH(&code.stk, &val3);
                code.ip++;
                break;
            case CMD_SIN:
                val1 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                val3 = sin(val1);
                STACK_PUSH(&code.stk, &val3);
                code.ip++;
                break;
            case CMD_COS:
                val1 = *(double*) (STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                val3 = cos(val1);
                STACK_PUSH(&code.stk, &val3);
                code.ip++;
                break;
            case CMD_OUT:
                fprintf(stdout, "%lg\n", *(double*) STACK_TOP(&code.stk));
                STACK_POP(&code.stk);
                code.ip++;
                break;
            case CMD_HLT:
                code.ip++;
                break;
            default:
                code.ip++;
                break;
        }

    }

    code.array -= HEADER_SIZE;
    free(code.array);
    fclose(RunFile);
}

void AsmDump(spu * code)
{
    fprintf(stdout, "ASM_SIZE = %d ASM_IP = %d\n", code->size, code->ip);

    for (int i = 0; i < code->size; i++)    fprintf(stdout, "%2lg ", code->array[i]);

    fprintf(stdout, "\n");

    for (int i = 0; i < code->ip - 1; i++)  fprintf(stdout, " ");

    fprintf(stdout, " ^");
    getchar();
}

void JmpAnalyzeCompiler(spu * code, const char * command)
{
    char func[DEF_SIZE] = "";
    double val = 0;

    sscanf(command, "%s %lg", func, &val);
    code->array[code->ip++] = CommandToEnum(func);
    code->array[code->ip++] = val;
}

int PushAnalyzeCompiler(spu * code, const char * command)
{
    double val = pzn;
    char func[100] = "";
    char reg[100] = "";
    char sign[10] = "";
    const char * symb = 0;
    code->array[code->ip++] = CMD_PUSH;

    if ((symb = strchr(command, '+')) == NULL)
    {
        if ((symb = strchr(command, '[')) == NULL)
        {
            sscanf(command, "%s %lg", func, &val);
            if (val != pzn)
            {
                code->array[code->ip++] = 1;
                code->array[code->ip++] = val;
            }
            else
            {
                code->array[code->ip++] = 2;
                sscanf("%s %s", func, reg);
                code->array[code->ip++] = CommandToEnum(reg);
            }
        }
        else
        {
            sscanf(symb,  "%lg", &val);
            if (val != pzn)
            {
                code->array[code->ip++] = 5;
                code->array[code->ip++] = val;
            }
            else
            {
                code->array[code->ip++] = 6;
                sscanf(command, "%s %s", func, reg);
                reg[strlen(reg) - 1] = 0;
                code->array[code->ip++] = CommandToEnum(reg);
            }
        }
    }
    else
    {
        if ((symb = strchr(command, '[')) == NULL)
        {
            code->array[code->ip++] = 3;
            sscanf(command, "%s %lg", func, &val);
            if (val == pzn)
            {
                sscanf(command, "%s %s %s %lg", func, reg, sign, &val);
                code->array[code->ip++] = val;
                code->array[code->ip++] = CommandToEnum(reg);
            }
            else
            {
                sscanf(command, "%s %lg %s %s", func, &val, sign, reg);
                code->array[code->ip++] = val;
                code->array[code->ip++] = CommandToEnum(reg);
            }
        }
        else
        {
            code->array[code->ip++] = 7;
            sscanf(command, "%s %lg", func, &val);
            if (val == pzn)
            {
                sscanf(command, "%s %s %s %lg", func, reg, sign, &val);
                code->array[code->ip++] = val;
                code->array[code->ip++] = CommandToEnum(reg);
            }
            else
            {
                sscanf(command, "%s %lg %s %s", func, &val, sign, reg);
                code->array[code->ip++] = val;
                reg[strlen(reg) - 1] = 0;
                code->array[code->ip++] = CommandToEnum(reg);
            }

        }
    }

    return 0;
}

int PushAnalyzeRun(spu * code)
{
    BEGIN_CHECK
    double arg = GetArgPush(code);
    STACK_PUSH(&code->stk, &arg);
    return 0;
}

double GetArgPush(spu * code)
{
    code->ip++;
    int ArgType = code->array[code->ip++];
    double result = 0;

    if (ArgType & 1) result = code->array[code->ip++];
    if (ArgType & 2) result += *(double*)((char*)code->reg + *(char*)((char*)code->array + (code->ip++)));
    if (ArgType & 4) result = code->RAM[(int) result];

    return result;
}

void CleanLine(char * line, size_t size)
{
    for (int i = 0; line[i] != 0; i++)
        line[i] = 0;
}

int CommandToEnum(char * command)
{
    if (strcmp(command, "push") == 0)       return CMD_PUSH;
    if (strcmp(command, "pop")  == 0)       return CMD_POP;
    if (strcmp(command, "jmp")  == 0)       return CMD_JMP;
    if (strcmp(command, "ja")   == 0)       return CMD_JA;
    if (strcmp(command, "jb")   == 0)       return CMD_JB;
    if (strcmp(command, "jae")  == 0)       return CMD_JAE;
    if (strcmp(command, "jbe")  == 0)       return CMD_JBE;
    if (strcmp(command, "je")   == 0)       return CMD_JE;
    if (strcmp(command, "jne")  == 0)       return CMD_JNE;
    if (strcmp(command, "add")  == 0)       return CMD_ADD;
    if (strcmp(command, "sub")  == 0)       return CMD_SUB;
    if (strcmp(command, "div")  == 0)       return CMD_DIV;
    if (strcmp(command, "mul")  == 0)       return CMD_MUL;
    if (strcmp(command, "sqrt") == 0)       return CMD_SQRT;
    if (strcmp(command, "sin")  == 0)       return CMD_SIN;
    if (strcmp(command, "cos")  == 0)       return CMD_COS;
    if (strcmp(command, "dump") == 0)       return CMD_DUMP;
    if (strcmp(command, "in")   == 0)       return CMD_IN;
    if (strcmp(command, "out")  == 0)       return CMD_OUT;
    if (strcmp(command, "hlt")  == 0)       return CMD_HLT;

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




