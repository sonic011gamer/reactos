#include "include/psxsrv.h"
#include "../psx.h"
#include <debug.h>

ULONG NTAPI
ApiPortListener (PVOID pArg)
{
    return 0;
}

ULONG NTAPI
SbApiPortListener (PVOID pArg)
{
    return 0;
}

ULONG NTAPI
SessionPortListener (PVOID pArg)
{
    return 0;
}