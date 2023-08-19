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
#define NDEBUG
#include <debug.h>

#ifdef NDEBUG
#define KdpDprintf(...)
#endif


/* GLOBALS ********************************************************************/

/* Freeze data */
KIRQL KiOldIrql;
ULONG KiFreezeFlag;

/* PRIVATE FUNCTIONS ***********************************************************/

VOID
NTAPI
KiFreezeTargetExecution(_In_ PKTRAP_FRAME TrapFrame,
                        _In_ PKEXCEPTION_FRAME ExceptionFrame)
{
    EXCEPTION_RECORD ExceptionRecord;
    PKPRCB Prcb;
    Prcb = KeGetCurrentPrcb();

    if (TrapFrame)
        KiSaveProcessorState(TrapFrame, ExceptionFrame);
    /* Multiple processors can write this value */
    InterlockedExchange((LONG*)&Prcb->IpiFrozen, IPI_FROZEN_HALTED);

    /* Wait for triggering AP to give the go ahead to thaw */
    while ((Prcb->IpiFrozen & 0xF) == IPI_FROZEN_HALTED)
    {
        YieldProcessor();
        /* We only continue on if we are thawing, otherwise something else has happened! */
        if ((Prcb->IpiFrozen & 0x20) != 0 )
        {
            RtlZeroMemory(&ExceptionRecord, sizeof(EXCEPTION_RECORD));
            ExceptionRecord.ExceptionAddress = (PVOID)Prcb->ProcessorState.ContextFrame.Eip;
            KdpSwitchProcessor(&ExceptionRecord,
                               (PCONTEXT)&Prcb->ProcessorState, FALSE);
        }
    }

    if (TrapFrame)
        KiRestoreProcessorControlState(&Prcb->ProcessorState);
    KeFlushCurrentTb();
    /* Notify AP we're running once again */
    InterlockedExchange((LONG*)&Prcb->IpiFrozen, IPI_FROZEN_RUNNING);
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
    PKPRCB Prcb;
    LONG i;
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
    Prcb = KeGetCurrentPrcb();
    TargetAffinity = KeActiveProcessors;
    TargetAffinity &= ~Prcb->SetMember;
    if (TargetAffinity)
    {
        for (i = 0, Current = 1; i < KeNumberProcessors; i++, Current <<= 1)
        {
            if (TargetAffinity & Current)
            {
                /* stop target processor */
                KiIpiSend(Current, IPI_FREEZE);
                TargetPrcb = KiProcessorBlock[i];

                /* Await for this processor to be frozen*/
                while (TargetPrcb->IpiFrozen != IPI_FROZEN_HALTED)
                {
                    YieldProcessor();
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
    KAFFINITY TargetAffinity;
    PKPRCB TargetPrcb;
    KAFFINITY Current;
    PKPRCB Prcb;
    LONG i;

    Prcb = KeGetCurrentPrcb();
    TargetAffinity = KeActiveProcessors;
    TargetAffinity &= ~Prcb->SetMember;

    /* Loop through every processor */
    for (i = 0, Current = 1; i < KeNumberProcessors; i++, Current <<= 1)
    {
        if (TargetAffinity & Current)
        {
            TargetPrcb = KiProcessorBlock[i];

            /* Multiple processors can write this value */
            InterlockedExchange((LONG*)&TargetPrcb->IpiFrozen, IPI_FROZEN_THAWING);
            while (Prcb->IpiFrozen != IPI_FROZEN_RUNNING)
            {
                YieldProcessor();
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
