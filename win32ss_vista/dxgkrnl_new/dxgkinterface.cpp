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
extern PDXGKRNL_PRIVATE_EXTENSION DxgkpExtension;

/*
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dispmprt/nc-dispmprt-dxgkcb_eval_acpi_method
 * @ UNIMPLEMENTED
 */
NTSTATUS
APIENTRY
DxgkCbEvalAcpiMethod(_In_ HANDLE DeviceHandle,
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

NTSTATUS
NTAPI
DxgkrnlSetupResourceList(_Inout_ PCM_RESOURCE_LIST* ResourceList);

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
DxgkCbGetDeviceInformation(_In_ HANDLE DeviceHandle,
                                _Out_ PDXGK_DEVICE_INFO DeviceInfo)
{
    PHYSICAL_ADDRESS PhyNull, HighestPhysicalAddress;
    NTSTATUS Status;

    PCM_RESOURCE_LIST TranslatedResourceList;
    PhyNull.QuadPart = NULL;
    HighestPhysicalAddress.QuadPart = 0x40000000;


    Status = DxgkrnlSetupResourceList(&TranslatedResourceList);
    if (Status != STATUS_SUCCESS)
    {
        DPRINT1("DxgkCbGetDeviceInformation: Failed with status: %X\n", Status);
    }
    DPRINT1("DxgkCbGetDeviceInformation: Called\n");
    DeviceInfo->TranslatedResourceList = TranslatedResourceList;
    DeviceInfo->MiniportDeviceContext = DxgkpExtension->MiniportFdo;
    DeviceInfo->PhysicalDeviceObject = DxgkpExtension->MiniportPdo;
    DeviceInfo->DockingState = DockStateUnsupported;
    DeviceInfo->SystemMemorySize.QuadPart = 0x30000000;
    DeviceInfo->AgpApertureBase = PhyNull;
    DeviceInfo->AgpApertureSize = 0;
    DeviceInfo->DeviceRegistryPath = DxgkpExtension->RegistryPath;
    DeviceInfo->HighestPhysicalAddress = HighestPhysicalAddress;
    DeviceInfo->MiniportDeviceContext = DxgkpExtension->MiniportContext;
    return STATUS_SUCCESS;
}

/*
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dispmprt/nc-dispmprt-dxgkcb_indicate_child_status
 * @ UNIMPLEMENTED
 */
NTSTATUS
APIENTRY
DxgkCbIndicateChildStatus(_In_ HANDLE DeviceHandle,
                               _In_ PDXGK_CHILD_STATUS ChildStatus)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return STATUS_UNSUCCESSFUL;
}


NTSTATUS NTAPI
MapPhysicalMemory(
   IN HANDLE Process,
   IN PHYSICAL_ADDRESS PhysicalAddress,
   IN ULONG SizeInBytes,
   IN ULONG Protect,
   IN OUT PVOID *VirtualAddress  OPTIONAL)
{
   OBJECT_ATTRIBUTES ObjAttribs;
   UNICODE_STRING UnicodeString;
   HANDLE hMemObj;
   NTSTATUS Status;
   SIZE_T Size;

   /* Initialize object attribs */
   RtlInitUnicodeString(&UnicodeString, L"\\Device\\PhysicalMemory");
   InitializeObjectAttributes(&ObjAttribs,
                              &UnicodeString,
                              OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                              NULL, NULL);

   /* Open physical memory section */
   Status = ZwOpenSection(&hMemObj, SECTION_ALL_ACCESS, &ObjAttribs);
   if (!NT_SUCCESS(Status))
   {
      DPRINT1("ZwOpenSection() failed! (0x%x)\n", Status);
      return Status;
   }

   /* Map view of section */
   Size = SizeInBytes;
   Status = ZwMapViewOfSection(hMemObj,
                               Process,
                               VirtualAddress,
                               0,
                               Size,
                               (PLARGE_INTEGER)(&PhysicalAddress),
                               &Size,
                               ViewUnmap,
                               0,
                               Protect);
   ZwClose(hMemObj);
   if (!NT_SUCCESS(Status))
   {
      DPRINT1("ZwMapViewOfSection() failed! (0x%x)\n", Status);
   }

   return Status;
}


