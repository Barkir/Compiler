#ifndef COMPILER_H
#define COMPILER_H

int Compile(const char * InFileName, const char * OutFileName);
int CompileIter(char * InBuf, Spu * code, FILE * InFile, FILE * OutFile);

#endif
