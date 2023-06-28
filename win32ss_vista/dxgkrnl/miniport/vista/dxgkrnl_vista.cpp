/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     WDDM 1.0-1.1 initilization
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include <dxgkrnl.h>
#define NDEBUG
#include <debug.h>

DXGKRNL_INTERFACE DxgkrnlInterface;
/*
 * I turned this into a internal function to keep better eventual seperation of the
 * WDDM 1.2+ and WDDM 1.0-1.1 APIs
 */
NTSTATUS
NTAPI
RDDM_SetupDxgkrnl(
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath)
{
    DxgkrnlInterface.Size = sizeof(DXGKRNL_INTERFACE);
    DxgkrnlInterface.Version = DXGKDDI_INTERFACE_VERSION_WIN7;
    DxgkrnlInterface.DeviceHandle = (HANDLE)DriverObject;
    DxgkrnlInterface.DxgkCbEvalAcpiMethod = RDDM_DxgkCbEvalAcpiMethod;
    DxgkrnlInterface.DxgkCbGetDeviceInformation = RDDM_DxgkCbGetDeviceInformation;
    DxgkrnlInterface.DxgkCbIndicateChildStatus = RDDM_DxgkCbIndicateChildStatus;
    DxgkrnlInterface.DxgkCbMapMemory = RDDM_DxgkCbMapMemory;
    DxgkrnlInterface.DxgkCbQueueDpc = RDDM_DxgkCbQueueDpc;
    DxgkrnlInterface.DxgkCbQueryServices = RDDM_DxgkCbQueryServices;
    DxgkrnlInterface.DxgkCbReadDeviceSpace = RDDM_DxgkCbReadDeviceSpace;
    DxgkrnlInterface.DxgkCbSynchronizeExecution = RDDM_DxgkCbSynchronizeExecution;
    DxgkrnlInterface.DxgkCbUnmapMemory = RDDM_DxgkCbUnmapMemory;
    DxgkrnlInterface.DxgkCbWriteDeviceSpace = RDDM_DxgkCbWriteDeviceSpace;
    DxgkrnlInterface.DxgkCbIsDevicePresent = RDDM_DxgkCbIsDevicePresent;
    DxgkrnlInterface.DxgkCbGetHandleData = RDDM_DxgkCbGetHandleData;
    DxgkrnlInterface.DxgkCbGetHandleParent = RDDM_DxgkCbGetHandleParent;
    DxgkrnlInterface.DxgkCbEnumHandleChildren = RDDM_DxgkCbEnumHandleChildren;
    DxgkrnlInterface.DxgkCbNotifyInterrupt = RDDM_DxgkCbNotifyInterrupt;
    DxgkrnlInterface.DxgkCbNotifyDpc = RDDM_DxgkCbNotifyDpc;
    DxgkrnlInterface.DxgkCbQueryVidPnInterface = RDDM_DxgkCbQueryVidPnInterface;
    DxgkrnlInterface.DxgkCbQueryMonitorInterface = RDDM_DxgkCbQueryMonitorInterface;
    DxgkrnlInterface.DxgkCbGetCaptureAddress = RDDM_DxgkCbGetCaptureAddress;
    DxgkrnlInterface.DxgkCbLogEtwEvent = RDDM_DxgkCbLogEtwEvent;
    DPRINT1("---------------------------ReactOS Display Driver Model---------------------------\n");
    DPRINT1("Targetting version: %X\n", DxgkrnlInterface.Version);
    DPRINT1("Dxgkrnl has started\n\n");
   // RDDM_AddDeviceMapLink();
    return STATUS_SUCCESS;
}
