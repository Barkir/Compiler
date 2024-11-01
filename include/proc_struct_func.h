#ifndef PROC_STRUCT_FUNC_H
#define PROC_STRUCT_FUNC_H

#include "analyze_proc.h"

struct ProcessorFunction
{
    ProcessorFunction_t * func;
};

static struct ProcessorFunction ProcFunc[FUNC_NUM]
{
    {   DefaultAnalyzeRun },
    {   PushAnalyzeRun    },
    {   PopAnalyzeRun     },
    {   AddAnalyzeRun     },
    {   SubAnalyzeRun     },
    {   DivAnalyzeRun     },
    {   MulAnalyzeRun     },
    {   SqrtAnalyzeRun    },
    {   SinAnalyzeRun     },
    {   CosAnalyzeRun     },
    {   DumpAnalyzeRun    },
    {   InAnalyzeRun      },
    {   OutAnalyzeRun     },
    {   JmpAnalyzeRun     },
    {   JaAnalyzeRun      },
    {   JaeAnalyzeRun     },
    {   JbAnalyzeRun      },
    {   JbeAnalyzeRun     },
    {   JeAnalyzeRun      },
    {   JneAnalyzeRun     },
    {                     },
    {   CallAnalyzeRun    },
    {   RetAnalyzeRun     },
    {                     },
    {                     },
    {   ShowAnalyzeRun    },
    {   LoadAnalyzeRun    }
};

#endif
