#include <stdio.h>
#include <string.h>

#include "const_enum_struct.h"
#include "analyze_comp.h"
#include "analyze_string.h"
#include "labels.h"

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

int PopAnalyzeCompiler(spu * code, char * command)
{
    char func[DEF_SIZE] = "";
    char reg[DEF_SIZE] = "";
    char * symb = 0;
    double val = pzn;
    if ((symb = strchr(command, '[')) == NULL)
    {
        sscanf(command, "%s %s", func, reg); // getchar() algo
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

int JmpAnalyzeCompiler(spu * code, const char * command)
{
    char func[DEF_SIZE] = "";
    char label[DEF_SIZE] = "";
    double val = pzn;

    sscanf(command, "%s %lg", func, &val);
    if (val != pzn)
    {
        code->array[code->ip++] = CommandToEnum(func);
        code->array[code->ip++] = val;
    }

    else
    {
        sscanf(command, "%s, %s", func, label);
        code->array[code->ip++] = CommandToEnum(func);
        code->array[code->ip++] = LabelFind(label);

    }
    return 0;
}

int LabelAnalyzeCompiler(spu * code, char * label)
{
    for (int i = 0; i < DEF_SIZE; i++)
    {
        if (Labels[i].name[0] == 0)
        {
            sscanf(label, "%s", Labels[i].name);
            Labels[i].ip = code->ip;
            return SUCCESS;
        }
    }
    return LABELS_OVERFLOW;
}

int LabelFind(char * label)
{
    for (int i = 0; i < DEF_SIZE; i++)
        if (strcmp(label, Labels[i].name) == 0) return Labels[i].ip;
    return LABEL_FIND_ERROR;
}
