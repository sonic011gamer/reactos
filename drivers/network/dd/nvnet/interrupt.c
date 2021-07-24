/*
 * PROJECT:     ReactOS nVidia nForce Ethernet Controller Driver
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Interrupt handling
 * COPYRIGHT:   Copyright 2021 Dmitry Borisov <di.sean@protonmail.com>
 */

/* INCLUDES *******************************************************************/

#include "nvnet.h"

#define NDEBUG
#include <debug.h>

/* GLOBALS ********************************************************************/

static
BOOLEAN
HandleLengthError(
    _In_ PVOID EthHeader,
    _Inout_ PUSHORT Length)
{
    /* TODO */
    return FALSE;
}

static
ULONG
ProcessTransmitDescriptors(
    _In_ PNVNET_ADAPTER Adapter,
    _Inout_ PLIST_ENTRY SendReadyList)
{
    ULONG TxProcessed = 0;

    NDIS_DbgPrint(MIN_TRACE, ("()\n"));

    while (Adapter->LastTx != Adapter->CurrentTx)
    {
        ULONG Flags;

        if (Adapter->Flags & DEV_HAS_HIGH_DMA)
            Flags = (&Adapter->TransmitDescriptors64[Adapter->LastTx])->FlagsLength;
        else
            Flags = (&Adapter->TransmitDescriptors32[Adapter->LastTx])->FlagsLength;

        if (Flags & NV_TX_VALID)
            break;

        if (Adapter->Flags & (DEV_HAS_HIGH_DMA | DEV_HAS_LARGEDESC))
        {
            if (Flags & NV_TX2_ERROR)
            {
                if ((Flags & NV_TX2_RETRYERROR) && !(Flags & NV_TX2_RETRYCOUNT_MASK))
                {
                    if (Adapter->Flags & DEV_HAS_GEAR_MODE)
                        NvNetBackoffReseedEx(Adapter);
                    else
                        NvNetBackoffReseed(Adapter);
                }
            }

            NDIS_DbgPrint(MIN_TRACE, ("Packet %d transmitted (flags %lX)\n",
                                      Adapter->LastTx, Flags & FLAG_MASK_V2));
        }
        else
        {
            if (Flags & NV_TX_ERROR)
            {
                ++Adapter->Statistics.TransmitErrors;

                if (Flags & NV_TX_UNDERFLOW)
                    ++Adapter->Statistics.TransmitUnderrunErrors;
                if (Flags & NV_TX_LATECOLLISION)
                    ++Adapter->Statistics.TransmitLateCollisions;
                if (Flags & NV_TX_CARRIERLOST)
                    ++Adapter->Statistics.TransmitLostCarrierSense;
                if (Flags & NV_TX_RETRYERROR)
                {
                    ++Adapter->Statistics.TransmitExcessiveCollisions;

                    if (!(Flags & NV_TX_RETRYCOUNT_MASK))
                    {
                        NvNetBackoffReseed(Adapter);
                    }
                }
            }
            else
            {
                ++Adapter->Statistics.TransmitOk;

                if (Flags & NV_TX_DEFERRED)
                {
                    ++Adapter->Statistics.TransmitDeferred;
                }
                if (!(Flags & NV_TX_RETRYCOUNT_MASK))
                    ++Adapter->Statistics.TransmitZeroRetry;
                else if ((Flags & NV_TX_RETRYCOUNT_MASK) == NV_TX_ONE_RETRY)
                    ++Adapter->Statistics.TransmitOneRetry;
            }

            NDIS_DbgPrint(MIN_TRACE, ("Packet %d transmitted (flags %lX)\n",
                                      Adapter->LastTx, Flags & FLAG_MASK_V1));
        }

        InsertTailList(SendReadyList, RemoveHeadList(&Adapter->SendBusyList));

        Adapter->LastTx = (Adapter->LastTx + 1) % NVNET_TRANSMIT_DESCRIPTORS;
        --Adapter->BusyTx;
        ++TxProcessed;
    }

    return TxProcessed;
}

