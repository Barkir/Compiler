#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

#include "compiler.h"
#include "analyzers.h"

// intercept-build
// pointer to functions

int Compile(const char * InFileName, const char * OutFileName)
{
    struct stat In;

    FILE * InFile = fopen(InFileName, "rb+");
    if (ferror(InFile)) return FILE_OPEN_ERROR;

    FILE * OutFile = fopen(OutFileName, "wb+");
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

        if (sscanf(ptr, "%[^\n]s", command) == -1) break;
        ptr += strlen(command) + 1;

        sscanf(command, "%s", func);

        int EnumFunc = CommandToEnum(func);

        switch (EnumFunc)
        {
            case CMD_PUSH:  PushAnalyzeCompiler(&code, command);
                            break;

            case CMD_POP:   PopAnalyzeCompiler(&code, command);
                            break;

            case CMD_JMP:   JmpAnalyzeCompiler(&code, command);
                            break;

            case CMD_JA:    JmpAnalyzeCompiler(&code, command);
                            break;

            case CMD_JB:    JmpAnalyzeCompiler(&code, command);
                            break;

            case CMD_JAE:   JmpAnalyzeCompiler(&code, command);
                            break;

            case CMD_JBE:   JmpAnalyzeCompiler(&code, command);
                            break;

            case CMD_JE:    JmpAnalyzeCompiler(&code, command);
                            break;

            case CMD_JNE:   JmpAnalyzeCompiler(&code, command);
                            break;

            case CMD_ADD:   code.array[code.ip++] = CMD_ADD;
                            break;

            case CMD_SUB:   code.array[code.ip++] = CMD_SUB;
                            break;

            case CMD_DIV:   code.array[code.ip++] = CMD_DIV;
                            break;

            case CMD_MUL:   code.array[code.ip++] = CMD_MUL;
                            break;

            case CMD_SQRT:  code.array[code.ip++] = CMD_SQRT;
                            break;

            case CMD_SIN:   code.array[code.ip++] = CMD_SIN;
                            break;

            case CMD_COS:   code.array[code.ip++] = CMD_COS;
                            break;

            case CMD_IN:    code.array[code.ip++] = CMD_IN;
                            break;

            case CMD_OUT:   code.array[code.ip++] = CMD_OUT;
                            break;

            case CMD_DUMP:  code.array[code.ip++] = CMD_DUMP;
                            break;

            case CMD_HLT:   code.array[code.ip++] = CMD_HLT;
                            break;

            default:        fprintf(stderr, "wrong code, won't compile! %s\n", func);
                            code.array -= HEADER_SIZE;
                            free(InBuf);
                            free(code.array);
                            fclose(InFile);
                            fclose(OutFile);
                            return COMPILE_ERROR;
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
    stat(RunFileName, &Run);

    spu code = {};
    if ((code.size = Run.st_size) == 0) return SIZE_ERROR;

    code.array  = (double*) calloc(code.size, 1);
    code.RAM    = (double*) calloc(RAM_SIZE, sizeof(double));

    fread(code.array, code.size, sizeof(double), RunFile);
    code.array += HEADER_SIZE;

    double val1, val2, val3 = pzn;

    BEGIN_CHECK
    STACK_CTOR(&code.stk, sizeof(double), DEF_STK_SIZE);

    while (code.array[code.ip] != CMD_HLT)
    {
        switch((int) code.array[code.ip])
        {
            case CMD_PUSH:  PushAnalyzeRun(&code);
                            break;

            case CMD_POP:   PopAnalyzeRun(&code);
                            break;

            case CMD_JMP:   JmpAnalyzeRun(&code);
                            break;

            case CMD_JA:    JmpAnalyzeRun(&code);
                            break;

            case CMD_JB:    JmpAnalyzeRun(&code);
                            break;

            case CMD_JAE:   JmpAnalyzeRun(&code);
                            break;

            case CMD_JBE:   JmpAnalyzeRun(&code);
                            break;

            case CMD_JE:    JmpAnalyzeRun(&code);
                            break;

            case CMD_JNE:   JmpAnalyzeRun(&code);
                            break;

            case CMD_ADD:   MathAnalyzeRun(&code);
                            break;

            case CMD_SUB:   MathAnalyzeRun(&code);
                            break;

            case CMD_MUL:   MathAnalyzeRun(&code);
                            break;

            case CMD_DIV:   MathAnalyzeRun(&code);
                            break;

            case CMD_SQRT:  MathAnalyzeRun(&code);
                            break;

            case CMD_SIN:   MathAnalyzeRun(&code);
                            break;

            case CMD_COS:   MathAnalyzeRun(&code);
                            break;

            case CMD_OUT:   fprintf(stdout, "%lg\n", *(double*) STACK_TOP(&code.stk));
                            STACK_POP(&code.stk);
                            code.ip++;
                            break;

            case CMD_DUMP:  AsmDump(&code);
                            code.ip++;
                            break;

            case CMD_HLT:   code.ip++;
                            break;

            default:    fprintf(stderr, "WHAT THE HEEELLL NO COMMAND LIKE THIS BYEEE!\n");
                        return RUN_ERROR;
        }

    }
    code.array -= HEADER_SIZE;
    free(code.array);
    free(code.RAM);
    fclose(RunFile);

    return SUCCESS;
}

void AsmDump(spu * code)
{
    fprintf(stdout, "ASM_SIZE = %d ASM_IP = %d\n", code->size, code->ip);
    fprintf(stdout, "reg[%p]\n", code->reg);
    fprintf(stdout, "RAM[%p]\n", code->RAM);

    for (int i = 0; i < code->size; i++)    fprintf(stdout, "%2lg ", code->array[i]);

    fprintf(stdout, "\n");

    for (int i = 0; i < RAM_SIZE / 10; i++)
    {
        if ((i + 1) % 8 == 0) fprintf(stdout, "%lg\n", code->RAM[i]);
        else            fprintf(stdout, "%lg ", code->RAM[i]);
    }
}


