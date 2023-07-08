#pragma once

NTSTATUS
NTAPI
RdPort_SetupGlobalState();

/* MINIPORT IOCTRLS *********************************************/

NTSTATUS
NTAPI
RdPort_DispatchCreateDevice(_In_    PDEVICE_OBJECT DeviceObject,
                            _Inout_ PIRP Irp);
NTSTATUS
NTAPI
RdPort_DispatchPnp(_In_ PDEVICE_OBJECT DeviceObject,
                   _In_ PVOID Tag);
PSTR
NTAPI
RdPort_DispatchPower(_In_ PDEVICE_OBJECT DeviceObject,
                     _In_ PSTR MutableMessage);

NTSTATUS
NTAPI
RdPort_DispatchIoctl(_In_    PDEVICE_OBJECT DeviceObject,
                     _Inout_ IRP *Irp);

NTSTATUS
NTAPI
RdPort_DispatchInternalIoctl(_In_ PDEVICE_OBJECT DeviceObject,
                                     _Inout_ IRP *Irp);
NTSTATUS
NTAPI
RdPort_DispatchSystemControl(_In_ PDEVICE_OBJECT DeviceObject,
                             _In_ PVOID Tag);
NTSTATUS
NTAPI
RdPort_DispatchCloseDevice(_In_ PDEVICE_OBJECT DeviceObject);

NTSTATUS
NTAPI
RdPort_AddDevice(_In_    DRIVER_OBJECT *DriverObject,
                 _Inout_ DEVICE_OBJECT *PhysicalDeviceObject);
NTSTATUS
NTAPI
RdPort_DriverUnload(_In_ PDRIVER_OBJECT DriverObject);
