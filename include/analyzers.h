#ifndef ANALYZERS_H
#define ANALYZERS_H

#include "compiler.h"

int PushAnalyzeCompiler(spu * code, const char * command);
int PushAnalyzeRun(spu * code);
double GetArgPush(spu * code);

int PopAnalyzeCompiler(spu * code, char * command);
int PopAnalyzeRun(spu * code);

int JmpAnalyzeCompiler(spu * code, const char * command);
int JmpAnalyzeRun(spu * code);

int MathAnalyzeRun(spu * code);

int ProcessCmd(int argc, char * argv[]);
int CommandToEnum(char * command);
#endif
