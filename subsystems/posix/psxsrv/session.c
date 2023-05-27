/*
 * PROJECT:     ReactOS Posix Subsystem
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     PSXSRV Session Support
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include "ppsxsrv.h"
//#define NDEBUG
#include <debug.h>

RTL_CRITICAL_SECTION PsxNtSessionLock;
LIST_ENTRY PsxNtSessionList;

NTSTATUS
NTAPI
PsxInitializeNtSessionList(VOID)
{
    /* Initialize the Session List */
    InitializeListHead(&PsxNtSessionList);

    /* Initialize the Session Lock */
    return RtlInitializeCriticalSection(&PsxNtSessionLock);
}

/*++
 * @name CsrAllocateNtSession
 *
 * The CsrAllocateNtSession routine allocates a new CSR NT Session.
 *
 * @param SessionId
 *        Session ID of the CSR NT Session to allocate.
 *
 * @return Pointer to the newly allocated CSR NT Session.
 *
 * @remarks None.
 *
 *--*/
#if 0
PCSR_NT_SESSION
NTAPI
CsrAllocateNtSession(IN ULONG SessionId)
{
    PCSR_NT_SESSION NtSession;

    /* Allocate an NT Session Object */
    NtSession = RtlAllocateHeap(CsrHeap, HEAP_ZERO_MEMORY, sizeof(CSR_NT_SESSION));
    if (NtSession)
    {
        /* Setup the Session Object */
        NtSession->SessionId = SessionId;
        NtSession->ReferenceCount = 1;

        /* Insert it into the Session List */
        CsrAcquireNtSessionLock();
        InsertHeadList(&CsrNtSessionList, &NtSession->SessionLink);
        CsrReleaseNtSessionLock();
    }
    else
    {
        ASSERT(NtSession != NULL);
    }

    /* Return the Session (or NULL) */
    return NtSession;
}

#endif