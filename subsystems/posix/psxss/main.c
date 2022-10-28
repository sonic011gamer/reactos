#define NTOS_MODE_USER
#include <ndk/exfuncs.h>
#include <ndk/psfuncs.h>
#include <ndk/rtlfuncs.h>

#include <csr/csrsrv.h>
#include "../psx.h"

#define NDEBUG
#include <debug.h>


int
_cdecl
_main(int argc,
      char *argv[],
      char *envp[],
      int DebugFlag)
{
    /* TODO: messing with this setting really shows how f**ked our scheduler is */
    KPRIORITY BasePriority = PROCESS_PRIORITY_NORMAL_FOREGROUND + 4;
    NTSTATUS Status;
    UNREFERENCED_PARAMETER(envp);
    UNREFERENCED_PARAMETER(DebugFlag);

    /* Set the base priority */
    NtSetInformationProcess(NtCurrentProcess(),
                            ProcessBasePriority,
                            &BasePriority,
                            sizeof(BasePriority));

    /* Initialize {PSX} through PSXSRV */
    Status = PsxServerInitialization(argc, (PWSTR*)argv);
    if (!NT_SUCCESS(Status))
    {
        /* Kill us */
        DPRINT1("PSXSS: Unable to initialize server, Status: 0xX\n", Status);
        NtTerminateProcess(NtCurrentProcess(), Status);
    }

    /* If this is Session 0, make sure killing us bugchecks the system */
    if (NtCurrentPeb()->SessionId == 0) RtlSetProcessIsCritical(TRUE, NULL, FALSE);

    /* Kill this thread. CSRSRV keeps us going */
    NtTerminateThread(NtCurrentThread(), Status);
    return 0;
}
