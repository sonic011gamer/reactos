/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DxgKrnl miniport Management
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <dxgkrnl.h>
//#define NDEBUG
#include <debug.h>





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
    DPRINT1("RdPortAddDevice Entry\n");
    return 0;
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
