/*
 * PROJECT:         ReactOS xHCI Driver
 * LICENSE:         GPLv2+ - See COPYING in the top level directory
 * PURPOSE:         Priate management functions of xHCI
 * COPYRIGHT:       Copyright 2021 Justin Miller <justinmiller100@gmail.com>
 */

#include "usbxhci.h"
//NDEBUG
#include <debug.h>
//NDEBUG_XHCI_TRACE
#include "dbg_xhci.h"

PXHCI_HC_RESOURCES HcResourcesVA;
PXHCI_TRB dequeue_pointer;
VOID
NTAPI
PXHCI_InitSlot(IN PXHCI_EXTENSION xhciExtension, ULONG PortID, ULONG SlotID)
{
    /* 4.3.3 of the Intel xHCI spec */
    PXHCI_OUTPUT_DEVICE_CONTEXT HcOutputDeviceContext;
    PXHCI_TRANSFER_RING HcTransferControlRing;
    PXHCI_INPUT_CONTEXT HcInputContext;
    PXHCI_SLOT_CONTEXT HcSlotContext;
    PXHCI_ENDPOINT_CONTEXT HcDefaultEndpoint;

    PXHCI_EXTENSION XhciExtension;
    PXHCI_EVENT_TRB eventTRB;
    PULONG OperationalRegs;
    ULONG CheckCompletion;
    ULONG_PTR TrDeqPtr;
    XHCI_TRB Trb;

    XhciExtension = (PXHCI_EXTENSION)xhciExtension;
    HcResourcesVA = XhciExtension -> HcResourcesVA;
    eventTRB = (PXHCI_EVENT_TRB)HcResourcesVA->EventRing.dequeue_pointer;

    OperationalRegs = xhciExtension->OperationalRegs;
    CheckCompletion = INVALID;

    HcOutputDeviceContext = ExAllocatePoolZero(NonPagedPool, sizeof(XHCI_OUTPUT_DEVICE_CONTEXT), 'TVED');
    HcTransferControlRing = ExAllocatePoolZero(NonPagedPool, sizeof(XHCI_TRANSFER_RING), 'TVED');
    HcDefaultEndpoint = ExAllocatePoolZero(NonPagedPool, sizeof(XHCI_ENDPOINT_CONTEXT), 'TVED');
    HcInputContext = ExAllocatePoolZero(NonPagedPool, sizeof(XHCI_INPUT_CONTEXT), 'TVED');
    HcSlotContext = ExAllocatePoolZero(NonPagedPool, sizeof(XHCI_SLOT_CONTEXT), 'TVED');

    RtlZeroMemory((PVOID)HcOutputDeviceContext, sizeof(XHCI_OUTPUT_DEVICE_CONTEXT));
    RtlZeroMemory((PVOID)HcTransferControlRing, sizeof(XHCI_TRANSFER_RING));
    RtlZeroMemory((PVOID)HcDefaultEndpoint, sizeof(XHCI_ENDPOINT_CONTEXT));
    RtlZeroMemory((PVOID)HcInputContext, sizeof(XHCI_INPUT_CONTEXT));
    RtlZeroMemory((PVOID)HcSlotContext, sizeof(XHCI_SLOT_CONTEXT));


    TrDeqPtr = (ULONG_PTR)HcTransferControlRing->firstSeg.XhciTrb;

    HcSlotContext->RouteString = 0;
    HcSlotContext->ParentPortNumber = PortID;
    HcSlotContext->ContextEntries = 1;
    HcSlotContext->ParentHubSlotID = SlotID;
    HcInputContext->InputContext.A0 = 1;
    HcInputContext->InputContext.A1 = 1;

    HcDefaultEndpoint->EPType = 4;
    HcDefaultEndpoint->MaxBurstSize = 0;
    HcDefaultEndpoint->TRDeqPtr = TrDeqPtr;
    HcDefaultEndpoint->DCS = 1;
    HcDefaultEndpoint->Interval = 0;
    HcDefaultEndpoint->MaxPStreams = 0;
    HcDefaultEndpoint->Mult = 0;
    HcDefaultEndpoint->CErr = 3;

    HcOutputDeviceContext->SlotContext = HcSlotContext;

    XHCI_Write64bitReg(OperationalRegs + XHCI_DCBAAP, (ULONG_PTR)HcOutputDeviceContext);

    Trb.CommandTRB.AddressDevice.InputContextPtrLow = (ULONG_PTR)HcInputContext->InputContext.RsvdZ1;
    Trb.CommandTRB.AddressDevice.InputContextPtrHigh = ((ULONG_PTR)HcInputContext->InputContext.RsvdZ1 + sizeof(XHCI_INPUT_CONTEXT));
    Trb.CommandTRB.AddressDevice.RsvdZ2 = 0;
    Trb.CommandTRB.AddressDevice.RsvdZ3 = 0;
    Trb.CommandTRB.AddressDevice.CycleBit = 0;
    Trb.CommandTRB.AddressDevice.RsvdZ4 = 0;
    Trb.CommandTRB.AddressDevice.TRBType = ADDRESS_DEVICE_COMMAND;

    XHCI_SendCommand(Trb,XhciExtension);

    while (!CheckCompletion)
    {
        SlotID = eventTRB->CommandCompletionTRB.SlotID;
        CheckCompletion = eventTRB->CommandCompletionTRB.CompletionCode;
        if(CheckCompletion == SUCCESS)
        {
            KeStallExecutionProcessor(10);
            break;
        }
    }

    __debugbreak();
}


