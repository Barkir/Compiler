#include <stdio.h>

#include "compiler.h"

/*
 *  This is my simple compiler
 *  You can work with numbers on stack using it
 *  Added conditional jumps
 *  Added registers
 *  Added RAM
 *  Added cmd processing
*/

int main(int argc, char * argv[])
{
    switch (ProcessCmd(argc, argv))
    {
        case CMD_RUN:
            RunProgram(argv[2]);
            return fprintf(stderr, "Success!\n");
        case CMD_COMPILE:
            if (Compile(argv[2], argv[3]) == SUCCESS) return fprintf(stderr, "Compiled!\n");
            return -1;
        case CMD_DEF_COMPILE:
            if (Compile(argv[2], "compiled.o") == SUCCESS) return fprintf(stderr, "Compiled!\n");
            return -1;
        default:
            break;
    }
    return -fprintf(stderr, "To Compile: asm <.asm> <.o>\n" "To Run: run <.o>\n");
}
