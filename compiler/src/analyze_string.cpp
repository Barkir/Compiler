#include <stdio.h>
#include <string.h>

#include "const_enum_struct.h"
#include "analyze_string.h"

int CommandToEnum(const char * command)
{
                        // Push & Pop

    if (strcmp(command, "push") == 0)           return CMD_PUSH;
    if (strcmp(command, "pop")  == 0)           return CMD_POP;

                        // Jump

    if (strcmp(command, "jmp")  == 0)           return CMD_JMP;
    if (strcmp(command, "ja")   == 0)           return CMD_JA;
    if (strcmp(command, "jb")   == 0)           return CMD_JB;
    if (strcmp(command, "jae")  == 0)           return CMD_JAE;
    if (strcmp(command, "jbe")  == 0)           return CMD_JBE;
    if (strcmp(command, "je")   == 0)           return CMD_JE;
    if (strcmp(command, "jne")  == 0)           return CMD_JNE;

                        // Math

    if (strcmp(command, "add")  == 0)           return CMD_ADD;
    if (strcmp(command, "sub")  == 0)           return CMD_SUB;
    if (strcmp(command, "div")  == 0)           return CMD_DIV;
    if (strcmp(command, "mul")  == 0)           return CMD_MUL;
    if (strcmp(command, "sqrt") == 0)           return CMD_SQRT;
    if (strcmp(command, "sin")  == 0)           return CMD_SIN;
    if (strcmp(command, "cos")  == 0)           return CMD_COS;

                        // Default Functions

    if (strcmp(command, "in")   == 0)           return CMD_IN;
    if (strcmp(command, "out")  == 0)           return CMD_OUT;
    if (strcmp(command, "call") == 0)           return CMD_CALL;
    if (strcmp(command, "ret")  == 0)           return CMD_RET;
    if (strcmp(command, "dump") == 0)           return CMD_DUMP;
    if (strcmp(command, "hlt")  == 0)           return CMD_HLT;
    if (strcmp(command, "show") == 0)           return CMD_SHOW;
    if (strcmp(command, "load") == 0)           return CMD_LOAD;

                        // Empty

    if (strcmp(command, "\0")   == 0)           return CMD_EMPTY;
    if (strcmp(command, "\n")   == 0)           return CMD_EMPTY;

                        // Registers

    if (strcmp(command, "AX")   == 0)           return AX;
    if (strcmp(command, "BX")   == 0)           return BX;
    if (strcmp(command, "CX")   == 0)           return CX;
    if (strcmp(command, "DX")   == 0)           return DX;
    if (strcmp(command, "EX")   == 0)           return EX;
    if (strcmp(command, "FX")   == 0)           return FX;
    if (strcmp(command, "GX")   == 0)           return GX;
    if (strcmp(command, "RX")   == 0)           return RX;

                        // Label

    if (command[strlen(command) - 1] == ':')    return CMD_LABEL;

                        // Error Handle

    fprintf(stderr, "CommandToEnum error: %s\n", command);
    return CMD_DEFAULT;
}


// int ProcessCmd(int argc, char * argv[])
// {
//     if (argc == 1                                   )  return CMD_DEFAULT;
//     if (argc == 3  &&  (strcmp("run", argv[1]) == 0))  return CMD_RUN;
//     if (argc == 3  &&  (strcmp("asm", argv[1]) == 0))  return CMD_DEF_COMPILE;
//     if (argc == 4  &&  (strcmp("asm", argv[1]) == 0))  return CMD_COMPILE;
//     return -fprintf(stderr, "Cmd Not Processed!\n");
// }
