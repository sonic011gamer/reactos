/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     Dxgkrnl workarounds for win32k
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <win32k.h>
#include <ntddvdeo.h>
//#define NDEBUG
#include <debug.h>

PFILE_OBJECT RDDM_FileObject;
PDEVICE_OBJECT RDDM_DeviceObject;

#define IOCTL_VIDEO_I_AM_REACTOS \
	CTL_CODE(FILE_DEVICE_VIDEO, 0xB, METHOD_NEITHER, FILE_ANY_ACCESS)

VOID
NTAPI
VideoPortCallout(
    _In_ PVOID Params);

NTSTATUS
NTAPI
SMgrNotifySessionChange(UINT32 State);

NTSTATUS
NTAPI
SMgrRegisterGdiCallout(PVOID Ptr);

/* Trigger Dxgkrnl to start the adapter thread the windows way */
NTSTATUS
NTAPI
DrvNotifySessionStateChange(UINT32 State)
{
    SMgrRegisterGdiCallout(&VideoPortCallout);
    return SMgrNotifySessionChange(State);
}

//FIXME: This entire function shouldn't even be needed, we need a watchdog
BOOL
TryHackedDxgkrnlStartAdapter()
{
    PIRP Irp;
    KEVENT Event;
    IO_STATUS_BLOCK IoStatusBlock;
    UNICODE_STRING DestinationString;
    NTSTATUS Status = STATUS_PROCEDURE_NOT_FOUND;

    DPRINT1("TryHackedDxgkrnlAdapterStart: Attempting to see if this is windows Dxgkrnl\n");
    /* First let's grab the RDDM objects */
    RtlInitUnicodeString(&DestinationString, L"\\Device\\DxgKrnl");
    Status = IoGetDeviceObjectPointer(&DestinationString, FILE_ALL_ACCESS, &RDDM_FileObject, &RDDM_DeviceObject);
    if(Status != STATUS_SUCCESS)
    {
        DPRINT1("Setting up DxgKrnl Failed\n");
        goto BypassDxgkrnl;
    }

    /* Build event and create IRP */
    DPRINT1("TryHackedDxgkrnlAdapterStart: Building IOCTRL with DxgKrnl\n");
    KeInitializeEvent(&Event, SynchronizationEvent, FALSE);
    Irp = IoBuildDeviceIoControlRequest(IOCTL_VIDEO_I_AM_REACTOS,
                                          RDDM_DeviceObject,
                                          NULL,
                                          0,
                                          NULL,
                                          0,
                                          TRUE,
                                          &Event,
                                          &IoStatusBlock);
    if ( !Irp )
      Status = IofCallDriver(RDDM_DeviceObject, Irp);
    DPRINT1("TryHackedDxgkrnlAdapterStart: Status %d\n", IoStatusBlock.Status);
    if (IoStatusBlock.Status != STATUS_SUCCESS)
    {
        DPRINT1("Wait... This is Windows DXGKNRL.SYS >:(\n");
        return FALSE;
    }
    else
    {
        DPRINT1("TryHackedDxgkrnlAdapterStart: ReactOS AdapterStart hack triggered\n");
        return TRUE;
    }
BypassDxgkrnl:
    DPRINT1("TryHackedDxgkrnlAdapterStart: Dxgkrnl is not loaded\n");
    return TRUE;
}
