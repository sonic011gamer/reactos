/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     RDDM Miniport IOCTRL Calls
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */


#include <dxgkrnl.h>
//#define NDEBUG
#include <debug.h>

extern DXGKRNL_INTERFACE DxgkrnlInterface;

NTSTATUS
NTAPI
RDDM_MiniportDispatchCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
NTAPI
RDDM_MiniportDispatchPnp(PDEVICE_OBJECT DeviceObject, PVOID Tag)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

PSTR
NTAPI
RDDM_MiniportDispatchPower(PDEVICE_OBJECT DeviceObject, PSTR MutableMessage)
{
    UNIMPLEMENTED;
    __debugbreak();
    return MutableMessage;
}

NTSTATUS
NTAPI
RDDM_MiniportDispatchIoctl(PDEVICE_OBJECT DeviceObject, IRP *Irp)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}


NTSTATUS
NTAPI
RDDM_MiniportDispatchInternalIoctl(PDEVICE_OBJECT DeviceObject, IRP *Irp)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
NTAPI
RDDM_MiniportDispatchSystemControl(PDEVICE_OBJECT DeviceObject, PVOID Tag)
{
        UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
NTAPI
RDDM_MiniportDispatchClose(PDEVICE_OBJECT DeviceObject)
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
 * @param TargetDevice - Pointer to DEVICE_OBJECT structure
 *
 * @return NTSTATUS
 *
 *  HALF-IMPLEMENTED
 */
NTSTATUS
NTAPI
RDDM_MiniportAddDevice(PDRIVER_OBJECT DriverObject, PDEVICE_OBJECT TargetDevice)
{
    PDXGKRNL_PRIVATE_EXTENSION Extension = NULL;
    PVOID MiniportDeviceContext = NULL;
    NTSTATUS Status = 0;
    DPRINT1("PVOID RDDM_MiniportAddDevice: Entry\n");

    /* Grab the DXGKRNL internal extension */
    Extension = (PDXGKRNL_PRIVATE_EXTENSION)IoGetDriverObjectExtension(DriverObject, DriverObject);
    if (!Extension)
    {
        DPRINT1("Could not gather DXGKRNL Extension\n");
    }

    /* Call the actual miniport routine */
    Status = Extension->DxgkDdiAddDevice(TargetDevice, &MiniportDeviceContext);
    DPRINT1("DxgkDdiAddDevice Status 0x%X\n", Status);
    __debugbreak();
    return Status;
}

NTSTATUS
NTAPI
RDDM_MiniportDriverUnload(PDRIVER_OBJECT DriverObject)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}