VOID
NTAPI
PXHCI_AssignSlot(IN PXHCI_EXTENSION xhciExtension, ULONG PortID)
{
    /* 4.3.2 of the Intel xHCI spec */
    PXHCI_HC_RESOURCES HcResourcesVA;
    ULONG SlotID, CheckCompletion;
    PXHCI_EXTENSION XhciExtension;
    PXHCI_TRB dequeue_pointer;
    XHCI_EVENT_TRB eventTRB;
    XHCI_TRB Trb;

    SlotID = 0;
    CheckCompletion = INVALID;

    XhciExtension = (PXHCI_EXTENSION)xhciExtension;
    HcResourcesVA = XhciExtension -> HcResourcesVA;
    dequeue_pointer = HcResourcesVA-> EventRing.dequeue_pointer;
    eventTRB = (*dequeue_pointer).EventTRB;

    /* Send enable slot command properly */
    Trb.CommandTRB.SlotEnable.RsvdZ1 = 0;
    Trb.CommandTRB.SlotEnable.RsvdZ2 = 0;
    Trb.CommandTRB.SlotEnable.RsvdZ3 = 0;
    Trb.CommandTRB.SlotEnable.RsvdZ4 = 0;
    Trb.CommandTRB.SlotEnable.CycleBit = 0;
    Trb.CommandTRB.SlotEnable.RsvdZ5 = 0;
    Trb.CommandTRB.SlotEnable.SlotType = 0;
    Trb.CommandTRB.SlotEnable.TRBType = ENABLE_SLOT_COMMAND;
    XHCI_SendCommand(Trb,XhciExtension);

    /* Check for completion and grab the Slot ID */
    DPRINT1("PXHCI_AssignSlot: Assigning Slot.\n");
    while (!CheckCompletion)
    {
        SlotID = eventTRB.CommandCompletionTRB.SlotID;
        CheckCompletion = eventTRB.CommandCompletionTRB.CompletionCode;
        if(CheckCompletion == SUCCESS)
        {
            KeStallExecutionProcessor(10);
            break;
        }
    }

    DPRINT1("PXHCI_AssignSlot: The Slot ID assigned is %X\n", SlotID);
    PXHCI_InitSlot(xhciExtension, PortID, SlotID);
}


VOID
NTAPI
XHCI_PortStatusChange(_Inout_ PXHCI_EXTENSION XhciExtension, _In_ ULONG PortID)
{
    ULONG RegValue;
    PULONG OperationalRegs;

    BOOLEAN DeviceInsertedEvent = TRUE;
    OperationalRegs = XhciExtension->OperationalRegs;
    RegValue = READ_REGISTER_ULONG(OperationalRegs + (XHCI_PORTSC + (0x10 * (PortID - 1))));

    DPRINT1("The Value of is %X\n", (RegValue |= PS_CSC));

    if(DeviceInsertedEvent == TRUE)
    {
        /* Attached: */
        DPRINT1("PXHCI_PortStatusChange: USB device has been inserted from port: %X\n", PortID);
        PXHCI_AssignSlot(XhciExtension, PortID);
    }
    else
    {
        /* Detached:
         *    - CCS -> 0
         *    - CSC -> 1
         */
        DPRINT1("PXHCI_PortStatusChange: USB device has been removed from port: %X\n", PortID);
        /* Run de-escalation code */
        /*
         * -> Submit a disable slot command
         * -> clear transfer rings of all TDs associated with device post deattach
         */
    }
    __debugbreak();
}