#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "processor.h"
#include "analyze_proc.h"

int RunProgram(const char * RunFileName)
{
    struct stat Run;
    FILE * RunFile = fopen(RunFileName, "rb");
    stat(RunFileName, &Run);

    spu code = {};
    if ((code.size = Run.st_size) == 0) return SIZE_ERROR;

    code.array  = (double*) calloc(code.size, 1);
    code.RAM    = (double*) calloc(RAM_SIZE, sizeof(double)); // nullptr

    fread(code.array, code.size, sizeof(double), RunFile);
    code.array += HEADER_SIZE;

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
