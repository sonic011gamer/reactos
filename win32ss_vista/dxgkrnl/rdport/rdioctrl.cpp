/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     RDDM Miniport IOCTRL Handlers
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <dxgkrnl.h>
//#define NDEBUG
#include <debug.h>

PDXGKRNL_PRIVATE_EXTENSION Extension;

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
RdPort_DispatchCreateDevice(_In_    PDEVICE_OBJECT DeviceObject,
                            _Inout_ PIRP Irp)
{
    UNIMPLEMENTED;
    //__debugbreak();
    return 0;
}

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
RdPort_DispatchPnp(_In_ PDEVICE_OBJECT DeviceObject,
                   _In_ PVOID Tag)
{
    UNIMPLEMENTED;
    //__debugbreak();
    return 0;
}

/*
 * @ UNIMPLEMENTED
 */
PSTR
NTAPI
RdPort_DispatchPower(_In_ PDEVICE_OBJECT DeviceObject,
                     _In_ PSTR MutableMessage)
{
    UNIMPLEMENTED;
    //__debugbreak();
    return MutableMessage;
}

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
RdPort_DispatchIoctl(_In_    PDEVICE_OBJECT DeviceObject,
                     _Inout_ IRP *Irp)
{
    UNIMPLEMENTED;
    //__debugbreak();
    return 0;
}

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
RdPort_DispatchInternalIoctl(_In_ PDEVICE_OBJECT DeviceObject,
                                     _Inout_ IRP *Irp)
{
    UNIMPLEMENTED;
    //__debugbreak();
    return 0;
}

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
RdPort_DispatchSystemControl(_In_ PDEVICE_OBJECT DeviceObject,
                                     _In_ PVOID Tag)
{
    UNIMPLEMENTED;
    //__debugbreak();
    return 0;
}

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
RdPort_DispatchCloseDevice(_In_ PDEVICE_OBJECT DeviceObject)
{
    UNIMPLEMENTED;
    //__debugbreak();
    return 0;
}

#define TAG_VIDEO_PORT          'PDIV'
 BOOLEAN       VideoPortUseNewKey = TRUE;