static
ULONG
ProcessReceiveDescriptors(
    _In_ PNVNET_ADAPTER Adapter,
    _In_ ULONG TotalRxProcessed)
{
    ULONG i, RxProcessed = 0;
    BOOLEAN IndicateComplete = FALSE;

    NDIS_DbgPrint(MIN_TRACE, ("()\n"));

    for (i = 0; i < NVNET_RECEIVE_DESCRIPTORS; ++i)
    {
        PNVNET_DESCRIPTOR_32 ReceiveDescriptor32;
        PNVNET_DESCRIPTOR_64 ReceiveDescriptor64;
        ULONG Flags;
        USHORT Length;
        PUCHAR EthHeader;

        if (Adapter->Flags & DEV_HAS_HIGH_DMA)
        {
            ReceiveDescriptor64 = &Adapter->ReceiveDescriptors64[Adapter->CurrentRx];
            Flags = ReceiveDescriptor64->FlagsLength;
        }
        else
        {
            ReceiveDescriptor32 = &Adapter->ReceiveDescriptors32[Adapter->CurrentRx];
            Flags = ReceiveDescriptor32->FlagsLength;
        }

        if (Flags & NV_RX_AVAIL)
            break;

        if (TotalRxProcessed + RxProcessed >= NVNET_RECEIVE_PROCESSING_LIMIT)
            break;

        if (!Adapter->PacketFilter)
            goto NextDescriptor;

        if (Adapter->Flags & (DEV_HAS_HIGH_DMA | DEV_HAS_LARGEDESC))
        {
            if (!(Flags & NV_RX2_DESCRIPTORVALID))
                goto NextDescriptor;

            Length = Flags & LEN_MASK_V2;
            EthHeader = &Adapter->ReceiveBuffer[Adapter->CurrentRx * NVNET_RECEIVE_BUFFER_SIZE];

            if (Flags & NV_RX2_ERROR)
            {
                if ((Flags & NV_RX2_ERROR_MASK) == NV_RX2_LENGTH_ERROR)
                {
                    if (!HandleLengthError(EthHeader, &Length))
                        goto NextDescriptor;
                }
                else if ((Flags & NV_RX2_ERROR_MASK) == NV_RX2_FRAMINGERR)
                {
                    if (Flags & NV_RX2_SUBTRACT1)
                        --Length;
                }
                else
                {
                    goto NextDescriptor;
                }
            }

            NDIS_DbgPrint(MIN_TRACE, ("Packet v2 %d received (length %d, flags %lX)\n",
                                      Adapter->CurrentRx, Length, Flags & FLAG_MASK_V2));
        }
        else
        {
            if (!(Flags & NV_RX_DESCRIPTORVALID))
                goto NextDescriptor;

            Length = Flags & LEN_MASK_V1;
            EthHeader = &Adapter->ReceiveBuffer[Adapter->CurrentRx * NVNET_RECEIVE_BUFFER_SIZE];

            if (Flags & NV_RX_ERROR)
            {
                if ((Flags & NV_RX_ERROR_MASK) == NV_RX_LENGTH_ERROR)
                {
                    if (!HandleLengthError(EthHeader, &Length))
                        goto NextDescriptor;
                }
                else if ((Flags & NV_RX_ERROR_MASK) == NV_RX_FRAMINGERR)
                {
                    if (Flags & NV_RX_SUBTRACT1)
                        --Length;
                }
                else
                {
                    ++Adapter->Statistics.ReceiveErrors;

                    if (Flags & NV_RX_MISSEDFRAME)
                        ++Adapter->Statistics.ReceiveNoBuffers;
                    if (Flags & NV_RX_FRAMINGERR)
                        ++Adapter->Statistics.ReceiveAlignmentErrors;
                    if (Flags & NV_RX_OVERFLOW)
                        ++Adapter->Statistics.ReceiveOverrunErrors;
                    if (Flags & NV_RX_CRCERR)
                        ++Adapter->Statistics.ReceiveCrcErrors;

                    goto NextDescriptor;
                }
            }
            ++Adapter->Statistics.ReceiveOk;

            NDIS_DbgPrint(MIN_TRACE, ("Packet v1 %d received (length %d, flags %lX)\n",
                                      Adapter->CurrentRx, Length, Flags & FLAG_MASK_V1));
        }

        NdisMEthIndicateReceive(Adapter->AdapterHandle,
                                NULL,
                                (PCHAR)EthHeader,
                                sizeof(ETH_HEADER),
                                EthHeader + sizeof(ETH_HEADER),
                                Length - sizeof(ETH_HEADER),
                                Length - sizeof(ETH_HEADER));
        IndicateComplete = TRUE;

NextDescriptor:
        /* Invalidate the buffer length and release the descriptor */
        if (Adapter->Flags & DEV_HAS_HIGH_DMA)
            ReceiveDescriptor64->FlagsLength = NV_RX2_AVAIL | NVNET_RECEIVE_BUFFER_SIZE;
        else
            ReceiveDescriptor32->FlagsLength = NV_RX_AVAIL | NVNET_RECEIVE_BUFFER_SIZE;

        Adapter->CurrentRx = (Adapter->CurrentRx + 1) % NVNET_RECEIVE_DESCRIPTORS;
        ++RxProcessed;
    }

    if (IndicateComplete)
    {
        NdisMEthIndicateReceiveComplete(Adapter->AdapterHandle);
    }

    return RxProcessed;
}

