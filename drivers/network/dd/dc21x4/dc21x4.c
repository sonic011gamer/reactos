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

CODE_SEG("PAGE")
NDIS_STATUS
Dc21x4RecognizeHardware(
    _Inout_ PDC21X4_ADAPTER Adapter)
{
    ULONG Bytes;
    PCI_COMMON_CONFIG PciConfig;

    PAGED_CODE();

    Bytes = NdisReadPciSlotInformation(Adapter->AdapterHandle,
                                       0,
                                       FIELD_OFFSET(PCI_COMMON_CONFIG, VendorID),
                                       &PciConfig,
                                       PCI_COMMON_HDR_LENGTH);
    if (Bytes != PCI_COMMON_HDR_LENGTH)
        return NDIS_STATUS_ADAPTER_NOT_FOUND;

    if (PciConfig.VendorID != 0x1011)
        return NDIS_STATUS_ADAPTER_NOT_FOUND;

    Adapter->DeviceId = PciConfig.DeviceID;
    Adapter->RevisionId = PciConfig.RevisionID;

    switch (PciConfig.DeviceID)
    {
        case 0x0002:
            Adapter->ChipType = DC21040;
            break;

        case 0x0009:
            Adapter->ChipType = DC21140;
            break;

        case 0x0014:
            Adapter->ChipType = DC21041;
            break;

        case 0x0019:
        {
            if ((PciConfig.RevisionID & 0xF0) < 0x20)
                Adapter->ChipType = DC21142;
            else
                Adapter->ChipType = DC21143;

            Adapter->HasPmFeatures = ((PciConfig.RevisionID & 0xF0) > 0x30);
            break;
        }

        default:
            return NDIS_STATUS_NOT_RECOGNIZED;
    }

    return NDIS_STATUS_SUCCESS;
}

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
