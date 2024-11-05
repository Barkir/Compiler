#ifndef ANALYZE_PROC_H
#define ANALYZE_PROC_H

#include "stack.h"
#include "hash.h"

typedef int ProcessorFunction_t (Spu * code);

            // Main Analyzer

struct ProcessorFunction AnalyzeRun(Spu * code);

            // Push Analyzer

int PushAnalyzeRun(Spu * code);
double GetArgPush(Spu * code);

            // Pop Analyzer

int PopAnalyzeRun(Spu * code);

            // Jmp Analyzers

int JmpAnalyzeRun(Spu * code);
int JaAnalyzeRun(Spu * code);
int JbAnalyzeRun(Spu * code);
int JaeAnalyzeRun(Spu * code);
int JbeAnalyzeRun(Spu * code);
int JeAnalyzeRun(Spu * code);
int JneAnalyzeRun(Spu * code);

            // Math Analyzers

int AddAnalyzeRun(Spu * code);
int SubAnalyzeRun(Spu * code);
int MulAnalyzeRun(Spu * code);
int DivAnalyzeRun(Spu * code);
int SinAnalyzeRun(Spu * code);
int CosAnalyzeRun(Spu * code);
int SqrtAnalyzeRun(Spu * code);

            // Default Analyzers

int CallAnalyzeRun(Spu * code);
int RetAnalyzeRun(Spu * code);
int InAnalyzeRun(Spu * code);
int OutAnalyzeRun(Spu * code);
int DumpAnalyzeRun(Spu * code);
int ShowAnalyzeRun(Spu * code);
int LoadAnalyzeRun(Spu * code);

            // Error Handle

int DefaultAnalyzeRun(Spu * code);


int bgra2char(char * x);

#endif
