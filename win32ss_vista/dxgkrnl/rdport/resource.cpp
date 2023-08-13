/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     RDDM Miniport IOCTRL Handlers
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <dxgkrnl.h>
//#define NDEBUG
#include <debug.h>

extern PDXGKRNL_PRIVATE_EXTENSION Extension;
#define TAG_DXGK_KRNL          'KFXD'


NTSTATUS
NTAPI
DxgkrnlCreateIds()
{
    PCI_SLOT_NUMBER SlotNumber;
    ULONG PciSlotNumber;
    ULONG Size;

    Size = sizeof(ULONG);
    IoGetDeviceProperty(Extension->MiniportPdo,
                                 DevicePropertyBusNumber,
                                 Size,
                                 &Extension->SystemIoBusNumber,
                                 &Size);
    Size = sizeof(ULONG);
    IoGetDeviceProperty(Extension->MiniportPdo,
                        DevicePropertyLegacyBusType,
                        Size,
                        &Extension->AdapterInterfaceType,
                        &Size);
    DPRINT1("AdapterInterfaceType :%d\n", Extension->AdapterInterfaceType);


    Size = sizeof(ULONG);
    IoGetDeviceProperty(Extension->MiniportPdo,
                        DevicePropertyAddress,
                        Size,
                        &PciSlotNumber,
                        &Size);
    SlotNumber.u.AsULONG = 0;
    SlotNumber.u.bits.DeviceNumber = (PciSlotNumber >> 16) & 0xFFFF;
    SlotNumber.u.bits.FunctionNumber = PciSlotNumber & 0xFFFF;
    Extension->SystemIoSlotNumber = SlotNumber.u.AsULONG;

    DPRINT1("Device Number: %d\n",  SlotNumber.u.bits.DeviceNumber);
    DPRINT1("FunctionNumber: %d\n", SlotNumber.u.bits.FunctionNumber);
    //__debugbreak();
    return 0;
}

NTSTATUS
NTAPI
DxgkrnlSetupResourceList(_Inout_ PCM_RESOURCE_LIST* ResourceList)
{
    NTSTATUS Status;
    PCI_SLOT_NUMBER PciSlotNumber;
    PCI_COMMON_CONFIG Config;
    ULONG ReturnedLength;



    PciSlotNumber.u.AsULONG = Extension->SystemIoSlotNumber;


    if (Extension->MiniportPdo != NULL)
    {
        PciSlotNumber.u.AsULONG = Extension->SystemIoSlotNumber;

        ReturnedLength = HalGetBusData(PCIConfiguration,
                                       Extension->SystemIoBusNumber,
                                       PciSlotNumber.u.AsULONG,
                                       &Config,
                                       sizeof(Config));

        if (ReturnedLength != sizeof(Config))
        {
            return STATUS_NO_MEMORY;
        }
    }
    else
    {
        __debugbreak();
    }

    Status = HalAssignSlotResources(&Extension->RegistryPath,
                                            NULL,
                                            Extension->DriverObject,
                                            Extension->DriverObject->DeviceObject,
                                            Extension->AdapterInterfaceType,
                                            Extension->SystemIoBusNumber,
                                            PciSlotNumber.u.AsULONG,
                                            ResourceList);
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("HalAssignSlotResources failed with status %x.\n",Status);
        __debugbreak();
        return Status;
    }
    /* ******************************************************************/
    DPRINT1("ResourceList loc %X\n", ResourceList);
    return 0;
}






NTSTATUS
NTAPI
DxgkrnlVerifyAccessRanges(_In_opt_ ULONG NumAccessRanges,
    _In_reads_opt_(NumAccessRanges) PVIDEO_ACCESS_RANGE AccessRanges,
    _Out_ PCM_RESOURCE_LIST ResourceList)
{
    #if 0
    BOOLEAN ConflictDetected;
    ULONG ResourceListSize;
    PCM_PARTIAL_RESOURCE_DESCRIPTOR PartialDescriptor;
    ULONG i;
    NTSTATUS Status;




    /* Create the resource list */
    ResourceListSize = sizeof(CM_RESOURCE_LIST)
        + (NumAccessRanges - 1) * sizeof(CM_PARTIAL_RESOURCE_DESCRIPTOR);
    ResourceList = (PVOID)ExAllocatePoolWithTag(PagedPool, ResourceListSize, TAG_DXGK_KRNL);
    if (!ResourceList)
    {
        DPRINT1("ExAllocatePool() failed\n");
        return STATUS_NO_MEMORY;
    }

    /* Fill resource list */
    ResourceList->Count = 1;
    ResourceList->List[0].InterfaceType = PCIBus;
   // ResourceList->List[0].BusNumber = Extension->SystemIoBusNumber;
    ResourceList->List[0].PartialResourceList.Version = 1;
    ResourceList->List[0].PartialResourceList.Revision = 1;
    ResourceList->List[0].PartialResourceList.Count = NumAccessRanges;
    for (i = 0; i < NumAccessRanges; i++, AccessRanges++)
    {
        PartialDescriptor = &ResourceList->List[0].PartialResourceList.PartialDescriptors[i];
        if (AccessRanges->RangeInIoSpace)
        {
            PartialDescriptor->Type = CmResourceTypePort;
            PartialDescriptor->u.Port.Start = AccessRanges->RangeStart;
            PartialDescriptor->u.Port.Length = AccessRanges->RangeLength;
        }
        else
        {
            PartialDescriptor->Type = CmResourceTypeMemory;
            PartialDescriptor->u.Memory.Start = AccessRanges->RangeStart;
            PartialDescriptor->u.Memory.Length = AccessRanges->RangeLength;
        }
        if (AccessRanges->RangeShareable)
            PartialDescriptor->ShareDisposition = CmResourceShareShared;
        else
            PartialDescriptor->ShareDisposition = CmResourceShareDeviceExclusive;
        PartialDescriptor->Flags = 0;
        if (AccessRanges->RangePassive & VIDEO_RANGE_PASSIVE_DECODE)
            PartialDescriptor->Flags |= CM_RESOURCE_PORT_PASSIVE_DECODE;
        if (AccessRanges->RangePassive & VIDEO_RANGE_10_BIT_DECODE)
            PartialDescriptor->Flags |= CM_RESOURCE_PORT_10_BIT_DECODE;
    }

    /* Try to acquire all resource ranges */
    Status = IoReportResourceForDetection(
                Extension->DriverObject,
                NULL, 0, /* Driver List */
                Extension->MiniportPdo,
                ResourceList, ResourceListSize,
                &ConflictDetected);

    ExFreePoolWithTag(ResourceList, TAG_DXGK_KRNL);
#endif
    return STATUS_SUCCESS;
}
