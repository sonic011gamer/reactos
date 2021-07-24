/*
 * PROJECT:     ReactOS nVidia nForce Ethernet Controller Driver
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Common header file
 * COPYRIGHT:   Copyright 2021 Dmitry Borisov <di.sean@protonmail.com>
 */

#ifndef _NVNET_PCH_
#define _NVNET_PCH_

#include <ndis.h>
#include <section_attribs.h>

#include "nic.h"
#include "phyreg.h"

#define NVNET_TAG 'teNn'

#if defined(SARCH_XBOX)
/* Reduce memory requirements on OG Xbox */
#define NVNET_TRANSMIT_DESCRIPTORS   24
#define NVNET_RECEIVE_DESCRIPTORS    24
#else
#define NVNET_TRANSMIT_DESCRIPTORS   512
#define NVNET_RECEIVE_DESCRIPTORS    512
#endif

#define NVNET_MAXIMUM_FRAME_SIZE        1514
#define NVNET_MAXIMUM_FRAME_SIZE_JUMBO  (9100 - NV_RX_HEADERS)
#define NVNET_MAXIMUM_VLAN_ID           0xFFF

#define NVNET_MULTICAST_LIST_SIZE    32

#define NVNET_RECEIVE_BUFFER_SIZE    2048

#define NVNET_PACKET_FILTERS ( \
    NDIS_PACKET_TYPE_DIRECTED | \
    NDIS_PACKET_TYPE_MULTICAST | \
    NDIS_PACKET_TYPE_BROADCAST | \
    NDIS_PACKET_TYPE_PROMISCUOUS | \
    NDIS_PACKET_TYPE_ALL_MULTICAST)

#define NVNET_RECEIVE_PROCESSING_LIMIT   64

#define NVNET_IM_THRESHOLD    4
#define NVNET_IM_MAX_IDLE     2048

#define NVNET_MEDIA_DETECTION_INTERVAL   5000

#define ETH_IS_LOCALLY_ADMINISTERED(Address) \
    (BOOLEAN)(((PUCHAR)(Address))[0] & ((UCHAR)0x02))

#define ETH_IS_EMPTY(Address) \
    (BOOLEAN)((((PUCHAR)(Address))[0] | ((PUCHAR)(Address))[1] | ((PUCHAR)(Address))[2] | \
               ((PUCHAR)(Address))[3] | ((PUCHAR)(Address))[5] | ((PUCHAR)(Address))[5]) == 0)

#define PACKET_ENTRY(Packet) ((PLIST_ENTRY)(&(Packet)->MiniportReserved[0]))

#include <pshpack1.h>
typedef struct _ETH_HEADER
{
    UCHAR Destination[ETH_LENGTH_OF_ADDRESS];
    UCHAR Source[ETH_LENGTH_OF_ADDRESS];
    USHORT PayloadType;
} ETH_HEADER, *PETH_HEADER;
#include <poppack.h>

typedef enum _NVNET_OPTIMIZATION_MODE
{
    NV_OPTIMIZATION_MODE_DYNAMIC = 0,
    NV_OPTIMIZATION_MODE_CPU,
    NV_OPTIMIZATION_MODE_THROUGHPUT
} NVNET_OPTIMIZATION_MODE;

typedef enum _NVNET_FLOW_CONTROL_MODE
{
    NV_FLOW_CONTROL_DISABLE = 0,
    NV_FLOW_CONTROL_AUTO,
    NV_FLOW_CONTROL_RX,
    NV_FLOW_CONTROL_TX,
    NV_FLOW_CONTROL_RX_TX
} NVNET_FLOW_CONTROL_MODE;

typedef struct _NVNET_STATISTICS
{
    ULONG64 HwTxCnt;
    ULONG64 HwTxZeroReXmt;
    ULONG64 HwTxOneReXmt;
    ULONG64 HwTxManyReXmt;
    ULONG64 HwTxLateCol;
    ULONG64 HwTxUnderflow;
    ULONG64 HwTxLossCarrier;
    ULONG64 HwTxExcessDef;
    ULONG64 HwTxRetryErr;
    ULONG64 HwRxFrameErr;
    ULONG64 HwRxExtraByte;
    ULONG64 HwRxLateCol;
    ULONG64 HwRxRunt;
    ULONG64 HwRxFrameTooLong;
    ULONG64 HwRxOverflow;
    ULONG64 HwRxFCSErr;
    ULONG64 HwRxFrameAlignErr;
    ULONG64 HwRxLenErr;
    ULONG64 HwTxDef;
    ULONG64 HwTxFrame;
    ULONG64 HwRxCnt;
    ULONG64 HwTxPause;
    ULONG64 HwRxPause;
    ULONG64 HwRxDropFrame;
    ULONG64 HwRxUnicast;
    ULONG64 HwRxMulticast;
    ULONG64 HwRxBroadcast;
    ULONG64 HwTxUnicast;
    ULONG64 HwTxMulticast;
    ULONG64 HwTxBroadcast;

    ULONG64 TransmitOk;
    ULONG64 ReceiveOk;
    ULONG64 TransmitErrors;
    ULONG64 ReceiveErrors;
    ULONG64 ReceiveNoBuffers;
    ULONG64 ReceiveDirected;
    ULONG64 ReceiveCrcErrors;
    ULONG64 ReceiveAlignmentErrors;
    ULONG64 TransmitDeferred;
    ULONG64 TransmitExcessiveCollisions;
    ULONG64 ReceiveOverrunErrors;
    ULONG64 TransmitUnderrunErrors;
    ULONG64 TransmitZeroRetry;
    ULONG64 TransmitOneRetry;
    ULONG64 TransmitLostCarrierSense;
    ULONG64 TransmitLateCollisions;

    ULONG ReceiveIrqNoBuffers;
} NVNET_STATISTICS, *PNVNET_STATISTICS;

