/*
 * PROJECT:     ReactOS nVidia nForce Ethernet Controller Driver
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Miniport driver entrypoint
 * COPYRIGHT:   Copyright 2021 Dmitry Borisov <di.sean@protonmail.com>
 */

/* INCLUDES *******************************************************************/

#include "nvnet.h"

#define NDEBUG
#include <debug.h>

/* GLOBALS ********************************************************************/

CODE_SEG("INIT")
DRIVER_INITIALIZE DriverEntry;

/* FUNCTIONS ******************************************************************/

static
CODE_SEG("PAGE")
VOID
QueryInteger(
    _In_ NDIS_HANDLE ConfigurationHandle,
    _In_ PCWSTR EntryName,
    _Out_ PULONG EntryContext,
    _In_ ULONG DefaultValue,
    _In_ ULONG Minimum,
    _In_ ULONG Maximum)
{
    NDIS_STATUS Status;
    UNICODE_STRING Keyword;
    PNDIS_CONFIGURATION_PARAMETER ConfigurationParameter;

    PAGED_CODE();

    NdisInitUnicodeString(&Keyword, EntryName);
    NdisReadConfiguration(&Status,
                          &ConfigurationParameter,
                          ConfigurationHandle,
                          &Keyword,
                          NdisParameterInteger);
    if (Status != NDIS_STATUS_SUCCESS)
    {
        NDIS_DbgPrint(MIN_TRACE, ("'%S' request failed\n", EntryName));

        *EntryContext = DefaultValue;
    }
    else
    {
        if (ConfigurationParameter->ParameterData.IntegerData >= Minimum &&
            ConfigurationParameter->ParameterData.IntegerData <= Maximum)
        {
            *EntryContext = ConfigurationParameter->ParameterData.IntegerData;
        }
        else
        {
            NDIS_DbgPrint(MIN_TRACE, ("'%S' value out of range\n", EntryName));

            *EntryContext = DefaultValue;
        }
    }

    NDIS_DbgPrint(MIN_TRACE, ("Set '%S' to %d\n", EntryName, *EntryContext));
}

