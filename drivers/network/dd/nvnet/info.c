/*
 * PROJECT:     ReactOS nVidia nForce Ethernet Controller Driver
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Miniport information callbacks
 * COPYRIGHT:   Copyright 2021 Dmitry Borisov <di.sean@protonmail.com>
 */

/* INCLUDES *******************************************************************/

#include "nvnet.h"

#define NDEBUG
#include <debug.h>

/* GLOBALS ********************************************************************/

static const NDIS_OID NvNetSupportedOidList[] =
{
    OID_GEN_SUPPORTED_LIST,
    OID_GEN_CURRENT_PACKET_FILTER,
    OID_GEN_HARDWARE_STATUS,
    OID_GEN_MEDIA_SUPPORTED,
    OID_GEN_MEDIA_IN_USE,
    OID_GEN_MAXIMUM_LOOKAHEAD,
    OID_GEN_MAXIMUM_FRAME_SIZE,
    OID_GEN_MAXIMUM_SEND_PACKETS,
    OID_GEN_LINK_SPEED,
    OID_GEN_TRANSMIT_BUFFER_SPACE,
    OID_GEN_RECEIVE_BUFFER_SPACE,
    OID_GEN_RECEIVE_BLOCK_SIZE,
    OID_GEN_TRANSMIT_BLOCK_SIZE,
    OID_GEN_VENDOR_ID,
    OID_GEN_VENDOR_DESCRIPTION,
    OID_GEN_VENDOR_DRIVER_VERSION,
    OID_GEN_CURRENT_LOOKAHEAD,
    OID_GEN_DRIVER_VERSION,
    OID_GEN_MAXIMUM_TOTAL_SIZE,
    OID_GEN_MAC_OPTIONS,
    OID_GEN_MEDIA_CONNECT_STATUS,
    OID_802_3_PERMANENT_ADDRESS,
    OID_802_3_CURRENT_ADDRESS,
    OID_802_3_MULTICAST_LIST,
    OID_802_3_MAXIMUM_LIST_SIZE,

    /* Statistics */
    OID_GEN_XMIT_OK,
    OID_GEN_RCV_OK,
    OID_GEN_XMIT_ERROR,
    OID_GEN_RCV_ERROR,
    OID_GEN_RCV_NO_BUFFER,
    OID_GEN_DIRECTED_FRAMES_RCV,
    OID_GEN_RCV_CRC_ERROR,
    OID_GEN_TRANSMIT_QUEUE_LENGTH,
    OID_802_3_RCV_ERROR_ALIGNMENT,
    OID_802_3_XMIT_ONE_COLLISION,
    OID_802_3_XMIT_MORE_COLLISIONS,
    OID_802_3_XMIT_DEFERRED,
    OID_802_3_XMIT_MAX_COLLISIONS,
    OID_802_3_RCV_OVERRUN,
    OID_802_3_XMIT_UNDERRUN,
    OID_802_3_XMIT_HEARTBEAT_FAILURE,
    OID_802_3_XMIT_TIMES_CRS_LOST,
    OID_802_3_XMIT_LATE_COLLISIONS,

    /* Offload */
    OID_TCP_TASK_OFFLOAD,

    /* Power management */
    OID_PNP_CAPABILITIES,
    OID_PNP_SET_POWER,
    OID_PNP_QUERY_POWER,
    OID_PNP_ADD_WAKE_UP_PATTERN,
    OID_PNP_REMOVE_WAKE_UP_PATTERN,
    OID_PNP_ENABLE_WAKE_UP
};

static
ULONG
NvNetGetLinkSpeed(
    _In_ PNVNET_ADAPTER Adapter)
{
    ULONG LinkSpeedMbps;

    switch (Adapter->LinkSpeed)
    {
        case NVREG_LINKSPEED_10:
            LinkSpeedMbps = 10;
            break;
        case NVREG_LINKSPEED_100:
            LinkSpeedMbps = 100;
            break;
        case NVREG_LINKSPEED_1000:
            LinkSpeedMbps = 1000;
            break;

        default:
            ASSERT(FALSE);
            UNREACHABLE;
            break;
    }

    return LinkSpeedMbps;
}