typedef struct _NVNET_ADAPTER
{
    volatile PUCHAR IoBase;
    NDIS_PHYSICAL_ADDRESS IoAddress;
    ULONG IoLength;

    NDIS_HANDLE AdapterHandle;
    NDIS_HANDLE WrapperConfigurationHandle;
    USHORT DeviceId;
    UCHAR RevisionId;

    BOOLEAN InterruptShared;
    ULONG InterruptVector;
    ULONG InterruptLevel;
    ULONG InterruptFlags;
    NDIS_MINIPORT_INTERRUPT Interrupt;

    ULONG PacketFilter;

    ULONG OriginalMacAddress[2];
    UCHAR PermanentMacAddress[ETH_LENGTH_OF_ADDRESS];
    UCHAR CurrentMacAddress[ETH_LENGTH_OF_ADDRESS];

    _Field_range_(0, NVNET_MULTICAST_LIST_SIZE)
    ULONG MulticastListSize;
    struct
    {
        UCHAR MacAddress[ETH_LENGTH_OF_ADDRESS];
    } MulticastList[NVNET_MULTICAST_LIST_SIZE];

    NVNET_OPTIMIZATION_MODE OptimizationMode;
    NVNET_FLOW_CONTROL_MODE FlowControlMode;
    NVNET_STATISTICS Statistics;
    NDIS_MINIPORT_TIMER MediaDetectionTimer;
    NDIS_SPIN_LOCK Lock;
    ULONG MaximumFrameSize;
    ULONG ReceiveBufferSize;
    ULONG VlanId;
    ULONG Flags;
    ULONG PhyAddress;
    ULONG PhyModel;
    ULONG PhyRevision;
    ULONG PhyOui;
    ULONG TxRxControl;
    ULONG VlanControl;
    ULONG PauseFlags;
    ULONG LinkSpeed;
    ULONG InterruptMask;
    ULONG InterruptStatus;
    ULONG InterruptIdleCount;
    BOOLEAN LimitTx;
    BOOLEAN Connected;
    BOOLEAN MacInUse;
    BOOLEAN UseSoftwareMacAddress;
    BOOLEAN UnitSemaphoreAcquired;
    BOOLEAN GigabitPhy;
    BOOLEAN FullDuplex;
    BOOLEAN PrioritySupported;
    BOOLEAN VlanTaggingSupported;
    BOOLEAN ChecksumOffload;
    BOOLEAN ForceSpeedAndDuplex;
    BOOLEAN ForceFullDuplex;
    UCHAR UserLinkSpeed;

    // NDIS_SPIN_LOCK SendLock;
    LIST_ENTRY SendBusyList;
    PNVNET_DESCRIPTOR_32 TransmitDescriptors32;
    PNVNET_DESCRIPTOR_64 TransmitDescriptors64;
    NDIS_PHYSICAL_ADDRESS TransmitDescriptorsPa;
    ULONG LastTx;
    ULONG CurrentTx;
    LONG BusyTx;

    // NDIS_SPIN_LOCK ReceiveLock;
    PNVNET_DESCRIPTOR_32 ReceiveDescriptors32;
    PNVNET_DESCRIPTOR_64 ReceiveDescriptors64;
    NDIS_PHYSICAL_ADDRESS ReceiveDescriptorsPa;
    PUCHAR ReceiveBuffer;
    NDIS_PHYSICAL_ADDRESS ReceiveBufferPa;
    ULONG CurrentRx;
} NVNET_ADAPTER, *PNVNET_ADAPTER;

CODE_SEG("PAGE")
NDIS_STATUS
NvNetRecognizeHardware(
    _Inout_ PNVNET_ADAPTER Adapter);

CODE_SEG("PAGE")
NDIS_STATUS
NvNetGetPermanentMacAddress(
    _Inout_ PNVNET_ADAPTER Adapter,
    _Out_writes_bytes_all_(ETH_LENGTH_OF_ADDRESS) PUCHAR MacAddress);

