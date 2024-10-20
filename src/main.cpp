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
        case RUN:
            RunProgram("compiled.o");
            break;
        case COMPILE:
            Compile(argv[2], argv[3]);
            return fprintf(stderr, "Compiled!\n");
            break;
        default:
            break;
    }
    return -fprintf(stderr, "To Compile: asm <.asm> <.o>\n" "To Run: run <.o>\n");
}
