#include "k32_vista.h"

#define NDEBUG
#include <debug.h>

DWORD 
GetActiveProcessorCount(WORD GroupNumber)
{
#ifdef CONFIG_SMP
    DWORD CpuCount;
    CpuCount = 1;
    return CpuCount;
#else
    return 1;
#endif
}

WORD
GetActiveProcessorGroupCount(VOID)
{
    /* Why? because we don't support processor groups so this is just 1. */
    return 1;
}

BOOL 
GetLogicalProcessorInformationEx(LOGICAL_PROCESSOR_RELATIONSHIP RelationshipType,
                                 PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX Buffer,
                                 PDWORD                            ReturnedLength)
{
    /* If the function succeeds, the return value is nonzero.*/
    return 0;
}

DWORD 
GetMaximumProcessorCount(WORD GroupNumber)
{
    return 64;
}

WORD
GetMaximumProcessorGroupCount(VOID)
{
    /* weird huh? this is just default*/
    return 64;
}

BOOL 
GetNumaAvailableMemoryNodeEx(USHORT Node,
                             PULONGLONG AvailableBytes)
{
    AvailableBytes = 0;
    /* If the function succeeds, the return value is nonzero.*/
    return 0;
}

BOOL 
GetNumaNodeNumberFromHandle(HANDLE  hFile,
                            PUSHORT NodeNumber)
{
    /* If the function succeeds, the return value is nonzero.*/
    return 0;
}

BOOL
GetNumaNodeProcessorMaskEx(USHORT Node,
                           PGROUP_AFFINITY ProcessorMask)
{
    /* If the specified node has no processors configured, 
    the Mask member is zero and the Group member is undefined. */
    ProcessorMask->Mask = 0;
    ProcessorMask->Group = 0;

    /* If the function succeeds, the return value is nonzero.*/
    return 1;
}

BOOL 
GetNumaProcessorNodeEx(PPROCESSOR_NUMBER Processor,
                       PUSHORT           NodeNumber)
{
    /* If the function succeeds, the return value is nonzero.*/
    return 0;
}

BOOL 
GetNumaProximityNodeEx(ULONG ProximityId,
                       PUSHORT NodeNumber)
{
    /* If the function succeeds, the return value is nonzero.*/
    return 0;
}

BOOL 
GetProcessGroupAffinity(HANDLE  hProcess,
                        PUSHORT GroupCount,
                        PUSHORT GroupArray)
{
    /* If the function succeeds, the return value is nonzero.*/
    return 0; 
}