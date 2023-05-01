/*
 * PROJECT:     FreeLoader UEFI Support
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Hardware detection routines
 * COPYRIGHT:   Copyright 2022 Justin Miller <justinmiller100@gmail.com>
 */

/* INCLUDES ******************************************************************/

#include <uefildr.h>

#include <debug.h>
DBG_DEFAULT_CHANNEL(WARNING);

/* GLOBALS *******************************************************************/

extern EFI_SYSTEM_TABLE * GlobalSystemTable;
extern EFI_HANDLE GlobalImageHandle;
extern UCHAR PcBiosDiskCount;
extern EFI_MEMORY_DESCRIPTOR* EfiMemoryMap;
extern UINT32 FreeldrDescCount;

BOOLEAN AcpiPresent = FALSE;

/* FUNCTIONS *****************************************************************/

static
PRSDP_DESCRIPTOR
FindAcpiBios(VOID)
{
    UINTN i;
    RSDP_DESCRIPTOR* rsdp = NULL;
    EFI_GUID acpi2_guid = EFI_ACPI_20_TABLE_GUID;

    for (i = 0; i < GlobalSystemTable->NumberOfTableEntries; i++)
    {
        if (!memcmp(&GlobalSystemTable->ConfigurationTable[i].VendorGuid, &acpi2_guid, sizeof(acpi2_guid)))
        {
            rsdp = (RSDP_DESCRIPTOR*)GlobalSystemTable->ConfigurationTable[i].VendorTable;
            break;
        }
    }

    return rsdp;
}

VOID
DetectAcpiBios(PCONFIGURATION_COMPONENT_DATA SystemKey, ULONG *BusNumber)
{
    PCONFIGURATION_COMPONENT_DATA BiosKey;
    PCM_PARTIAL_RESOURCE_LIST PartialResourceList;
    PCM_PARTIAL_RESOURCE_DESCRIPTOR PartialDescriptor;
    PRSDP_DESCRIPTOR Rsdp;
    PACPI_BIOS_DATA AcpiBiosData;
    ULONG TableSize;

    Rsdp = FindAcpiBios();

    if (Rsdp)
    {
        /* Set up the flag in the loader block */
        AcpiPresent = TRUE;

        /* Calculate the table size */
        TableSize = FreeldrDescCount * sizeof(BIOS_MEMORY_MAP) +
            sizeof(ACPI_BIOS_DATA) - sizeof(BIOS_MEMORY_MAP);

        /* Set 'Configuration Data' value */
        PartialResourceList = FrLdrHeapAlloc(sizeof(CM_PARTIAL_RESOURCE_LIST) +
                                             TableSize, TAG_HW_RESOURCE_LIST);
        if (PartialResourceList == NULL)
        {
            ERR("Failed to allocate resource descriptor\n");
            return;
        }

        RtlZeroMemory(PartialResourceList, sizeof(CM_PARTIAL_RESOURCE_LIST) + TableSize);
        PartialResourceList->Version = 0;
        PartialResourceList->Revision = 0;
        PartialResourceList->Count = 1;

        PartialDescriptor = &PartialResourceList->PartialDescriptors[0];
        PartialDescriptor->Type = CmResourceTypeDeviceSpecific;
        PartialDescriptor->ShareDisposition = CmResourceShareUndetermined;
        PartialDescriptor->u.DeviceSpecificData.DataSize = TableSize;

        /* Fill the table */
        AcpiBiosData = (PACPI_BIOS_DATA)&PartialResourceList->PartialDescriptors[1];

        if (Rsdp->revision > 0)
        {
            TRACE("ACPI >1.0, using XSDT address\n");
            AcpiBiosData->RSDTAddress.QuadPart = Rsdp->xsdt_physical_address;
        }
        else
        {
            TRACE("ACPI 1.0, using RSDT address\n");
            AcpiBiosData->RSDTAddress.LowPart = Rsdp->rsdt_physical_address;
        }

        AcpiBiosData->Count = FreeldrDescCount;
        memcpy(AcpiBiosData->MemoryMap, EfiMemoryMap,
            FreeldrDescCount * sizeof(BIOS_MEMORY_MAP));

        TRACE("RSDT %p, data size %x\n", Rsdp->rsdt_physical_address,
            TableSize);

        /* Create new bus key */
        FldrCreateComponentKey(SystemKey,
                               AdapterClass,
                               MultiFunctionAdapter,
                               0x0,
                               0x0,
                               0xFFFFFFFF,
                               "ACPI BIOS",
                               PartialResourceList,
                               sizeof(CM_PARTIAL_RESOURCE_LIST) + TableSize,
                               &BiosKey);

        /* Increment bus number */
        (*BusNumber)++;
    }
}

