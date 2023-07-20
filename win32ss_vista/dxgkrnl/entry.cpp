/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DXGKRNL entry
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <include/dxgkrnl.h>
//#define NDEBUG
#include <debug.h>

/**
 * @brief The first real transaction between WDDM Miniport and Dxgkrnl, this internal routine
 * gets called by Displib and passes the callback list into a internal struct
 *
 * @param DriverObject DriverObject of the miniport driver itself
 *
 * @param SourceString The registry path provided by Displib
 *
 * @param DriverInitData The callback list provided by a WDDM miniport driver via displib
 *
 * @return NTSTATUS
 */
NTSTATUS
NTAPI /* In Windows this is called DpiInitialize, But i don't care. */
RdPortInitializeMiniport(PDRIVER_OBJECT DriverObject, PUNICODE_STRING SourceString, DRIVER_INITIALIZATION_DATA *DriverInitData)
{
    /*
     *  @ IMPLEMENTED
     *  However this may need some adjustments still as we learn more about the
     *  internal DXGKRNL_EXTENSION.
     */
    NTSTATUS Status;
    PDRIVER_EXTENSION DriverExtend;
    PDXGKRNL_PRIVATE_EXTENSION DriverObjectExtension;

    /* A new WDDM Miniport has been added to the system, let's see it! */
    PAGED_CODE();
    DPRINT("RdPortInitializeMiniport: Entry point\n");
    DPRINT("RdPortInitializeMiniport: WDDM Miniport driver Reports version: 0x%X\n", DriverInitData->Version);
    if (!DriverObject || !SourceString)
    {
        return STATUS_INVALID_PARAMETER;
    }

    /* Setup Global state for Miniport handling */
    Status = RdPortSetupGlobalState();
    if (Status != STATUS_SUCCESS)
    {
        DPRINT1("RdPortInitializeMiniport: Couldn't setup global state status: 0x%X", Status);
        return Status;
    }

    /* let's make sure we can allocate the private extension */
    Status = IoAllocateDriverObjectExtension(DriverObject, DriverObject, sizeof(DXGKRNL_PRIVATE_EXTENSION), (PVOID*)&DriverObjectExtension);
    if (Status != STATUS_SUCCESS)
    {
        DPRINT1("RdPortInitializeMiniport: Couldn't allocate object extension status: 0x%X", Status);
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    /* Fill out the internal structure - WIP */
    DriverObjectExtension->DriverInitData = *DriverInitData;
    DriverObjectExtension->DriverObject = DriverObject;

    /* Fill out the public dispatch routines */
    DriverExtend = DriverObject->DriverExtension;
    DriverObject->MajorFunction[IRP_MJ_CREATE] = (PDRIVER_DISPATCH)RdPortDispatchCreateDevice;
    DriverObject->MajorFunction[IRP_MJ_PNP] = (PDRIVER_DISPATCH)RdPortDispatchPnp;
    DriverObject->MajorFunction[IRP_MJ_POWER] = (PDRIVER_DISPATCH)RdPortDispatchPower;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = (PDRIVER_DISPATCH)RdPortDispatchIoctl;
    DriverObject->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = (PDRIVER_DISPATCH)RdPortDispatchInternalIoctl;
    DriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = (PDRIVER_DISPATCH)RdPortDispatchSystemControl;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = (PDRIVER_DISPATCH)RdPortDispatchCloseDevice;

    DriverExtend->AddDevice = RdPortAddDevice;
    DriverObject->DriverUnload = (PDRIVER_UNLOAD)RdPortDriverUnload;
    DPRINT("RdPortInitializeMiniport: Finished\n");
    return STATUS_SUCCESS;
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
            *OutputBuffer = (PVOID)RdPortInitializeMiniport;
            DPRINT("IOCTL_VIDEO_DDI_FUNC_REGISTER - Queued RDDM_InitializeMiniport up\n");
            break;
        case IOCTL_VIDEO_I_AM_REACTOS:
            DPRINT1("This Dxgkrnl is from reactos\n");
            Irp->IoStatus.Status = STATUS_SUCCESS;
            break;
        default:
            DPRINT("RdPortInternalIoctl: unknown IOCTRL Code: %X\n", IoControlCode);
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

EXTERN_C
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