static
ULONG
PacketFilterToMask(
    _In_ ULONG PacketFilter)
{
    ULONG FilterMask = NVREG_PFF_ALWAYS | NVREG_PFF_MYADDR | NVREG_PFF_PROMISC;

    if (PacketFilter & NDIS_PACKET_TYPE_PROMISCUOUS)
    {
        FilterMask &= ~NVREG_PFF_MYADDR;
    }

    if (PacketFilter & NDIS_PACKET_TYPE_BROADCAST)
    {
        FilterMask &= ~NVREG_PFF_PROMISC;
    }

    return FilterMask;
}

static
VOID
NvNetApplyPacketFilter(
    _In_ PNVNET_ADAPTER Adapter)
{
    UCHAR Address[ETH_LENGTH_OF_ADDRESS];
    UCHAR Mask[ETH_LENGTH_OF_ADDRESS];
    ULONG FilterMask;

    if (Adapter->PacketFilter & NDIS_PACKET_TYPE_ALL_MULTICAST)
    {
        NdisZeroMemory(Address, sizeof(Address));
        NdisZeroMemory(Mask, sizeof(Mask));

        Address[0] |= NVREG_MCASTADDRA_FORCE;
        Mask[0] |= NVREG_MCASTADDRA_FORCE;
    }
    else if (Adapter->PacketFilter & NDIS_PACKET_TYPE_MULTICAST)
    {
        if (Adapter->MulticastListSize > 0)
        {
            ULONG i, j;

            NdisFillMemory(Address, sizeof(Address), 0xFF);
            NdisFillMemory(Mask, sizeof(Mask), 0xFF);

            for (i = 0; i < Adapter->MulticastListSize; ++i)
            {
                PUCHAR MacAddress = Adapter->MulticastList[i].MacAddress;

                for (j = 0; j < ETH_LENGTH_OF_ADDRESS; ++j)
                {
                    Address[j] &= MacAddress[j];
                    Mask[j] &= ~MacAddress[j];
                }
            }

            for (j = 0; j < ETH_LENGTH_OF_ADDRESS; ++j)
            {
                Mask[j] |= Address[j];
            }
        }
        else
        {
            NdisZeroMemory(Address, sizeof(Address));
            NdisZeroMemory(Mask, sizeof(Mask));
        }
    }
    else
    {
        NdisZeroMemory(Address, sizeof(Address));
        NdisFillMemory(Mask, sizeof(Mask), 0xFF);
    }

    FilterMask = NV_READ(Adapter, NvRegPacketFilterFlags) & NVREG_PFF_PAUSE_RX;
    FilterMask |= PacketFilterToMask(Adapter->PacketFilter);

    NvNetStopReceiver(Adapter);

    NV_WRITE(Adapter, NvRegMulticastAddrA,
             Address[3] << 24 | Address[2] << 16 | Address[1] << 8 | Address[0]);
    NV_WRITE(Adapter, NvRegMulticastAddrB, Address[5] << 8 | Address[4]);
    NV_WRITE(Adapter, NvRegMulticastMaskA,
             Mask[3] << 24 | Mask[2] << 16 | Mask[1] << 8 | Mask[0]);
    NV_WRITE(Adapter, NvRegMulticastMaskB, Mask[5] << 8 | Mask[4]);

    NV_WRITE(Adapter, NvRegPacketFilterFlags, FilterMask);

    NvNetStartReceiver(Adapter);
}

