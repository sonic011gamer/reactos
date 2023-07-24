/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     Dxgkrnl d3dkmt functions
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <win32k.h>
#include <ntddvdeo.h>
#include "../../../win32ss_vista/include/locd3d.h"
//#define NDEBUG
#include <debug.h>

/*
 * A little bit about the way the D3DKMT APIs work just for the record:
 * These syscalls directly call logic inside the modern WDDM stack.
 *
 * for example:
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/display/providing-kernel-mode-support-to-the-opengl-installable-client-driver
 *
 * I've tried to organize this by windows versions as WDDM drivers and opengl ICDs will directly
 * call these using the syscall code and order matters blah blah blah.
 * When you change ReactOS's syscalls to try to test these please align to ONE SINGLE WINDOWS VERSION.
 * AND adjust the TARGETTING VERSION in DXGKRNL accoridngly. They need to match up.
 *
 * These also happen to break WINE quite bad (oops :P)
 */

/* Windows Vista *************************************************************************************/

/*
 * GDI CALL:
 * D3DKMTCheckMonitorPowerState
 * Documentation:
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmthk/nf-d3dkmthk-d3dkmtcheckmonitorpowerstate
 * @ UNIMPLEMENTED
 */
NTSTATUS
APIENTRY
NtGdiDdDDICheckMonitorPowerState(_In_ D3DKMT_CHECKMONITORPOWERSTATE *unnamedParam1)
{
    if (!unnamedParam1)
        STATUS_INVALID_PARAMETER;

    /* Not really sure if i understand what the point of this API is */
    UNIMPLEMENTED;
    return STATUS_SUCCESS; /* I lied i didn't verify shit :P */
}

/*
 * GDI CALL:
 * D3DKMTCheckOcclusion
 * Documentation:
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmthk/nf-d3dkmthk-d3dkmtcheckocclusion
 * @ UNIMPLEMENTED
 */
NTSTATUS
APIENTRY
NtGdiDdDDICheckOcclusion(_In_ D3DKMT_CHECKOCCLUSION *unnamedParam1)
{
    if (!unnamedParam1)
        STATUS_INVALID_PARAMETER;

    /*
     * Later chrome is quite rude about this.
     * It seems chrome assumes this ever returning STATUS_SUCCESS means that
     * the monitor has fallen asleep. Because later chrome operates under the assumption
     * desktop composition is always running. This is likely a affect of dropping
     * windows 7 - thanks chrome.
     *
     * returning this state is double edge because now we're assuming
     * DWM is always running.. Spoiler alert: No.
     */
    DPRINT1("NtGdiDdDDICheckOcclusion: Is returning something stupid - this will break\n");
    UNIMPLEMENTED;
    return STATUS_SUCCESS;
}

/*
 * GDI CALL:
 * D3DKMTCloseAdapter
 * Documentation:
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmthk/nf-d3dkmthk-d3dkmtcloseadapter
 * @ UNIMPLEMENTED
 */
NTSTATUS
APIENTRY
NtGdiDdDDICloseAdapter(_In_ D3DKMT_CLOSEADAPTER *unnamedParam1)
{
    /* Oh apperently this return can also mean WDDM is never used - ok :D*/
    if (!unnamedParam1)
        STATUS_INVALID_PARAMETER;

    UNIMPLEMENTED;
    return STATUS_SUCCESS;
}

/*
 * GDI CALL:
 * D3DKMTCreateAllocation
 * Documentation:
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmthk/nf-d3dkmthk-d3dkmtcreateallocation
 * @ UNIMPLEMENTED
 */
NTSTATUS
APIENTRY
NtGdiDdDDICreateAllocation(_Inout_ D3DKMT_CREATEALLOCATION *unnamedParam1)
{
    if (!unnamedParam1)
        STATUS_INVALID_PARAMETER;

    UNIMPLEMENTED;
    /* this is one of the foundational components to OpenGL ICDs in WDDM */
    __debugbreak();
    return STATUS_NO_MEMORY;
}

/*
 * GDI CALL:
 * D3DKMTCreateContext
 * Documentation:
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmthk/nf-d3dkmthk-d3dkmtcreatecontext?redirectedfrom=MSDN
 * @ UNIMPLEMENTED
 */
NTSTATUS
APIENTRY
NtGdiDdDDICreateContext(D3DKMT_CREATECONTEXT *unnamedParam1)
{
    if (!unnamedParam1)
        STATUS_INVALID_PARAMETER;

    UNIMPLEMENTED;
    /* this is one of the foundational components to OpenGL ICDs in WDDM */
    __debugbreak();
    return STATUS_DEVICE_REMOVED;
}

