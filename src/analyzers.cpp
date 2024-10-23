#include <stdio.h>
#include <string.h>

#include "analyzers.h"

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
                sscanf(command, "%s %s", func, reg);
                code->array[code->ip++] = CommandToEnum(reg);
            }
        }
        else
        {
            sscanf(++symb,  "%lg", &val);
            if (val != pzn)
            {
                code->array[code->ip++] = 5;
                code->array[code->ip++] = val;
            }
            else
            {
                code->array[code->ip++] = 6;
                sscanf(symb, "%s", reg);
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
            sscanf(++symb, "%lg", &val);
            if (val == pzn)
            {
                sscanf(symb, "%s %s %lg", reg, sign, &val);
                code->array[code->ip++] = val;
                code->array[code->ip++] = CommandToEnum(reg);
            }
            else
            {
                sscanf(symb, "%lg %s %s", &val, sign, reg);
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

int PopAnalyzeCompiler(spu * code, char * command)
{
    char func[100] = "";
    char reg[100] = "";
    char * symb = 0;
    double val = pzn;
    if ((symb = strchr(command, '[')) == NULL)
    {
        sscanf(command, "%s %s", func, reg);
        if (reg[0] == 0 && func[0] != 0)
        {
            code->array[code->ip++] = CMD_POP;
            code->array[code->ip++] = 1;
        }
        else
        {
            code->array[code->ip++] = CMD_POP;
            code->array[code->ip++] = 2;
            code->array[code->ip++] = CommandToEnum(reg);
        }
    }
    else
    {
        sscanf(++symb, "%lg", &val);
        if (val != pzn)
        {
            code->array[code->ip++] = CMD_POP;
            code->array[code->ip++]  = 3;
            code->array[code->ip++] = val;
        }
        else
        {
            sscanf(symb, "%s", reg);
            code->array[code->ip++] = CMD_POP;
            code->array[code->ip++] = 4;
            reg[strlen(reg) - 1] = 0;
            code->array[code->ip++] = CommandToEnum(reg);
        }
    }
    return 0;
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

int JmpAnalyzeCompiler(spu * code, const char * command)
{
    char func[DEF_SIZE] = "";
    double val = 0;

    sscanf(command, "%s %lg", func, &val);
    code->array[code->ip++] = CommandToEnum(func);
    code->array[code->ip++] = val;
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
    return -fprintf(stderr, "CommandToEnum error: %s\n", command);
}


int ProcessCmd(int argc, char * argv[])
{
    if (argc == 1                                   )  return CMD_DEFAULT;
    if (argc == 3  &&  (strcmp("run", argv[1]) == 0))  return CMD_RUN;
    if (argc == 3  &&  (strcmp("asm", argv[1]) == 0))  return CMD_DEF_COMPILE;
    if (argc == 4  &&  (strcmp("asm", argv[1]) == 0))  return CMD_COMPILE;
    return -fprintf(stderr, "Cmd Not Processed!\n");
}