static
VOID
QueryHardwareCounters(
    _Inout_ PNVNET_ADAPTER Adapter)
{
    Adapter->Statistics.HwTxCnt += NV_READ(Adapter, NvRegTxCnt);
    Adapter->Statistics.HwTxZeroReXmt += NV_READ(Adapter, NvRegTxZeroReXmt);
    Adapter->Statistics.HwTxOneReXmt += NV_READ(Adapter, NvRegTxOneReXmt);
    Adapter->Statistics.HwTxManyReXmt += NV_READ(Adapter, NvRegTxManyReXmt);
    Adapter->Statistics.HwTxLateCol += NV_READ(Adapter, NvRegTxLateCol);
    Adapter->Statistics.HwTxUnderflow += NV_READ(Adapter, NvRegTxUnderflow);
    Adapter->Statistics.HwTxLossCarrier += NV_READ(Adapter, NvRegTxLossCarrier);
    Adapter->Statistics.HwTxExcessDef += NV_READ(Adapter, NvRegTxExcessDef);
    Adapter->Statistics.HwTxRetryErr += NV_READ(Adapter, NvRegTxRetryErr);
    Adapter->Statistics.HwRxFrameErr += NV_READ(Adapter, NvRegRxFrameErr);
    Adapter->Statistics.HwRxExtraByte += NV_READ(Adapter, NvRegRxExtraByte);
    Adapter->Statistics.HwRxLateCol += NV_READ(Adapter, NvRegRxLateCol);
    Adapter->Statistics.HwRxRunt += NV_READ(Adapter, NvRegRxRunt);
    Adapter->Statistics.HwRxFrameTooLong += NV_READ(Adapter, NvRegRxFrameTooLong);
    Adapter->Statistics.HwRxOverflow += NV_READ(Adapter, NvRegRxOverflow);
    Adapter->Statistics.HwRxFCSErr += NV_READ(Adapter, NvRegRxFCSErr);
    Adapter->Statistics.HwRxFrameAlignErr += NV_READ(Adapter, NvRegRxFrameAlignErr);
    Adapter->Statistics.HwRxLenErr += NV_READ(Adapter, NvRegRxLenErr);
    Adapter->Statistics.HwRxUnicast += NV_READ(Adapter, NvRegRxUnicast);
    Adapter->Statistics.HwRxMulticast += NV_READ(Adapter, NvRegRxMulticast);
    Adapter->Statistics.HwRxBroadcast += NV_READ(Adapter, NvRegRxBroadcast);

    if (Adapter->Flags & DEV_HAS_STATISTICS_V2)
    {
        Adapter->Statistics.HwTxDef += NV_READ(Adapter, NvRegTxDef);
        Adapter->Statistics.HwTxFrame += NV_READ(Adapter, NvRegTxFrame);
        Adapter->Statistics.HwRxCnt += NV_READ(Adapter, NvRegRxCnt);
        Adapter->Statistics.HwTxPause += NV_READ(Adapter, NvRegTxPause);
        Adapter->Statistics.HwRxPause += NV_READ(Adapter, NvRegRxPause);
        Adapter->Statistics.HwRxDropFrame += NV_READ(Adapter, NvRegRxDropFrame);
    }

    if (Adapter->Flags & DEV_HAS_STATISTICS_V3)
    {
        Adapter->Statistics.HwTxUnicast += NV_READ(Adapter, NvRegTxUnicast);
        Adapter->Statistics.HwTxMulticast += NV_READ(Adapter, NvRegTxMulticast);
        Adapter->Statistics.HwTxBroadcast += NV_READ(Adapter, NvRegTxBroadcast);
    }

    if (Adapter->Flags & DEV_HAS_STATISTICS_V2)
    {
        Adapter->Statistics.TransmitOk = Adapter->Statistics.HwTxFrame;
    }
    else
    {
        Adapter->Statistics.TransmitOk = (Adapter->Statistics.HwTxZeroReXmt +
                                          Adapter->Statistics.HwTxOneReXmt +
                                          Adapter->Statistics.HwTxManyReXmt);

    }
    Adapter->Statistics.ReceiveOk = (Adapter->Statistics.HwRxUnicast +
                                     Adapter->Statistics.HwRxMulticast +
                                     Adapter->Statistics.HwRxBroadcast);
    Adapter->Statistics.TransmitErrors = (Adapter->Statistics.HwTxRetryErr +
                                          Adapter->Statistics.HwTxLateCol +
                                          Adapter->Statistics.HwTxUnderflow +
                                          Adapter->Statistics.HwTxLossCarrier +
                                          Adapter->Statistics.HwTxExcessDef);
    Adapter->Statistics.ReceiveErrors = (Adapter->Statistics.HwRxFrameAlignErr +
                                         Adapter->Statistics.HwRxLenErr +
                                         Adapter->Statistics.HwRxRunt +
                                         Adapter->Statistics.HwRxFrameTooLong +
                                         Adapter->Statistics.HwRxFCSErr +
                                         Adapter->Statistics.HwRxFrameErr +
                                         Adapter->Statistics.HwRxExtraByte +
                                         Adapter->Statistics.HwRxLateCol);
    Adapter->Statistics.ReceiveNoBuffers = (Adapter->Statistics.HwRxDropFrame +
                                            Adapter->Statistics.HwRxOverflow +
                                            Adapter->Statistics.ReceiveIrqNoBuffers);
    Adapter->Statistics.ReceiveDirected = Adapter->Statistics.HwRxUnicast;
    Adapter->Statistics.ReceiveCrcErrors = Adapter->Statistics.HwRxFCSErr;
    Adapter->Statistics.ReceiveAlignmentErrors = Adapter->Statistics.HwRxFrameErr; // ???
    Adapter->Statistics.TransmitDeferred = Adapter->Statistics.HwTxDef;
    Adapter->Statistics.TransmitExcessiveCollisions = Adapter->Statistics.HwTxRetryErr;
    Adapter->Statistics.ReceiveOverrunErrors = Adapter->Statistics.HwRxOverflow;
    Adapter->Statistics.TransmitUnderrunErrors = Adapter->Statistics.HwTxUnderflow;
    Adapter->Statistics.TransmitZeroRetry = Adapter->Statistics.HwTxZeroReXmt;
    Adapter->Statistics.TransmitOneRetry = Adapter->Statistics.HwTxOneReXmt;
    Adapter->Statistics.TransmitLostCarrierSense = Adapter->Statistics.HwTxLossCarrier;
    Adapter->Statistics.TransmitLateCollisions = Adapter->Statistics.HwTxLateCol;
}

