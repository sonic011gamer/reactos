/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     D3DKMT APIs
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <precomp.h>
#include "../../../win32ss_vista/include/locd3d.h"
#include <debug.h>

/* Windows Vista *************************************************************************************/

NTSTATUS
WINAPI
D3DKMTCheckMonitorPowerState(_In_ D3DKMT_CHECKMONITORPOWERSTATE *unnamedParam1)
{
    return NtGdiDdDDICheckMonitorPowerState(unnamedParam1);
}

NTSTATUS
WINAPI
D3DKMTCheckOcclusion(_In_ D3DKMT_CHECKOCCLUSION *unnamedParam1)
{
    return NtGdiDdDDICheckOcclusion(unnamedParam1);
}

NTSTATUS
WINAPI
D3DKMTCloseAdapter(_In_ D3DKMT_CLOSEADAPTER *unnamedParam1)
{
    return NtGdiDdDDICloseAdapter(unnamedParam1);
}

NTSTATUS
WINAPI
D3DKMTCreateAllocation(_Inout_ D3DKMT_CREATEALLOCATION *unnamedParam1)
{
    return NtGdiDdDDICreateAllocation(unnamedParam1);
}

NTSTATUS
WINAPI
D3DKMTCreateContext(D3DKMT_CREATECONTEXT *unnamedParam1)
{
    return NtGdiDdDDICreateContext(unnamedParam1);
}

NTSTATUS
WINAPI
D3DKMTCreateDevice(D3DKMT_CREATEDEVICE *unnamedParam1)
{
    return NtGdiDdDDICreateDevice(unnamedParam1);
}

NTSTATUS
WINAPI
D3DKMTCreateOverlay(D3DKMT_CREATEOVERLAY *unnamedParam1)
{
    return NtGdiDdDDICreateOverlay(unnamedParam1);
}

NTSTATUS
WINAPI
D3DKMTCreateSynchronizationObject(D3DKMT_CREATESYNCHRONIZATIONOBJECT *unnamedParam1)
{
    return NtGdiDdDDICreateSynchronizationObject(unnamedParam1);
}

/*
 * GDI CALL:
 * D3DKMTOpenAdapterFromGdiDisplayName
 * Documentation:
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmthk/nf-d3dkmthk-d3dkmtopenadapterfromgdidisplayname
 * @ UNIMPLEMENTED
 */
NTSTATUS
WINAPI
D3DKMTOpenAdapterFromGdiDisplayName(_Inout_ D3DKMT_OPENADAPTERFROMGDIDISPLAYNAME *unnamedParam1)
{
    if (!unnamedParam1)
        STATUS_INVALID_PARAMETER;

    /* '
     * This is one of the few that actually have a stack trace inside the routine
     * Proably due to the fact it's handling some stuff inside the routine
     */
    DPRINT1("D3DKMTOpenAdapterFromGdiDisplayName: DeviceName: %s\n", unnamedParam1->DeviceName);
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
WINAPI
D3DKMTDestroyAllocation(_In_ D3DKMT_DESTROYALLOCATION *unnamedParam1)
{
    return NtGdiDdDDIDestroyAllocation(unnamedParam1);
}

NTSTATUS
WINAPI
D3DKMTDestroyContext(_In_ D3DKMT_DESTROYCONTEXT *unnamedParam1)
{
    return NtGdiDdDDIDestroyContext(unnamedParam1);
}

NTSTATUS
WINAPI
D3DKMTDestroyDevice(_In_ D3DKMT_DESTROYDEVICE *unnamedParam1)
{
    return NtGdiDdDDIDestroyDevice(unnamedParam1);
}

NTSTATUS
WINAPI
D3DKMTDestroyOverlay(_In_ D3DKMT_DESTROYOVERLAY *unnamedParam1)
{
    return NtGdiDdDDIDestroyOverlay(unnamedParam1);
}

NTSTATUS
WINAPI
D3DKMTDestroySynchronizationObject(_In_ D3DKMT_DESTROYSYNCHRONIZATIONOBJECT *unnamedParam1)
{
    return NtGdiDdDDIDestroySynchronizationObject(unnamedParam1);
}

NTSTATUS
WINAPI
D3DKMTEscape(_In_ D3DKMT_ESCAPE *unnamedParam1)
{
    return NtGdiDdDDIEscape(unnamedParam1);
}

/* Windows 7 *****************************************************************************************/

NTSTATUS
WINAPI
D3DKMTCheckVidPnExclusiveOwnership(_In_ D3DKMT_CHECKVIDPNEXCLUSIVEOWNERSHIP *unnamedParam1)
{
    return NtGdiDdDDICheckVidPnExclusiveOwnership(unnamedParam1);
}
