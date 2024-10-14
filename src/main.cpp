#include <stdio.h>

#include "compiler.h"

/*
 *  This is my simple compiler
 *  You can work with numbers on stack using it
 *  Added conditional jumps
 *  Added registers
*/

int main(void)
{
    Compile("code.asm", "compiled.o");
    RunProgram("compiled.o");
}
