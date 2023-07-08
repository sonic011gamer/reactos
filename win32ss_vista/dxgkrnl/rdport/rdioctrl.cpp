/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     RDDM Miniport IOCTRL Handlers
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <dxgkrnl.h>
//#define NDEBUG
#include <debug.h>

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

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
RdPort_AddDevice(_In_    DRIVER_OBJECT *DriverObject,
                 _Inout_ DEVICE_OBJECT *PhysicalDeviceObject)
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
RdPort_DriverUnload(_In_ PDRIVER_OBJECT DriverObject)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}
