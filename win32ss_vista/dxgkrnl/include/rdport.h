#pragma once

NTSTATUS
NTAPI
RdPortSetupGlobalState();

/* MINIPORT IOCTRLS *********************************************/

NTSTATUS
NTAPI
RdPortDispatchCreateDevice(_In_    PDEVICE_OBJECT DeviceObject,
                            _Inout_ PIRP Irp);
NTSTATUS
NTAPI
RdPortDispatchPnp(_In_ PDEVICE_OBJECT DeviceObject,
                   _In_ PVOID Tag);
PSTR
NTAPI
RdPortDispatchPower(_In_ PDEVICE_OBJECT DeviceObject,
                     _In_ PSTR MutableMessage);

NTSTATUS
NTAPI
RdPortDispatchIoctl(_In_    PDEVICE_OBJECT DeviceObject,
                     _Inout_ IRP *Irp);

NTSTATUS
NTAPI
RdPortDispatchInternalIoctl(_In_ PDEVICE_OBJECT DeviceObject,
                                     _Inout_ IRP *Irp);
NTSTATUS
NTAPI
RdPortDispatchSystemControl(_In_ PDEVICE_OBJECT DeviceObject,
                             _In_ PVOID Tag);
NTSTATUS
NTAPI
RdPortDispatchCloseDevice(_In_ PDEVICE_OBJECT DeviceObject);

NTSTATUS
NTAPI
RdPortAddDevice(_In_    DRIVER_OBJECT *DriverObject,
                 _Inout_ DEVICE_OBJECT *PhysicalDeviceObject);
NTSTATUS
NTAPI
RdPortDriverUnload(_In_ PDRIVER_OBJECT DriverObject);
