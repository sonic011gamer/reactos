/*
 * PROJECT:     ReactOS APFS driver
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Core APFS driver file
 * COPYRIGHT:   Copyright 2021 Justin Miller <justinmiller100@gmail.com>
 */

/* INCLUDES **************************************************************************************/

#include "apfs.h"
#define NDEBUG
#include <debug.h>

/* GLOBALS ***************************************************************************************/

/* FUNCTIONS *************************************************************************************/

/*
 * FUNCTION: Called by the system to initialize the driver
 * ARGUMENTS:
 *           DriverObject = object describing this driver
 *           RegistryPath = path to our configuration entries
 * RETURNS: Success or failure
 */
CODE_SEG("INIT")
NTSTATUS
NTAPI
DriverEntry(PDRIVER_OBJECT DriverObject,
            PUNICODE_STRING RegistryPath)
{
    DPRINT1("Start reactos APFS driver");
    return STATUS_SUCCESS;
}
