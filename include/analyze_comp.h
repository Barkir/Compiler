#ifndef ANALYZE_COMP_H
#define ANALYZE_COMP_H

typedef int CompilerFunction_t (Spu * code, const char * command);

                // Main Analyzer

struct CompilerFunction AnalyzeCompiler(const char * command);

                // Push & Pop Analyzers

int PushAnalyzeCompiler(Spu * code, const char * command);
int PopAnalyzeCompiler(Spu * code, const char * command);

                // Jump Analyzers

int JmpAnalyzeCompiler(Spu * code, const char * command);
int JaAnalyzeCompiler(Spu * code, const char * command);
int JbAnalyzeCompiler(Spu * code, const char * command);
int JaeAnalyzeCompiler(Spu * code, const char * command);
int JbeAnalyzeCompiler(Spu * code, const char * command);
int JeAnalyzeCompiler(Spu * code, const char * command);
int JneAnalyzeCompiler(Spu * code, const char * command);

                // Math Analyzers

int AddAnalyzeCompiler(Spu * code, const char * command);
int SubAnalyzeCompiler(Spu * code, const char * command);
int DivAnalyzeCompiler(Spu * code, const char * command);
int MulAnalyzeCompiler(Spu * code, const char * command);
int SinAnalyzeCompiler(Spu * code, const char * command);
int CosAnalyzeCompiler(Spu * code, const char * command);
int SqrtAnalyzeCompiler(Spu * code, const char * command);

                // Default Analyzers

int InAnalyzeCompiler(Spu * code, const char * command);
int OutAnalyzeCompiler(Spu * code, const char * command);
int CallAnalyzeCompiler(Spu * code, const char * command);
int RetAnalyzeCompiler(Spu * code, const char * command);
int DumpAnalyzeCompiler(Spu * code, const char * command);
int HltAnalyzeCompiler(Spu * code, const char * command);
int EmptyAnalyzeCompiler(Spu * code, const char * command);
int ShowAnalyzeCompiler(Spu * code, const char * command);
int LoadAnalyzeCompiler(Spu * code, const char * command);

                // Label Analyzers

int LabelAnalyzeCompiler(Spu * code, const char * label);
int LabelFind(const char * label);

                // Error Handle

int DefaultAnalyzeCompiler(Spu * code, const char * command);

#endif
