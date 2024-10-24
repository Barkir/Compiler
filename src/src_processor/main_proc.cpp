#include <stdio.h>

#include "processor.h"

int main(int argc, char * argv[])
{
    switch(argc)
    {
        case 2:
            RunProgram(argv[1]);
            return fprintf(stderr, "Successfull init!\n");
        default:
            return -fprintf(stderr, "<.out> <procfile (.out)>\n");
    }
}
