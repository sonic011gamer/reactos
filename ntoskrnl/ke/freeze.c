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
    ULONG OldValue;
    PKPRCB Prcb;
    Prcb = KeGetCurrentPrcb();
    OldValue = _InterlockedExchange(&Prcb->IpiFrozen, IPI_FROZEN_HALTED);
    while (Prcb->IpiFrozen != IPI_FROZEN_THAWING)
    {
        KeStallExecutionProcessor(10);
    }
    Prcb->IpiFrozen = IPI_FROZEN_RUNNING;
}

/* FUNCTIONS ******************************************************************/

BOOLEAN
NTAPI
KeFreezeExecution(IN PKTRAP_FRAME TrapFrame,
                  IN PKEXCEPTION_FRAME ExceptionFrame)
{
#ifdef CONFIG_SMP
    KAFFINITY TargetAffinity;
    PKPRCB TargetPrcb;
    KAFFINITY Current;
    ULONG i;
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
    if (TargetAffinity)
    {
        for (i = 0, Current = 1; i < KeNumberProcessors; i++, Current <<= 1)
        {
            if (TargetAffinity & Current)
            {
                if (i != KeGetCurrentProcessorNumber())
                {
                    /* Incrementaly stop all processors */
                    KiIpiSend(Current, IPI_FREEZE);
                    TargetPrcb = KiProcessorBlock[i];
                    while (TargetPrcb->IpiFrozen != IPI_FROZEN_HALTED)
                    {
                        /* Do nothing, we're trying to synch */
                    }
                }
            }
        }
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
            TargetPrcb = KiProcessorBlock[i];
            _InterlockedExchange(&TargetPrcb->IpiFrozen, IPI_FROZEN_THAWING);
            while (Prcb->IpiFrozen != IPI_FROZEN_RUNNING)
            {
            }
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