NTSTATUS
APIENTRY
DxgkCbMapMemory(_In_ HANDLE DeviceHandle,
                     _In_ PHYSICAL_ADDRESS TranslatedAddress,
                     _In_ ULONG Length,
                     _In_ BOOLEAN InIoSpace,
                     _In_ BOOLEAN MapToUserMode,
                     _In_ MEMORY_CACHING_TYPE CacheType,
                     _Outptr_ PVOID *VirtualAddress)
{
    NTSTATUS Status;

    DPRINT1("DxgkCbMapMemory Entry\n");
    if (InIoSpace == TRUE)
    {
        DPRINT1("Mapping InIoSpace\n");
        *VirtualAddress = (PVOID)TranslatedAddress.LowPart;
    }
    else
    {
        if (MapToUserMode == TRUE)
        {
                    /* Map to userspace */
                Status = MapPhysicalMemory((HANDLE)0xFFFFFFFF,
                               TranslatedAddress,
                               Length,
                               PAGE_READWRITE/* | PAGE_WRITECOMBINE*/,
                               VirtualAddress);

        if (!NT_SUCCESS(Status))
         {
            DPRINT1("DxgkCbMapMemory: MapPhysicalMemory() failed! (0x%x)\n", Status);
            *VirtualAddress =  NULL;
            return Status;
         }
        }
        else
        {
            *VirtualAddress = MmMapIoSpace(TranslatedAddress, Length, CacheType);
        }
    }

    if (*VirtualAddress == NULL)
    {
        DPRINT1("VirtualAddress is still NULL\n");
        return STATUS_UNSUCCESSFUL;
    }
    DPRINT1("DxgkCbMapMemory Exit\n");
    return STATUS_SUCCESS;
}

BOOLEAN
APIENTRY
DxgkCbQueueDpc(_In_ HANDLE DeviceHandle)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return FALSE;
}


NTSTATUS
APIENTRY
DxgkCbQueryServices(_In_ HANDLE DeviceHandle,
                         _In_ DXGK_SERVICES ServicesType,
                         _Inout_ PINTERFACE Interface)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    __debugbreak();
    return STATUS_UNSUCCESSFUL;
}


NTSTATUS
APIENTRY
DxgkCbSynchronizeExecution(_In_ HANDLE DeviceHandle,
                                _In_ PKSYNCHRONIZE_ROUTINE SynchronizeRoutine,
                                _In_ PVOID Context,
                                _In_ ULONG MessageNumber,
                                _Out_ PBOOLEAN ReturnValue)
{

    SynchronizeRoutine(Context);
    *ReturnValue = TRUE;
    __debugbreak();
    return STATUS_SUCCESS;
}

NTSTATUS
APIENTRY
DxgkCbUnmapMemory(_In_ HANDLE DeviceHandle,
                       _In_ PVOID VirtualAddress)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return STATUS_SUCCESS;
}


NTSTATUS
APIENTRY
DxgkCbIsDevicePresent(_In_ HANDLE DeviceHandle,
                           _In_ PPCI_DEVICE_PRESENCE_PARAMETERS DevicePresenceParameters,
                           _Out_ PBOOLEAN DevicePresent)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    __debugbreak();
    return STATUS_UNSUCCESSFUL;
}

VOID*
APIENTRY
CALLBACK
DxgkCbGetHandleData(IN_CONST_PDXGKARGCB_GETHANDLEDATA GetHandleData)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return NULL;
}

D3DKMT_HANDLE
APIENTRY
CALLBACK
DxgkCbGetHandleParent(IN_D3DKMT_HANDLE hAllocation)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return STATUS_UNSUCCESSFUL;
}

D3DKMT_HANDLE
APIENTRY
CALLBACK
DxgkCbEnumHandleChildren(IN_CONST_PDXGKARGCB_ENUMHANDLECHILDREN EnumHandleChildren)
{
        //TODO: Implement meh
    UNIMPLEMENTED;
    return STATUS_UNSUCCESSFUL;
}

VOID
APIENTRY
CALLBACK
DxgkCbNotifyInterrupt(IN_CONST_HANDLE hAdapter,
                           IN_CONST_PDXGKARGCB_NOTIFY_INTERRUPT_DATA NotifyInterruptData)
{
   //TODO: Implement meh
    UNIMPLEMENTED;
}

VOID
APIENTRY
CALLBACK
DxgkCbNotifyDpc(IN_CONST_HANDLE hAdapter)
{
   //TODO: Implement meh
    UNIMPLEMENTED;
    __debugbreak();
}