static
CODE_SEG("PAGE")
NDIS_STATUS
NvNetReadConfiguration(
    _Inout_ PNVNET_ADAPTER Adapter)
{
    NDIS_STATUS Status;
    NDIS_HANDLE ConfigurationHandle;
    PUCHAR NetworkAddress;
    UINT Length;
    ULONG GenericUlong;

    PAGED_CODE();

    NDIS_DbgPrint(MIN_TRACE, ("()\n"));

    NdisOpenConfiguration(&Status, &ConfigurationHandle, Adapter->WrapperConfigurationHandle);
    if (Status != NDIS_STATUS_SUCCESS)
        return Status;

    QueryInteger(ConfigurationHandle,
                 L"OptimizationMode",
                 &GenericUlong,
                 NV_OPTIMIZATION_MODE_DYNAMIC,
                 NV_OPTIMIZATION_MODE_THROUGHPUT,
                 NV_OPTIMIZATION_MODE_DYNAMIC);
    Adapter->OptimizationMode = GenericUlong;

    QueryInteger(ConfigurationHandle,
                 L"FlowControl",
                 &GenericUlong,
                 NV_FLOW_CONTROL_AUTO,
                 NV_FLOW_CONTROL_DISABLE,
                 NV_FLOW_CONTROL_RX_TX);
    Adapter->FlowControlMode = GenericUlong;

    QueryInteger(ConfigurationHandle,
                 L"SpeedDuplex",
                 &GenericUlong,
                 0,
                 0,
                 4);
    switch (GenericUlong)
    {
        case 1:
            Adapter->UserLinkSpeed = 10;
            Adapter->ForceFullDuplex = FALSE;
            Adapter->ForceSpeedAndDuplex = TRUE;
            break;
        case 2:
            Adapter->UserLinkSpeed = 10;
            Adapter->ForceFullDuplex = TRUE;
            Adapter->ForceSpeedAndDuplex = TRUE;
            break;
        case 3:
            Adapter->UserLinkSpeed = 100;
            Adapter->ForceFullDuplex = FALSE;
            Adapter->ForceSpeedAndDuplex = TRUE;
            break;
        case 4:
            Adapter->UserLinkSpeed = 100;
            Adapter->ForceFullDuplex = TRUE;
            Adapter->ForceSpeedAndDuplex = TRUE;
            break;

        default:
            break;
    }

    QueryInteger(ConfigurationHandle,
                 L"ChecksumOffload",
                 &GenericUlong,
                 0,
                 0,
                 1);
    Adapter->ChecksumOffload = !!GenericUlong;

    QueryInteger(ConfigurationHandle,
                 L"JumboSize",
                 &GenericUlong,
                 NVNET_MAXIMUM_FRAME_SIZE,
                 NVNET_MAXIMUM_FRAME_SIZE,
                 NVNET_MAXIMUM_FRAME_SIZE_JUMBO);
    Adapter->MaximumFrameSize = GenericUlong;

    QueryInteger(ConfigurationHandle,
                 L"Priority",
                 &GenericUlong,
                 0,
                 0,
                 1);
    Adapter->PrioritySupported = !!GenericUlong;

    QueryInteger(ConfigurationHandle,
                 L"VlanTag",
                 &GenericUlong,
                 0,
                 0,
                 1);
    Adapter->VlanTaggingSupported = !!GenericUlong;

    QueryInteger(ConfigurationHandle,
                 L"VlanID",
                 &GenericUlong,
                 0,
                 0,
                 NVNET_MAXIMUM_VLAN_ID);

    NdisReadNetworkAddress(&Status,
                           (PVOID*)&NetworkAddress,
                           &Length,
                           ConfigurationHandle);
    if ((Status == NDIS_STATUS_SUCCESS) && (Length == ETH_LENGTH_OF_ADDRESS))
    {
        if ((ETH_IS_MULTICAST(NetworkAddress) || ETH_IS_BROADCAST(NetworkAddress)) ||
            !ETH_IS_LOCALLY_ADMINISTERED(NetworkAddress))
        {
            NDIS_DbgPrint(MIN_TRACE, ("Invalid software MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
                                      NetworkAddress[0],
                                      NetworkAddress[1],
                                      NetworkAddress[2],
                                      NetworkAddress[3],
                                      NetworkAddress[4],
                                      NetworkAddress[5]));
        }
        else
        {
            NDIS_DbgPrint(MIN_TRACE, ("Using software MAC\n"));
            ETH_COPY_NETWORK_ADDRESS(Adapter->CurrentMacAddress, NetworkAddress);

            Adapter->UseSoftwareMacAddress = TRUE;
        }
    }
    Status = NDIS_STATUS_SUCCESS;

    NdisCloseConfiguration(ConfigurationHandle);

    return Status;
}

