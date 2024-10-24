#include <stdio.h>

#include "compiler.h"
#include "const_enum_struct.h"

int main (int argc, char * argv[])
{
    int error = 0;
    switch(argc)
    {
        case 2:
            if((error = Compile(argv[1], "compiled.out")) != SUCC) return error;
            return fprintf(stderr, "Your processor file is generated with default name (compiled.out).\n");

        case 3:
            if((error = Compile(argv[1], argv[2])) != SUCC) return error;
            return fprintf(stderr, "Your processor file is generated (%s)", argv[2]);

        default:
            return -fprintf(stderr, "To compile: <.out> <infile (.asm)> <outfile (.out)>\n");
    }
}