NTSTATUS
NTAPI
IntCreateRegistryPath(
    IN PCUNICODE_STRING DriverRegistryPath,
    IN ULONG DeviceNumber,
    OUT PUNICODE_STRING DeviceRegistryPath)
{
    static WCHAR RegistryMachineSystem[] = L"\\REGISTRY\\MACHINE\\SYSTEM\\";
    static WCHAR CurrentControlSet[] = L"CURRENTCONTROLSET\\";
    static WCHAR ControlSet[] = L"CONTROLSET";
    static WCHAR Insert1[] = L"Hardware Profiles\\Current\\System\\CurrentControlSet\\";
    static WCHAR Insert2[] = L"\\Device";
    UNICODE_STRING DeviceNumberString;
    WCHAR DeviceNumberBuffer[20];
    BOOLEAN Valid;
    UNICODE_STRING AfterControlSet;
    NTSTATUS Status;

    AfterControlSet = *DriverRegistryPath;

    /* Convert DeviceNumber to string */
    DeviceNumberString.Length = 0;
    DeviceNumberString.MaximumLength = sizeof(DeviceNumberBuffer);
    DeviceNumberString.Buffer = DeviceNumberBuffer;
    Status = RtlIntegerToUnicodeString(DeviceNumber, 10, &DeviceNumberString);
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("RtlIntegerToUnicodeString(%u) returned 0x%08x\n", DeviceNumber, Status);
        return Status;
    }

    /* Check if path begins with \\REGISTRY\\MACHINE\\SYSTEM\\ */
    Valid = (DriverRegistryPath->Length > sizeof(RegistryMachineSystem) &&
             0 == _wcsnicmp(DriverRegistryPath->Buffer, RegistryMachineSystem,
                            wcslen(RegistryMachineSystem)));
    if (Valid)
    {
        AfterControlSet.Buffer += wcslen(RegistryMachineSystem);
        AfterControlSet.Length -= sizeof(RegistryMachineSystem) - sizeof(UNICODE_NULL);

        /* Check if path contains CURRENTCONTROLSET */
        if (AfterControlSet.Length > sizeof(CurrentControlSet) &&
            0 == _wcsnicmp(AfterControlSet.Buffer, CurrentControlSet, wcslen(CurrentControlSet)))
        {
            AfterControlSet.Buffer += wcslen(CurrentControlSet);
            AfterControlSet.Length -= sizeof(CurrentControlSet) - sizeof(UNICODE_NULL);
        }
        /* Check if path contains CONTROLSETnum */
        else if (AfterControlSet.Length > sizeof(ControlSet) &&
                 0 == _wcsnicmp(AfterControlSet.Buffer, ControlSet, wcslen(ControlSet)))
        {
            AfterControlSet.Buffer += wcslen(ControlSet);
            AfterControlSet.Length -= sizeof(ControlSet) - sizeof(UNICODE_NULL);
            while (AfterControlSet.Length > 0 &&
                    *AfterControlSet.Buffer >= L'0' &&
                    *AfterControlSet.Buffer <= L'9')
            {
                AfterControlSet.Buffer++;
                AfterControlSet.Length -= sizeof(WCHAR);
            }

            Valid = (AfterControlSet.Length > 0 && L'\\' == *AfterControlSet.Buffer);
            AfterControlSet.Buffer++;
            AfterControlSet.Length -= sizeof(WCHAR);
            AfterControlSet.MaximumLength = AfterControlSet.Length;
        }
        else
        {
            Valid = FALSE;
        }
    }

    if (!VideoPortUseNewKey)
    {
        DPRINT1("Using old registry key as 'UseNewKey' is FALSE\n");
        Valid = FALSE;
    }
    else if (Valid)
    {
        DeviceRegistryPath->MaximumLength = DriverRegistryPath->Length + sizeof(Insert1) + sizeof(Insert2)
                                          + DeviceNumberString.Length;
        DeviceRegistryPath->Buffer = (PWCH)ExAllocatePoolWithTag(PagedPool,
                                                           DeviceRegistryPath->MaximumLength,
                                                           TAG_VIDEO_PORT);
        if (DeviceRegistryPath->Buffer != NULL)
        {
            /* Build device path */
            wcsncpy(DeviceRegistryPath->Buffer,
                    DriverRegistryPath->Buffer,
                    AfterControlSet.Buffer - DriverRegistryPath->Buffer);
            DeviceRegistryPath->Length = (AfterControlSet.Buffer - DriverRegistryPath->Buffer) * sizeof(WCHAR);
            RtlAppendUnicodeToString(DeviceRegistryPath, Insert1);
            RtlAppendUnicodeStringToString(DeviceRegistryPath, &AfterControlSet);
            RtlAppendUnicodeToString(DeviceRegistryPath, Insert2);
            RtlAppendUnicodeStringToString(DeviceRegistryPath, &DeviceNumberString);

            /* Check if registry key exists */
            Valid = NT_SUCCESS(RtlCheckRegistryKey(RTL_REGISTRY_ABSOLUTE, DeviceRegistryPath->Buffer));

            if (!Valid)
                ExFreePoolWithTag(DeviceRegistryPath->Buffer, TAG_VIDEO_PORT);
        }
        else
        {
            Valid = FALSE;
        }
    }
    else
    {
        DPRINT1("Unparsable registry path %wZ\n", DriverRegistryPath);
    }

    /* If path doesn't point to *ControlSet*, use DriverRegistryPath directly */
    if (!Valid)
    {
        DeviceRegistryPath->MaximumLength = DriverRegistryPath->Length + sizeof(Insert2) + DeviceNumberString.Length;
        DeviceRegistryPath->Buffer = (PWCH)ExAllocatePoolWithTag(NonPagedPool,
                                                           DeviceRegistryPath->MaximumLength,
                                                           TAG_VIDEO_PORT);

        if (!DeviceRegistryPath->Buffer)
            return STATUS_NO_MEMORY;

        RtlCopyUnicodeString(DeviceRegistryPath, DriverRegistryPath);
        RtlAppendUnicodeToString(DeviceRegistryPath, Insert2);
        RtlAppendUnicodeStringToString(DeviceRegistryPath, &DeviceNumberString);
    }

    DPRINT1("Formatted registry key '%wZ' -> '%wZ'\n",
           DriverRegistryPath, DeviceRegistryPath);

    return STATUS_SUCCESS;
}


