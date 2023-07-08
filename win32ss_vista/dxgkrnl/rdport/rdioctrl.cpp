/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     RDDM Miniport IOCTRL Handlers
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <dxgkrnl.h>
//#define NDEBUG
#include <debug.h>

PDXGKRNL_PRIVATE_EXTENSION Extension;

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
RdPort_DispatchCreateDevice(_In_    PDEVICE_OBJECT DeviceObject,
                            _Inout_ PIRP Irp)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
RdPort_DispatchPnp(_In_ PDEVICE_OBJECT DeviceObject,
                   _In_ PVOID Tag)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

/*
 * @ UNIMPLEMENTED
 */
PSTR
NTAPI
RdPort_DispatchPower(_In_ PDEVICE_OBJECT DeviceObject,
                     _In_ PSTR MutableMessage)
{
    UNIMPLEMENTED;
    __debugbreak();
    return MutableMessage;
}

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
RdPort_DispatchIoctl(_In_    PDEVICE_OBJECT DeviceObject,
                     _Inout_ IRP *Irp)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
RdPort_DispatchInternalIoctl(_In_ PDEVICE_OBJECT DeviceObject,
                                     _Inout_ IRP *Irp)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
RdPort_DispatchSystemControl(_In_ PDEVICE_OBJECT DeviceObject,
                                     _In_ PVOID Tag)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
RdPort_DispatchCloseDevice(_In_ PDEVICE_OBJECT DeviceObject)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

/**
 * @brief Intercepts and calls the AddDevice Miniport call back
 *
 * @param DriverObject - Pointer to DRIVER_OBJECT structure
 *
 * @param PhysicalDeviceObject - Pointer to DEVICE_OBJECT structure
 *
 * @return NTSTATUS
 */
NTSTATUS
NTAPI
RdPort_AddDevice(_In_    DRIVER_OBJECT *DriverObject,
                 _Inout_ DEVICE_OBJECT *PhysicalDeviceObject)
{
    NTSTATUS Status;
    PDEVICE_OBJECT Fdo;
    IO_STATUS_BLOCK IoStatusBlock;

    PAGED_CODE();

    Extension = NULL;
    ULONG_PTR Context = 0;
    /* MS does a whole bunch of bullcrap here so we will try to track it */
    if (!DriverObject || !PhysicalDeviceObject)
    {
        DPRINT1("RDDM_MiniportAddDevice: Something has seriously fucked up\n");
        return STATUS_INVALID_PARAMETER;
    }

    /*
     * This routine implemented the Miniport AddDevice along with trying enumerate what
     * the GPU is "attached to", On the off chance a GPU isn't enumerated over PCI we use ACPI here,
     * This routine also decides when to use MSI-(X) Interrupts; A Royal pain in the butt.
     *
     * 1) First lets get the device extension for DXGKRNL
     * 2) nextlet's call a private API to handle calling Add Device.
     * 3) After that we will call IoCreateDevice to create the device
     */

    /* Grab the DXGKRNL internal extension */
    Extension = (PDXGKRNL_PRIVATE_EXTENSION)IoGetDriverObjectExtension(DriverObject, DriverObject);
    if (!Extension)
    {
        DPRINT1("Could not gather DXGKRNL Extension\n");
    }

    /* Call the miniport Routine */
    Status = Extension->DriverInitData.DxgkDdiAddDevice(PhysicalDeviceObject, (PVOID*)&Context);
    if(Status != STATUS_SUCCESS)
    {
        DPRINT1("RDDM_MiniportAddDevice: AddDevice Miniport call failed with status %X\n", Status);
    }
    else{
        DPRINT1("RDDM_MiniportAddDevice: AddDevice Miniport call has continued with success\n");
    }

    Extension->MiniportContext = (PVOID)Context;
    IoStatusBlock.Information = 1024;
    Status = IoCreateDevice(DriverObject,
                            IoStatusBlock.Information,
                            FALSE,
                            FILE_DEVICE_VIDEO,
                            FILE_DEVICE_SECURE_OPEN,
                            FALSE,
                            &Fdo);

    Extension->MiniportFdo = Fdo;
    Extension->MiniportPdo = PhysicalDeviceObject;
    DPRINT1("RdPort_AddDevice: Created Device\n");
    __debugbreak();
    return 0;
}

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
RdPort_DriverUnload(_In_ PDRIVER_OBJECT DriverObject)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}