PCHAR
GetHarddiskIdentifier(UCHAR DriveNumber);
PCONFIGURATION_COMPONENT_DATA ControllerKey;

static
PCM_PARTIAL_RESOURCE_LIST
XboxGetHarddiskConfigurationData(UCHAR DriveNumber, ULONG* pSize)
{
    PCM_PARTIAL_RESOURCE_LIST PartialResourceList;
    PCM_DISK_GEOMETRY_DEVICE_DATA DiskGeometry;
    //EXTENDED_GEOMETRY ExtGeometry;
    GEOMETRY Geometry;
    ULONG Size;

    //
    // Initialize returned size
    //
    *pSize = 0;

    /* Set 'Configuration Data' value */
    Size = sizeof(CM_PARTIAL_RESOURCE_LIST) +
           sizeof(CM_DISK_GEOMETRY_DEVICE_DATA);
    PartialResourceList = FrLdrHeapAlloc(Size, TAG_HW_RESOURCE_LIST);
    if (PartialResourceList == NULL)
    {
        ERR("Failed to allocate resource descriptor\n");
        return NULL;
    }

    RtlZeroMemory(PartialResourceList, Size);
    PartialResourceList->Version = 1;
    PartialResourceList->Revision = 1;
    PartialResourceList->Count = 1;
    PartialResourceList->PartialDescriptors[0].Type =
        CmResourceTypeDeviceSpecific;
//  PartialResourceList->PartialDescriptors[0].ShareDisposition =
//  PartialResourceList->PartialDescriptors[0].Flags =
    PartialResourceList->PartialDescriptors[0].u.DeviceSpecificData.DataSize =
        sizeof(CM_DISK_GEOMETRY_DEVICE_DATA);

    /* Get pointer to geometry data */
    DiskGeometry = (PVOID)(((ULONG_PTR)PartialResourceList) + sizeof(CM_PARTIAL_RESOURCE_LIST));

    /* Get the disk geometry */
    //ExtGeometry.Size = sizeof(EXTENDED_GEOMETRY);

    if (MachDiskGetDriveGeometry(DriveNumber, &Geometry))
    {
        DiskGeometry->BytesPerSector = Geometry.BytesPerSector;
        DiskGeometry->NumberOfCylinders = Geometry.Cylinders;
        DiskGeometry->SectorsPerTrack = Geometry.Sectors;
        DiskGeometry->NumberOfHeads = Geometry.Heads;
    }
    else
    {
        ERR("Reading disk geometry failed\n");
        FrLdrHeapFree(PartialResourceList, TAG_HW_RESOURCE_LIST);
        return NULL;
    }
    TRACE("Disk %x: %u Cylinders  %u Heads  %u Sectors  %u Bytes\n",
          DriveNumber,
          DiskGeometry->NumberOfCylinders,
          DiskGeometry->NumberOfHeads,
          DiskGeometry->SectorsPerTrack,
          DiskGeometry->BytesPerSector);

    //
    // Return configuration data
    //
    *pSize = Size;
    return PartialResourceList;
}