/*
 * GDI CALL:
 * D3DKMTCreateDCFromMemory
 * Documentation:
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmthk/nf-d3dkmthk-d3dkmtcreatedcfrommemory
 * @ HALF-IMPLEMENTED
 */
/*
 * Check ntddraw/d3dkmt.c
 * -> This routine does more then just this, but this is all we need for now.
 */

/*
 * GDI CALL:
 * D3DKMTCreateDevice
 * Documentation:
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmthk/nf-d3dkmthk-d3dkmtcreatedevice?redirectedfrom=MSDN
 * @ UNIMPLEMENTED
 */
NTSTATUS
APIENTRY
NtGdiDdDDICreateDevice(D3DKMT_CREATEDEVICE *unnamedParam1)
{
    if (!unnamedParam1)
        STATUS_INVALID_PARAMETER;

    UNIMPLEMENTED;
    /* Huzzzah call the win32k interface */
    __debugbreak();
    return STATUS_NO_MEMORY;
}

/*
 * GDI CALL:
 * D3DKMTCreateOverlay
 * Documentation:
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmthk/nf-d3dkmthk-d3dkmtcreateoverlay?redirectedfrom=MSDN
 * @ UNIMPLEMENTED
 */
NTSTATUS
APIENTRY
NtGdiDdDDICreateOverlay(D3DKMT_CREATEOVERLAY *unnamedParam1)
{
    if (!unnamedParam1)
        STATUS_INVALID_PARAMETER;

    UNIMPLEMENTED;
    return STATUS_NO_MEMORY;
}

/*
 * GDI CALL:
 * D3DKMTCreateSynchronizationObject
 * Documentation:
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmthk/nf-d3dkmthk-d3dkmtcreatesynchronizationobject
 * @ UNIMPLEMENTED
 */
NTSTATUS
APIENTRY
NtGdiDdDDICreateSynchronizationObject(D3DKMT_CREATESYNCHRONIZATIONOBJECT *unnamedParam1)
{
    if (!unnamedParam1)
        STATUS_INVALID_PARAMETER;

    UNIMPLEMENTED;
    return STATUS_NO_MEMORY;
}

/*
 * GDI CALL:
 * D3DKMTDestroyAllocation
 * Documentation:
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmthk/nf-d3dkmthk-d3dkmtdestroyallocation
 * @ UNIMPLEMENTED
 */
NTSTATUS
APIENTRY
NtGdiDdDDIDestroyAllocation(_In_ D3DKMT_DESTROYALLOCATION *unnamedParam1)
{
    if (!unnamedParam1)
        STATUS_INVALID_PARAMETER;

    UNIMPLEMENTED;
    return STATUS_SUCCESS; /* This causes a leak */
}

/*
 * GDI CALL:
 * D3DKMTDestroyContext
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */
NTSTATUS
APIENTRY
NtGdiDdDDIDestroyContext(_In_ D3DKMT_DESTROYCONTEXT *unnamedParam1)
{
    if (!unnamedParam1)
        STATUS_INVALID_PARAMETER;

    UNIMPLEMENTED;
    return STATUS_SUCCESS; /* This causes a leak */
}

/*
 * GDI CALL:
 * D3DKMTDestroyDCFromMemory
 * Documentation:
 *
 * @ HALF-IMPLEMENTED
 */
/*
 * Check ntddraw/d3dkmt.c
 * -> This routine does more then just this, but this is all we need for now.
 */

/*
 * GDI CALL:
 * D3DKMTDestroyDevice
 * Documentation:
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmthk/nf-d3dkmthk-d3dkmtdestroydevice
 * @ UNIMPLEMENTED
 */
NTSTATUS
APIENTRY
NtGdiDdDDIDestroyDevice(_In_ D3DKMT_DESTROYDEVICE *unnamedParam1)
{
    if (!unnamedParam1)
        STATUS_INVALID_PARAMETER;

    UNIMPLEMENTED;
    return STATUS_SUCCESS; /* This causes a leak */
}

/*
 * GDI CALL:
 * D3DKMTDestroyOverlay
 * Documentation:
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmthk/nf-d3dkmthk-d3dkmtdestroyoverlay
 * @ UNIMPLEMENTED
 */
NTSTATUS
APIENTRY
NtGdiDdDDIDestroyOverlay(_In_ D3DKMT_DESTROYOVERLAY *unnamedParam1)
{
    if (!unnamedParam1)
        STATUS_INVALID_PARAMETER;

    UNIMPLEMENTED;
    return STATUS_SUCCESS; /* This causes a leak */
}

/*
 * GDI CALL:
 * D3DKMTDestroySynchronizationObject
 * Documentation:
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmthk/nf-d3dkmthk-d3dkmtdestroysynchronizationobject
 * @ UNIMPLEMENTED
 */