static
CODE_SEG("PAGE")
NDIS_STATUS
NvNetInitializeAdapterResources(
    _Inout_ PNVNET_ADAPTER Adapter)
{
    NDIS_STATUS Status;
    PNDIS_RESOURCE_LIST AssignedResources = NULL;
    UINT i, ResourceListSize = 0;

    PAGED_CODE();

    NDIS_DbgPrint(MIN_TRACE, ("()\n"));

    NdisMQueryAdapterResources(&Status,
                               Adapter->WrapperConfigurationHandle,
                               AssignedResources,
                               &ResourceListSize);
    if (Status != NDIS_STATUS_RESOURCES)
    {
        NdisWriteErrorLogEntry(Adapter->AdapterHandle, NDIS_ERROR_CODE_RESOURCE_CONFLICT, 0);
        return NDIS_STATUS_FAILURE;
    }

    Status = NdisAllocateMemoryWithTag((PVOID*)&AssignedResources,
                                       ResourceListSize,
                                       NVNET_TAG);
    if (Status != NDIS_STATUS_SUCCESS)
    {
        NdisWriteErrorLogEntry(Adapter->AdapterHandle, NDIS_ERROR_CODE_OUT_OF_RESOURCES, 0);
        return Status;
    }

    NdisMQueryAdapterResources(&Status,
                               Adapter->WrapperConfigurationHandle,
                               AssignedResources,
                               &ResourceListSize);
    if (Status != NDIS_STATUS_SUCCESS)
    {
        NdisWriteErrorLogEntry(Adapter->AdapterHandle, NDIS_ERROR_CODE_RESOURCE_CONFLICT, 0);
        goto Cleanup;
    }

    for (i = 0; i < AssignedResources->Count; ++i)
    {
        PCM_PARTIAL_RESOURCE_DESCRIPTOR Descriptor = &AssignedResources->PartialDescriptors[i];

        switch (Descriptor->Type)
        {
            case CmResourceTypeMemory:
            {
                Adapter->IoAddress = Descriptor->u.Memory.Start;
                Adapter->IoLength = Descriptor->u.Memory.Length;
                break;
            }

            case CmResourceTypeInterrupt:
            {
                Adapter->InterruptVector = Descriptor->u.Interrupt.Vector;
                Adapter->InterruptLevel = Descriptor->u.Interrupt.Level;
                Adapter->InterruptShared = (Descriptor->ShareDisposition == CmResourceShareShared);
                Adapter->InterruptFlags = Descriptor->Flags;
                break;
            }

            default:
                break;
        }
    }

    if (!Adapter->IoAddress.QuadPart || !Adapter->InterruptVector)
    {
        Status = NDIS_STATUS_RESOURCES;
        NdisWriteErrorLogEntry(Adapter->AdapterHandle, NDIS_ERROR_CODE_RESOURCE_CONFLICT, 0);
        goto Cleanup;
    }

    NDIS_DbgPrint(MIN_TRACE, ("MEM at [%I64X-%I64X]\n",
                              Adapter->IoAddress.QuadPart,
                              Adapter->IoAddress.QuadPart + Adapter->IoLength));
    NDIS_DbgPrint(MIN_TRACE, ("IRQ Vector %d Level %d\n",
                              Adapter->InterruptVector,
                              Adapter->InterruptLevel));

    Status = NdisMMapIoSpace((PVOID*)&Adapter->IoBase,
                             Adapter->AdapterHandle,
                             Adapter->IoAddress,
                             Adapter->IoLength);
    if (Status != NDIS_STATUS_SUCCESS)
    {
        ASSERT(FALSE);
        NdisWriteErrorLogEntry(Adapter->AdapterHandle, NDIS_ERROR_CODE_RESOURCE_CONFLICT, 0);
        goto Cleanup;
    }

Cleanup:
    NdisFreeMemory(AssignedResources, ResourceListSize, 0);

    return Status;
}

static
CODE_SEG("PAGE")
NDIS_STATUS
NvNetAllocateTransmitMemory(
    _Inout_ PNVNET_ADAPTER Adapter)
{
    BOOLEAN Success;
    PVOID DestinationVa;
    ULONG DescriptorSize;

    PAGED_CODE();

    NDIS_DbgPrint(MIN_TRACE, ("()\n"));

    if (Adapter->Flags & DEV_HAS_HIGH_DMA)
    {
        DestinationVa = (PVOID*)&Adapter->TransmitDescriptors64;
        DescriptorSize = sizeof(NVNET_DESCRIPTOR_64);
    }
    else
    {
        DestinationVa = (PVOID*)&Adapter->TransmitDescriptors32;
        DescriptorSize = sizeof(NVNET_DESCRIPTOR_32);
    }
    NdisMAllocateSharedMemory(Adapter->AdapterHandle,
                              DescriptorSize * NVNET_TRANSMIT_DESCRIPTORS,
                              FALSE,
                              DestinationVa,
                              &Adapter->TransmitDescriptorsPa);
    Success = Adapter->Flags & DEV_HAS_HIGH_DMA ? (Adapter->TransmitDescriptors64 != NULL)
                                                : (Adapter->TransmitDescriptors32 != NULL);
    if (!Success)
    {
        ASSERT(FALSE);
        return NDIS_STATUS_RESOURCES;
    }

    return NDIS_STATUS_SUCCESS;
}

