/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DXGKRNL entry
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <include/dxgkrnl.h>
//#define NDEBUG
#include <debug.h>

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI /* In Windows this is called DpiInitialize, But i don't care. */
RdPort_InitializeMiniport(PDRIVER_OBJECT DriverObject, PUNICODE_STRING SourceString, DRIVER_INITIALIZATION_DATA *DriverInitData)
{
    PAGED_CODE();
    DPRINT("RdPort_InitializeMiniport: Entry point\n");
    __debugbreak();
    return STATUS_FAILED_DRIVER_ENTRY;
}

/*
 * @ HALF-IMPLEMENTED
 */
NTSTATUS
NTAPI
DxgkInternalDeviceControl(DEVICE_OBJECT *DeviceObject, IRP *Irp)
{
    ULONG IoControlCode;
    PVOID *OutputBuffer;
    PIO_STACK_LOCATION IrpStack;

    PAGED_CODE();

    /* First let's grab the IOCTRL code */
    IrpStack = Irp->Tail.Overlay.CurrentStackLocation;
    IoControlCode = IrpStack->Parameters.Read.ByteOffset.LowPart;
    Irp->IoStatus.Status = STATUS_SUCCESS;

    /* Handle all internal IOCTRLs*/
    switch (IoControlCode) 
    {
        case IOCTL_VIDEO_DDI_FUNC_REGISTER:
            /*
             * Grab a reference to the InitializeMiniport function so we can acquire the Miniport
             * callback list and continue setup
             */
            OutputBuffer = (PVOID*)Irp->UserBuffer;
            Irp->IoStatus.Information = 0;
            Irp->IoStatus.Status = STATUS_SUCCESS;
            *OutputBuffer = (PVOID)RdPort_InitializeMiniport;
            DPRINT("IOCTL_VIDEO_DDI_FUNC_REGISTER - Queued RDDM_InitializeMiniport up\n");
            break;
        case IOCTL_VIDEO_I_AM_REACTOS:
            DPRINT1("This Dxgkrnl is from reactos\n");
            Irp->IoStatus.Status = STATUS_SUCCESS;
            break;
        default:
            DPRINT("RdPort_InternalIoctl: unknown IOCTRL Code: %X\n", IoControlCode);
            break;
    }

    /* We won! let's continue */
    IofCompleteRequest(Irp, 0);
    return STATUS_SUCCESS;
}


/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
DxgkUnload(_In_ DRIVER_OBJECT *DriverObject)
{
    PAGED_CODE();
    /* For now a stub, and breakpoint so we can backtrace */
    DPRINT("DxgkUnload: Entry Point\n");
    __debugbreak();
    return STATUS_SUCCESS;
}

/*
 * @ IMPLEMENTED
 * This doesn't do anything from what i can see and is just to handle the IOCTRL
 */
NTSTATUS
NTAPI
DxgkCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    PAGED_CODE();
    DPRINT("DxgkCreateClose: Entry Point\n");
    Irp->IoStatus.Information = 0;
    Irp->IoStatus.Status = STATUS_SUCCESS;
    IofCompleteRequest(Irp, 0);
    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI 
DriverEntry(
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath)
{
    NTSTATUS Status;
    UNICODE_STRING DestinationString;
    PDEVICE_OBJECT DxgkrnlDeviceObject;

    /* First fillout dispatch table */
    DriverObject->MajorFunction[IRP_MJ_CREATE] = (PDRIVER_DISPATCH)DxgkCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = (PDRIVER_DISPATCH)DxgkCreateClose;
    DriverObject->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = (PDRIVER_DISPATCH)DxgkInternalDeviceControl;
    DriverObject->DriverUnload = (PDRIVER_UNLOAD)DxgkUnload;

    /* Grab Le strong*/
    RtlInitUnicodeString(&DestinationString, L"\\Device\\DxgKrnl");

    DPRINT1("---------------------------ReactOS Display Driver Model---------------------------\n");
    DPRINT1("Targetting Version: 0x%X\n", VERSION_WDDM_REACTOS);
    Status = IoCreateDevice(DriverObject,
                            0,
                            &DestinationString,
                            FILE_DEVICE_UNKNOWN,
                            FILE_DEVICE_SECURE_OPEN,
                            FALSE,
                            &DxgkrnlDeviceObject);

    if (!NT_SUCCESS(Status))
        DPRINT1("DriverEntry Failed with status %X", Status);

    return Status;
}

