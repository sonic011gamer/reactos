/*
 * PROJECT:     ReactOS Posix Subsystem
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     PSXSS Entry Point
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#define NTOS_MODE_USER
#include <ndk/exfuncs.h>
#include <ndk/psfuncs.h>
#include <ndk/rtlfuncs.h>

#include <posixsubsytem/psxsrv.h>

#define NDEBUG
#include <debug.h>

VOID
NTAPI
PsxpSetDefaultProcessHardErrorMode(VOID)
{
    ULONG DefaultHardErrorMode = 0;

    /* Disable hard errors */
    NtSetInformationProcess(NtCurrentProcess(),
                            ProcessDefaultHardErrorMode,
                            &DefaultHardErrorMode,
                            sizeof(DefaultHardErrorMode));
}

int
_cdecl
_main(int argc,
      char *argv[],
      char *envp[],
      int DebugFlag)
{
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

    /* Disable errors */
    PsxpSetDefaultProcessHardErrorMode();

    /* If this is Session 0, make sure killing us bugchecks the system */
    if (NtCurrentPeb()->SessionId == 0) RtlSetProcessIsCritical(TRUE, NULL, FALSE);

    /* Kill this thread. PSXSRV keeps us going */
    NtTerminateThread(NtCurrentThread(), Status);
    return 0;
}