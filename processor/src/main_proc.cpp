#include <stdio.h>

#include "processor.h"

int main(int argc, char * argv[])
{
    switch(argc)
    {
        case 2:
            if(RunProgram(argv[1]) == SUCC)
                return fprintf(stderr, "Successfull init!\n");
        default:
            return -fprintf(stderr, "<.out> <procfile (.out)>\n");
    }
}
