/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DXGKRNL entry
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include <include/dxgkrnl.h>
//#define NDEBUG
#include <debug.h>

/**
 * @brief Setups all handles for the needed IRPs and passes the Miniports callback list into a internal strucutre
 *
 * @param DriverObject
 *
 * @param SourceString
 *
 * @param DriverInitData The callback list provided by a WDDM miniport driver
 *
 * @return NTSTATUS
 *
 *  HALF-IMPLEMENTED
 */
NTSTATUS
NTAPI
DpiInitialize(PDRIVER_OBJECT DriverObject, PCUNICODE_STRING SourceString, DRIVER_INITIALIZATION_DATA *DriverInitData)
{
    NTSTATUS Status;
    PDRIVER_EXTENSION DriverExtend;
    PDXGKRNL_PRIVATE_EXTENSION DriverObjectExtension;

    PAGED_CODE();
    /*
     * Okay so clearly this func does quite a few annoying things so why is it half implemented? you may ask if you're looking at this code
     * (Though why are you lol these notes are mostly for me to come back and fix crap)
     */

    /*
     * Windows Vista/7, Hell Even 8 (Though that's a bigger can of beans) validate it's API versions here and also has a extra later of protection
     * This should be defined as a FIXME: In windows a WDDM minport driver can vary in size, not filling out every callback in the list.
     * Windows does a validation to make sure a specific set of these callbacks are implemented. On top of this the DxgkDdi.. Call backs seem to be
     * implemented at the very end of the internal struct. The IoALlocateDriverObjectExtension call will be fed the maximum size. However DXGKNRL will keep track
     * of the implemented call back size
     *
     * On top of the above some basic global structures are setup here that I don't fully understand what they do yet. Overall this API works
     * I Just am not matching ALL of window's behaviors yet,
     */

    DPRINT1("DpiInitialize: Entry Point\n");
    /* This function is the first real thing that happens to interact with the WDDM Miniport */
    DPRINT1("DpiInitialize: Wddm Miniport driver Reports version: 0x%X\n", DriverInitData->Version);

    Status = IoAllocateDriverObjectExtension(DriverObject, DriverObject, sizeof(DXGKRNL_PRIVATE_EXTENSION), (PVOID*)&DriverObjectExtension);

    /* Fill out the internal structure - WIP */
    DriverObjectExtension->DriverInitData = DriverInitData;
    DriverObjectExtension->DriverObject = DriverObject;

    /* Fill out the call backs - This can be done in a way that's not cursed but i wanna see them all visible for now - mostly for testing */
    DriverObjectExtension->DxgkDdiAddDevice = DriverInitData-> DxgkDdiAddDevice;
    DriverObjectExtension->DxgkDdiStartDevice = DriverInitData-> DxgkDdiStartDevice;
    DriverObjectExtension->DxgkDdiAddDevice = DriverInitData-> DxgkDdiAddDevice;
    DriverObjectExtension->DxgkDdiStartDevice = DriverInitData-> DxgkDdiStartDevice;
    DriverObjectExtension->DxgkDdiStopDevice = DriverInitData-> DxgkDdiStopDevice;
    DriverObjectExtension->DxgkDdiRemoveDevice = DriverInitData-> DxgkDdiRemoveDevice;
    DriverObjectExtension->DxgkDdiDispatchIoRequest = DriverInitData-> DxgkDdiDispatchIoRequest;
    DriverObjectExtension->DxgkDdiInterruptRoutine = DriverInitData-> DxgkDdiInterruptRoutine;
    DriverObjectExtension->DxgkDdiDpcRoutine = DriverInitData-> DxgkDdiDpcRoutine;
    DriverObjectExtension->DxgkDdiQueryChildRelations = DriverInitData-> DxgkDdiQueryChildRelations;
    DriverObjectExtension->DxgkDdiQueryChildStatus = DriverInitData-> DxgkDdiQueryChildStatus;
    DriverObjectExtension->DxgkDdiQueryDeviceDescriptor = DriverInitData-> DxgkDdiQueryDeviceDescriptor;
    DriverObjectExtension->DxgkDdiSetPowerState = DriverInitData-> DxgkDdiSetPowerState;
    DriverObjectExtension->DxgkDdiNotifyAcpiEvent = DriverInitData-> DxgkDdiNotifyAcpiEvent;
    DriverObjectExtension->DxgkDdiResetDevice = DriverInitData-> DxgkDdiResetDevice;
    DriverObjectExtension->DxgkDdiUnload = DriverInitData-> DxgkDdiUnload;
    DriverObjectExtension->DxgkDdiQueryInterface = DriverInitData-> DxgkDdiQueryInterface;
    DriverObjectExtension->DxgkDdiControlEtwLogging = DriverInitData-> DxgkDdiControlEtwLogging;
    DriverObjectExtension->DxgkDdiQueryAdapterInfo = DriverInitData-> DxgkDdiQueryAdapterInfo;
    DriverObjectExtension->DxgkDdiCreateDevice = DriverInitData-> DxgkDdiCreateDevice;
    DriverObjectExtension->DxgkDdiCreateAllocation = DriverInitData-> DxgkDdiCreateAllocation;
    DriverObjectExtension->DxgkDdiDestroyAllocation = DriverInitData-> DxgkDdiDestroyAllocation;
    DriverObjectExtension->DxgkDdiDescribeAllocation = DriverInitData-> DxgkDdiDescribeAllocation;
    DriverObjectExtension->DxgkDdiGetStandardAllocationDriverData = DriverInitData-> DxgkDdiGetStandardAllocationDriverData;
    DriverObjectExtension->DxgkDdiAcquireSwizzlingRange = DriverInitData-> DxgkDdiAcquireSwizzlingRange;
    DriverObjectExtension->DxgkDdiReleaseSwizzlingRange = DriverInitData-> DxgkDdiReleaseSwizzlingRange;
    DriverObjectExtension->DxgkDdiPatch = DriverInitData-> DxgkDdiPatch;
    DriverObjectExtension->DxgkDdiSubmitCommand = DriverInitData-> DxgkDdiSubmitCommand;
    DriverObjectExtension->DxgkDdiPreemptCommand = DriverInitData-> DxgkDdiPreemptCommand;
    DriverObjectExtension->DxgkDdiBuildPagingBuffer = DriverInitData-> DxgkDdiBuildPagingBuffer;
    DriverObjectExtension->DxgkDdiSetPalette = DriverInitData-> DxgkDdiSetPalette;
    DriverObjectExtension->DxgkDdiSetPointerPosition = DriverInitData-> DxgkDdiSetPointerPosition;
    DriverObjectExtension->DxgkDdiSetPointerShape = DriverInitData-> DxgkDdiSetPointerShape;
    DriverObjectExtension->DxgkDdiResetFromTimeout = DriverInitData-> DxgkDdiResetFromTimeout;
    DriverObjectExtension->DxgkDdiRestartFromTimeout = DriverInitData-> DxgkDdiRestartFromTimeout;
    DriverObjectExtension->DxgkDdiEscape = DriverInitData-> DxgkDdiEscape;
    DriverObjectExtension->DxgkDdiCollectDbgInfo = DriverInitData-> DxgkDdiCollectDbgInfo;
    DriverObjectExtension->DxgkDdiQueryCurrentFence = DriverInitData-> DxgkDdiQueryCurrentFence;
    DriverObjectExtension->DxgkDdiIsSupportedVidPn = DriverInitData-> DxgkDdiIsSupportedVidPn;
    DriverObjectExtension->DxgkDdiRecommendFunctionalVidPn = DriverInitData-> DxgkDdiRecommendFunctionalVidPn;
    DriverObjectExtension->DxgkDdiEnumVidPnCofuncModality = DriverInitData-> DxgkDdiEnumVidPnCofuncModality;
    DriverObjectExtension->DxgkDdiSetVidPnSourceAddress = DriverInitData-> DxgkDdiSetVidPnSourceAddress;
    DriverObjectExtension->DxgkDdiSetVidPnSourceVisibility = DriverInitData-> DxgkDdiSetVidPnSourceVisibility;
    DriverObjectExtension->DxgkDdiCommitVidPn = DriverInitData-> DxgkDdiCommitVidPn;
    DriverObjectExtension->DxgkDdiUpdateActiveVidPnPresentPath = DriverInitData-> DxgkDdiUpdateActiveVidPnPresentPath;
    DriverObjectExtension->DxgkDdiRecommendMonitorModes = DriverInitData-> DxgkDdiRecommendMonitorModes;
    DriverObjectExtension->DxgkDdiRecommendVidPnTopology = DriverInitData-> DxgkDdiRecommendVidPnTopology;
    DriverObjectExtension->DxgkDdiGetScanLine = DriverInitData-> DxgkDdiGetScanLine;
    DriverObjectExtension->DxgkDdiStopCapture = DriverInitData-> DxgkDdiStopCapture;
    DriverObjectExtension->DxgkDdiControlInterrupt = DriverInitData-> DxgkDdiControlInterrupt;
    DriverObjectExtension->DxgkDdiCreateOverlay = DriverInitData-> DxgkDdiCreateOverlay;
    DriverObjectExtension->DxgkDdiDestroyDevice = DriverInitData-> DxgkDdiDestroyDevice;
    DriverObjectExtension->DxgkDdiOpenAllocation = DriverInitData-> DxgkDdiOpenAllocation;
    DriverObjectExtension->DxgkDdiCloseAllocation = DriverInitData-> DxgkDdiCloseAllocation;
    DriverObjectExtension->DxgkDdiRender = DriverInitData-> DxgkDdiRender;
    DriverObjectExtension->DxgkDdiPresent = DriverInitData-> DxgkDdiPresent;
    DriverObjectExtension->DxgkDdiUpdateOverlay = DriverInitData-> DxgkDdiUpdateOverlay;
    DriverObjectExtension->DxgkDdiFlipOverlay = DriverInitData-> DxgkDdiFlipOverlay;
    DriverObjectExtension->DxgkDdiDestroyOverlay = DriverInitData-> DxgkDdiDestroyOverlay;
    DriverObjectExtension->DxgkDdiCreateContext = DriverInitData-> DxgkDdiCreateContext;
    DriverObjectExtension->DxgkDdiDestroyContext = DriverInitData-> DxgkDdiDestroyContext;
    DriverObjectExtension->DxgkDdiLinkDevice = DriverInitData-> DxgkDdiLinkDevice;
    DriverObjectExtension->DxgkDdiSetDisplayPrivateDriverFormat = DriverInitData-> DxgkDdiSetDisplayPrivateDriverFormat;

    /* Fill out the dispatch routines */;
    DriverExtend = DriverObject->DriverExtension;
    DriverObject->MajorFunction[IRP_MJ_CREATE] = (PDRIVER_DISPATCH)RDDM_MiniportDispatchCreate;
    DriverObject->MajorFunction[IRP_MJ_PNP] = (PDRIVER_DISPATCH)RDDM_MiniportDispatchPnp;
    DriverObject->MajorFunction[IRP_MJ_POWER] = (PDRIVER_DISPATCH)RDDM_MiniportDispatchPower;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = (PDRIVER_DISPATCH)RDDM_MiniportDispatchIoctl;
    DriverObject->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = (PDRIVER_DISPATCH)RDDM_MiniportDispatchInternalIoctl;
    DriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = (PDRIVER_DISPATCH)RDDM_MiniportDispatchSystemControl;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = (PDRIVER_DISPATCH)RDDM_MiniportDispatchClose;
  // DriverObject->DriverInit =
   DriverExtend->AddDevice = RDDM_MiniportAddDevice;
    DriverObject->DriverUnload = (PDRIVER_UNLOAD)RDDM_MiniportDriverUnload;
    DPRINT1("DpiInitialize: Finished\n");
    return STATUS_SUCCESS;
}

