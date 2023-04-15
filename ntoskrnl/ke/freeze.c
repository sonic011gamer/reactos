/*
 * PROJECT:         ReactOS Kernel
 * LICENSE:         GPL - See COPYING in the top level directory
 * FILE:            ntoskrnl/ke/freeze.c
 * PURPOSE:         Routines for freezing and unfreezing processors for
 *                  kernel debugger synchronization.
 * PROGRAMMERS:     Alex Ionescu (alex.ionescu@reactos.org)
 */

/* INCLUDES *******************************************************************/

#include <ntoskrnl.h>
//#define NDEBUG
#include <debug.h>

#ifdef NDEBUG
#define KdpDprintf(...)
#endif

#define IPI_FROZEN_RUNNING 0
#define IPI_FROZEN_THAWING 3
#define IPI_FROZEN_HALTED 2

/* GLOBALS ********************************************************************/

/* Freeze data */
KIRQL KiOldIrql;
ULONG KiFreezeFlag;

VOID
NTAPI
KiFreezeTargetExecution(_In_ PKTRAP_FRAME TrapFrame,
                        _In_ PKEXCEPTION_FRAME ExceptionFrame)
{
    _disable();
    KiSaveProcessorState(TrapFrame, NULL);
 //   KdpDprintf("CPU %d entering FROZEN state\n", KeGetCurrentProcessorNumber());
    PKPRCB Prcb;
    ULONG IpiFreezeState;
    Prcb = KeGetCurrentPrcb();

    Prcb->IpiFrozen = IPI_FROZEN_HALTED;
    IpiFreezeState = Prcb->IpiFrozen;
    while (IpiFreezeState != IPI_FROZEN_THAWING)
    {
        KeStallExecutionProcessor(100);
        IpiFreezeState = Prcb->IpiFrozen;
    }
  //  KdpDprintf("CPU %d returning to RUNNING state\n", KeGetCurrentProcessorNumber());
    Prcb->IpiFrozen = IPI_FROZEN_RUNNING;
    //InterlockedBitTestAndReset((PLONG)&Prcb->IpiFrozen, IPI_FROZEN_RUNNING);
    //KiRestoreProcessorState(TrapFrame, NULL);
     _enable();
}

/* FUNCTIONS ******************************************************************/

BOOLEAN
NTAPI
KeFreezeExecution(IN PKTRAP_FRAME TrapFrame,
                  IN PKEXCEPTION_FRAME ExceptionFrame)
{
#ifdef CONFIG_SMP
    KAFFINITY TargetAffinity;
   // KAFFINITY Current;
    PKPRCB Prcb = KeGetCurrentPrcb();
#endif
    BOOLEAN Enable;
    KIRQL OldIrql;

#ifndef CONFIG_SMP
    UNREFERENCED_PARAMETER(TrapFrame);
    UNREFERENCED_PARAMETER(ExceptionFrame);
#endif

    /* Disable interrupts, get previous state and set the freeze flag */
    Enable = KeDisableInterrupts();
    KiFreezeFlag = 4;

#ifndef CONFIG_SMP
    /* Raise IRQL if we have to */
    OldIrql = KeGetCurrentIrql();
    if (OldIrql < DISPATCH_LEVEL)
        OldIrql = KeRaiseIrqlToDpcLevel();
#else
    /* Raise IRQL to HIGH_LEVEL */
    KeRaiseIrql(HIGH_LEVEL, &OldIrql);
#endif

#ifdef CONFIG_SMP
    //KdpDprintf("Freezing CPUs\n");
    TargetAffinity = KeActiveProcessors;
    TargetAffinity &= ~Prcb->SetMember;

        /* Make sure this is MP */
    if (TargetAffinity)
    {
        //KdpDprintf("Shooting out IPIs\n");
        KiIpiSend(TargetAffinity, IPI_FREEZE);
        //KdpDprintf("Success\n");
    }
#endif

    /* Save the old IRQL to be restored on unfreeze */
    KiOldIrql = OldIrql;

    /* Return whether interrupts were enabled */
    return Enable;
}

VOID
NTAPI
KeThawExecution(IN BOOLEAN Enable)
{
#ifdef CONFIG_SMP
    LONG i;
    PKPRCB TargetPrcb;
    KAFFINITY Current;
    KAFFINITY TargetAffinity;

    PKPRCB Prcb = KeGetCurrentPrcb();
    TargetAffinity = KeActiveProcessors;
    TargetAffinity &= ~Prcb->SetMember;

    for (i = 0, Current = 1; i < KeNumberProcessors; i++, Current <<= 1)
    {
        if (TargetAffinity & Current)
        {
            //KdpDprintf("Unfreezing Processor %d\n", i);
            TargetPrcb = KiProcessorBlock[i];
            TargetPrcb->IpiFrozen = IPI_FROZEN_THAWING;
            while (Prcb->IpiFrozen != IPI_FROZEN_RUNNING)
            {
                KeStallExecutionProcessor(100);
               // KdpDprintf("Waiting for processor: %d\n", i);
            }

           // KdpDprintf("thawed processor: %d\n", i);
        }
    }
#endif

    /* Clear the freeze flag */
    KiFreezeFlag = 0;

    /* Cleanup CPU caches */
    KeFlushCurrentTb();

    /* Restore the old IRQL */
#ifndef CONFIG_SMP
    if (KiOldIrql < DISPATCH_LEVEL)
#endif
    KeLowerIrql(KiOldIrql);

    /* Re-enable interrupts */
    KeRestoreInterrupts(Enable);
}
