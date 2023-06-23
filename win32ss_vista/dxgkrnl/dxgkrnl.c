/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DXGKRNL entry
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include <include/dxgkrnl.h>
//#define NDEBUG
#include <debug.h>

/* This function is the first real thing that happens to interact with the WDDM Miniport */
NTSTATUS
NTAPI
DpiInitialize(PDRIVER_OBJECT DriverObject, PCUNICODE_STRING SourceString, DRIVER_INITIALIZATION_DATA *DriverInitData)
{
    DPRINT1("DpiInitialize: Entry Point\n");
    DPRINT1("DpiInitialize: Wddm Miniport driver Reports version: 0x%X\n", DriverInitData->Version);

    PDRIVER_EXTENSION DriverExtend;
    DriverExtend = DriverObject->DriverExtension;
    DriverObject->MajorFunction[IRP_MJ_CREATE] = (PDRIVER_DISPATCH)RDDM_MiniportDispatchCreate;
    DriverObject->MajorFunction[IRP_MJ_PNP] = (PDRIVER_DISPATCH)RDDM_MiniportDispatchPnp;
    DriverObject->MajorFunction[IRP_MJ_POWER] = (PDRIVER_DISPATCH)RDDM_MiniportDispatchPower;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = (PDRIVER_DISPATCH)RDDM_MiniportDispatchIoctl;
    DriverObject->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = (PDRIVER_DISPATCH)RDDM_MiniportDispatchInternalIoctl;
    DriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = (PDRIVER_DISPATCH)RDDM_MiniportDispatchSystemControl;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = (PDRIVER_DISPATCH)RDDM_MiniportDispatchClose;
    DriverExtend->AddDevice = RDDM_MiniportAddDevice;
    DriverObject->DriverUnload = (PDRIVER_UNLOAD)RDDM_MiniportDriverUnload;
    DPRINT1("DpiInitialize: Finished\n");
    return STATUS_SUCCESS;
}


NTSTATUS
NTAPI   //TODO: Implement me
DxgkInternalDeviceIoctl(DEVICE_OBJECT *DeviceObject, IRP *Irp)
{
    IO_STACK_LOCATION *IrpStack = Irp->Tail.Overlay.CurrentStackLocation;
    ULONG IoControlCode = IrpStack->Parameters.Read.ByteOffset.LowPart;
    switch (IoControlCode)
    {
        default:
            DPRINT1("unknown IOCTRL Code: %X\n", IoControlCode);
            break;
    }

   Irp->IoStatus.Status = STATUS_SUCCESS;
   IofCompleteRequest(Irp, 0);
   return STATUS_SUCCESS;
}


NTSTATUS
NTAPI
DxgkUnload(_In_ DRIVER_OBJECT *DriverObject)
{
    /* For now a stub, and breakpoint so we can backtrace */
    DPRINT1("DxgkUnload: Entry Point\n");
    __debugbreak();
    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI /* This doesn't do anything*/
DxgkCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    DPRINT1("DxgkCreateClose: Entry Point\n");
    Irp->IoStatus.Information = 0;
    Irp->IoStatus.Status = STATUS_SUCCESS;
    IofCompleteRequest(Irp, 0);
    return 0;
}

NTSTATUS
NTAPI 
DriverEntry(
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath)
{
    PDEVICE_OBJECT DxgkrnlDeviceObject;
    UNICODE_STRING DestinationString;
    NTSTATUS Status;

    /* First fillout dispatch table */
    DriverObject->MajorFunction[IRP_MJ_CREATE] = (PDRIVER_DISPATCH)DxgkCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = (PDRIVER_DISPATCH)DxgkCreateClose;
    DriverObject->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = (PDRIVER_DISPATCH)DxgkInternalDeviceIoctl;
    DriverObject->DriverUnload = (PDRIVER_UNLOAD)DxgkUnload;

    /* Grab Le strong*/
    RtlInitUnicodeString(&DestinationString, L"\\Device\\DxgKrnl");

    DPRINT("DXGKNRL: Adding Device\n");
    Status = IoCreateDevice(DriverObject,
                            0, /* lol okay vista */
                            &DestinationString,
                            FILE_DEVICE_UNKNOWN,
                            FILE_DEVICE_SECURE_OPEN,
                            FALSE,
                            &DxgkrnlDeviceObject);

    if (!NT_SUCCESS(Status))
        return Status;

    return RDDM_SetupDxgkrnl(DriverObject, RegistryPath);
}

