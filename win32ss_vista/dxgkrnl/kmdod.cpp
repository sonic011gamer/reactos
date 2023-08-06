/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DXGKRNL KMDOD Handling
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <include/dxgkrnl.h>
//#define NDEBUG
#include <debug.h>

NTSTATUS
NTAPI
DpiKmdDodInitialize(PDRIVER_OBJECT DriverObject,
                    PUNICODE_STRING SourceString,
                    PKMDDOD_INITIALIZATION_DATA KmdDodInitializationData)
{
    DPRINT("DpiKmdDodInitialize: Entry point\n");
    DPRINT("DpiKmdDodInitialize: KMDOD WDDM Miniport driver Reports version: 0x%X\n", KmdDodInitializationData->Version);
    
    /* Let's just make sure we can continue */
    if (KmdDodInitializationData->DxgkDdiAddDevice)
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
    DriverObjectExtension->DriverInitData = *KmdDodInitializationData;
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
    DPRINT("DpiKmdDodInitialize: Finished\n");
    return STATUS_SUCCESS;
    }

    return 0;
}

