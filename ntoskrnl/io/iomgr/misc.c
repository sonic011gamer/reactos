
#undef NTDDI_VERSION
#undef _WIN32_WINNT
#define NTDDI_VERSION 0x06020400
#define _WIN32_WINNT 0x602

#include <ntifs.h>

NTSTATUS
NTAPI
IoGetIrpExtraCreateParameter(IN PIRP Irp,
                             OUT PECP_LIST *ExtraCreateParameter)
{
    /* Check we have a create operation */
    if (!BooleanFlagOn(Irp->Flags, IRP_CREATE_OPERATION))
    {
        return STATUS_INVALID_PARAMETER;
    }

    /* If so, return user buffer */
    *ExtraCreateParameter = Irp->UserBuffer;
    return STATUS_SUCCESS;
}

#undef NTDDI_VERSION
#undef _WIN32_WINNT
#define NTDDI_VERSION 0x05020400
#define _WIN32_WINNT 0x502
