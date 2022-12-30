/*
 * PROJECT:         ReactOS Kernel
 * LICENSE:         GPL - See COPYING in the top level directory
 * FILE:            ntoskrnl/ke/ipi.c
 * PURPOSE:         Inter-Processor Packet Interface
 * PROGRAMMERS:     Alex Ionescu (alex.ionescu@reactos.org)
 */

/* INCLUDES ******************************************************************/

#include <ntoskrnl.h>
#define NDEBUG
#include <debug.h>

/* GLOBALS *******************************************************************/

extern KSPIN_LOCK KiReverseStallIpiLock;

/* PRIVATE FUNCTIONS *********************************************************/

VOID
NTAPI
KiIpiGenericCallTarget(IN PKIPI_CONTEXT PacketContext,
                       IN PVOID BroadcastFunction,
                       IN PVOID Argument,
                       IN PVOID Count)
{
    /* AP Has reached the broadcaster, Let's increment */
    (UINT32)Count -= 1;

    /* Infinitely Loop until count has reached 0 */
    do
    {
        KeStallExecutionProcessor(10);
    } while(Count != 0);

    /* When Count is 0 it means we are all good to uniformaly call our work */
    void (*BroadcastFunctionGrab)(PVOID) = (PVOID)&BroadcastFunction;
    (*BroadcastFunctionGrab)(Argument);

    /* IPI has finished, continue with the signal */
    KiIpiSignalPacketDone(PacketContext);
}

VOID
FASTCALL
KiIpiSend(IN KAFFINITY TargetProcessors,
          IN ULONG IpiRequest)
{
#ifdef CONFIG_SMP
    LONG i;
    PKPRCB Prcb;
    KAFFINITY Current;

    for (i = 0, Current = 1; i < KeNumberProcessors; i++, Current <<= 1)
    {
        if (TargetProcessors & Current)
        {
            /* Get the PRCB for this CPU */
            Prcb = KiProcessorBlock[i];

            InterlockedBitTestAndSet((PLONG)&Prcb->IpiFrozen, IpiRequest);
        }
    }

    /* HalRequestIpi does its own mask check =*/
    HalRequestIpi(TargetProcessors);

    return;
#endif
}

VOID
NTAPI
KiIpiSendPacket(IN KAFFINITY TargetProcessors,
                IN PKIPI_WORKER WorkerFunction,
                IN PKIPI_BROADCAST_WORKER BroadcastFunction,
                IN ULONG_PTR Context,
                IN PULONG Count)
{
#ifdef CONFIG_SMP
   // DPRINT1("KiIpiSendPacket: Attempting IPI on KAFFINITY of %X\n", TargetProcessors);
    KAFFINITY Processor;
    LONG i;
    PKPRCB Prcb, CurrentPrcb;
    KIRQL oldIrql;

    ASSERT(KeGetCurrentIrql() == SYNCH_LEVEL);

    CurrentPrcb = KeGetCurrentPrcb();
    (void)InterlockedExchangeUL(&CurrentPrcb->TargetSet, TargetProcessors);
    (void)InterlockedExchangeUL(&CurrentPrcb->WorkerRoutine, (ULONG_PTR)WorkerFunction);
    (void)InterlockedExchangeUL(&CurrentPrcb->CurrentPacket[0], BroadcastFunction);
    (void)InterlockedExchangePointer(&CurrentPrcb->CurrentPacket[1], &Context);
    (void)InterlockedExchangeUL(&CurrentPrcb->CurrentPacket[2], Count);

    for (i = 0, Processor = 1; i < KeNumberProcessors; i++, Processor <<= 1)
    {
        if (TargetProcessors & Processor)
        {
            Prcb = KiProcessorBlock[i];

            while (0 != InterlockedCompareExchangeUL(&Prcb->SignalDone, (LONG)CurrentPrcb, 0));
            InterlockedBitTestAndSet((PLONG)&Prcb->IpiFrozen, IPI_SYNCH_REQUEST);
            if (Processor != CurrentPrcb->SetMember)
            {
               // DPRINT1("Count is %X\n", Count);
            }
        }
    }
#if 1
    if (TargetProcessors & CurrentPrcb->SetMember)
    {
        KeRaiseIrql(IPI_LEVEL, &oldIrql);
        KiIpiServiceRoutine(NULL, NULL);
        KeLowerIrql(oldIrql);
    }
#endif
    HalRequestIpi(TargetProcessors);
#endif
}

VOID
FASTCALL
KiIpiSignalPacketDone(IN PKIPI_CONTEXT PacketContext)
{
    /* We have to restore context */
    //TODO: Handle null.
    /* signal done? */
}

