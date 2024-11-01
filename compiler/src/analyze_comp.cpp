#include <stdio.h>
#include <string.h>

            // Header files

#include "const_enum_struct.h"
#include "analyze_string.h"
#include "comp_struct_func.h"
#include "labels.h"

            // Main Analyzer

struct CompilerFunction AnalyzeCompiler(const char * command)
{
    return CompFunc[CommandToEnum(command)];
}

            // Push Analyzer

int PushAnalyzeCompiler(Spu * code, const char * command)
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

    return CMD_PUSH;
}

            // Pop Analyzer

int PopAnalyzeCompiler(Spu * code, const char * command)
{
    char func[DEF_SIZE] = "";
    char reg[DEF_SIZE] = "";
    const char * symb = 0;
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
    return CMD_POP;
}

            // Jmp Analyzers

int JmpAnalyzeCompiler(Spu * code, const char * command)
{
    char func[DEF_SIZE] = "";
    char label[DEF_SIZE] = "";
    double val = pzn;

    sscanf(command, "%s %lg", func, &val);
    code->array[code->ip++] = CMD_JMP;

    if (val != pzn)
        code->array[code->ip++] = val;
    else
    {
        sscanf(command, "%s %s", func, label);
        code->array[code->ip++] = LabelFind(label);
    }
    return CMD_JMP;
}

int JaAnalyzeCompiler(Spu * code, const char * command)
{
    char func[DEF_SIZE] = "";
    char label[DEF_SIZE] = "";
    double val = pzn;

    sscanf(command, "%s %lg", func, &val);
    code->array[code->ip++] = CMD_JA;

    if (val != pzn)
        code->array[code->ip++] = val;
    else
    {
        sscanf(command, "%s %s", func, label);
        code->array[code->ip++] = LabelFind(label);
    }
    return CMD_JA;
}
int JbAnalyzeCompiler(Spu * code, const char * command)
{
    char func[DEF_SIZE] = "";
    char label[DEF_SIZE] = "";
    double val = pzn;

    sscanf(command, "%s %lg", func, &val);
    code->array[code->ip++] = CMD_JB;

    if (val != pzn)
        code->array[code->ip++] = val;
    else
    {
        sscanf(command, "%s %s", func, label);
        code->array[code->ip++] = LabelFind(label);
    }
    return CMD_JB;
}

int JaeAnalyzeCompiler(Spu * code, const char * command)
{
    char func[DEF_SIZE] = "";
    char label[DEF_SIZE] = "";
    double val = pzn;

    sscanf(command, "%s %lg", func, &val);
    code->array[code->ip++] = CMD_JAE;

    if (val != pzn)
        code->array[code->ip++] = val;
    else
    {
        sscanf(command, "%s %s", func, label);
        code->array[code->ip++] = LabelFind(label);
    }
    return CMD_JAE;
}

int JbeAnalyzeCompiler(Spu * code, const char * command)
{
    char func[DEF_SIZE] = "";
    char label[DEF_SIZE] = "";
    double val = pzn;

    sscanf(command, "%s %lg", func, &val);
    code->array[code->ip++] = CMD_JBE;

    if (val != pzn)
        code->array[code->ip++] = val;
    else
    {
        sscanf(command, "%s %s", func, label);
        code->array[code->ip++] = LabelFind(label);
    }
    return CMD_JBE;
}

int JeAnalyzeCompiler(Spu * code, const char * command)
{
    char func[DEF_SIZE] = "";
    char label[DEF_SIZE] = "";
    double val = pzn;

    sscanf(command, "%s %lg", func, &val);
    code->array[code->ip++] = CMD_JE;

    if (val != pzn)
        code->array[code->ip++] = val;
    else
    {
        sscanf(command, "%s %s", func, label);
        code->array[code->ip++] = LabelFind(label);
    }
    return CMD_JE;
}

int JneAnalyzeCompiler(Spu * code, const char * command)
{
    char func[DEF_SIZE] = "";
    char label[DEF_SIZE] = "";
    double val = pzn;

    sscanf(command, "%s %lg", func, &val);
    code->array[code->ip++] = CMD_JNE;

    if (val != pzn)
        code->array[code->ip++] = val;
    else
    {
        sscanf(command, "%s %s", func, label);
        code->array[code->ip++] = LabelFind(label);
    }
    return CMD_JNE;
}

            // Math Analyzers

