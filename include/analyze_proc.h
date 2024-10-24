#ifndef ANALYZE_PROC_H
#define ANALYZE_PROC_H

#include "stack.h"
#include "hash.h"

int PushAnalyzeRun(spu * code);
double GetArgPush(spu * code);
int PopAnalyzeRun(spu * code);
int JmpAnalyzeRun(spu * code);
int MathAnalyzeRun(spu * code);

#endif