static
CODE_SEG("PAGE")
VOID
NvNetInitializeTransmitMemory(
    _Inout_ PNVNET_ADAPTER Adapter)
{
    ULONG i;

    PAGED_CODE();

    NDIS_DbgPrint(MIN_TRACE, ("()\n"));

    // NdisAllocateSpinLock(&Adapter->SendLock);
    InitializeListHead(&Adapter->SendBusyList);

    if (Adapter->Flags & DEV_HAS_HIGH_DMA)
    {
        for (i = 0; i < NVNET_TRANSMIT_DESCRIPTORS; ++i)
        {
            PNVNET_DESCRIPTOR_64 TransmitDescriptor64 = &Adapter->TransmitDescriptors64[i];

            NdisZeroMemory(TransmitDescriptor64, sizeof(NVNET_DESCRIPTOR_64));
        }
    }
    else
    {
        for (i = 0; i < NVNET_TRANSMIT_DESCRIPTORS; ++i)
        {
            PNVNET_DESCRIPTOR_32 TransmitDescriptor32 = &Adapter->TransmitDescriptors32[i];

            NdisZeroMemory(TransmitDescriptor32, sizeof(NVNET_DESCRIPTOR_32));
        }
    }
}

static
CODE_SEG("PAGE")
NDIS_STATUS
NvNetAllocateReceiveMemory(
    _Inout_ PNVNET_ADAPTER Adapter)
{
    BOOLEAN Success;
    PVOID DestinationVa;
    ULONG DescriptorSize;

    PAGED_CODE();

    NDIS_DbgPrint(MIN_TRACE, ("()\n"));

    if (Adapter->Flags & DEV_HAS_HIGH_DMA)
    {
        DestinationVa = (PVOID*)&Adapter->ReceiveDescriptors64;
        DescriptorSize = sizeof(NVNET_DESCRIPTOR_64);
    }
    else
    {
        DestinationVa = (PVOID*)&Adapter->ReceiveDescriptors32;
        DescriptorSize = sizeof(NVNET_DESCRIPTOR_32);
    }
    NdisMAllocateSharedMemory(Adapter->AdapterHandle,
                              DescriptorSize * NVNET_RECEIVE_DESCRIPTORS,
                              FALSE,
                              DestinationVa,
                              &Adapter->ReceiveDescriptorsPa);
    Success = Adapter->Flags & DEV_HAS_HIGH_DMA ? (Adapter->ReceiveDescriptors64 != NULL)
                                                : (Adapter->ReceiveDescriptors32 != NULL);
    if (!Success)
    {
        ASSERT(FALSE);
        return NDIS_STATUS_RESOURCES;
    }

    NdisMAllocateSharedMemory(Adapter->AdapterHandle,
                              NVNET_RECEIVE_BUFFER_SIZE * NVNET_RECEIVE_DESCRIPTORS,
                              FALSE,
                              (PVOID*)&Adapter->ReceiveBuffer,
                              &Adapter->ReceiveBufferPa);
    if (!Adapter->ReceiveBuffer)
    {
        NDIS_DbgPrint(MIN_TRACE, ("Unable to allocate receive buffer\n"));
        ASSERT(FALSE);
        return NDIS_STATUS_RESOURCES;
    }

    return NDIS_STATUS_SUCCESS;
}

