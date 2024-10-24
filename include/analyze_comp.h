#ifndef ANALYZE_COMP_H
#define ANALYZE_COMP_H

int PushAnalyzeCompiler(spu * code, const char * command);
int PopAnalyzeCompiler(spu * code, char * command);
int JmpAnalyzeCompiler(spu * code, const char * command);
int LabelAnalyzeCompiler(spu * code, char * label);
int LabelFind(char * label);

#endif
