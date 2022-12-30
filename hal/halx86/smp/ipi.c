/*
 * PROJECT:     ReactOS Kernel
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Source file for IPI management
 * COPYRIGHT:   Copyright 2021 Justin Miller <justinmiller100@gmail.com>
 */

/* INCLUDES ******************************************************************/

#include <hal.h>
#include <smp.h>
#define NDEBUG
#include <debug.h>

/* GLOBALS *******************************************************************/
    #define APIC_IPI_VECTOR      0xE1 // IRQL 14 (IPI_LEVEL) - KiIpiInterrupt

VOID
NTAPI
HalRequestIpi(KAFFINITY TargetProcessors)
{
    HalpRequestIpi(TargetProcessors);
}

VOID
FASTCALL
HalpIpiInterruptHandler(IN PKTRAP_FRAME TrapFrame)
{
    DPRINT1("FUCL");
       KIRQL oldIrql;

   HalBeginSystemInterrupt(IPI_LEVEL,
                           APIC_IPI_VECTOR,
			   &oldIrql);
   _enable();
#if 1
   DPRINT1("(%s:%d) HalpIpiInterruptHandler on CPU%d, current irql is %d\n",
            __FILE__,__LINE__, KeGetCurrentProcessorNumber(), KeGetCurrentIrql());
#endif

   KiIpiServiceRoutine(NULL, NULL);

#if 1
   DPRINT1("(%s:%d) HalpIpiInterruptHandler on CPU%d done\n", __FILE__,__LINE__, KeGetCurrentProcessorNumber());
#endif

   _disable();
   HalEndSystemInterrupt(oldIrql, 0);
}