static
CODE_SEG("PAGE")
VOID
NvNetInitializeReceiveMemory(
    _Inout_ PNVNET_ADAPTER Adapter)
{
    ULONG i;

    PAGED_CODE();

    NDIS_DbgPrint(MIN_TRACE, ("()\n"));

    // NdisAllocateSpinLock(&Adapter->ReceiveLock);

    if (Adapter->Flags & DEV_HAS_HIGH_DMA)
    {
        for (i = 0; i < NVNET_RECEIVE_DESCRIPTORS; ++i)
        {
            PNVNET_DESCRIPTOR_64 ReceiveDescriptor64 = &Adapter->ReceiveDescriptors64[i];

            ReceiveDescriptor64->AddressHigh = Adapter->ReceiveBufferPa.HighPart
                                               + i * NVNET_RECEIVE_BUFFER_SIZE;
            ReceiveDescriptor64->AddressLow = Adapter->ReceiveBufferPa.LowPart
                                              + i * NVNET_RECEIVE_BUFFER_SIZE;
            ReceiveDescriptor64->VlanTag = 0;
            ReceiveDescriptor64->FlagsLength = NV_RX2_AVAIL | NVNET_RECEIVE_BUFFER_SIZE;
        }
    }
    else
    {
        for (i = 0; i < NVNET_RECEIVE_DESCRIPTORS; ++i)
        {
            PNVNET_DESCRIPTOR_32 ReceiveDescriptor32 = &Adapter->ReceiveDescriptors32[i];

            ReceiveDescriptor32->Address = Adapter->ReceiveBufferPa.LowPart
                                           + i * NVNET_RECEIVE_BUFFER_SIZE;
            ReceiveDescriptor32->FlagsLength = NV_RX_AVAIL | NVNET_RECEIVE_BUFFER_SIZE;
        }
    }
}

static
CODE_SEG("PAGE")
VOID
NvNetFreeAdapter(
    _In_ PNVNET_ADAPTER Adapter)
{
    PAGED_CODE();

    NDIS_DbgPrint(MIN_TRACE, ("()\n"));

    if (Adapter->Interrupt.InterruptObject)
    {
        NdisMDeregisterInterrupt(&Adapter->Interrupt);
        Adapter->Interrupt.InterruptObject = NULL;
    }

    if (Adapter->TransmitDescriptors32)
    {
        NdisMFreeSharedMemory(Adapter->AdapterHandle,
                              sizeof(NVNET_DESCRIPTOR_32) * NVNET_TRANSMIT_DESCRIPTORS,
                              FALSE,
                              Adapter->TransmitDescriptors32,
                              Adapter->TransmitDescriptorsPa);
        Adapter->TransmitDescriptors32 = NULL;
    }
    if (Adapter->TransmitDescriptors64)
    {
        NdisMFreeSharedMemory(Adapter->AdapterHandle,
                              sizeof(NVNET_DESCRIPTOR_64) * NVNET_TRANSMIT_DESCRIPTORS,
                              FALSE,
                              Adapter->TransmitDescriptors64,
                              Adapter->TransmitDescriptorsPa);
        Adapter->TransmitDescriptors64 = NULL;
    }

    if (Adapter->ReceiveDescriptors32)
    {
        NdisMFreeSharedMemory(Adapter->AdapterHandle,
                              sizeof(NVNET_DESCRIPTOR_32) * NVNET_RECEIVE_DESCRIPTORS,
                              FALSE,
                              Adapter->ReceiveDescriptors32,
                              Adapter->ReceiveDescriptorsPa);
        Adapter->ReceiveDescriptors32 = NULL;
    }
    if (Adapter->ReceiveDescriptors64)
    {
        NdisMFreeSharedMemory(Adapter->AdapterHandle,
                              sizeof(NVNET_DESCRIPTOR_64) * NVNET_RECEIVE_DESCRIPTORS,
                              FALSE,
                              Adapter->ReceiveDescriptors64,
                              Adapter->ReceiveDescriptorsPa);
        Adapter->ReceiveDescriptors64 = NULL;
    }

    if (Adapter->ReceiveBuffer)
    {
        NdisMFreeSharedMemory(Adapter->AdapterHandle,
                              NVNET_RECEIVE_BUFFER_SIZE * NVNET_RECEIVE_DESCRIPTORS,
                              FALSE,
                              Adapter->ReceiveBuffer,
                              Adapter->ReceiveBufferPa);
        Adapter->ReceiveBuffer = NULL;
    }

    if (Adapter->IoBase)
    {
        NdisMUnmapIoSpace(Adapter->AdapterHandle,
                          Adapter->IoBase,
                          Adapter->IoLength);
    }
}