int AddAnalyzeCompiler(Spu * code, const char * command)
{
    code->array[code->ip++] = CMD_ADD;
    return CMD_ADD;
}

int SubAnalyzeCompiler(Spu * code, const char * command)
{
    code->array[code->ip++] = CMD_SUB;
    return CMD_SUB;
}

int MulAnalyzeCompiler(Spu * code, const char * command)
{
    code->array[code->ip++] = CMD_MUL;
    return CMD_MUL;
}

int DivAnalyzeCompiler(Spu * code, const char * command)
{
    code->array[code->ip++] = CMD_DIV;
    return CMD_DIV;
}

int SinAnalyzeCompiler(Spu * code, const char * command)
{
    code->array[code->ip++] = CMD_SIN;
    return CMD_SIN;
}

int CosAnalyzeCompiler(Spu * code, const char * command)
{
    code->array[code->ip++] = CMD_COS;
    return CMD_COS;
}

int SqrtAnalyzeCompiler(Spu * code, const char * command)
{
    code->array[code->ip++] = CMD_SQRT;
    return CMD_SQRT;
}

        // Default Analyzers

int InAnalyzeCompiler(Spu * code, const char * command)
{
    code->array[code->ip++] = CMD_IN;
    return CMD_IN;
}

int OutAnalyzeCompiler(Spu * code, const char * command)
{
    code->array[code->ip++] = CMD_OUT;
    return CMD_OUT;
}

int DumpAnalyzeCompiler(Spu * code, const char * command)
{
    code->array[code->ip++] = CMD_DUMP;
    return CMD_DUMP;
}

int HltAnalyzeCompiler(Spu * code, const char * command)
{
    code->array[code->ip++] = CMD_HLT;
    return CMD_HLT;
}

            // Label Analyzer

int LabelAnalyzeCompiler(Spu * code, const char * label)
{
    for (int i = 0; i < DEF_SIZE; i++)
    {
        if (strcmp(Labels->name, label) == 0) break;
        if (Labels[i].name[0] == 0)
        {
            sscanf(label, "%s", Labels[i].name);
            Labels[i].ip = code->ip;
            return CMD_LABEL;
        }
    }
    return LABELS_OVERFLOW;
}

int LabelFind(const char * label)
{
    for (int i = 0; i < DEF_SIZE; i++)
        if (strcmp(label, Labels[i].name) == 0) return Labels[i].ip;
    return LABEL_FIND_ERROR;
}

            // Call Analyzer

int CallAnalyzeCompiler(Spu * code, const char * command)
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
        sscanf(command, "%s %s", func, label);
        code->array[code->ip++] = CommandToEnum(func);
        code->array[code->ip++] = LabelFind(label);
    }
    return CMD_CALL;
}

            // Ret Analyzer

int RetAnalyzeCompiler(Spu * code, const char * command)
{
    code->array[code->ip++] = CMD_RET;
    return CMD_RET;
}

<<<<<<< HEAD
int ShowAnalyzeCompiler(Spu * code, const char * command)
{
    code->array[code->ip++] = CMD_SHOW;
    return CMD_SHOW;
}

int LoadAnalyzeCompiler(Spu * code, const char * command)
{
    char func[DEF_SIZE] = "";
    char filename[DEF_SIZE] = "";
    sscanf(command, "%s %s", func, filename);
    code->array[code->ip++] = CMD_LOAD;
    code->array[code->ip++] = strlen(filename);
    for (int i = 0; i < strlen(filename); i++)
        code->array[code->ip++] = (double) filename[i];
}

=======
>>>>>>> parent of f62a21a (videomemory(badapple.mp4))
            // Empty Analyzer

int EmptyAnalyzeCompiler(Spu * code, const char * command)
{
    return CMD_EMPTY;
}

            // Error Handling

int DefaultAnalyzeCompiler(Spu * code, const char * command)
{
    fprintf(stderr, "wrong code, won't compile! %s\n", command);
    return COMPILE_ERROR;
}
