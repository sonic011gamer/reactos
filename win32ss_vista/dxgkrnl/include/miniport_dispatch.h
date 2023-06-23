#pragma once

NTSTATUS
NTAPI
RDDM_MiniportDispatchCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp);

NTSTATUS
NTAPI
RDDM_MiniportDispatchPnp(PDEVICE_OBJECT DeviceObject, PVOID Tag);

PSTR
NTAPI
RDDM_MiniportDispatchPower(PDEVICE_OBJECT DeviceObject, PSTR MutableMessage);

NTSTATUS
NTAPI
RDDM_MiniportDispatchIoctl(PDEVICE_OBJECT DeviceObject, IRP *Irp);

NTSTATUS
NTAPI
RDDM_MiniportDispatchInternalIoctl(PDEVICE_OBJECT DeviceObject, IRP *Irp);

NTSTATUS
NTAPI
RDDM_MiniportDispatchSystemControl(PDEVICE_OBJECT DeviceObject, PVOID Tag);

NTSTATUS
NTAPI
RDDM_MiniportDispatchClose(PDEVICE_OBJECT DeviceObject);

NTSTATUS
NTAPI
RDDM_MiniportAddDevice(PDRIVER_OBJECT DriverObject, PDEVICE_OBJECT TargetDevice);

NTSTATUS
NTAPI
RDDM_MiniportDriverUnload(PDRIVER_OBJECT DriverObject);