VOID
RDDM_CreateWin32kInterface(PDXGKWIN32K_INTERFACE Interface, ULONG *Size);

NTSTATUS
NTAPI   //TODO: Implement me
DxgkInternalDeviceIoctl(DEVICE_OBJECT *DeviceObject, IRP *Irp)
{
    ULONG Size = 0;
    PVOID *OutputBuffer;
    PDXGKWIN32K_INTERFACE Interface;
    IO_STACK_LOCATION *IrpStack = Irp->Tail.Overlay.CurrentStackLocation;
    ULONG IoControlCode = IrpStack->Parameters.Read.ByteOffset.LowPart;
    switch (IoControlCode)
    {
        case IOCTL_VIDEO_DDI_FUNC_REGISTER:
            OutputBuffer = (PVOID*)Irp->UserBuffer;
            Irp->IoStatus.Information = 0;
            Irp->IoStatus.Status = STATUS_SUCCESS;
            *OutputBuffer = DpiInitialize;
            DPRINT1("IOCTL_VIDEO_DDI_FUNC_REGISTER - Queued DpiInitialize up\n");
            break;
        case 0x23E057:
            /* Convert to le function pointer list */
            Interface = (PDXGKWIN32K_INTERFACE)Irp->UserBuffer;
            /* gather le list */
            RDDM_CreateWin32kInterface(Interface, &Size);
            /* Le wants size for some reason */
            Irp->IoStatus.Information = Size;
            DPRINT1("IOCTL_VIDEO_REGISTER_WIN32K_INTERFACE\n");
            break;
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