static
inline
VOID
ChangeInterruptMode(
    _In_ PNVNET_ADAPTER Adapter,
    _In_ ULONG Workload)
{
    NDIS_DbgPrint(MIN_TRACE, ("()\n"));

    if (Workload > NVNET_IM_THRESHOLD)
    {
        Adapter->InterruptIdleCount = 0;

        /* High activity, polling based strategy */
        Adapter->InterruptMask = NVREG_IRQMASK_CPU;
    }
    else
    {
        if (Adapter->InterruptIdleCount < NVNET_IM_MAX_IDLE)
        {
            ++Adapter->InterruptIdleCount;
        }
        else
        {
            /* Low activity, 1 interrupt per packet */
            Adapter->InterruptMask = NVREG_IRQMASK_THROUGHPUT;
        }
    }
}

static
VOID
HandleLinkStateChange(
    _In_ PNVNET_ADAPTER Adapter)
{
    ULONG MiiStatus;
    BOOLEAN Connected, Report = FALSE;

    NDIS_DbgPrint(MIN_TRACE, ("()\n"));

    NdisDprAcquireSpinLock(&Adapter->Lock);

    MiiStatus = NV_READ(Adapter, NvRegMIIStatus);

    /* Clear the link change interrupt */
    NV_WRITE(Adapter, NvRegMIIStatus, NVREG_MIISTAT_LINKCHANGE);

    if (MiiStatus & NVREG_MIISTAT_LINKCHANGE)
    {
        Connected = NvNetUpdateLinkSpeed(Adapter);
        if (Adapter->Connected != Connected)
        {
            Adapter->Connected = Connected;
            Report = TRUE;

            if (Connected)
            {
                /* Link up */
                NvNetToggleClockPowerGating(Adapter, FALSE);
                NvNetStartReceiver(Adapter);
            }
            else
            {
                /* Link down */
                NvNetToggleClockPowerGating(Adapter, TRUE);
                NvNetStopReceiver(Adapter);
            }
        }
    }

    NdisDprReleaseSpinLock(&Adapter->Lock);

    if (Report)
    {
        NdisMIndicateStatus(Adapter->AdapterHandle,
                            Connected ? NDIS_STATUS_MEDIA_CONNECT : NDIS_STATUS_MEDIA_DISCONNECT,
                            NULL,
                            0);
        NdisMIndicateStatusComplete(Adapter->AdapterHandle);
    }
}

static
VOID
HandleRecoverableError(
    _In_ PNVNET_ADAPTER Adapter)
{
    NDIS_DbgPrint(MIN_TRACE, ("()\n"));

    NdisDprAcquireSpinLock(&Adapter->Lock);

    NvNetStopReceiver(Adapter);
    NvNetStopTransmitter(Adapter);
    NvNetResetMac(Adapter);
    NvNetResetReceiverAndTransmitter(Adapter);

    /* TODO */

    NV_WRITE(Adapter, NvRegOffloadConfig, (NVNET_MAXIMUM_FRAME_SIZE - sizeof(ETH_HEADER))
                                          + NV_RX_HEADERS);

    NV_WRITE(Adapter, NvRegRxRingPhysAddr,
             NdisGetPhysicalAddressLow(Adapter->ReceiveDescriptorsPa));
    if (Adapter->Flags & DEV_HAS_HIGH_DMA)
    {
        NV_WRITE(Adapter, NvRegRxRingPhysAddrHigh,
                 NdisGetPhysicalAddressHigh(Adapter->ReceiveDescriptorsPa));
    }

    NV_WRITE(Adapter, NvRegTxRingPhysAddr,
             NdisGetPhysicalAddressLow(Adapter->TransmitDescriptorsPa));
    if (Adapter->Flags & DEV_HAS_HIGH_DMA)
    {
        NV_WRITE(Adapter, NvRegTxRingPhysAddrHigh,
                 NdisGetPhysicalAddressHigh(Adapter->TransmitDescriptorsPa));
    }

    NV_WRITE(Adapter, NvRegRingSizes,
             (NVNET_RECEIVE_DESCRIPTORS - 1) << NVREG_RINGSZ_RXSHIFT |
             (NVNET_TRANSMIT_DESCRIPTORS - 1) << NVREG_RINGSZ_TXSHIFT);

    NV_WRITE(Adapter, NvRegTxRxControl, Adapter->TxRxControl | NVREG_TXRXCTL_KICK);

    NV_WRITE(Adapter, NvRegIrqStatus, NVREG_IRQSTAT_MASK);

    NvNetStartReceiver(Adapter);
    NvNetStartTransmitter(Adapter);

    NdisDprReleaseSpinLock(&Adapter->Lock);
}

