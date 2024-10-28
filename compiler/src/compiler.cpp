#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "const_enum_struct.h"
#include "compiler.h"
#include "analyze_string.h"
#include "labels.h"
#include "analyze_comp.h"
#include "comp_struct_func.h"

int Compile(const char * InFileName, const char * OutFileName)
{
    struct stat In = {};

    FILE * InFile = fopen(InFileName, "rb+");
    if (ferror(InFile)) return FILE_OPEN_ERROR;

    FILE * OutFile = fopen(OutFileName, "wb+");
    if (ferror(OutFile)) return FILE_OPEN_ERROR;

    stat(InFileName, &In);
    size_t InSize = In.st_size + In.st_size / 2;

    char * InBuf = (char*) calloc(InSize, sizeof(char));
    if (!InBuf) return ALLOCATE_MEMORY_ERROR;

    fread(InBuf, sizeof(char), InSize, InFile);
    if (ferror(InFile)) return FILE_READ_ERROR;

    Spu code = {};

    code.array = (double*) calloc(InSize + HEADER_SIZE, sizeof(double));
    if (!code.array) return ALLOCATE_MEMORY_ERROR;

    code.array += HEADER_SIZE;
    code.size = InSize;

    for (int i = 0; i < DEF_SIZE; i++) Labels[i].ip = -1;

    if (CompileIter(InBuf, &code, InFile, OutFile)) return COMPILE_ERROR;
    if (CompileIter(InBuf, &code, InFile, OutFile)) return COMPILE_ERROR;

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

int CompileIter(char * InBuf, Spu * code, FILE * InFile, FILE * OutFile)
{

    char * ptr = InBuf;
    code->ip = 0;

    while (1)
    {
        char command[DEF_SIZE] = "";
        char func[DEF_SIZE] = "";

        if (sscanf(ptr, "%[^\n]s", command) == -1) break;
        ptr += strlen(command) + 1;

        sscanf(command, "%s", func);
        CompilerFunction comp = AnalyzeCompiler(func);
        if (comp.func(code, command) == CMD_DEFAULT)
        {
            printf("%s\n", command);
            code->array -= HEADER_SIZE;
            free(InBuf);
            free(code->array);
            fclose(InFile);
            fclose(OutFile);
            return COMPILE_ERROR;
        }
    }
    return SUCC;
}



