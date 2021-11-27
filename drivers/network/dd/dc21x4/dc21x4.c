/*
 * PROJECT:     ReactOS DC21x4 Driver
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Driver entrypoint
 * COPYRIGHT:   Copyright 2021 Dmitry Borisov <di.sean@protonmail.com>
 */

/* INCLUDES *******************************************************************/

#include "dc21x4.h"

#define NDEBUG
#include <debug.h>

/* GLOBALS ********************************************************************/

CODE_SEG("INIT")
DRIVER_INITIALIZE DriverEntry;

/* FUNCTIONS ******************************************************************/


CODE_SEG("INIT")
NTSTATUS
NTAPI
DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath)
{
    DbgPrint("#\n");
    DbgPrint("==== DC21X4 DRIVER ====\n");

    return NDIS_STATUS_FAILURE;
}