CODE_SEG("PAGE")
VOID
NvNetSetupMacAddress(
    _In_ PNVNET_ADAPTER Adapter,
    _In_reads_bytes_(ETH_LENGTH_OF_ADDRESS) PUCHAR MacAddress);

CODE_SEG("PAGE")
NDIS_STATUS
NvNetInitNIC(
    _In_ PNVNET_ADAPTER Adapter);

CODE_SEG("PAGE")
NDIS_STATUS
NvNetFindPhyDevice(
    _In_ PNVNET_ADAPTER Adapter);

CODE_SEG("PAGE")
NDIS_STATUS
NvNetPhyInit(
    _In_ PNVNET_ADAPTER Adapter);

KSYNCHRONIZE_ROUTINE NvNetInitPhaseSynchronized;
NDIS_TIMER_FUNCTION NvNetMediaDetectionDpc;

BOOLEAN
MiiWrite(
    _In_ PNVNET_ADAPTER Adapter,
    _In_ ULONG PhyAddress,
    _In_ ULONG RegAddress,
    _In_ ULONG Data);

BOOLEAN
MiiRead(
    _In_ PNVNET_ADAPTER Adapter,
    _In_ ULONG PhyAddress,
    _In_ ULONG RegAddress,
    _Out_ PULONG Data);

BOOLEAN
NvNetUpdateLinkSpeed(
    _In_ PNVNET_ADAPTER Adapter);

VOID
NvNetResetMac(
    _In_ PNVNET_ADAPTER Adapter);

VOID
NvNetResetReceiverAndTransmitter(
    _In_ PNVNET_ADAPTER Adapter);

VOID
NvNetStartReceiver(
    _In_ PNVNET_ADAPTER Adapter);

VOID
NvNetStartTransmitter(
    _In_ PNVNET_ADAPTER Adapter);

VOID
NvNetStopReceiver(
    _In_ PNVNET_ADAPTER Adapter);

VOID
NvNetStopTransmitter(
    _In_ PNVNET_ADAPTER Adapter);

VOID
NvNetUpdatePauseFrame(
    _Inout_ PNVNET_ADAPTER Adapter,
    _In_ ULONG PauseFlags);

VOID
NvNetToggleClockPowerGating(
    _In_ PNVNET_ADAPTER Adapter,
    _In_ BOOLEAN Gate);

CODE_SEG("PAGE")
VOID
NvNetBackoffSetSlotTime(
    _In_ PNVNET_ADAPTER Adapter);

VOID
NvNetBackoffReseed(
    _In_ PNVNET_ADAPTER Adapter);

VOID
NvNetBackoffReseedEx(
    _In_ PNVNET_ADAPTER Adapter);

NDIS_STATUS
NTAPI
MiniportSend(
    _In_ NDIS_HANDLE MiniportAdapterContext,
    _In_ PNDIS_PACKET Packet,
    _In_ UINT Flags);

VOID
NTAPI
MiniportISR(
  _Out_ PBOOLEAN InterruptRecognized,
  _Out_ PBOOLEAN QueueMiniportHandleInterrupt,
  _In_ NDIS_HANDLE MiniportAdapterContext);

VOID
NTAPI
MiniportHandleInterrupt(
    _In_ NDIS_HANDLE MiniportAdapterContext);

NDIS_STATUS
NTAPI
MiniportQueryInformation(
  _In_ NDIS_HANDLE MiniportAdapterContext,
  _In_ NDIS_OID Oid,
  _In_ PVOID InformationBuffer,
  _In_ ULONG InformationBufferLength,
  _Out_ PULONG BytesWritten,
  _Out_ PULONG BytesNeeded);

NDIS_STATUS
NTAPI
MiniportSetInformation(
  _In_ NDIS_HANDLE MiniportAdapterContext,
  _In_ NDIS_OID Oid,
  _In_ PVOID InformationBuffer,
  _In_ ULONG InformationBufferLength,
  _Out_ PULONG BytesRead,
  _Out_ PULONG BytesNeeded);

FORCEINLINE
VOID
NV_WRITE(
    _In_ PNVNET_ADAPTER Adapter,
    _In_ NVNET_REGISTER Register,
    _In_ ULONG Value)
{
    NdisWriteRegisterUlong((PULONG)(Adapter->IoBase + Register), Value);
}

FORCEINLINE
ULONG
NV_READ(
    _In_ PNVNET_ADAPTER Adapter,
    _In_ NVNET_REGISTER Register)
{
    ULONG Value;

    NdisReadRegisterUlong((PULONG)(Adapter->IoBase + Register), &Value);
    return Value;
}

FORCEINLINE
VOID
NvNetDisableInterrupts(
    _In_ PNVNET_ADAPTER Adapter)
{
    NV_WRITE(Adapter, NvRegIrqMask, 0);
}

FORCEINLINE
VOID
NvNetApplyInterruptMask(
    _In_ PNVNET_ADAPTER Adapter)
{
    NV_WRITE(Adapter, NvRegIrqMask, Adapter->InterruptMask);
}

#endif /* _NVNET_PCH_ */