NTSTATUS
APIENTRY
NtGdiDdDDIDestroySynchronizationObject(_In_ D3DKMT_DESTROYSYNCHRONIZATIONOBJECT *unnamedParam1)
{
    if (!unnamedParam1)
        STATUS_INVALID_PARAMETER;

    UNIMPLEMENTED;
    return STATUS_SUCCESS; /* This causes a leak */
}

/*
 * GDI CALL:
 * D3DKMTEscape
 * Documentation:
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmthk/nf-d3dkmthk-d3dkmtescape
 * @ UNIMPLEMENTED
 */
NTSTATUS
APIENTRY
NtGdiDdDDIEscape(_In_ D3DKMT_ESCAPE *unnamedParam1)
{
    if (!unnamedParam1)
        STATUS_INVALID_PARAMETER;

    UNIMPLEMENTED;
    __debugbreak(); /* IF we got here, we got bigger things going on */
    return STATUS_DEVICE_REMOVED; /* This causes a leak */
}

/*
 * GDI CALL:
 * D3DKMTFlipOverlay
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTGetContextSchedulingPriority
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTGetDeviceState
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTGetDisplayModeList
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTGetMultisampleMethodList
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTGetPresentHistory
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTGetProcessSchedulingPriorityClass
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTGetRuntimeData
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTGetScanLine
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */


/*
 * GDI CALL:
 * D3DKMTGetSharedPrimaryHandle
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */


/*
 * GDI CALL:
 * D3DKMTInvalidateActiveVidPn
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */


/*
 * GDI CALL:
 * D3DKMTLock
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */


/*
 * GDI CALL:
 * D3DKMTOpenAdapterFromDeviceName
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */


/*
 * GDI CALL:
 * D3DKMTOpenAdapterFromHdc
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */


/*
 * GDI CALL:
 * D3DKMTOpenResource
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTPollDisplayChildren
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTPresent
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTQueryAdapterInfo
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTQueryAllocationResidency
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTQueryResourceInfo
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTQueryStatistics
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTReleaseProcessVidPnSourceOwner
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTRender
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTSetAllocationPriority
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTSetContextSchedulingPriority
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTSetDisplayMode
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTSetDisplayPrivateDriverFormat
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTSetGammaRamp
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */


/*
 * GDI CALL:
 * D3DKMTSetProcessSchedulingPriorityClass
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */


/*
 * GDI CALL:
 * D3DKMTSetQueuedLimit
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 *
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTSetVidPnSourceOwner
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTSharedPrimaryLockNotification
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTSharedPrimaryUnLockNotification
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTSignalSynchronizationObject
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTUnlock
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTUpdateOverlay
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTWaitForIdle
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTWaitForSynchronizationObject
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 *  D3DKMTWaitForVerticalBlankEvent
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 *
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/*
 * GDI CALL:
 * D3DKMTCheckExclusiveOwnership
 * Documentation:
 *
 * @ UNIMPLEMENTED
 */

/* Windows 7 *****************************************************************************************/

/*
 * GDI CALL:
 * D3DKMTCheckVidPnExclusiveOwnership
 * Documentation:
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmthk/nf-d3dkmthk-d3dkmtcheckvidpnexclusiveownership
 * @ UNIMPLEMENTED
 */
NTSTATUS
APIENTRY
NtGdiDdDDICheckVidPnExclusiveOwnership(_In_ PD3DKMT_CHECKVIDPNEXCLUSIVEOWNERSHIP unnamedParam1)
{
    /* We need a dxgknrl handle */
    if (!unnamedParam1->hAdapter)
        return STATUS_INVALID_PARAMETER;

    /* According to MSDN we check this seperately */
    if (!unnamedParam1->VidPnSourceId)
        return STATUS_GRAPHICS_INVALID_VIDEO_PRESENT_SOURCE;
    /*
     * looks like it just calls into dxgkrnl and checks
     * to see if the video device is being used excusively,
     *
     * id do it like this:
     * 1) Grab GPU Handle and index into dxgkrnl
     * 2) Inside dxgkrnl get the VidPn global array list
     * 3) check state of the VidPn and see if Exclusivity is set as a falg.
     * 4) Return state STATUS_GRAPHICS_VIDPN_SOURCE_IN_USE if it's used otherwise SUCCESS
     */
    DPRINT1("D3DKMTCheckVidPnExclusiveOwnership: request to check excusiveity of VidPN %d owned by GPU %X", unnamedParam1->VidPnSourceId, unnamedParam1->hAdapter);
    UNIMPLEMENTED;
    return STATUS_SUCCESS;
}

/* Windows 8 *****************************************************************************************/