/* NDIS CALLBACKS *************************************************************/

static
CODE_SEG("PAGE")
NDIS_STATUS
NTAPI
MiniportInitialize(
    _Out_ PNDIS_STATUS OpenErrorStatus,
    _Out_ PUINT SelectedMediumIndex,
    _In_ PNDIS_MEDIUM MediumArray,
    _In_ UINT MediumArraySize,
    _In_ NDIS_HANDLE MiniportAdapterHandle,
    _In_ NDIS_HANDLE WrapperConfigurationContext)
{
    UINT i;
    PNVNET_ADAPTER Adapter;
    NDIS_STATUS Status;

    PAGED_CODE();

    NDIS_DbgPrint(MIN_TRACE, ("()\n"));

    for (i = 0; i < MediumArraySize; ++i)
    {
        if (MediumArray[i] == NdisMedium802_3)
        {
            *SelectedMediumIndex = i;
            break;
        }
    }
    if (i == MediumArraySize)
    {
        NDIS_DbgPrint(MIN_TRACE, ("No supported media\n"));
        return NDIS_STATUS_UNSUPPORTED_MEDIA;
    }

    Status = NdisAllocateMemoryWithTag((PVOID*)&Adapter,
                                       sizeof(NVNET_ADAPTER),
                                       NVNET_TAG);
    if (Status != NDIS_STATUS_SUCCESS)
    {
        NDIS_DbgPrint(MIN_TRACE, ("Failed to allocate adapter\n"));
        return NDIS_STATUS_RESOURCES;
    }

    NdisZeroMemory(Adapter, sizeof(NVNET_ADAPTER));
    Adapter->AdapterHandle = MiniportAdapterHandle;
    Adapter->WrapperConfigurationHandle = WrapperConfigurationContext;

    Status = NvNetReadConfiguration(Adapter);
    if (Status != NDIS_STATUS_SUCCESS)
    {
        ASSERT(FALSE);
        goto Failure;
    }

    NdisMSetAttributesEx(MiniportAdapterHandle,
                         Adapter,
                         0,
                         NDIS_ATTRIBUTE_BUS_MASTER, // TODO: | NDIS_ATTRIBUTE_DESERIALIZE,
                         NdisInterfacePci);

    Status = NvNetRecognizeHardware(Adapter);
    if (Status != NDIS_STATUS_SUCCESS)
    {
        if (Status == NDIS_STATUS_ADAPTER_NOT_FOUND)
        {
            NdisWriteErrorLogEntry(Adapter->AdapterHandle,
                                   NDIS_ERROR_CODE_ADAPTER_NOT_FOUND,
                                   0);
        }
        else if (Status == NDIS_STATUS_NOT_RECOGNIZED)
        {
            NdisWriteErrorLogEntry(Adapter->AdapterHandle,
                                   NDIS_ERROR_CODE_UNSUPPORTED_CONFIGURATION,
                                   0);
        }

        ASSERT(FALSE);
        goto Failure;
    }

    Status = NvNetInitializeAdapterResources(Adapter);
    if (Status != NDIS_STATUS_SUCCESS)
    {
        ASSERT(FALSE);
        goto Failure;
    }

    Status = NdisMInitializeScatterGatherDma(Adapter->AdapterHandle,
                                             !!(Adapter->Flags & DEV_HAS_HIGH_DMA),
                                             NVNET_MAXIMUM_FRAME_SIZE);
                                             // ^TODO: NVNET_MAX_DMA_TRANSFER);
    if (Status != NDIS_STATUS_SUCCESS)
    {
        NDIS_DbgPrint(MIN_TRACE, ("Failed to initialize DMA\n"));
        goto NoMemory;
    }

    Status = NvNetAllocateTransmitMemory(Adapter);
    if (Status != NDIS_STATUS_SUCCESS)
    {
        NDIS_DbgPrint(MIN_TRACE, ("Failed to allocate transmit memory\n"));
        goto NoMemory;
    }

    Status = NvNetAllocateReceiveMemory(Adapter);
    if (Status != NDIS_STATUS_SUCCESS)
    {
        NDIS_DbgPrint(MIN_TRACE, ("Failed to allocate receive memory\n"));
        goto NoMemory;
    }

    NvNetInitializeTransmitMemory(Adapter);
    NvNetInitializeReceiveMemory(Adapter);

    Status = NvNetGetPermanentMacAddress(Adapter, Adapter->PermanentMacAddress);
    if (Status != NDIS_STATUS_SUCCESS)
    {
        NdisWriteErrorLogEntry(Adapter->AdapterHandle, NDIS_ERROR_CODE_NETWORK_ADDRESS, 0);
        goto Failure;
    }

    if (!Adapter->UseSoftwareMacAddress)
    {
        ETH_COPY_NETWORK_ADDRESS(Adapter->CurrentMacAddress, Adapter->PermanentMacAddress);
    }

    NvNetSetupMacAddress(Adapter, Adapter->CurrentMacAddress);

    Status = NvNetInitNIC(Adapter);
    if (Status != NDIS_STATUS_SUCCESS)
    {
        NDIS_DbgPrint(MIN_TRACE, ("Failed to initialize the NIC\n"));
        NdisWriteErrorLogEntry(Adapter->AdapterHandle, NDIS_ERROR_CODE_HARDWARE_FAILURE, 0);
        goto Failure;
    }

    NvNetDisableInterrupts(Adapter);
    NV_WRITE(Adapter, NvRegMIIStatus, NVREG_MIISTAT_MASK_ALL);
    NV_WRITE(Adapter, NvRegIrqStatus, NVREG_IRQSTAT_MASK);

/* FIXME: Bug in the PIC HAL? */
#if defined(SARCH_XBOX)
    Status = NdisMRegisterInterrupt(&Adapter->Interrupt,
                                    Adapter->AdapterHandle,
                                    Adapter->InterruptVector,
                                    Adapter->InterruptLevel,
                                    TRUE, /* Request ISR calls */
                                    FALSE,
                                    NdisInterruptLatched);
#else
    Status = NdisMRegisterInterrupt(&Adapter->Interrupt,
                                    Adapter->AdapterHandle,
                                    Adapter->InterruptVector,
                                    Adapter->InterruptLevel,
                                    TRUE, /* Request ISR calls */
                                    TRUE, /* Shared */
                                    NdisInterruptLevelSensitive);
#endif
    if (Status != NDIS_STATUS_SUCCESS)
    {
        ASSERT(FALSE);
        NdisWriteErrorLogEntry(Adapter->AdapterHandle, NDIS_ERROR_CODE_INTERRUPT_CONNECT, 0);
        goto Failure;
    }

    NdisMSynchronizeWithInterrupt(&Adapter->Interrupt, NvNetInitPhaseSynchronized, Adapter);

    /* Setup the link timer right after the NIC is initialized */
    if (Adapter->Flags & DEV_NEED_LINKTIMER)
    {
        NdisMSetPeriodicTimer(&Adapter->MediaDetectionTimer, NVNET_MEDIA_DETECTION_INTERVAL);
    }

    return NDIS_STATUS_SUCCESS;

NoMemory:
    NdisWriteErrorLogEntry(Adapter->AdapterHandle, NDIS_ERROR_CODE_OUT_OF_RESOURCES, 0);
Failure:
    NvNetFreeAdapter(Adapter);
    NdisFreeMemory(Adapter, sizeof(NVNET_ADAPTER), 0);

    return Status;
}

