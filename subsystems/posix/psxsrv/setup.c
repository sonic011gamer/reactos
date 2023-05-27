/*
 * PROJECT:     ReactOS Posix Subsystem
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     PSXSRV Entry Point
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include "ppsxsrv.h"
//#define NDEBUG
#include <debug.h>
HANDLE PsxApiPort = NULL;
HANDLE PsxHeap = NULL;

BOOL
NTAPI
DllMain(IN HINSTANCE hInstanceDll,
        IN DWORD dwReason,
        IN LPVOID lpReserved)
{
    /* We don't do much */
    UNREFERENCED_PARAMETER(hInstanceDll);
    UNREFERENCED_PARAMETER(dwReason);
    UNREFERENCED_PARAMETER(lpReserved);

    return TRUE;
}

/*
 *  PdxRunServer
 *  @brief
 */
PRIVATE
NTSTATUS
NTAPI
PdxRunServer (VOID)
{
    NTSTATUS Status = 0;
    UNICODE_STRING Program = {0, 0, NULL};
    RtlInitUnicodeString (&Program, L"POSIX+");
    Status = SmConnectToSm(&Program,
                           (HANDLE) 0,
                           IMAGE_SUBSYSTEM_POSIX_CUI,
                           &PsxApiPort);
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("PSXSRV: %s: SmConnectToSm() failed with Status = %08x\n",
                    __FUNCTION__,
                    Status);
        return Status;
    }

    DPRINT1("THE POSIX SUBSYSTEM HAS BEGUN\n");
    return STATUS_SUCCESS;
}
/**********************************************************************
 *      PsxServerInitialization/2
 *
 * DESCRIPTION
 *      Initialize the PSX server process.
 */
NTSTATUS NTAPI
PsxServerInitialization (
    IN ULONG ArgumentCount,
    IN PWSTR *ArgumentArray
    )
{
    NTSTATUS Status;
	Status = 0;

    PsxHeap = RtlGetProcessHeap();

    /* Set up Session Support */
    Status = PsxInitializeNtSessionList();
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("PSXSRV:%s: CsrInitializeSessions failed (Status=0x%08lx)\n",
                __FUNCTION__, Status);
        return Status;
    }

    /* Now initialize our API Port */
    Status = PsxApiPortInitialize();
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("PSXSRV:%s: PsxApiPortInitialize failed (Status=0x%08lx)\n",
                __FUNCTION__, Status);
        return Status;
    }

    /* Initialize the API Port for SM communication */
    Status = PsxSbApiPortInitialize();
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("PSXSRV:%s: PsxSbApiPortInitialize failed (Status=0x%08lx)\n",
                __FUNCTION__, Status);
        return Status;
    }

    Status = PdxRunServer ();
    /* Done */
    return Status;
}