VOID
FASTCALL
KiIpiSignalPacketDoneAndStall(IN PKIPI_CONTEXT PacketContext,
                              IN volatile PULONG ReverseStall)
{
        /* We have to restore context */
    //TODO: Handle null.
    /* signal done? */
}

/* PUBLIC FUNCTIONS **********************************************************/

/*
 * @implemented
 */
BOOLEAN
NTAPI
KiIpiServiceRoutine(IN PKTRAP_FRAME TrapFrame,
                    IN PKEXCEPTION_FRAME ExceptionFrame)
{
#ifdef CONFIG_SMP
    PKPRCB Prcb;
    ASSERT(KeGetCurrentIrql() == IPI_LEVEL);

    Prcb = KeGetCurrentPrcb();

    if (InterlockedBitTestAndReset((PLONG)&Prcb->IpiFrozen, IPI_APC))
    {
        HalRequestSoftwareInterrupt(APC_LEVEL);
    }

    if (InterlockedBitTestAndReset((PLONG)&Prcb->IpiFrozen, IPI_DPC))
    {
        Prcb->DpcInterruptRequested = TRUE;
        HalRequestSoftwareInterrupt(DISPATCH_LEVEL);
    }

    if (InterlockedBitTestAndReset((PLONG)&Prcb->IpiFrozen, IPI_SYNCH_REQUEST))
    {
#if defined(_M_ARM) || defined(_M_AMD64)
        DbgBreakPoint();
#else
        (void)InterlockedDecrementUL(&Prcb->SignalDone->CurrentPacket[1]);
        if (InterlockedCompareExchangeUL(&Prcb->SignalDone->CurrentPacket[2], 0, 0))
        {
            while (0 != InterlockedCompareExchangeUL(&Prcb->SignalDone->CurrentPacket[1], 0, 0));
        }
        ((VOID (NTAPI*)(PVOID))(Prcb->SignalDone->WorkerRoutine))(Prcb->SignalDone->CurrentPacket[0]);
        InterlockedBitTestAndReset((PLONG)&Prcb->SignalDone->TargetSet, KeGetCurrentProcessorNumber());
        if (InterlockedCompareExchangeUL(&Prcb->SignalDone->CurrentPacket[2], 0, 0))
        {
            while (0 != InterlockedCompareExchangeUL(&Prcb->SignalDone->TargetSet, 0, 0));
        }
        (void)InterlockedExchangePointer((PVOID*)&Prcb->SignalDone, NULL);
#endif // _M_ARM
    }
#endif
   return TRUE;
}

/*
 * @implemented
 */
ULONG_PTR
NTAPI
KeIpiGenericCall(IN PKIPI_BROADCAST_WORKER Function,
                 IN ULONG_PTR Argument)
{
    ULONG_PTR Status;
    KIRQL OldIrql, OldIrql2;
#ifdef CONFIG_SMP
    KAFFINITY Affinity;
    ULONG Count;
    PKPRCB Prcb = KeGetCurrentPrcb();
#endif

    /* Raise to DPC level if required */
    OldIrql = KeGetCurrentIrql();
    if (OldIrql < DISPATCH_LEVEL) KeRaiseIrql(DISPATCH_LEVEL, &OldIrql);

#ifdef CONFIG_SMP
    /* Get current processor count and affinity */
    Count = KeNumberProcessors;
    Affinity = KeActiveProcessors;

    /* Exclude ourselves */
    Affinity &= ~Prcb->SetMember;
#endif

    /* Acquire the IPI lock */
    KeAcquireSpinLockAtDpcLevel(&KiReverseStallIpiLock);

#ifdef CONFIG_SMP
    /* Make sure this is MP */
    if (Affinity)
    {
        /* Send an IPI */
        KiIpiSendPacket(Affinity,
                        KiIpiGenericCallTarget,
                        Function,
                        Argument,
                        &Count);

        /* Spin until the other processors are ready */
        while (Count != 1)
        {
            YieldProcessor();
            KeMemoryBarrierWithoutFence();
        }
    }
#endif

    /* Raise to IPI level */
    KeRaiseIrql(IPI_LEVEL, &OldIrql2);

#ifdef CONFIG_SMP
    /* Let the other processors know it is time */
    Count = 0;
#endif

    /* Call the function */
    Status = Function(Argument);

#ifdef CONFIG_SMP
    /* If this is MP, wait for the other processors to finish */
    if (Affinity)
    {
        /* Sanity check */
        ASSERT(Prcb == KeGetCurrentPrcb());

        /* FIXME: TODO */
        ASSERTMSG("Not yet implemented\n", FALSE);
    }
#endif

    /* Release the lock */
    KeReleaseSpinLockFromDpcLevel(&KiReverseStallIpiLock);

    /* Lower IRQL back */
    KeLowerIrql(OldIrql);
    return Status;
}
