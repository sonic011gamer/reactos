/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     RDDM Miniport IOCTRL Handlers
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <dxgkrnl.h>
//#define NDEBUG
#include <debug.h>

DXGKRNL_INTERFACE DxgkrnlInterface;
extern PDXGKRNL_PRIVATE_EXTENSION Extension;
/*
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dispmprt/nc-dispmprt-dxgkcb_eval_acpi_method
 * @ UNIMPLEMENTED
 */
NTSTATUS
APIENTRY
RDDM_DxgkCbEvalAcpiMethod(_In_ HANDLE DeviceHandle,
                          _In_ ULONG DeviceUid,
                          _In_reads_bytes_(AcpiInputSize) PACPI_EVAL_INPUT_BUFFER_COMPLEX AcpiInputBuffer,
                          _In_range_(>=, sizeof(ACPI_EVAL_INPUT_BUFFER_COMPLEX)) ULONG AcpiInputSize,
                          _Out_writes_bytes_(AcpiOutputSize) PACPI_EVAL_OUTPUT_BUFFER AcpiOutputBuffer,
                          _In_range_(>=, sizeof(ACPI_EVAL_OUTPUT_BUFFER)) ULONG AcpiOutputSize)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return STATUS_UNSUCCESSFUL;
}

/**
 * @brief Fills out the DXGK_DEVICE_INFO parameter allocated
 *  by a miniport driver
 *
 * @param DeviceHandle HANDLE Obtained via the DXGKRNL_INTERFACE passed to miniport
 *
 * @param DeviceInfo Strucutre that includes many use bits of information for miniports, Including the IO Ranges
 *
 * @return NTSTATUS
 */
NTSTATUS
APIENTRY
RDDM_DxgkCbGetDeviceInformation(_In_ HANDLE DeviceHandle,
                                _Out_ PDXGK_DEVICE_INFO DeviceInfo)
{

    DeviceInfo->MiniportDeviceContext = Extension->MiniportFdo;
    DeviceInfo->PhysicalDeviceObject = Extension->MiniportPdo;
   // DeviceInfo->TranslatedResourceList =
    __debugbreak();
    return STATUS_SUCCESS;
}

/*
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dispmprt/nc-dispmprt-dxgkcb_indicate_child_status
 * @ UNIMPLEMENTED
 */
NTSTATUS
APIENTRY
RDDM_DxgkCbIndicateChildStatus(_In_ HANDLE DeviceHandle,
                               _In_ PDXGK_CHILD_STATUS ChildStatus)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return STATUS_UNSUCCESSFUL;
}


NTSTATUS
APIENTRY
RDDM_DxgkCbMapMemory(_In_ HANDLE DeviceHandle,
                     _In_ PHYSICAL_ADDRESS TranslatedAddress,
                     _In_ ULONG Length,
                     _In_ BOOLEAN InIoSpace,
                     _In_ BOOLEAN MapToUserMode,
                     _In_ MEMORY_CACHING_TYPE CacheType,
                     _Outptr_ PVOID *VirtualAddress)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return STATUS_UNSUCCESSFUL;
}

BOOLEAN
APIENTRY
RDDM_DxgkCbQueueDpc(_In_ HANDLE DeviceHandle)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return FALSE;
}


NTSTATUS
APIENTRY
RDDM_DxgkCbQueryServices(_In_ HANDLE DeviceHandle,
                         _In_ DXGK_SERVICES ServicesType,
                         _Inout_ PINTERFACE Interface)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return STATUS_UNSUCCESSFUL;
}

NTSTATUS
APIENTRY
RDDM_DxgkCbReadDeviceSpace(_In_ HANDLE DeviceHandle,
                           _In_ ULONG DataType,
                           _Out_writes_bytes_to_(Length, *BytesRead) PVOID Buffer,
                           _In_ ULONG Offset,
                           _In_ ULONG Length,
                           _Out_ PULONG BytesRead)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return STATUS_UNSUCCESSFUL;
}

NTSTATUS
APIENTRY
RDDM_DxgkCbSynchronizeExecution(_In_ HANDLE DeviceHandle,
                                _In_ PKSYNCHRONIZE_ROUTINE SynchronizeRoutine,
                                _In_ PVOID Context,
                                _In_ ULONG MessageNumber,
                                _Out_ PBOOLEAN ReturnValue)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return STATUS_UNSUCCESSFUL;
}

