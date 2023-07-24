
/*
 * 
 * extracreateparameters.c
 * 
 */

#undef NTDDI_VERSION
#undef _WIN32_WINNT
#define NTDDI_VERSION 0x06020400
#define _WIN32_WINNT 0x602

#include <ntifs.h>

NTKERNELAPI
NTSTATUS
NTAPI
FsRtlGetEcpListFromIrp(IN PIRP Irp,
                       OUT PECP_LIST *EcpList)
{
    /* Call Io */
    return IoGetIrpExtraCreateParameter(Irp, EcpList);
}

NTKERNELAPI
NTSTATUS
NTAPI
FsRtlGetNextExtraCreateParameter(IN PECP_LIST EcpList,
                                 IN PVOID CurrentEcpContext,
                                 OUT LPGUID NextEcpType OPTIONAL,
                                 OUT PVOID *NextEcpContext,
                                 OUT PULONG NextEcpContextSize OPTIONAL)
{
    PECP_HEADER CurrentEntry;

    /* If we have no context ... */
    if (CurrentEcpContext == NULL)
    {
        if (IsListEmpty(&EcpList->EcpList))
        {
            goto FailEmpty;
        }

        /* Simply consider first entry */
        CurrentEntry = CONTAINING_RECORD(EcpList->EcpList.Flink, ECP_HEADER, ListEntry);
    }
    else
    {
        /* Otherwise, consider the entry matching the given context */
        CurrentEntry = ECP_CONTEXT_TO_HEADER(CurrentEcpContext);

        /* Make sure we didn't reach the end */
        if (&CurrentEntry->ListEntry == &EcpList->EcpList)
        {
            goto FailEmpty;
        }
    }

    /* We must have an entry */
    if (CurrentEntry == NULL)
    {
        goto FailEmpty;
    }

    /* If caller wants a context, give it */
    if (NextEcpContext != NULL)
    {
        *NextEcpContext = ECP_HEADER_TO_CONTEXT(CurrentEntry);
    }

    /* Same for its size (which the size minus the header overhead) */
    if (NextEcpContextSize != NULL)
    {
         *NextEcpContextSize = CurrentEntry->Size - sizeof(ECP_HEADER);
    }

    /* And copy the type if asked to */
    if (NextEcpType != NULL)
    {
        RtlCopyMemory(NextEcpType, &CurrentEntry->EcpType, sizeof(GUID));
    }

    /* Job done */
    return STATUS_SUCCESS;

    /* Failure case: just zero everything */
FailEmpty:
    if (NextEcpContext != NULL)
    {
        *NextEcpContext = NULL;
    }

    if (NextEcpContextSize != NULL)
    {
        *NextEcpContextSize = 0;
    }

    if (NextEcpType != NULL)
    {
        RtlZeroMemory(NextEcpType, sizeof(GUID));
    }

    /* And return failure */
    return STATUS_NOT_FOUND;
}

#undef NTDDI_VERSION
#undef _WIN32_WINNT
#define NTDDI_VERSION 0x05020400
#define _WIN32_WINNT 0x502
