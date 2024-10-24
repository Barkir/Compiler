#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "const_enum_struct.h"
#include "analyze_proc.h"

int PushAnalyzeRun(spu * code)
{
    double arg = GetArgPush(code);

    BEGIN_CHECK
    STACK_PUSH(&code->stk, &arg);
    return 0;
}

double GetArgPush(spu * code)
{
    code->ip++;

    int ArgType = code->array[code->ip++];
    double result = 0;

    if (ArgType & 1) result = code->array[code->ip++];
    if (ArgType & 2) result += code->reg[(int) code->array[code->ip++]];
    if (ArgType & 4) result = code->RAM[(int) result];

    return result;
}


int PopAnalyzeRun(spu * code)
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
    return 0;
}

int JmpAnalyzeRun(spu * code)
{
    BEGIN_CHECK
    double val1 = 0;
    double val2 = 0;
    if (code->array[code->ip] != CMD_JMP)
    {
        val1 = *(double*) (STACK_TOP(&code->stk));
        STACK_POP(&code->stk);
        val2 = *(double*) (STACK_TOP(&code->stk));
        STACK_POP(&code->stk);
    }
    switch ((int) code->array[code->ip])
    {
        case CMD_JMP:   code->ip = code->array[code->ip + 1];
                        break;

        case CMD_JA:    if (val2 > val1) code->ip = code->array[code->ip + 1];
                        else
                        {
                            STACK_PUSH(&code->stk, &val2);
                            STACK_PUSH(&code->stk, &val1);
                            code->ip++;
                        }
                        break;

        case CMD_JB:    if (val2 < val1) code->ip = code->array[code->ip + 1];
                        else
                        {
                            STACK_PUSH(&code->stk, &val2);
                            STACK_PUSH(&code->stk, &val1);
                            code->ip++;
                        }
                        break;
        case CMD_JE:    if (val2 == val1) code->ip = code->array[code->ip + 1];
                        else
                        {
                            STACK_PUSH(&code->stk, &val2);
                            STACK_PUSH(&code->stk, &val1);
                            code->ip++;
                        }
                        break;
        case CMD_JAE:   if (val2 >= val1) code->ip = code->array[code->ip + 1];
                        else
                        {
                            STACK_PUSH(&code->stk, &val2);
                            STACK_PUSH(&code->stk, &val1);
                            code->ip++;
                        }
                        break;
        case CMD_JBE:   if (val2 <= val1) code->ip = code->array[code->ip + 1];
                        else
                        {
                            STACK_PUSH(&code->stk, &val2);
                            STACK_PUSH(&code->stk, &val1);
                            code->ip++;
                        }
                        break;
        case CMD_JNE:   if (val2 != val1) code->ip = code->array[code->ip + 1];
                        else
                        {
                            STACK_PUSH(&code->stk, &val2);
                            STACK_PUSH(&code->stk, &val1);
                            code->ip++;
                        }
                        break;
    }

    return 0;
}

int MathAnalyzeRun(spu * code)
{
    double val1 = pzn, val2 = pzn, val3 = pzn;

    BEGIN_CHECK
    switch ((int) code->array[code->ip])
    {
    case CMD_ADD:   val1 = *(double*) (STACK_TOP(&code->stk));
                    STACK_POP(&code->stk);
                    val2 = *(double*) (STACK_TOP(&code->stk));
                    STACK_POP(&code->stk);
                    val3 = val2 + val1;
                    STACK_PUSH(&code->stk, &val3);
                    break;

    case CMD_SUB:   val1 = *(double*) (STACK_TOP(&code->stk));
                    STACK_POP(&code->stk);
                    val2 = *(double*) (STACK_TOP(&code->stk));
                    STACK_POP(&code->stk);
                    val3 = val2 - val1;
                    STACK_PUSH(&code->stk, &val3);
                    break;

    case CMD_MUL:   val1 = *(double*) (STACK_TOP(&code->stk));
                    STACK_POP(&code->stk);
                    val2 = *(double*) (STACK_TOP(&code->stk));
                    STACK_POP(&code->stk);
                    val3 = val2 * val1;
                    STACK_PUSH(&code->stk, &val3);
                    break;

    case CMD_DIV:   val1 = *(double*) (STACK_TOP(&code->stk));
                    STACK_POP(&code->stk);
                    val2 = *(double*) (STACK_TOP(&code->stk));
                    STACK_POP(&code->stk);
                    val3 = val2 / val1;
                    STACK_PUSH(&code->stk, &val3);
                    break;

    case CMD_SQRT:  val1 = *(double*) (STACK_TOP(&code->stk));
                    STACK_POP(&code->stk);
                    val3 = sqrt(val1);
                    STACK_PUSH(&code->stk, &val3);
                    code->ip++;
                    break;

    case CMD_SIN:   val1 = *(double*) (STACK_TOP(&code->stk));
                    STACK_POP(&code->stk);
                    val3 = sin(val1);
                    STACK_PUSH(&code->stk, &val3);
                    break;

    case CMD_COS:   val1 = *(double*) (STACK_TOP(&code->stk));
                    STACK_POP(&code->stk);
                    val3 = cos(val1);
                    STACK_PUSH(&code->stk, &val3);
                    break;
    }
    code->ip++;
    return 0;
}
