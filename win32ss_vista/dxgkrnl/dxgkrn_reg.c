/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DXGKRNL Registry managment
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */


#include <dxgkrnl.h>
//#define NDEBUG
#include <debug.h>


/*
 * You might have noticed this is taking from videoprt, You're right :D
 */
NTSTATUS
RDDM_AddDeviceMapLink()
{
    PUNICODE_STRING RegistryPath;
    WCHAR DeviceBuffer[20];
    UNICODE_STRING DeviceName;
    WCHAR SymlinkBuffer[20];
    UNICODE_STRING SymlinkName;
    ULONG DeviceNumber;
    NTSTATUS Status;

    /* Create a unicode device name. */
    DeviceNumber = 0;//TODO: Obtain videoprt device, make Parameter
    swprintf(DeviceBuffer, L"\\Device\\Video%lu", DeviceNumber);

        RegistryPath = NULL; //TODO: obtain registry key, make parameter

    /* Add entry to DEVICEMAP\VIDEO key in registry. */
    Status = RtlWriteRegistryValue(RTL_REGISTRY_DEVICEMAP,
                                   L"VIDEO",
                                   DeviceBuffer,
                                   REG_SZ,
                                   RegistryPath->Buffer,
                                   RegistryPath->Length + sizeof(UNICODE_NULL));
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("Failed to create DEVICEMAP registry entry: 0x%X\n", Status);
        return Status;
    }

    Status = RtlWriteRegistryValue(RTL_REGISTRY_DEVICEMAP,
                                   L"VIDEO",
                                   L"MaxObjectNumber",
                                   REG_DWORD,
                                   &DeviceNumber,
                                   sizeof(DeviceNumber));
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("Failed to write MaxObjectNumber: 0x%X\n", Status);
        return Status;
    }

    /* Create symbolic link "\??\DISPLAYx" */
    swprintf(SymlinkBuffer, L"\\??\\DISPLAY%lu", DeviceNumber + 1);
    RtlInitUnicodeString(&SymlinkName, SymlinkBuffer);
    RtlInitUnicodeString(&DeviceName, DeviceBuffer);
    Status = IoCreateSymbolicLink(&SymlinkName, &DeviceName);
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("Failed to create symbolic link: 0x%X\n", Status);
        return Status;
    }

    return STATUS_SUCCESS;
}