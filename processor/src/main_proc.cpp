#include <stdio.h>

#include "processor.h"

int main(int argc, char * argv[])
{
    switch(argc)
    {
        case 2:
            if(RunProgram(argv[1]) == SUCC)
<<<<<<< HEAD
                // return fprintf(stderr, "Successfull init!\n");
                return 0;
=======
                return fprintf(stderr, "Successfull init!\n");
>>>>>>> parent of f62a21a (videomemory(badapple.mp4))
        default:
            return -fprintf(stderr, "<.out> <procfile (.out)>\n");
    }
}
