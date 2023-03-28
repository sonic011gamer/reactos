#include "drivertest.h"
#include <debug.h>

NTSTATUS DxgkInitialize(
  _In_ PDRIVER_OBJECT              DriverObject,
  _In_ PUNICODE_STRING             RegistryPath,
  _In_ PDRIVER_INITIALIZATION_DATA DriverInitializationData
);

NTSTATUS
DriverEntry(
    _In_  DRIVER_OBJECT*  pDriverObject,
    _In_  UNICODE_STRING* pRegistryPath)
{
    __debugbreak();
    //DRIVER_INITIALIZATION_DATA DriverInitializationData = {'\0'};

    if (!pDriverObject ||
        !pRegistryPath)
    {
        return STATUS_INVALID_PARAMETER;
    }


    NTSTATUS Status = 1;//DxgkInitialize(pDriverObject, pRegistryPath, &DriverInitializationData);
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("DriverEntry failed with Status: 0x%I64x", Status);
        return Status;
    }


    return Status;
}
