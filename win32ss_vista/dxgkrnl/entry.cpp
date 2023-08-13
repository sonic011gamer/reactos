/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DXGKRNL entry
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <include/dxgkrnl.h>
#include "../include/d3dukmdt.h"
#include "../include/d3dkmdt.h"
#include "../include/d3dkmthk.h"
#include "../include/d3dkmddi.h"
#include "../../cdd/cdd_testinterface.h"
//#define NDEBUG
#include <debug.h>

extern PDXGKRNL_PRIVATE_EXTENSION Extension;

NTSTATUS
NTAPI
DpiKmdDodInitialize(PDRIVER_OBJECT DriverObject,
                    PUNICODE_STRING SourceString,
                    PKMDDOD_INITIALIZATION_DATA KmdDodInitializationData);
extern DXGKRNL_INTERFACE DxgkrnlInterface;
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
RdPort_InitializeMiniport(PDRIVER_OBJECT DriverObject, PUNICODE_STRING SourceString, DRIVER_INITIALIZATION_DATA *DriverInitData)
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
    DPRINT("RdPort_InitializeMiniport: Entry point\n");
    DPRINT("RdPort_InitializeMiniport: WDDM Miniport driver Reports version: 0x%X\n", DriverInitData->Version);
    if (!DriverObject || !SourceString)
    {
        return STATUS_INVALID_PARAMETER;
    }

    /* Setup Global state for Miniport handling */
    Status = RdPort_SetupGlobalState();
    if (Status != STATUS_SUCCESS)
    {
        DPRINT1("RdPort_InitializeMiniport: Couldn't setup global state status: 0x%X", Status);
        return Status;
    }

    /* let's make sure we can allocate the private extension */
    Status = IoAllocateDriverObjectExtension(DriverObject, DriverObject, sizeof(DXGKRNL_PRIVATE_EXTENSION), (PVOID*)&DriverObjectExtension);
    if (Status != STATUS_SUCCESS)
    {
        DPRINT1("RdPort_InitializeMiniport: Couldn't allocate object extension status: 0x%X", Status);
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    /* Fill out the internal structure - WIP */
    //DriverObjectExtension->DriverInitData = (PVOID)DriverInitData;
    DriverObjectExtension->DriverObject = DriverObject;

    /* Fill out the public dispatch routines */
    DriverExtend = DriverObject->DriverExtension;
    DriverObject->MajorFunction[IRP_MJ_CREATE] = (PDRIVER_DISPATCH)RdPort_DispatchCreateDevice;
    DriverObject->MajorFunction[IRP_MJ_PNP] = (PDRIVER_DISPATCH)RdPort_DispatchPnp;
    DriverObject->MajorFunction[IRP_MJ_POWER] = (PDRIVER_DISPATCH)RdPort_DispatchPower;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = (PDRIVER_DISPATCH)RdPort_DispatchIoctl;
    DriverObject->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = (PDRIVER_DISPATCH)RdPort_DispatchInternalIoctl;
    DriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = (PDRIVER_DISPATCH)RdPort_DispatchSystemControl;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = (PDRIVER_DISPATCH)RdPort_DispatchCloseDevice;

    DriverExtend->AddDevice = RdPortAddDevice;
    DriverObject->DriverUnload = (PDRIVER_UNLOAD)RdPort_DriverUnload;
    DPRINT("RdPort_InitializeMiniport: Finished\n");
    return STATUS_SUCCESS;
}

VOID
NTAPI
DxgkrnlVidPnAdapterTest()
{
}

NTSTATUS
NTAPI
DpiStartAdapter()
{
    NTSTATUS Status;
    DXGK_START_INFO     DxgkStartInfo = {0};
    //HACK: Yeah. No.
    DxgkStartInfo.AdapterGuid = {1, 0};
       //DxgkStartInfo.TcbPrivilege = {SE_TCB_PRIVILEGE, 0};
    ULONG              NumberOfVideoPresentSources;
    ULONG              NumberOfChildren;
    DPRINT1("DpiStartAdapter: EntryPoint\n");
    Status = Extension->DriverInitData.DxgkDdiStartDevice(Extension->MiniportContext, &DxgkStartInfo,
                                                 &DxgkrnlInterface, &NumberOfVideoPresentSources, &NumberOfChildren);
    if (Status == STATUS_SUCCESS)
    {
        DPRINT1("DxgkDdiStartDevice: Device has started\n");
    }
    else{
        DPRINT1("DxgkDdiStartDevice: Failed with Status %d\n", Status);
    }

    return Status;
}

DXGKCDD_INTERFACE Interface;
#if 0
typedef struct _D3DKMT_DISPLAYMODE
{
    UINT                                   Width;
    UINT                                   Height;
    D3DDDIFORMAT                           Format;
    UINT                                   IntegerRefreshRate;
    D3DDDI_RATIONAL                        RefreshRate;
    D3DDDI_VIDEO_SIGNAL_SCANLINE_ORDERING  ScanLineOrdering;
    D3DDDI_ROTATION                        DisplayOrientation;
    UINT                                   DisplayFixedOutput;
    D3DKMDT_DISPLAYMODE_FLAGS              Flags;
} D3DKMT_DISPLAYMODE;
#endif
NTSTATUS
APIENTRY
DxgkCdd_GetDisplayMode(PVOID DxgAdater, D3DKMT_GETDISPLAYMODELIST **GetDisplayModeList)
{

    DPRINT1("DxgkCdd_GetDisplayMode: Called\n");
   // D3DKMT_DISPLAYMODE Mode = {0};
   // GetDisplayModeList->pModeList[0] = Mode;
    return 0;
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
            DpiStartAdapter();
            Irp->IoStatus.Status = STATUS_SUCCESS;
            break;
        case 0x23E05B:
            OutputBuffer = (PVOID*)Irp->UserBuffer;
            Irp->IoStatus.Information = 0;
            Irp->IoStatus.Status = STATUS_SUCCESS;
            *OutputBuffer = (PVOID)Interface.DxgkCddWaitForVerticalBlankEvent;
            break;
        case 0x230047:
            /*
             * Grab a reference to the InitializeMiniport function so we can acquire the Miniport
             * callback list and continue setup
             */
            OutputBuffer = (PVOID*)Irp->UserBuffer;
            Irp->IoStatus.Information = 0;
            Irp->IoStatus.Status = STATUS_SUCCESS;
            *OutputBuffer = (PVOID)RdPort_InitializeMiniport;
            DPRINT("0x230047 - Queued DpiKmdDodInitialize up\n");
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
    //__debugbreak();
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
RDDM_SetupDxgkrnl(
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath);

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
     Interface.Version = 1;
     Interface.DxgkCddGetDisplayModeList = DxgkCdd_GetDisplayMode;
    PHYSICAL_ADDRESS Address;
    Address.QuadPart = 0x80000000;
     Interface.DxgkCddWaitForVerticalBlankEvent = MmMapIoSpace(Address,
                                   0x1D4C00,
                                   MmNonCached);

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
    RDDM_SetupDxgkrnl(DriverObject, RegistryPath);
    return Status;
}



/* 
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmdt/ns-d3dkmdt-_dxgk_display_information
 */