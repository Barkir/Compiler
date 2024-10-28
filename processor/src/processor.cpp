#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "processor.h"
#include "analyze_proc.h"
#include "proc_struct_func.h"

int RunProgram(const char * RunFileName)
{
    BEGIN_CHECK

    struct stat Run = {};
    FILE * RunFile = fopen(RunFileName, "rb");
    stat(RunFileName, &Run);

    Spu code = {};
    if ((code.size = Run.st_size) == 0) return SIZE_ERROR;

    if (!(code.array  = (double*) calloc(code.size, 1))) return ALLOCATE_MEMORY_ERROR;
    if (!(code.RAM    = (double*) calloc(RAM_SIZE, sizeof(double)))) return ALLOCATE_MEMORY_ERROR; // nullptr

    fread(code.array, code.size, sizeof(double), RunFile);
    if (ferror(RunFile)) return FILE_READ_ERROR;
    code.array += HEADER_SIZE;

    STACK_CTOR(&code.func_stk, sizeof(size_t), DEF_STK_SIZE);
    STACK_CTOR(&code.stk, sizeof(double), DEF_STK_SIZE);

    while (code.array[code.ip] != CMD_HLT)  // fixed-point
    {
        ProcessorFunction proc = AnalyzeRun(&code);
        if (proc.func(&code) == RUN_ERROR) return RUN_ERROR;
    }

    STACK_DTOR(&code.stk);
    STACK_DTOR(&code.func_stk);
    code.array -= HEADER_SIZE;
    free(code.array);
    free(code.RAM);
    fclose(RunFile);

    return SUCCESS;
}
