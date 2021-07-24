/*
 * PROJECT:     ReactOS nVidia nForce Ethernet Controller Driver
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Packet sending
 * COPYRIGHT:   Copyright 2021 Dmitry Borisov <di.sean@protonmail.com>
 */

/* INCLUDES *******************************************************************/

#include "nvnet.h"

#define NDEBUG
#include <debug.h>

/* GLOBALS ********************************************************************/

static
VOID
NvNetTransmitPacket(
    _In_ PNVNET_ADAPTER Adapter,
    _In_ PHYSICAL_ADDRESS PhysicalAddress,
    _In_ ULONG Length)
{
    NDIS_DbgPrint(MIN_TRACE, ("Packet %d queued\n", Adapter->CurrentTx));

    if (Adapter->Flags & DEV_HAS_HIGH_DMA)
    {
        volatile PNVNET_DESCRIPTOR_64 Descriptor64;

        Descriptor64 = &Adapter->TransmitDescriptors64[Adapter->CurrentTx];
        Descriptor64->AddressHigh = NdisGetPhysicalAddressHigh(PhysicalAddress);
        Descriptor64->AddressLow = NdisGetPhysicalAddressLow(PhysicalAddress);
        Descriptor64->VlanTag = 0;
        Descriptor64->FlagsLength = NV_TX2_VALID | NV_TX2_LASTPACKET | (Length - 1);
    }
    else
    {
        volatile PNVNET_DESCRIPTOR_32 Descriptor32;

        Descriptor32 = &Adapter->TransmitDescriptors32[Adapter->CurrentTx];
        Descriptor32->Address = NdisGetPhysicalAddressLow(PhysicalAddress);

        if (Adapter->Flags & DEV_HAS_LARGEDESC)
            Descriptor32->FlagsLength = NV_TX2_VALID | NV_TX2_LASTPACKET | (Length - 1);
        else
            Descriptor32->FlagsLength = NV_TX_VALID | NV_TX_LASTPACKET | (Length - 1);
    }

    Adapter->CurrentTx = (Adapter->CurrentTx + 1) % NVNET_TRANSMIT_DESCRIPTORS;
    ++Adapter->BusyTx;

    NV_WRITE(Adapter, NvRegTxRxControl, Adapter->TxRxControl | NVREG_TXRXCTL_KICK);
}

NDIS_STATUS
NTAPI
MiniportSend(
    _In_ NDIS_HANDLE MiniportAdapterContext,
    _In_ PNDIS_PACKET Packet,
    _In_ UINT Flags)
{
    PNVNET_ADAPTER Adapter = (PNVNET_ADAPTER)MiniportAdapterContext;
    PSCATTER_GATHER_LIST SgList;
    NDIS_STATUS Status;

    NDIS_DbgPrint(MIN_TRACE, ("()\n"));

    SgList = NDIS_PER_PACKET_INFO_FROM_PACKET(Packet, ScatterGatherListPacketInfo);

    /* FIXME: Use the proper send function (MiniportSendPackets) and coalescing logic! */
    ASSERT(SgList->NumberOfElements == 1);
    ASSERT(SgList->Elements[0].Length <= NVNET_MAXIMUM_FRAME_SIZE);

    NdisDprAcquireSpinLock(&Adapter->Lock);

    if (!Adapter->Connected)
    {
        NDIS_DbgPrint(MIN_TRACE, ("No connection\n"));
        Status = NDIS_STATUS_NO_CABLE;
    }
    else if (Adapter->BusyTx > NVNET_TRANSMIT_DESCRIPTORS ||
             (Adapter->LimitTx && Adapter->BusyTx == NV_TX_LIMIT_COUNT))
    {
        NDIS_DbgPrint(MIN_TRACE, ("The transmit ring is full\n"));
        Status = NDIS_STATUS_RESOURCES;
    }
    else
    {
        InsertTailList(&Adapter->SendBusyList, PACKET_ENTRY(Packet));
        NvNetTransmitPacket(Adapter, SgList->Elements[0].Address, SgList->Elements[0].Length);
        Status = NDIS_STATUS_PENDING;
    }

    NdisDprReleaseSpinLock(&Adapter->Lock);

    return Status;
}
