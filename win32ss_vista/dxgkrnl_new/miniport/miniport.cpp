/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DxgKrnl miniport Management
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <dxgkrnl.h>
//#define NDEBUG
#include <debug.h>

extern PDXGKRNL_PRIVATE_EXTENSION DxgkpExtension;
extern DXGKRNL_INTERFACE DxgkrnlInterface;
NTSTATUS
NTAPI
DxgkPortStartAdapter()
{
    NTSTATUS Status;
    DXGK_START_INFO     DxgkStartInfo = {0};
    //HACK: Yeah. No.
    DxgkStartInfo.AdapterGuid = {1, 0};
       //DxgkStartInfo.TcbPrivilege = {SE_TCB_PRIVILEGE, 0};
    ULONG              NumberOfVideoPresentSources;
    ULONG              NumberOfChildren;
    DPRINT1("DxgkPortStartAdapter: EntryPoint\n");
    Status = DxgkpExtension->DxgkDdiStartDevice(DxgkpExtension->MiniportContext, &DxgkStartInfo,
                                                 &DxgkrnlInterface, &NumberOfVideoPresentSources, &NumberOfChildren);
    if (Status == STATUS_SUCCESS)
    {
        DPRINT1("DxgkPortStartAdapter: Device has started\n");
    }
    else{
        DPRINT1("DxgkPortStartAdapter: Failed with Status %d\n", Status);
    }
    __debugbreak();
    return Status;
}


/**
 * @brief Intercepts and calls the AddDevice Miniport call back
 *
 * @param DriverObject - Pointer to DRIVER_OBJECT structure
 *
 * @param PhysicalDeviceObject - Pointer to Miniport DEVICE_OBJECT structure
 *
 * @return NTSTATUS
 */
NTSTATUS
NTAPI
DxgkPortAddDevice(_In_    DRIVER_OBJECT *DriverObject,
                  _Inout_ DEVICE_OBJECT *PhysicalDeviceObject)
{
    DPRINT1("DxgkPortAddDevice Entry\n");
    NTSTATUS Status;
    PDEVICE_OBJECT Fdo;
    IO_STATUS_BLOCK IoStatusBlock;
    WCHAR DeviceBuffer[20];
    UNICODE_STRING DeviceName;
    PAGED_CODE();
    ULONG_PTR Context = 0;
    /* MS does a whole bunch of bullcrap here so we will try to track it */
    if (!DriverObject || !PhysicalDeviceObject)
    {
        DPRINT1("DxgkPortAddDevice: Something has seriously fucked up\n");
        return STATUS_INVALID_PARAMETER;
    }

    /* Grab the DXGKRNL internal extension */
    #if 0
        /* maybe let's call the miniport extension locally instead of globally? */
    DxgkpExtension = (PDXGKRNL_PRIVATE_EXTENSION)IoGetDriverObjectExtension(DriverObject, DriverObject);
    if (!DxgkpExtension)
    {
        DPRINT1("Could not gather DXGKRNL Extension\n");
    }
    #endif
    DPRINT1("Calling the Miniport Device\n");
    /* Call the miniport Routine */
    Status = DxgkpExtension->DxgkDdiAddDevice(PhysicalDeviceObject, (PVOID*)&Context);
    if(Status != STATUS_SUCCESS)
    {
        DPRINT1("DxgkPortAddDevice: AddDevice Miniport call failed with status %X\n", Status);
    }
    else{
        DPRINT1("DxgkPortAddDevice: AddDevice Miniport call has continued with success\n");
    }

    swprintf(DeviceBuffer, L"\\Device\\Video%lu", 0);
    RtlInitUnicodeString(&DeviceName, DeviceBuffer);

    IoStatusBlock.Information = 1024; //TODO: ehhh
    DxgkpExtension->MiniportContext = (PVOID)Context;
    Status = IoCreateDevice(DriverObject,
                            IoStatusBlock.Information,
                            &DeviceName,
                            FILE_DEVICE_VIDEO,
                            FILE_DEVICE_SECURE_OPEN,
                            FALSE,
                            &Fdo);
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("IoCreateDevice() failed with status 0x%08x\n", Status);
        return Status;
    }

    Status = IntCreateRegistryPath(&DxgkpExtension->RegistryPath,
                                   0,
                                   &DxgkpExtension->RegistryPath);
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("IntCreateRegistryPath() failed with status 0x%08x\n", Status);
        return Status;
    }

    DxgkpExtension->MiniportFdo = Fdo;
    DxgkpExtension->MiniportPdo = PhysicalDeviceObject;
    DxgkpCreateIds();

    /* Remove the initializing flag */
    (DriverObject->DeviceObject)->Flags &= ~DO_DEVICE_INITIALIZING;
    DxgkpExtension->NextDeviceObject = IoAttachDeviceToDeviceStack(
                                                DriverObject->DeviceObject,
                                                PhysicalDeviceObject);


    Status = IntCreateNewRegistryPath(DxgkpExtension);
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("IntCreateNewRegistryPath() failed with status 0x%08x\n", Status);
        return Status;
    }

    /* Set up the VIDEO/DEVICEMAP registry keys */
    Status = IntVideoPortAddDeviceMapLink(DxgkpExtension);
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("IntVideoPortAddDeviceMapLink() failed with status 0x%08x\n", Status);
        return Status;
    }

    return Status;
}

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
DxgkPortDriverUnload(_In_ PDRIVER_OBJECT DriverObject)
{
    UNIMPLEMENTED;
    //__debugbreak();
    return 0;
}


NTSTATUS
NTAPI
DxgkPortDispatchCreateDevice(_In_    PDEVICE_OBJECT DeviceObject,
                            _Inout_ PIRP Irp)
{
    UNIMPLEMENTED;
    //__debugbreak();
    return 0;
}

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
DxgkPortDispatchPnp(_In_ PDEVICE_OBJECT DeviceObject,
                   _In_ PVOID Tag)
{
    UNIMPLEMENTED;
    //__debugbreak();
    return 0;
}

/*
 * @ UNIMPLEMENTED
 */
PSTR
NTAPI
DxgkPortDispatchPower(_In_ PDEVICE_OBJECT DeviceObject,
                     _In_ PSTR MutableMessage)
{
    UNIMPLEMENTED;
    //__debugbreak();
    return MutableMessage;
}

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
DxgkPortDispatchIoctl(_In_    PDEVICE_OBJECT DeviceObject,
                     _Inout_ IRP *Irp)
{
    UNIMPLEMENTED;
    //__debugbreak();
    return 0;
}

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
DxgkPortDispatchInternalIoctl(_In_ PDEVICE_OBJECT DeviceObject,
                                     _Inout_ IRP *Irp)
{
    UNIMPLEMENTED;
    //__debugbreak();
    return 0;
}

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
DxgkPortDispatchSystemControl(_In_ PDEVICE_OBJECT DeviceObject,
                                     _In_ PVOID Tag)
{
    UNIMPLEMENTED;
    //__debugbreak();
    return 0;
}

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
DxgkPortDispatchCloseDevice(_In_ PDEVICE_OBJECT DeviceObject)
{
    UNIMPLEMENTED;
    //__debugbreak();
    return 0;
}