static
ULONG64
NvNetQueryStatisticsCounter(
    _In_ PNVNET_ADAPTER Adapter,
    _In_ NDIS_OID Oid)
{
    if (Adapter->Flags & DEV_HAS_STATISTICS_COUNTERS)
    {
        QueryHardwareCounters(Adapter);
    }

    switch (Oid)
    {
        case OID_GEN_XMIT_OK:
            return Adapter->Statistics.TransmitOk;
        case OID_GEN_RCV_OK:
            return Adapter->Statistics.ReceiveOk;
        case OID_GEN_XMIT_ERROR:
            return Adapter->Statistics.TransmitErrors;
        case OID_GEN_RCV_ERROR:
            return Adapter->Statistics.ReceiveErrors;
        case OID_GEN_RCV_NO_BUFFER:
            return Adapter->Statistics.ReceiveNoBuffers;
        case OID_GEN_DIRECTED_FRAMES_RCV:
            return Adapter->Statistics.ReceiveDirected;
        case OID_GEN_RCV_CRC_ERROR:
            return Adapter->Statistics.ReceiveCrcErrors;
        case OID_GEN_TRANSMIT_QUEUE_LENGTH:
            return Adapter->BusyTx;
        case OID_802_3_RCV_ERROR_ALIGNMENT:
            return Adapter->Statistics.ReceiveAlignmentErrors;
        case OID_802_3_XMIT_ONE_COLLISION:
            return Adapter->Statistics.TransmitOneRetry;
        case OID_802_3_XMIT_MORE_COLLISIONS:
            return (Adapter->Statistics.TransmitOk -
                    Adapter->Statistics.TransmitOneRetry -
                    Adapter->Statistics.TransmitZeroRetry);
        case OID_802_3_XMIT_DEFERRED:
            return Adapter->Statistics.TransmitDeferred;
        case OID_802_3_XMIT_MAX_COLLISIONS:
            return Adapter->Statistics.TransmitExcessiveCollisions;
        case OID_802_3_RCV_OVERRUN:
            return Adapter->Statistics.ReceiveOverrunErrors;
        case OID_802_3_XMIT_UNDERRUN:
            return Adapter->Statistics.TransmitUnderrunErrors;
        case OID_802_3_XMIT_HEARTBEAT_FAILURE:
            return Adapter->Statistics.TransmitZeroRetry;
        case OID_802_3_XMIT_TIMES_CRS_LOST:
            return Adapter->Statistics.TransmitLostCarrierSense;
        case OID_802_3_XMIT_LATE_COLLISIONS:
            return Adapter->Statistics.TransmitLateCollisions;

        default:
            ASSERT(FALSE);
            UNREACHABLE;
            break;
    }
}

