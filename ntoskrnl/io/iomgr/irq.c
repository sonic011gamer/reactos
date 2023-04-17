/*
 * PROJECT:         ReactOS Kernel
 * LICENSE:         GPL - See COPYING in the top level directory
 * FILE:            ntoskrnl/io/iomgr/irq.c
 * PURPOSE:         I/O Wrappers (called Completion Ports) for Kernel Queues
 * PROGRAMMERS:     Alex Ionescu (alex.ionescu@reactos.org)
 */

/* INCLUDES *****************************************************************/

#include <ntoskrnl.h>
#define NDEBUG
#include <debug.h>

/* FUNCTIONS *****************************************************************/

/*
 * @implemented
 */
NTSTATUS
NTAPI
IoConnectInterrupt(OUT PKINTERRUPT *InterruptObject,
                   IN PKSERVICE_ROUTINE ServiceRoutine,
                   IN PVOID ServiceContext,
                   IN PKSPIN_LOCK SpinLock,
                   IN ULONG Vector,
                   IN KIRQL Irql,
                   IN KIRQL SynchronizeIrql,
                   IN KINTERRUPT_MODE InterruptMode,
                   IN BOOLEAN ShareVector,
                   IN KAFFINITY ProcessorEnableMask,
                   IN BOOLEAN FloatingSave)
{
    PKINTERRUPT Interrupt;
    PKINTERRUPT InterruptUsed;
    PIO_INTERRUPT IoInterrupt;
    PKSPIN_LOCK SpinLockUsed;
    BOOLEAN FirstRun;
    CCHAR Count = 0;
    KAFFINITY Affinity;
    PAGED_CODE();

    /* Assume failure */
    *InterruptObject = NULL;

    /* Get the affinity */
    Affinity = ProcessorEnableMask & KeActiveProcessors;
    while (Affinity)
    {
        /* Increase count */
        if (Affinity & 1) Count++;
        Affinity >>= 1;
    }

    /* Make sure we have a valid CPU count */
    if (!Count) return STATUS_INVALID_PARAMETER;

    /* Allocate the array of I/O Interrupts */
    IoInterrupt = ExAllocatePoolWithTag(NonPagedPool,
                                        (Count - 1) * sizeof(KINTERRUPT) +
                                        sizeof(IO_INTERRUPT),
                                        TAG_KINTERRUPT);
    if (!IoInterrupt) return STATUS_INSUFFICIENT_RESOURCES;

    /* Select which Spinlock to use */
    SpinLockUsed = SpinLock ? SpinLock : &IoInterrupt->SpinLock;

    /* We first start with a built-in Interrupt inside the I/O Structure */
    *InterruptObject = &IoInterrupt->FirstInterrupt;
    Interrupt = (PKINTERRUPT)(IoInterrupt + 1);
    FirstRun = TRUE;

    /* Start with a fresh structure */
    RtlZeroMemory(IoInterrupt, sizeof(IO_INTERRUPT));

    /* Now create all the interrupts */
    Affinity = ProcessorEnableMask & KeActiveProcessors;
    for (Count = 0; Affinity; Count++, Affinity >>= 1)
    {
        /* Check if it's enabled for this CPU */
        if (Affinity & 1)
        {
            /* Check which one we will use */
            InterruptUsed = FirstRun ? &IoInterrupt->FirstInterrupt : Interrupt;

            /* Initialize it */
            KeInitializeInterrupt(InterruptUsed,
                                  ServiceRoutine,
                                  ServiceContext,
                                  SpinLockUsed,
                                  Vector,
                                  Irql,
                                  SynchronizeIrql,
                                  InterruptMode,
                                  ShareVector,
                                  Count,
                                  FloatingSave);

            /* Connect it */
            if (!KeConnectInterrupt(InterruptUsed))
            {
                /* Check how far we got */
                if (FirstRun)
                {
                    /* We failed early so just free this */
                    ExFreePoolWithTag(IoInterrupt, TAG_KINTERRUPT);
                }
                else
                {
                    /* Far enough, so disconnect everything */
                    IoDisconnectInterrupt(&IoInterrupt->FirstInterrupt);
                }

                /* And fail */
                return STATUS_INVALID_PARAMETER;
            }

            /* Now we've used up our First Run */
            if (FirstRun)
            {
                FirstRun = FALSE;
            }
            else
            {
                /* Move on to the next one */
                IoInterrupt->Interrupt[(UCHAR)Count] = Interrupt++;
            }
        }
    }

    /* Return Success */
    return STATUS_SUCCESS;
}

/*
 * @implemented
 */
VOID
NTAPI
IoDisconnectInterrupt(PKINTERRUPT InterruptObject)
{
    LONG i;
    PIO_INTERRUPT IoInterrupt;
    PAGED_CODE();

    /* Get the I/O Interrupt */
    IoInterrupt = CONTAINING_RECORD(InterruptObject,
                                    IO_INTERRUPT,
                                    FirstInterrupt);

    /* Disconnect the first one */
    KeDisconnectInterrupt(&IoInterrupt->FirstInterrupt);

    /* Now disconnect the others */
    for (i = 0; i < KeNumberProcessors; i++)
    {
        /* Make sure one was registered */
        if (IoInterrupt->Interrupt[i])
        {
            /* Disconnect it */
            KeDisconnectInterrupt(&InterruptObject[i]);
        }
    }

    /* Free the I/O Interrupt */
    ExFreePool(IoInterrupt); // ExFreePoolWithTag(IoInterrupt, TAG_KINTERRUPT);
}

#if 0
    if (FxIsProcessorGroupSupported()) {
        connectParams.Version = CONNECT_FULLY_SPECIFIED_GROUP;
    }
    else {
        connectParams.Version = CONNECT_FULLY_SPECIFIED;
    }

    connectParams.FullySpecified.PhysicalDeviceObject = m_Device->GetPhysicalDevice();
    connectParams.FullySpecified.InterruptObject      = &m_Interrupt;
    connectParams.FullySpecified.ServiceRoutine       = _InterruptThunk;
    connectParams.FullySpecified.ServiceContext       = this;
    connectParams.FullySpecified.SpinLock             = m_SpinLock;
    connectParams.FullySpecified.FloatingSave         = m_FloatingSave;
    connectParams.FullySpecified.Vector               = m_InterruptInfo.Vector;
    connectParams.FullySpecified.Irql                 = m_InterruptInfo.Irql;
    connectParams.FullySpecified.ProcessorEnableMask  = m_InterruptInfo.TargetProcessorSet;
    connectParams.FullySpecified.Group                = m_InterruptInfo.Group;
    connectParams.FullySpecified.InterruptMode        = m_InterruptInfo.Mode;
    connectParams.FullySpecified.ShareVector          =
        m_InterruptInfo.ShareDisposition == CmResourceShareShared ? TRUE : FALSE;
    connectParams.FullySpecified.SynchronizeIrql      = m_SynchronizeIrql;
#endif
/* Vista+ */
NTSTATUS
NTAPI
IoConnectInterruptEx(
  _Inout_ PIO_CONNECT_INTERRUPT_PARAMETERS Parameters)
{
    DPRINT1("Calling IoConnectInterruptEx");
    __debugbreak();
    return 1;
}

VOID
NTAPI
IoDisconnectInterruptEx(
  _In_ PIO_DISCONNECT_INTERRUPT_PARAMETERS Parameters)
{
    DPRINT1("Calling IoDsiconnectInterruptEx");
    __debugbreak();
}
/* EOF */