VOID
DetectBiosDisks(PCONFIGURATION_COMPONENT_DATA SystemKey,
                PCONFIGURATION_COMPONENT_DATA BusKey)
{
    PCONFIGURATION_COMPONENT_DATA DiskKey;
    PCM_PARTIAL_RESOURCE_LIST PartialResourceList;
    PCM_INT13_DRIVE_PARAMETER Int13Drives;
    GEOMETRY Geometry;
    UCHAR DiskCount, DriveNumber;
    USHORT i;
    ULONG Size;

    /* The pre-enumeration of the BIOS disks was already done in InitializeBootDevices() */
    DiskCount = PcBiosDiskCount;

    /* Allocate resource descriptor */
    Size = sizeof(CM_PARTIAL_RESOURCE_LIST) +
           sizeof(CM_INT13_DRIVE_PARAMETER) * DiskCount;
    PartialResourceList = FrLdrHeapAlloc(Size, TAG_HW_RESOURCE_LIST);
    if (PartialResourceList == NULL)
    {
        ERR("Failed to allocate resource descriptor\n");
        return;
    }

    /* Initialize resource descriptor */
    RtlZeroMemory(PartialResourceList, Size);
    PartialResourceList->Version = 1;
    PartialResourceList->Revision = 1;
    PartialResourceList->Count = 1;
    PartialResourceList->PartialDescriptors[0].Type = CmResourceTypeDeviceSpecific;
    PartialResourceList->PartialDescriptors[0].ShareDisposition = 0;
    PartialResourceList->PartialDescriptors[0].Flags = 0;
    PartialResourceList->PartialDescriptors[0].u.DeviceSpecificData.DataSize =
        sizeof(CM_INT13_DRIVE_PARAMETER) * DiskCount;

    /* Get harddisk Int13 geometry data */
    Int13Drives = (PVOID)(((ULONG_PTR)PartialResourceList) + sizeof(CM_PARTIAL_RESOURCE_LIST));
    for (i = 0; i < DiskCount; i++)
    {
        DriveNumber = 0x80 + i;

        if (MachDiskGetDriveGeometry(DriveNumber, &Geometry))
        {
            Int13Drives[i].DriveSelect = DriveNumber;
            Int13Drives[i].MaxCylinders = Geometry.Cylinders - 1;
            Int13Drives[i].SectorsPerTrack = (USHORT)Geometry.Sectors;
            Int13Drives[i].MaxHeads = (USHORT)Geometry.Heads - 1;
            Int13Drives[i].NumberDrives = DiskCount;

            TRACE("Disk %x: %u Cylinders  %u Heads  %u Sectors  %u Bytes\n",
                  DriveNumber,
                  Geometry.Cylinders - 1,
                  Geometry.Heads - 1,
                  Geometry.Sectors,
                  Geometry.BytesPerSector);
        }
    }

    /* Update the 'System' key's configuration data with BIOS INT13h information */
    FldrSetConfigurationData(SystemKey, PartialResourceList, Size);

    /* Create and fill subkey for each harddisk */
    for (i = 0; i < DiskCount; i++)
    {
        PCHAR Identifier;

        DriveNumber = 0x80 + i;

        /* Get disk values */
        PartialResourceList = XboxGetHarddiskConfigurationData(DriveNumber, &Size);
        Identifier = GetHarddiskIdentifier(DriveNumber);

        /* Create disk key */
        FldrCreateComponentKey(ControllerKey,
                               PeripheralClass,
                               DiskPeripheral,
                               Output | Input,
                               i,
                               0xFFFFFFFF,
                               Identifier,
                               PartialResourceList,
                               Size,
                               &DiskKey);
    }
}

static
VOID
DetectIsaBios(PCONFIGURATION_COMPONENT_DATA SystemKey, ULONG *BusNumber)
{
    PCM_PARTIAL_RESOURCE_LIST PartialResourceList;
    PCONFIGURATION_COMPONENT_DATA BusKey;
    ULONG Size;

    /* Set 'Configuration Data' value */
    Size = sizeof(CM_PARTIAL_RESOURCE_LIST) -
           sizeof(CM_PARTIAL_RESOURCE_DESCRIPTOR);
    PartialResourceList = FrLdrHeapAlloc(Size, TAG_HW_RESOURCE_LIST);
    if (PartialResourceList == NULL)
    {
        ERR("Failed to allocate resource descriptor\n");
        return;
    }

    /* Initialize resource descriptor */
    RtlZeroMemory(PartialResourceList, Size);
    PartialResourceList->Version = 1;
    PartialResourceList->Revision = 1;
    PartialResourceList->Count = 0;

    /* Create new bus key */
    FldrCreateComponentKey(SystemKey,
                           AdapterClass,
                           MultiFunctionAdapter,
                           0,
                           0,
                           0xFFFFFFFF,
                           "ISA",
                           PartialResourceList,
                           Size,
                           &BusKey);

    /* Increment bus number */
    (*BusNumber)++;

    /* Detect ISA/BIOS devices */
    DetectBiosDisks(SystemKey, BusKey);
    /* FIXME: Detect more ISA devices */
}

PCONFIGURATION_COMPONENT_DATA
UefiHwDetect(VOID)
{
    PCONFIGURATION_COMPONENT_DATA SystemKey;
    ULONG BusNumber = 0;

    TRACE("DetectHardware()\n");

    /* Create the 'System' key */
#if defined(_M_IX86) || defined(_M_AMD64) /* Taken from Windows 11 */
    FldrCreateSystemKey(&SystemKey, "AT/AT COMPATIBLE");
#elif defined(_M_IA64) /* Taken from Windows XP 64-bit*/
    FldrCreateSystemKey(&SystemKey, "Intel Itanium processor family");
#elif defined(_M_ARM) || defined(_M_ARM64) /* Taken from Windows RT/11 */
    FldrCreateSystemKey(&SystemKey, "ARM processor family");
#else
    #error Please define a system key for your architecture
#endif

    /* Detect ACPI */
    DetectAcpiBios(SystemKey, &BusNumber);
    DetectIsaBios(SystemKey, &BusNumber);

    TRACE("DetectHardware() Done\n");
    return SystemKey;
}