static
CODE_SEG("PAGE")
VOID
NTAPI
MiniportHalt(
    _In_ NDIS_HANDLE MiniportAdapterContext)
{
    NDIS_DbgPrint(MIN_TRACE, ("%s(%p)\n", MiniportAdapterContext));

    PAGED_CODE();

    /* FIXME */
}

static
NDIS_STATUS
NTAPI
MiniportReset(
  _Out_ PBOOLEAN AddressingReset,
  _In_ NDIS_HANDLE MiniportAdapterContext)
{
    NDIS_DbgPrint(MIN_TRACE, ("(%p, %p)\n", AddressingReset, MiniportAdapterContext));

    /* FIXME */
    ASSERT(FALSE);
    *AddressingReset = FALSE;
    return NDIS_STATUS_FAILURE;
}

static
CODE_SEG("PAGE")
VOID
NTAPI
MiniportUnload(
    _In_ PDRIVER_OBJECT DriverObject)
{
    PAGED_CODE();

    NDIS_DbgPrint(MIN_TRACE, ("(%p)\n", DriverObject));

    /* FIXME */
}

CODE_SEG("INIT")
NTSTATUS
NTAPI
DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath)
{
    NDIS_HANDLE WrapperHandle;
    NDIS_MINIPORT_CHARACTERISTICS Characteristics = { 0 };
    NDIS_STATUS Status;

    PAGED_CODE();

    NDIS_DbgPrint(MIN_TRACE, ("(%p, %wZ)\n", DriverObject, RegistryPath));

    Characteristics.MajorNdisVersion = NDIS_MINIPORT_MAJOR_VERSION;
    Characteristics.MinorNdisVersion = NDIS_MINIPORT_MINOR_VERSION;
    Characteristics.CheckForHangHandler = NULL;
    Characteristics.DisableInterruptHandler = NULL;
    Characteristics.EnableInterruptHandler = NULL;
    Characteristics.HaltHandler = MiniportHalt;
    Characteristics.HandleInterruptHandler = MiniportHandleInterrupt;
    Characteristics.InitializeHandler = MiniportInitialize;
    Characteristics.ISRHandler = MiniportISR;
    Characteristics.QueryInformationHandler = MiniportQueryInformation;
    Characteristics.ReconfigureHandler = NULL;
    Characteristics.ResetHandler = MiniportReset;
    Characteristics.SendHandler = MiniportSend;
    Characteristics.SetInformationHandler = MiniportSetInformation;
    Characteristics.TransferDataHandler = NULL;
    Characteristics.ReturnPacketHandler = NULL;
    Characteristics.SendPacketsHandler = NULL;
    Characteristics.AllocateCompleteHandler = NULL;

    NdisMInitializeWrapper(&WrapperHandle, DriverObject, RegistryPath, NULL);
    if (!WrapperHandle)
    {
        return NDIS_STATUS_FAILURE;
    }

    Status = NdisMRegisterMiniport(WrapperHandle, &Characteristics, sizeof(Characteristics));
    if (Status != NDIS_STATUS_SUCCESS)
    {
        NdisTerminateWrapper(WrapperHandle, NULL);
        return NDIS_STATUS_FAILURE;
    }

    NdisMRegisterUnloadHandler(WrapperHandle, MiniportUnload);

    return NDIS_STATUS_SUCCESS;
}