NTSTATUS
APIENTRY
RDDM_DxgkCbUnmapMemory(_In_ HANDLE DeviceHandle,
                       _In_ PVOID VirtualAddress)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return STATUS_UNSUCCESSFUL;
}

NTSTATUS
APIENTRY
RDDM_DxgkCbWriteDeviceSpace(_In_ HANDLE DeviceHandle,
                            _In_ ULONG DataType,
                            _In_reads_bytes_(Length) PVOID Buffer,
                            _In_ ULONG Offset,
                            _In_ ULONG Length,
                            _Out_ _Out_range_(<=, Length) PULONG BytesWritten)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return STATUS_UNSUCCESSFUL;
}

NTSTATUS
APIENTRY
RDDM_DxgkCbIsDevicePresent(_In_ HANDLE DeviceHandle,
                           _In_ PPCI_DEVICE_PRESENCE_PARAMETERS DevicePresenceParameters,
                           _Out_ PBOOLEAN DevicePresent)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return STATUS_UNSUCCESSFUL;
}

VOID*
APIENTRY
CALLBACK
RDDM_DxgkCbGetHandleData(IN_CONST_PDXGKARGCB_GETHANDLEDATA GetHandleData)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return NULL;
}

D3DKMT_HANDLE
APIENTRY
CALLBACK
RDDM_DxgkCbGetHandleParent(IN_D3DKMT_HANDLE hAllocation)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return STATUS_UNSUCCESSFUL;
}

D3DKMT_HANDLE
APIENTRY
CALLBACK
RDDM_DxgkCbEnumHandleChildren(IN_CONST_PDXGKARGCB_ENUMHANDLECHILDREN EnumHandleChildren)
{
        //TODO: Implement meh
    UNIMPLEMENTED;
    return STATUS_UNSUCCESSFUL;
}

VOID
APIENTRY
CALLBACK
RDDM_DxgkCbNotifyInterrupt(IN_CONST_HANDLE hAdapter,
                           IN_CONST_PDXGKARGCB_NOTIFY_INTERRUPT_DATA NotifyInterruptData)
{
   //TODO: Implement meh
    UNIMPLEMENTED;
}

VOID
APIENTRY
CALLBACK
RDDM_DxgkCbNotifyDpc(IN_CONST_HANDLE hAdapter)
{
   //TODO: Implement meh
    UNIMPLEMENTED;
}

NTSTATUS
APIENTRY
CALLBACK
RDDM_DxgkCbQueryVidPnInterface(IN_CONST_D3DKMDT_HVIDPN                hVidPn,
                               IN_CONST_DXGK_VIDPN_INTERFACE_VERSION  VidPnInterfaceVersion,
                               DEREF_OUT_CONST_PPDXGK_VIDPN_INTERFACE ppVidPnInterface)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return STATUS_UNSUCCESSFUL;
}

NTSTATUS
APIENTRY
CALLBACK
RDDM_DxgkCbQueryMonitorInterface(IN_CONST_HANDLE                          hAdapter,
                                 IN_CONST_DXGK_MONITOR_INTERFACE_VERSION  MonitorInterfaceVersion,
                                 DEREF_OUT_CONST_PPDXGK_MONITOR_INTERFACE ppMonitorInterface)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return STATUS_UNSUCCESSFUL;
}

NTSTATUS
APIENTRY
CALLBACK
RDDM_DxgkCbGetCaptureAddress(INOUT_PDXGKARGCB_GETCAPTUREADDRESS GetCaptureAddress)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return STATUS_UNSUCCESSFUL;
}

VOID
APIENTRY
RDDM_DxgkCbLogEtwEvent(_In_ CONST LPCGUID EventGuid,
                       _In_ UCHAR Type,
                       _In_ USHORT EventBufferSize,
                       _In_reads_bytes_(EventBufferSize) PVOID EventBuffer)
{
   //TODO: Implement meh
    UNIMPLEMENTED;
}

