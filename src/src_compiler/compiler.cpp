#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "const_enum_struct.h"
#include "compiler.h"
#include "analyze_comp.h"
#include "analyze_string.h"
#include "labels.h"

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

    for (int i = 0; i < DEF_SIZE; i++) Labels[i].ip = -1;

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

            case CMD_LABEL: LabelAnalyzeCompiler(&code, command);
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

    ptr = InBuf;
    code.ip = 0;

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

            case CMD_LABEL: LabelAnalyzeCompiler(&code, command);
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

    return SUCC;
}