static
NTSTATUS
IntVideoPortAddDeviceMapLink(
    PDXGKRNL_PRIVATE_EXTENSION DeviceExtension)
{
    PUNICODE_STRING RegistryPath;
    WCHAR DeviceBuffer[20];
    UNICODE_STRING DeviceName;
    WCHAR SymlinkBuffer[20];
    UNICODE_STRING SymlinkName;
    ULONG DeviceNumber;
    NTSTATUS Status;

    /* Create a unicode device name. */
    DeviceNumber = 0;
    swprintf(DeviceBuffer, L"\\Device\\Video%lu", 0);

    if (VideoPortUseNewKey)
        RegistryPath = &DeviceExtension->NewRegistryPath;
    else
        RegistryPath = &DeviceExtension->RegistryPath;

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

    /* Update MaxObjectNumber */
   // VideoPortMaxObjectNumber = DeviceNumber;

    return STATUS_SUCCESS;
}



/**
 * @brief Intercepts and calls the AddDevice Miniport call back
 *
 * @param DriverObject - Pointer to DRIVER_OBJECT structure
 *
 * @param PhysicalDeviceObject - Pointer to DEVICE_OBJECT structure
 *
 * @return NTSTATUS
 */
NTSTATUS
NTAPI
RdPortAddDevice(_In_    DRIVER_OBJECT *DriverObject,
                _Inout_ DEVICE_OBJECT *PhysicalDeviceObject)
{
    DPRINT1("RdPortAddDevice Entry\n");
    NTSTATUS Status;
    PDEVICE_OBJECT Fdo;
    IO_STATUS_BLOCK IoStatusBlock;
    WCHAR DeviceBuffer[20];
    UNICODE_STRING DeviceName;
    PAGED_CODE();

    Extension = NULL;
    ULONG_PTR Context = 0;
    /* MS does a whole bunch of bullcrap here so we will try to track it */
    if (!DriverObject || !PhysicalDeviceObject)
    {
        DPRINT1("RDDM_MiniportAddDevice: Something has seriously fucked up\n");
        return STATUS_INVALID_PARAMETER;
    }

    /*
     * This routine implemented the Miniport AddDevice along with trying enumerate what
     * the GPU is "attached to", On the off chance a GPU isn't enumerated over PCI we use ACPI here,
     * This routine also decides when to use MSI-(X) Interrupts; A Royal pain in the butt.
     *
     * 1) First lets get the device extension for DXGKRNL
     * 2) nextlet's call a private API to handle calling Add Device.
     * 3) After that we will call IoCreateDevice to create the device
     */

    /* Grab the DXGKRNL internal extension */
    Extension = (PDXGKRNL_PRIVATE_EXTENSION)IoGetDriverObjectExtension(DriverObject, DriverObject);
    if (!Extension)
    {
        DPRINT1("Could not gather DXGKRNL Extension\n");
    }

    /* Call the miniport Routine */
    Status = Extension->DriverInitData.DxgkDdiAddDevice(PhysicalDeviceObject, (PVOID*)&Context);
    if(Status != STATUS_SUCCESS)
    {
        DPRINT1("RDDM_MiniportAddDevice: AddDevice Miniport call failed with status %X\n", Status);
    }
    else{
        DPRINT1("RDDM_MiniportAddDevice: AddDevice Miniport call has continued with success\n");
    }

    swprintf(DeviceBuffer, L"\\Device\\Video%lu", 0);
    RtlInitUnicodeString(&DeviceName, DeviceBuffer);

    Extension->MiniportContext = (PVOID)Context;
    IoStatusBlock.Information = 1024;
    Status = IoCreateDevice(DriverObject,
                            IoStatusBlock.Information,
                            &DeviceName,
                            FILE_DEVICE_VIDEO,
                            FILE_DEVICE_SECURE_OPEN,
                            FALSE,
                            &Fdo);
   Status = IntCreateRegistryPath(&Extension->RegistryPath,
                                   0,
                                   &Extension->RegistryPath);
    Extension->MiniportFdo = Fdo;
    Extension->MiniportPdo = PhysicalDeviceObject;
    DPRINT1("RdPortAddDevice: Created Device\n");

#if 0
    Status = IntCreateNewRegistryPath(Extension);
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("IntCreateNewRegistryPath() failed with status 0x%08x\n", Status);
      //  goto Failure;
    }
#endif

    /* Set up the VIDEO/DEVICEMAP registry keys */
    Status = IntVideoPortAddDeviceMapLink(Extension);
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("IntVideoPortAddDeviceMapLink() failed with status 0x%08x\n", Status);

    }
    return 0;
}

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
RdPort_DriverUnload(_In_ PDRIVER_OBJECT DriverObject)
{
    UNIMPLEMENTED;
    //__debugbreak();
    return 0;
}
