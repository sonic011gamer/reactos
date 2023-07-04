
#include <win32k.h>
#include <ntddvdeo.h>
#include <debug.h>

PFILE_OBJECT RDDM_FileObject;
PDEVICE_OBJECT RDDM_DeviceObject;

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

NTSTATUS
NTAPI
DrvNotifySessionStateChange(UINT32 State)
{
  SMgrRegisterGdiCallout(&VideoPortCallout);
  return SMgrNotifySessionChange(State);
}

#define IOCTL_VIDEO_I_AM_REACTOS \
	CTL_CODE( FILE_DEVICE_VIDEO, 0xB, METHOD_NEITHER, FILE_ANY_ACCESS  )


BOOL
TryHackedDxgkrnlAdapterStart()
{
    PIRP Irp;
    KEVENT Event;
    IO_STATUS_BLOCK IoStatusBlock; // [esp+6Ch] [ebp-14h] BYREF
    UNICODE_STRING DestinationString; // [esp+74h] [ebp-Ch] BY
    NTSTATUS Status = STATUS_PROCEDURE_NOT_FOUND;


    DPRINT1("Attempting to see if this is windows Dxgkrnl\n");
    /* First let's grab the RDDM objects */
    RtlInitUnicodeString(&DestinationString, L"\\Device\\DxgKrnl");
    Status = IoGetDeviceObjectPointer(&DestinationString, FILE_ALL_ACCESS, &RDDM_FileObject, &RDDM_DeviceObject);
    if(Status != STATUS_SUCCESS)
    {
        DPRINT1("Setting up DxgKrnl Failed\n");
        goto BypassDxgkrnl;
    }
    DPRINT1("TryHackedDxgkrnlAdapterStart: Building IOCTRL with DxgKrnl\n");
    KeInitializeEvent(&Event, SynchronizationEvent, FALSE);
    Irp = IoBuildDeviceIoControlRequest(IOCTL_VIDEO_I_AM_REACTOS, //TODO: Fuck you vista
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
        return TRUE;
    }
    DPRINT1("Win32k has grabbed dxgkrnl interface with status %d\n", Status);
BypassDxgkrnl:
    return TRUE;
}

