#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "const_enum_struct.h"
#include "analyze_proc.h"
#include "proc_struct_func.h"

            // Main Analyzer

struct ProcessorFunction AnalyzeRun(Spu * code)
{
    return ProcFunc[(int) code->array[code->ip]];
}

            // Push Analyzer

int PushAnalyzeRun(Spu * code)
{
    double arg = GetArgPush(code);

    BEGIN_CHECK
    STACK_PUSH(&code->stk, &arg);
    return CMD_PUSH;
}

double GetArgPush(Spu * code)
{
    code->ip++;

    int ArgType = code->array[code->ip++];
    double result = 0;

    if (ArgType & 1) result = code->array[code->ip++];
    if (ArgType & 2) result += code->reg[(int) code->array[code->ip++]];
    if (ArgType & 4) result = code->RAM[(int) result];

    return result;
}

            // Pop Analyzer

int PopAnalyzeRun(Spu * code)
{
    code->ip++;

    BEGIN_CHECK
    switch((int) code->array[code->ip])
    {
        case 1: STACK_POP(&code->stk);
                break;

        case 2: code->reg[(int) code->array[++code->ip]] = *(double*) STACK_TOP(&code->stk);
                STACK_POP(&code->stk);
                break;

        case 3: code->RAM[(int) code->array[++code->ip]] = *(double*) STACK_TOP(&code->stk);
                STACK_POP(&code->stk);
                break;

        case 4: code->RAM[(int) code->reg[(int) code->array[++code->ip]]] = *(double*) STACK_TOP(&code->stk);
                STACK_POP(&code->stk);
                break;
    }

    code->ip++;
    return CMD_POP;
}

            // Jmp Analyzers

int JmpAnalyzeRun(Spu * code)
{
    code->ip = code->array[code->ip + 1];
    return CMD_JMP;
}

int JaAnalyzeRun(Spu * code)
{
    BEGIN_CHECK
    double val1 = 0, val2 = 0;
    val1 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);
    val2 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);

    if (val2 > val1) code->ip = code->array[code->ip + 1];
    else
    {
        STACK_PUSH(&code->stk, &val2);
        STACK_PUSH(&code->stk, &val1);
        code->ip += 2;
    }

    return CMD_JA;
}

int JbAnalyzeRun(Spu * code)
{
    BEGIN_CHECK
    double val1 = 0, val2 = 0;
    val1 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);
    val2 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);

    if (val2 < val1) code->ip = code->array[code->ip + 1];
    else
    {
        STACK_PUSH(&code->stk, &val2);
        STACK_PUSH(&code->stk, &val1);
        code->ip += 2;
    }

    return CMD_JB;
}

int JaeAnalyzeRun(Spu * code)
{
    BEGIN_CHECK
    double val1 = 0, val2 = 0;
    val1 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);
    val2 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);

    if (val2 >= val1) code->ip = code->array[code->ip + 1];
    else
    {
        STACK_PUSH(&code->stk, &val2);
        STACK_PUSH(&code->stk, &val1);
        code->ip += 2;
    }

    return CMD_JAE;
}

int JbeAnalyzeRun(Spu * code)
{
    BEGIN_CHECK
    double val1 = 0, val2 = 0;
    val1 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);
    val2 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);

    if (val2 <= val1) code->ip = code->array[code->ip + 1];
    else
    {
        STACK_PUSH(&code->stk, &val2);
        STACK_PUSH(&code->stk, &val1);
        code->ip += 2;
    }

    return CMD_JBE;
}

int JeAnalyzeRun(Spu * code)
{
    BEGIN_CHECK
    double val1 = 0, val2 = 0;
    val1 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);
    val2 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);

    if (val2 == val1) code->ip = code->array[code->ip + 1];
    else
    {
        STACK_PUSH(&code->stk, &val2);
        STACK_PUSH(&code->stk, &val1);
        code->ip += 2;
    }

    return CMD_JE;
}

int JneAnalyzeRun(Spu * code)
{
    BEGIN_CHECK
    double val1 = 0, val2 = 0;
    val1 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);
    val2 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);

    if (val2 != val1) code->ip = code->array[code->ip + 1];
    else
    {
        STACK_PUSH(&code->stk, &val2);
        STACK_PUSH(&code->stk, &val1);
        code->ip += 2;
    }

    return CMD_JNE;
}

            // Math Analyzers

int AddAnalyzeRun(Spu * code)
{
    BEGIN_CHECK
    double val1 = pzn, val2 = pzn, val3 = pzn;
    val1 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);
    val2 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);
    val3 = val2 + val1;
    STACK_PUSH(&code->stk, &val3);
    code->ip++;
    return CMD_ADD;
}

int SubAnalyzeRun(Spu * code)
{
    BEGIN_CHECK
    double val1 = pzn, val2 = pzn, val3 = pzn;
    val1 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);
    val2 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);
    val3 = val2 - val1;
    STACK_PUSH(&code->stk, &val3);
    code->ip++;
    return CMD_SUB;
}

int MulAnalyzeRun(Spu * code)
{
    BEGIN_CHECK
    double val1 = pzn, val2 = pzn, val3 = pzn;
    val1 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);
    val2 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);
    val3 = val2 * val1;
    STACK_PUSH(&code->stk, &val3);
    code->ip++;
    return CMD_MUL;
}