static
NDIS_STATUS
NvNetFillPowerManagementCapabilities(
    _In_ PNVNET_ADAPTER Adapter,
    _Out_ PNDIS_PNP_CAPABILITIES Capabilities)
{
    NDIS_DEVICE_POWER_STATE PowerState;

    if (Adapter->Flags & DEV_HAS_POWER_CNTRL)
    {
        ULONG HwCapabilities = NV_READ(Adapter, NvRegPowerCap);

        if (HwCapabilities & NVREG_POWERCAP_D3SUPP)
            PowerState = NdisDeviceStateD3;
        else if (HwCapabilities & NVREG_POWERCAP_D2SUPP)
            PowerState = NdisDeviceStateD2;
        else if (HwCapabilities & NVREG_POWERCAP_D1SUPP)
            PowerState = NdisDeviceStateD1;
        else
            PowerState = NdisDeviceStateD0;
    }
    else
    {
        PowerState = NdisDeviceStateUnspecified;
    }

    Capabilities->WakeUpCapabilities.MinMagicPacketWakeUp =
    Capabilities->WakeUpCapabilities.MinPatternWakeUp =
    Capabilities->WakeUpCapabilities.MinLinkChangeWakeUp = PowerState;

    /* All hardware is PM-aware */
    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
NTAPI
MiniportQueryInformation(
  _In_ NDIS_HANDLE MiniportAdapterContext,
  _In_ NDIS_OID Oid,
  _In_ PVOID InformationBuffer,
  _In_ ULONG InformationBufferLength,
  _Out_ PULONG BytesWritten,
  _Out_ PULONG BytesNeeded)
{
    PNVNET_ADAPTER Adapter = (PNVNET_ADAPTER)MiniportAdapterContext;
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    NDIS_PNP_CAPABILITIES PowerManagementCapabilities;
    ULONG Info, InfoLength = sizeof(Info);
    PVOID InfoPtr = &Info;

    switch (Oid)
    {
        case OID_GEN_SUPPORTED_LIST:
            InfoPtr = (PVOID)&NvNetSupportedOidList;
            InfoLength = sizeof(NvNetSupportedOidList);
            break;

        case OID_GEN_HARDWARE_STATUS:
            Info = NdisHardwareStatusReady;
            break;

        case OID_GEN_MEDIA_SUPPORTED:
        case OID_GEN_MEDIA_IN_USE:
        {
            static const NDIS_MEDIUM Medium = NdisMedium802_3;
            InfoPtr = (PVOID)&Medium;
            InfoLength = sizeof(Medium);
            break;
        }

        case OID_GEN_CURRENT_LOOKAHEAD:
        case OID_GEN_MAXIMUM_LOOKAHEAD:
        {
            Info = Adapter->MaximumFrameSize - sizeof(ETH_HEADER);
            break;
        }

        case OID_GEN_MAXIMUM_FRAME_SIZE:
        {
            Info = Adapter->MaximumFrameSize;
            break;
        }

        case OID_GEN_LINK_SPEED:
        {
            Info = NvNetGetLinkSpeed(Adapter) * 10000;
            break;
        }

        case OID_GEN_TRANSMIT_BUFFER_SPACE:
        {
            /* TODO: Change this later, once the driver can handle multipacket sends */
            Info = Adapter->MaximumFrameSize;
            break;
        }

        case OID_GEN_RECEIVE_BUFFER_SPACE:
        {
            Info = Adapter->MaximumFrameSize * NVNET_RECEIVE_DESCRIPTORS;
        }

        case OID_GEN_MAXIMUM_TOTAL_SIZE:
        case OID_GEN_TRANSMIT_BLOCK_SIZE:
        case OID_GEN_RECEIVE_BLOCK_SIZE:
        {
            Info = Adapter->MaximumFrameSize;
            break;
        }

        case OID_GEN_VENDOR_ID:
        {
            Info = 0;
            Info |= (Adapter->PermanentMacAddress[0] << 16);
            Info |= (Adapter->PermanentMacAddress[1] << 8);
            Info |= (Adapter->PermanentMacAddress[2] & 0xFF);
            break;
        }

        case OID_GEN_DRIVER_VERSION:
        {
            static const USHORT DriverVersion = (NDIS_MINIPORT_MAJOR_VERSION << 8) |
                                                NDIS_MINIPORT_MINOR_VERSION;
            InfoPtr = (PVOID)&DriverVersion;
            InfoLength = sizeof(DriverVersion);
            break;
        }

        case OID_GEN_VENDOR_DESCRIPTION:
        {
            static const UCHAR VendorDesc[] = "nVidia nForce Ethernet Controller";
            InfoPtr = (PVOID)&VendorDesc;
            InfoLength = sizeof(VendorDesc);
            break;
        }

        case OID_GEN_CURRENT_PACKET_FILTER:
        {
            Info = Adapter->PacketFilter;
            break;
        }

        case OID_GEN_MAC_OPTIONS:
        {
            Info = NDIS_MAC_OPTION_COPY_LOOKAHEAD_DATA |
                   NDIS_MAC_OPTION_TRANSFERS_NOT_PEND |
                   NDIS_MAC_OPTION_NO_LOOPBACK;

            if (Adapter->PrioritySupported)
                Info |= NDIS_MAC_OPTION_8021P_PRIORITY;
            if (Adapter->VlanTaggingSupported)
                Info |= NDIS_MAC_OPTION_8021Q_VLAN;
            break;
        }

        case OID_GEN_MEDIA_CONNECT_STATUS:
        {
            Info = Adapter->Connected ? NdisMediaStateConnected : NdisMediaStateDisconnected;
            break;
        }

        case OID_GEN_MAXIMUM_SEND_PACKETS:
        {
            /* TODO: Multipacket sends */
            Info = 1;
            break;
        }

        case OID_GEN_VENDOR_DRIVER_VERSION:
        {
            /* 1.0.0 */
            Info = 0x100;
            break;
        }

        case OID_GEN_XMIT_OK:
        case OID_GEN_RCV_OK:
        case OID_GEN_XMIT_ERROR:
        case OID_GEN_RCV_ERROR:
        case OID_GEN_RCV_NO_BUFFER:
        case OID_GEN_DIRECTED_FRAMES_RCV:
        case OID_GEN_RCV_CRC_ERROR:
        case OID_GEN_TRANSMIT_QUEUE_LENGTH:
        case OID_802_3_RCV_ERROR_ALIGNMENT:
        case OID_802_3_XMIT_ONE_COLLISION:
        case OID_802_3_XMIT_MORE_COLLISIONS:
        case OID_802_3_XMIT_DEFERRED:
        case OID_802_3_XMIT_MAX_COLLISIONS:
        case OID_802_3_RCV_OVERRUN:
        case OID_802_3_XMIT_UNDERRUN:
        case OID_802_3_XMIT_HEARTBEAT_FAILURE:
        case OID_802_3_XMIT_TIMES_CRS_LOST:
        case OID_802_3_XMIT_LATE_COLLISIONS:
        {
            ULONG64 Info64 = NvNetQueryStatisticsCounter(Adapter, Oid);

            /* Statistics need special handling */
            *BytesNeeded = sizeof(ULONG64);
            if (InformationBufferLength < sizeof(ULONG))
            {
                *BytesWritten = 0;
                return NDIS_STATUS_BUFFER_TOO_SHORT;
            }
            if (InformationBufferLength >= sizeof(ULONG64))
            {
                *BytesWritten = sizeof(ULONG64);
                NdisMoveMemory(InformationBuffer, &Info64, sizeof(ULONG64));
            }
            else
            {
                *BytesWritten = sizeof(ULONG);
                NdisMoveMemory(InformationBuffer, &Info64, sizeof(ULONG));
            }
            return NDIS_STATUS_SUCCESS;
        }

        case OID_802_3_PERMANENT_ADDRESS:
        {
            InfoPtr = Adapter->PermanentMacAddress;
            InfoLength = ETH_LENGTH_OF_ADDRESS;
            break;
        }

        case OID_802_3_CURRENT_ADDRESS:
        {
            InfoPtr = Adapter->CurrentMacAddress;
            InfoLength = ETH_LENGTH_OF_ADDRESS;
            break;
        }

        case OID_802_3_MULTICAST_LIST:
        {
            InfoPtr = Adapter->MulticastList;
            InfoLength = Adapter->MulticastListSize * ETH_LENGTH_OF_ADDRESS;
            break;
        }

        case OID_802_3_MAXIMUM_LIST_SIZE:
        {
            Info = NVNET_MULTICAST_LIST_SIZE;
            break;
        }

        case OID_PNP_CAPABILITIES:
        {
            InfoPtr = &PowerManagementCapabilities;
            InfoLength = sizeof(PowerManagementCapabilities);

            Status = NvNetFillPowerManagementCapabilities(Adapter, &PowerManagementCapabilities);
            break;
        }

        case OID_PNP_QUERY_POWER:
        case OID_PNP_ENABLE_WAKE_UP:
            Status = NDIS_STATUS_NOT_SUPPORTED;
            break;

        default:
            Status = NDIS_STATUS_INVALID_OID;
            break;
    }

    if (Status == NDIS_STATUS_SUCCESS)
    {
        if (InfoLength > InformationBufferLength)
        {
            *BytesWritten = 0;
            *BytesNeeded = InfoLength;
            Status = NDIS_STATUS_BUFFER_TOO_SHORT;
        }
        else
        {
            NdisMoveMemory(InformationBuffer, InfoPtr, InfoLength);
            *BytesWritten = InfoLength;
            *BytesNeeded = 0;
        }
    }
    else
    {
        *BytesWritten = 0;
        *BytesNeeded = 0;
    }

    return Status;
}

NDIS_STATUS
NTAPI
MiniportSetInformation(
  _In_ NDIS_HANDLE MiniportAdapterContext,
  _In_ NDIS_OID Oid,
  _In_ PVOID InformationBuffer,
  _In_ ULONG InformationBufferLength,
  _Out_ PULONG BytesRead,
  _Out_ PULONG BytesNeeded)
{
    PNVNET_ADAPTER Adapter = (PNVNET_ADAPTER)MiniportAdapterContext;
    NDIS_STATUS Status;

    *BytesRead = 0;
    *BytesNeeded = 0;

    switch (Oid)
    {
        case OID_802_3_MULTICAST_LIST:
        {
            if (InformationBufferLength % ETH_LENGTH_OF_ADDRESS)
            {
                *BytesNeeded = (InformationBufferLength / ETH_LENGTH_OF_ADDRESS) *
                               ETH_LENGTH_OF_ADDRESS;
                Status = NDIS_STATUS_INVALID_LENGTH;
                break;
            }

            if (InformationBufferLength > sizeof(Adapter->MulticastList))
            {
                *BytesNeeded = sizeof(Adapter->MulticastList);
                Status = NDIS_STATUS_MULTICAST_FULL;
                break;
            }

            *BytesRead = InformationBufferLength;
            NdisMoveMemory(Adapter->MulticastList, InformationBuffer, InformationBufferLength);

            Adapter->MulticastListSize = InformationBufferLength / ETH_LENGTH_OF_ADDRESS;

            ASSERT(KeGetCurrentIrql() == DISPATCH_LEVEL);

            NdisDprAcquireSpinLock(&Adapter->Lock);

            NvNetApplyPacketFilter(Adapter);

            NdisDprReleaseSpinLock(&Adapter->Lock);

            Status = NDIS_STATUS_SUCCESS;
            break;
        }

        case OID_GEN_CURRENT_PACKET_FILTER:
        {
            ULONG PacketFilter;

            if (InformationBufferLength < sizeof(ULONG))
            {
                *BytesNeeded = sizeof(ULONG);
                Status = NDIS_STATUS_INVALID_LENGTH;
                break;
            }

            *BytesRead = sizeof(ULONG);
            NdisMoveMemory(&PacketFilter, InformationBuffer, sizeof(ULONG));

            NDIS_DbgPrint(MIN_TRACE, ("Requested filter 0x%lx\n", PacketFilter));

            if (PacketFilter & ~NVNET_PACKET_FILTERS)
            {
                Status = NDIS_STATUS_NOT_SUPPORTED;
                break;
            }

            if (Adapter->PacketFilter == PacketFilter)
            {
                Status = NDIS_STATUS_SUCCESS;
                break;
            }

            Adapter->PacketFilter = PacketFilter;

            ASSERT(KeGetCurrentIrql() == DISPATCH_LEVEL);

            NdisDprAcquireSpinLock(&Adapter->Lock);

            NvNetApplyPacketFilter(Adapter);

            NdisDprReleaseSpinLock(&Adapter->Lock);

            Status = NDIS_STATUS_SUCCESS;
            break;
        }

        case OID_GEN_CURRENT_LOOKAHEAD:
        {
            if (InformationBufferLength < sizeof(ULONG))
            {
                *BytesNeeded = sizeof(ULONG);
                Status = NDIS_STATUS_INVALID_LENGTH;
                break;
            }

            /* Nothing to do */
            *BytesRead = sizeof(ULONG);
            Status = NDIS_STATUS_SUCCESS;
            break;
        }

        /* TODO */
        case OID_TCP_TASK_OFFLOAD:
        case OID_PNP_SET_POWER:
        case OID_PNP_ADD_WAKE_UP_PATTERN:
        case OID_PNP_REMOVE_WAKE_UP_PATTERN:
        case OID_PNP_ENABLE_WAKE_UP:
            Status = NDIS_STATUS_NOT_SUPPORTED;
            break;

        default:
            Status = NDIS_STATUS_NOT_SUPPORTED;
            break;
    }

    return Status;
}
