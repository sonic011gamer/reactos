/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DXGKRNL Windows 8+ style initialization
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include <dxgkrnl.h>
#define NDEBUG
#include <debug.h>

NTSTATUS
NTAPI
RDDM_SetupDxgkrnlWin8(
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath)
{
    /* This will setup fucntion pointers to the VISTA/7 interface APIs + Windows 8+ */
    __debugbreak();
    return STATUS_SUCCESS;
}