int DivAnalyzeRun(Spu * code)
{
    BEGIN_CHECK
    double val1 = pzn, val2 = pzn, val3 = pzn;
    val1 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);
    val2 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);
    val3 = val2 / val1;
    STACK_PUSH(&code->stk, &val2);
    code->ip++;
    return CMD_DIV;
}

int SqrtAnalyzeRun(Spu * code)
{
    BEGIN_CHECK
    double val1 = pzn, val2 = pzn;
    val1 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);
    val2 = sqrt(val1);
    STACK_PUSH(&code->stk, &val2);
    code->ip++;
    return CMD_SQRT;
}

int SinAnalyzeRun(Spu * code)
{
    BEGIN_CHECK
    double val1 = pzn, val2 = pzn;
    val1 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);
    val2 = sin(val1);
    STACK_PUSH(&code->stk, &val2);
    code->ip++;
    return CMD_SIN;
}

int CosAnalyzeRun(Spu * code)
{
    BEGIN_CHECK
    double val1 = pzn, val2 = pzn;
    val1 = *(double*) (STACK_TOP(&code->stk));
    STACK_POP(&code->stk);
    val2 = cos(val1);
    STACK_PUSH(&code->stk, &val2);
    code->ip++;
    return CMD_COS;
}

            // Default Analyzers

int CallAnalyzeRun(Spu * code)
{
    BEGIN_CHECK
    size_t val = code->ip + 2;
    STACK_PUSH(&code->func_stk, &val);
    code->ip = (size_t) code->array[++code->ip];
    return CMD_CALL;
}

int RetAnalyzeRun(Spu * code)
{
    BEGIN_CHECK
    code->ip = *(size_t*) STACK_TOP(&code->func_stk);
    STACK_POP(&code->func_stk);
    return CMD_RET;
}

int InAnalyzeRun(Spu * code)
{
    double val = 0;
    BEGIN_CHECK
    scanf("%lg", &val);
    // fprintf(stderr, "Hello");
    STACK_PUSH(&code->stk, &val);
    code->ip++;
    return CMD_IN;

}

int OutAnalyzeRun(Spu * code)
{
    BEGIN_CHECK
    fprintf(stdout, "%lg\n", *(double*) STACK_TOP(&code->stk));
    STACK_POP(&code->stk);
    code->ip++;
    return CMD_OUT;
}

int DumpAnalyzeRun(Spu * code)
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
    code->ip++;
    return CMD_DUMP;
}

int ShowAnalyzeRun(Spu * code)
{
    struct stat vid = {};
    FILE * video = fopen("output.raw", "rb");
    stat("output.raw", &vid);


    while (vid.st_size > RAM_SIZE)
    {
        RAM_SIZE *= 2;
    }
    char * check = (char *) realloc(code->RAM, RAM_SIZE);
    if (!check) return ALLOCATE_MEMORY_ERROR;
    code->RAM = (double *) check;
    unsigned char * frame = (unsigned char *) calloc(FRAME_SIZE * 2, sizeof(char));
    if (!frame) return ALLOCATE_MEMORY_ERROR;

    fread(code->RAM, sizeof(char), RAM_SIZE, video);
    char * RAM = (char*) code->RAM;
    for (int i = 0; i < vid.st_size - 4; i+=4)
    {
        frame[(i / 4) % (FRAME_SIZE * 2)] = bgra2char(RAM + i);

        if ((i / 4) % 200 == 0)
        {
            frame[(i / 4) % (FRAME_SIZE * 2)] = '\n';
        }
        if ((i / 4) % (FRAME_SIZE * 2) == 0)
        {
            system("clear");
            printf("\033[999;999H\033[6n\n");
            printf("%s", frame);
            usleep(60000);
            system("clear");
        }
    }
    printf("\n");
    free(frame);

    return CMD_SHOW;
}

int LoadAnalyzeRun(Spu * code)
{
    char filename[100] = "";
    char command[300] = "";
    int filename_size = code->array[++code->ip];

    for (int i = 0; i < filename_size; i++)
    {
        code->ip++;
        filename[i] = (char) code->array[code->ip];
    }
    sprintf(command, "ffmpeg -i %s -s 200x200 -vf format=gray -filter:v format=pix_fmts=bgra -vcodec rawvideo -f rawvideo - > output.raw", filename);
    FILE * video = popen(command, "r");

    code->ip++;
    return CMD_LOAD;
}
            // Error Handling

int DefaultAnalyzeRun(Spu * code)
{
    fprintf(stderr, "Wrong command on ip %d - %lg\n", code->ip, code->array[code->ip]);
    return RUN_ERROR;
}

int bgra2char(char * x)
{
    unsigned int b = x[0] / 3, g = x[1] / 2, r = x[2] / 4;
    if (b + g + r < 20) return ' ';
    if (b + g + r < 50) return '.';
    if (b + g + r < 100) return '/';
    if (b + g + r < 150) return ';';
    if (b + g + r < 200) return 'g';
    if (b + g + r < 250) return 'H';
    if (b + g + r < 300) return '$';
    if (b + g + r < 350) return 'O';
    if (b + g + r < 400) return 'X';
    return '#';
}
