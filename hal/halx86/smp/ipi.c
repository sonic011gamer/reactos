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
