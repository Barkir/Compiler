#ifndef COMP_STRUCT_FUNC_H
#define COMP_STRUCT_FUNC_H

#include "analyze_comp.h"

struct CompilerFunction
{
    CompilerFunction_t * func;
};

static struct CompilerFunction CompFunc[FUNC_NUM]
{
    {   DefaultAnalyzeCompiler },
    {   PushAnalyzeCompiler    },
    {   PopAnalyzeCompiler     },
    {   AddAnalyzeCompiler     },
    {   SubAnalyzeCompiler     },
    {   DivAnalyzeCompiler     },
    {   MulAnalyzeCompiler     },
    {   SqrtAnalyzeCompiler    },
    {   SinAnalyzeCompiler     },
    {   CosAnalyzeCompiler     },
    {   DumpAnalyzeCompiler    },
    {   InAnalyzeCompiler      },
    {   OutAnalyzeCompiler     },
    {   JmpAnalyzeCompiler     },
    {   JaAnalyzeCompiler      },
    {   JaeAnalyzeCompiler     },
    {   JbAnalyzeCompiler      },
    {   JbeAnalyzeCompiler     },
    {   JeAnalyzeCompiler      },
    {   JneAnalyzeCompiler     },
    {   LabelAnalyzeCompiler   },
    {   CallAnalyzeCompiler    },
    {   RetAnalyzeCompiler     },
    {   HltAnalyzeCompiler     },
    {   EmptyAnalyzeCompiler   },
    {   ShowAnalyzeCompiler    },
    {   LoadAnalyzeCompiler    }
};

#endif