NTSTATUS
APIENTRY
CALLBACK
DxgkCbQueryMonitorInterface(IN_CONST_HANDLE                          hAdapter,
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
DxgkCbGetCaptureAddress(INOUT_PDXGKARGCB_GETCAPTUREADDRESS GetCaptureAddress)
{
    //TODO: Implement meh
    UNIMPLEMENTED;
    return STATUS_UNSUCCESSFUL;
}

VOID
APIENTRY
DxgkCbLogEtwEvent(_In_ CONST LPCGUID EventGuid,
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
    DisplayInfo->Width = 800;
    DisplayInfo->Height = 600;
    DisplayInfo->Pitch = 4;
    DisplayInfo->ColorFormat = D3DDDIFMT_R8G8B8;
    DisplayInfo->PhysicAddress.QuadPart = 0x80000000;
    DisplayInfo->TargetId = 1;
    DisplayInfo->AcpiId = 0;
    return STATUS_SUCCESS;
}

NTSTATUS
APIENTRY
CALLBACK
HandleUnimplemented()
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
NTAPI
DxgkCbExcluseAdapterAccess(_In_ HANDLE DeviceHandle,
                           _In_ ULONG Attributes,
                           _In_ DXGKDDI_PROTECTED_CALLBACK rotectedCallback,
                           _In_ PVOID ProtectedCallbackContext)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
CALLBACK
DxgkCbQueryVidPnInterface(IN_CONST_D3DKMDT_HVIDPN                hVidPn,
                               IN_CONST_DXGK_VIDPN_INTERFACE_VERSION  VidPnInterfaceVersion,
                               DEREF_OUT_CONST_PPDXGK_VIDPN_INTERFACE ppVidPnInterface)
{
     UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

CODE_SEG("PAGE")
NTSTATUS
DxgkpQueryInterface(
    _In_ PDXGKRNL_PRIVATE_EXTENSION DxgkpExtension,
    _In_ const GUID* Guid,
    _Out_ PVOID Interface,
    _In_ ULONG Size)
{
    KEVENT Event;
    IO_STATUS_BLOCK IoStatus;
    PIRP Irp;
    PIO_STACK_LOCATION Stack;
    NTSTATUS Status;

    PAGED_CODE();

    KeInitializeEvent(&Event, SynchronizationEvent, FALSE);

    Irp = IoBuildSynchronousFsdRequest(IRP_MJ_PNP,
                                       DxgkpExtension->MiniportPdo,
                                       NULL,
                                       0,
                                       NULL,
                                       &Event,
                                       &IoStatus);
    if (!Irp)
        return STATUS_INSUFFICIENT_RESOURCES;

    Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
    Irp->IoStatus.Information = 0;

    Stack = IoGetNextIrpStackLocation(Irp);
    Stack->MinorFunction = IRP_MN_QUERY_INTERFACE;
    Stack->Parameters.QueryInterface.InterfaceType = Guid;
    Stack->Parameters.QueryInterface.Version = 1;
    Stack->Parameters.QueryInterface.Size = Size;
    Stack->Parameters.QueryInterface.Interface = (PINTERFACE)Interface;
    Stack->Parameters.QueryInterface.InterfaceSpecificData = NULL;

    Status = IoCallDriver(DxgkpExtension->MiniportPdo, Irp);
    if (Status == STATUS_PENDING)
    {
        KeWaitForSingleObject(&Event, Executive, KernelMode, FALSE, NULL);
        Status = IoStatus.Status;
    }

    return Status;
}


NTSTATUS
APIENTRY
DxgkCbReadDeviceSpace(_In_ HANDLE DeviceHandle,
                      _In_ ULONG DataType,
                      _Out_writes_bytes_to_(Length, *BytesRead) PVOID Buffer,
                      _In_ ULONG Offset,
                      _In_ ULONG Length,
                      _Out_ PULONG BytesRead)
{
    switch (DataType)
    {
        case DXGK_WHICHSPACE_BRIDGE:
            DPRINT1("DxgkCbReadDeviceSpace: DXGK_WHICHSPACE_BRIDGE\n");
            UNIMPLEMENTED;
            break;
        case DXGK_WHICHSPACE_CONFIG:
            DPRINT1("DxgkCbReadDeviceSpace: DXGK_WHICHSPACE_CONFIG\n");
            *BytesRead = (DxgkpExtension->BusInterface.GetBusData)(DxgkpExtension->BusInterface.Context,
                                                                   PCI_WHICHSPACE_CONFIG,
                                                                   Buffer,
                                                                   Offset,
                                                                   Length);
             DPRINT1("DxgkCbReadDeviceSpace: DXGK_WHICHSPACE_CONFIG - SUCCESS\n");
            break;
        case DXGK_WHICHSPACE_MCH:
            DPRINT1("DxgkCbReadDeviceSpace: DXGK_WHICHSPACE_MCH");
            UNIMPLEMENTED;
            break;
        case DXGK_WHICHSPACE_ROM:
            DPRINT1("DxgkCbReadDeviceSpace: DXGK_WHICHSPACE_ROM");
            UNIMPLEMENTED;
            break;
    }
    return STATUS_SUCCESS;
}

NTSTATUS
APIENTRY
DxgkCbWriteDeviceSpace(_In_ HANDLE DeviceHandle,
                       _In_ ULONG DataType,
                       _In_reads_bytes_(Length) PVOID Buffer,
                       _In_ ULONG Offset,
                       _In_ ULONG Length,
                       _Out_ _Out_range_(<=, Length) PULONG BytesWritten)
{
    switch (DataType)
    {
        case DXGK_WHICHSPACE_BRIDGE:
            DPRINT1("DxgkCbWriteDeviceSpace: DXGK_WHICHSPACE_BRIDGE\n");
            UNIMPLEMENTED;
            break;
        case DXGK_WHICHSPACE_CONFIG:
            DPRINT1("DxgkCbWriteDeviceSpace: DXGK_WHICHSPACE_CONFIG");
            UNIMPLEMENTED;
            break;
        case DXGK_WHICHSPACE_MCH:
            DPRINT1("DxgkCbWriteDeviceSpace: DXGK_WHICHSPACE_MCH");
            UNIMPLEMENTED;
            break;
        case DXGK_WHICHSPACE_ROM:
            DPRINT1("DxgkCbWriteDeviceSpace: DXGK_WHICHSPACE_ROM");
            UNIMPLEMENTED;
            break;
    }

    return STATUS_SUCCESS;
}

/*
 * I turned this into a internal function to keep better eventual seperation of the
 * WDDM 1.2+ and WDDM 1.0-1.1 APIs
 */
NTSTATUS
NTAPI
DxgkpSetupDxgkrnl(
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath)
{
    DXGKRNL_INTERFACE DxgkrnlInterfaceLoc = {0};
    DxgkrnlInterfaceLoc.Size = sizeof(DXGKRNL_INTERFACE);
    DxgkrnlInterfaceLoc.Version = DXGKDDI_INTERFACE_VERSION_VISTA_SP1;
    DxgkrnlInterfaceLoc.DeviceHandle = (HANDLE)DriverObject;
    DxgkrnlInterfaceLoc.DxgkCbEvalAcpiMethod = DxgkCbEvalAcpiMethod;
    DxgkrnlInterfaceLoc.DxgkCbGetDeviceInformation = DxgkCbGetDeviceInformation;
    DxgkrnlInterfaceLoc.DxgkCbIndicateChildStatus = DxgkCbIndicateChildStatus;
    DxgkrnlInterfaceLoc.DxgkCbMapMemory = DxgkCbMapMemory;
    DxgkrnlInterfaceLoc.DxgkCbQueueDpc = DxgkCbQueueDpc;
    DxgkrnlInterfaceLoc.DxgkCbQueryServices = DxgkCbQueryServices;
    DxgkrnlInterfaceLoc.DxgkCbReadDeviceSpace = DxgkCbReadDeviceSpace;
    DxgkrnlInterfaceLoc.DxgkCbSynchronizeExecution = DxgkCbSynchronizeExecution;
    DxgkrnlInterfaceLoc.DxgkCbUnmapMemory = DxgkCbUnmapMemory;
    DxgkrnlInterfaceLoc.DxgkCbWriteDeviceSpace = DxgkCbWriteDeviceSpace;
    DxgkrnlInterfaceLoc.DxgkCbIsDevicePresent = DxgkCbIsDevicePresent;
    DxgkrnlInterfaceLoc.DxgkCbGetHandleData = DxgkCbGetHandleData;
    DxgkrnlInterfaceLoc.DxgkCbGetHandleParent = DxgkCbGetHandleParent;
    DxgkrnlInterfaceLoc.DxgkCbEnumHandleChildren = DxgkCbEnumHandleChildren;
    DxgkrnlInterfaceLoc.DxgkCbNotifyInterrupt = DxgkCbNotifyInterrupt;
    DxgkrnlInterfaceLoc.DxgkCbNotifyDpc = DxgkCbNotifyDpc;
    DxgkrnlInterfaceLoc.DxgkCbQueryVidPnInterface = DxgkCbQueryVidPnInterface;
    DxgkrnlInterfaceLoc.DxgkCbQueryMonitorInterface = DxgkCbQueryMonitorInterface;
    DxgkrnlInterfaceLoc.DxgkCbGetCaptureAddress = DxgkCbGetCaptureAddress;
    DxgkrnlInterfaceLoc.DxgkCbLogEtwEvent = DxgkCbLogEtwEvent;
    DxgkrnlInterfaceLoc.DxgkCbExcludeAdapterAccess = DxgkCbExcluseAdapterAccess;
#if 0
    DxgkrnlInterfaceLoc.DxgkCbCreateContextAllocation = (DXGKCB_CREATECONTEXTALLOCATION          )HandleUnimplemented;
    DxgkrnlInterfaceLoc.DxgkCbDestroyContextAllocation = (DXGKCB_DESTROYCONTEXTALLOCATION         )HandleUnimplemented;
    DxgkrnlInterfaceLoc.DxgkCbSetPowerComponentActive = (DXGKCB_SETPOWERCOMPONENTACTIVE          )HandleUnimplemented;
    DxgkrnlInterfaceLoc.DxgkCbSetPowerComponentIdle = (DXGKCB_SETPOWERCOMPONENTIDLE            )HandleUnimplemented;
    DxgkrnlInterfaceLoc.DxgkCbAcquirePostDisplayOwnership = DxgkCbAcquirePostDisplayOwnership;
    DxgkrnlInterfaceLoc.DxgkCbPowerRuntimeControlRequest = (DXGKCB_POWERRUNTIMECONTROLREQUEST       )HandleUnimplemented;
    DxgkrnlInterfaceLoc.DxgkCbSetPowerComponentLatency = (DXGKCB_SETPOWERCOMPONENTLATENCY         )HandleUnimplemented;
    DxgkrnlInterfaceLoc.DxgkCbSetPowerComponentResidency = (DXGKCB_SETPOWERCOMPONENTRESIDENCY       )HandleUnimplemented;
    DxgkrnlInterfaceLoc.DxgkCbCompleteFStateTransition = (DXGKCB_COMPLETEFSTATETRANSITION         )HandleUnimplemented;
    DxgkrnlInterfaceLoc.DxgkCbCompletePStateTransition = (DXGKCB_COMPLETEPSTATETRANSITION         )HandleUnimplemented;
    DxgkrnlInterfaceLoc.DxgkCbMapContextAllocation = (DXGKCB_MAPCONTEXTALLOCATION             )HandleUnimplemented;
    DxgkrnlInterfaceLoc.DxgkCbUpdateContextAllocation = (DXGKCB_UPDATECONTEXTALLOCATION          )HandleUnimplemented;
    DxgkrnlInterfaceLoc.DxgkCbReserveGpuVirtualAddressRange = (DXGKCB_RESERVEGPUVIRTUALADDRESSRANGE    )HandleUnimplemented;
    DxgkrnlInterfaceLoc.DxgkCbAcquireHandleData = (DXGKCB_ACQUIREHANDLEDATA                )HandleUnimplemented;
    DxgkrnlInterfaceLoc.DxgkCbReleaseHandleData = (DXGKCB_RELEASEHANDLEDATA                )HandleUnimplemented;
    DxgkrnlInterfaceLoc.DxgkCbHardwareContentProtectionTeardown = (DXGKCB_HARDWARECONTENTPROTECTIONTEARDOWN)HandleUnimplemented;
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
    DxgkrnlInterface = DxgkrnlInterfaceLoc;
    DPRINT1("Targetting version: %X\n", DxgkrnlInterface.Version);
    DPRINT1("Dxgkrnl has started\n\n");
   // AddDeviceMapLink();
    return STATUS_SUCCESS;
}