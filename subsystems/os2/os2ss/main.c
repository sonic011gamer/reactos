#include "os2ss.h"

#include <test.h>
#include <debug.h>

int
_cdecl
WinMainCRTStartup(int argc,
      char *argv[],
      char *envp[],
      int DebugFlag)
{
    NTSTATUS Status = 0;
    DPRINT1("Starting OS/2 Subsystem\n");
    UNREFERENCED_PARAMETER(envp);
    UNREFERENCED_PARAMETER(DebugFlag);
    Status = ServerDllInitialization(argc, (LPWSTR*)argv);
    if (!NT_SUCCESS(Status))
    {
        /* Kill us */
        DPRINT1("Unable to start OS/2 Subsystem, Status: 0x%08lx\n", Status);
        NtTerminateProcess(NtCurrentProcess(), Status);
    }
    return 0;

}