NTSTATUS
APIENTRY
CALLBACK
DxgkCbAcquirePostDisplayOwnership(_In_ HANDLE DeviceHandle,
                                  _Out_ PDXGK_DISPLAY_INFORMATION DisplayInfo)
{
    DPRINT1("DxgkCbAcquirePostDisplayOwnership: Enter with device handle %X\n", DeviceHandle);
    DisplayInfo = {0};
    DisplayInfo->Width = 10;
    return 0;
}
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
    DxgkrnlInterface.Version = DXGKDDI_INTERFACE_VERSION_VISTA_SP1;
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

    DxgkrnlInterface.DxgkCbCreateContextAllocation = (DXGKCB_CREATECONTEXTALLOCATION          )NULL;
    DxgkrnlInterface.DxgkCbDestroyContextAllocation = (DXGKCB_DESTROYCONTEXTALLOCATION         )NULL;
    DxgkrnlInterface.DxgkCbSetPowerComponentActive = (DXGKCB_SETPOWERCOMPONENTACTIVE          )NULL;
    DxgkrnlInterface.DxgkCbSetPowerComponentIdle = (DXGKCB_SETPOWERCOMPONENTIDLE            )NULL;
    DxgkrnlInterface.DxgkCbAcquirePostDisplayOwnership = DxgkCbAcquirePostDisplayOwnership;
    DxgkrnlInterface.DxgkCbPowerRuntimeControlRequest = (DXGKCB_POWERRUNTIMECONTROLREQUEST       )NULL;
    DxgkrnlInterface.DxgkCbSetPowerComponentLatency = (DXGKCB_SETPOWERCOMPONENTLATENCY         )NULL;
    DxgkrnlInterface.DxgkCbSetPowerComponentResidency = (DXGKCB_SETPOWERCOMPONENTRESIDENCY       )NULL;
    DxgkrnlInterface.DxgkCbCompleteFStateTransition = (DXGKCB_COMPLETEFSTATETRANSITION         )NULL;
    DxgkrnlInterface.DxgkCbCompletePStateTransition = (DXGKCB_COMPLETEPSTATETRANSITION         )NULL;
    DxgkrnlInterface.DxgkCbMapContextAllocation = (DXGKCB_MAPCONTEXTALLOCATION             )NULL;
    DxgkrnlInterface.DxgkCbUpdateContextAllocation = (DXGKCB_UPDATECONTEXTALLOCATION          )NULL;
    DxgkrnlInterface.DxgkCbReserveGpuVirtualAddressRange = (DXGKCB_RESERVEGPUVIRTUALADDRESSRANGE    )NULL;
    DxgkrnlInterface.DxgkCbAcquireHandleData = (DXGKCB_ACQUIREHANDLEDATA                )NULL;
    DxgkrnlInterface.DxgkCbReleaseHandleData = (DXGKCB_RELEASEHANDLEDATA                )NULL;
    DxgkrnlInterface.DxgkCbHardwareContentProtectionTeardown = (DXGKCB_HARDWARECONTENTPROTECTIONTEARDOWN)NULL;
#if 0
    DXGKCB_CREATECONTEXTALLOCATION           DxgkCbCreateContextAllocation;
    DXGKCB_DESTROYCONTEXTALLOCATION          DxgkCbDestroyContextAllocation;
    DXGKCB_SETPOWERCOMPONENTACTIVE           DxgkCbSetPowerComponentActive;
    DXGKCB_SETPOWERCOMPONENTIDLE             DxgkCbSetPowerComponentIdle;
    DXGKCB_ACQUIRE_POST_DISPLAY_OWNERSHIP    DxgkCbAcquirePostDisplayOwnership;
    DXGKCB_POWERRUNTIMECONTROLREQUEST        DxgkCbPowerRuntimeControlRequest;
    DXGKCB_SETPOWERCOMPONENTLATENCY          DxgkCbSetPowerComponentLatency;
    DXGKCB_SETPOWERCOMPONENTRESIDENCY        DxgkCbSetPowerComponentResidency;
    DXGKCB_COMPLETEFSTATETRANSITION          DxgkCbCompleteFStateTransition;
    DXGKCB_COMPLETEPSTATETRANSITION          DxgkCbCompletePStateTransition;
    DXGKCB_MAPCONTEXTALLOCATION              DxgkCbMapContextAllocation;
    DXGKCB_UPDATECONTEXTALLOCATION           DxgkCbUpdateContextAllocation;
    DXGKCB_RESERVEGPUVIRTUALADDRESSRANGE     DxgkCbReserveGpuVirtualAddressRange;
    DXGKCB_ACQUIREHANDLEDATA                 DxgkCbAcquireHandleData;
    DXGKCB_RELEASEHANDLEDATA                 DxgkCbReleaseHandleData;
    DXGKCB_HARDWARECONTENTPROTECTIONTEARDOWN DxgkCbHardwareContentProtectionTeardown;
#endif
    DPRINT1("Targetting version: %X\n", DxgkrnlInterface.Version);
    DPRINT1("Dxgkrnl has started\n\n");
   // RDDM_AddDeviceMapLink();
    return STATUS_SUCCESS;
}