VOID
NTAPI
MiniportHandleInterrupt(
    _In_ NDIS_HANDLE MiniportAdapterContext)
{
    PNVNET_ADAPTER Adapter = (PNVNET_ADAPTER)MiniportAdapterContext;
    ULONG InterruptStatus = Adapter->InterruptStatus;
    ULONG RxProcessed, TotalTxProcessed = 0, TotalRxProcessed = 0;
    LIST_ENTRY SendReadyList;

    NDIS_DbgPrint(MIN_TRACE, ("() Events 0x%lX\n", InterruptStatus));

    InitializeListHead(&SendReadyList);

    /* Process the rings and measure network activity */
    do
    {
        NdisDprAcquireSpinLock(&Adapter->Lock);

        TotalTxProcessed += ProcessTransmitDescriptors(Adapter, &SendReadyList);

        NdisDprReleaseSpinLock(&Adapter->Lock);

        while (!IsListEmpty(&SendReadyList))
        {
            PLIST_ENTRY Entry = RemoveHeadList(&SendReadyList);

            NdisMSendComplete(Adapter->AdapterHandle,
                              CONTAINING_RECORD(Entry, NDIS_PACKET, MiniportReserved),
                              NDIS_STATUS_SUCCESS);
        }

        RxProcessed = ProcessReceiveDescriptors(Adapter, TotalRxProcessed);
        TotalRxProcessed += RxProcessed;
    }
    while (RxProcessed > 0 && TotalRxProcessed < NVNET_RECEIVE_PROCESSING_LIMIT);

    NDIS_DbgPrint(MIN_TRACE, ("Total TX %d RX %d\n", TotalTxProcessed, TotalRxProcessed));

    /* Moderate the interrupts */
    if (Adapter->OptimizationMode == NV_OPTIMIZATION_MODE_DYNAMIC)
    {
        ChangeInterruptMode(Adapter, TotalTxProcessed + TotalRxProcessed);
    }

    if (InterruptStatus & NVREG_IRQ_RX_NOBUF)
    {
        ++Adapter->Statistics.ReceiveIrqNoBuffers;
    }

    if (InterruptStatus & NVREG_IRQ_LINK)
    {
        HandleLinkStateChange(Adapter);
    }

    if (InterruptStatus & NVREG_IRQ_RECOVER_ERROR)
    {
        HandleRecoverableError(Adapter);
    }

    /* Enable interrupts on the NIC */
    NvNetApplyInterruptMask(Adapter);
}

VOID
NTAPI
MiniportISR(
  _Out_ PBOOLEAN InterruptRecognized,
  _Out_ PBOOLEAN QueueMiniportHandleInterrupt,
  _In_ NDIS_HANDLE MiniportAdapterContext)
{
    PNVNET_ADAPTER Adapter = (PNVNET_ADAPTER)MiniportAdapterContext;
    ULONG InterruptStatus;

    NDIS_DbgPrint(MIN_TRACE, ("()\n"));

    InterruptStatus = NV_READ(Adapter, NvRegIrqStatus);

    /* Clear any interrupt events */
    NV_WRITE(Adapter, NvRegIrqStatus, InterruptStatus);

    if (InterruptStatus & Adapter->InterruptMask)
    {
        /* Disable further interrupts */
        NvNetDisableInterrupts(Adapter);

        Adapter->InterruptStatus = InterruptStatus;

        *InterruptRecognized = TRUE;
        *QueueMiniportHandleInterrupt = TRUE;
    }
    else
    {
        /* This interrupt is not ours */
        *InterruptRecognized = FALSE;
        *QueueMiniportHandleInterrupt = FALSE;
    }
}

CODE_SEG("PAGE")
VOID
NvNetInitializeInterruptHandlers(
    _Inout_ PNVNET_ADAPTER Adapter)
{

}
