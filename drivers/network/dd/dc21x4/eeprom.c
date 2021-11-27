/*
 * PROJECT:     ReactOS DC21x4 Driver
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     EEPROM manipulation and parsing
 * COPYRIGHT:   Copyright 2021 Dmitry Borisov <di.sean@protonmail.com>
 */

/* INCLUDES *******************************************************************/

#include "dc21x4.h"

#define NDEBUG
#include <debug.h>

/* GLOBALS ********************************************************************/

/* Access to unaligned memory */
#define SRomRetrieveWordByOffset(P, O) ((P)[(O)] | (P)[(O) + 1] << 8)
#define SRomRetrieveWord(P) SRomRetrieveWordByOffset(P, 0)

#define SRomIsBlockExtended(Block) ((Block)[0] & 0x80)
#define SRomGetExtendedBlockLength(Block) (((Block)[0] & 0x7F) + 1)
#define SRomGetExtendedBlockType(Block) ((Block)[1])

/* FUNCTIONS ******************************************************************/

static
CODE_SEG("PAGE")
BOOLEAN
Dc21x4GetAdapterDeviceNumber(
    _In_ PDC21X4_ADAPTER Adapter,
    _Out_ PUSHORT DeviceNumber)
{
    PDEVICE_OBJECT Pdo;
    NTSTATUS Status;
    ULONG PropertyAddress, Dummy;

    PAGED_CODE();

    NdisMGetDeviceProperty(Adapter->AdapterHandle,
                           &Pdo,
                           NULL,
                           NULL,
                           NULL,
                           NULL);

    Status = IoGetDeviceProperty(Pdo,
                                 DevicePropertyAddress,
                                 sizeof(PropertyAddress),
                                 &PropertyAddress,
                                 &Dummy);
    if (!NT_SUCCESS(Status))
        return FALSE;

    /* PCI devices only */
    *DeviceNumber = (PropertyAddress >> 16) & 0xFFFF;
    return TRUE;
}

static
CODE_SEG("PAGE")
PVOID
SRomParse21041Block(
    _Inout_ PDC21X4_ADAPTER Adapter,
    _In_ PUCHAR BlockBase)
{
    PAGED_CODE();

    INFO("*** 21041 Media Block ***\n");

    return BlockBase + 4;
}

static
CODE_SEG("PAGE")
PVOID
SRomParseCompactBlock(
    _Inout_ PDC21X4_ADAPTER Adapter,
    _In_ PUCHAR BlockBase)
{
    PAGED_CODE();

    INFO("*** Compact Block ***\n");

    return BlockBase + 4;
}

static
CODE_SEG("PAGE")
PVOID
SRomParseExtendedBlock(
    _Inout_ PDC21X4_ADAPTER Adapter,
    _In_ PUCHAR BlockBase)
{
    ULONG Length, Type;

    PAGED_CODE();

    Length = SRomGetExtendedBlockLength(BlockBase);
    Type = SRomGetExtendedBlockType(BlockBase);

    INFO("Length %u, Type %d\n", Length, Type);

    switch (Type)
    {
        case SROM_BLOCK_NON_MII:
        {
            INFO("*** Non-MII Block ***\n");
            break;
        }
        case SROM_BLOCK_MII_1:
        {
            INFO("*** MII Block (21140) ***\n");
            break;
        }
        case SROM_BLOCK_MII_2:
        {
            INFO("*** MII Block (21142+) ***\n");
            break;
        }
        case SROM_BLOCK_SIA:
        {
            INFO("*** SIA Block ***\n");
            break;
        }
        case SROM_BLOCK_SYM:
        {
            INFO("*** SYM Block ***\n");
            break;
        }
        case SROM_BLOCK_RESET:
        {
            INFO("*** RESET Block ***\n");
            break;
        }
        case SROM_BLOCK_PHY_SHUTDOWN:
        {
            INFO("*** PHY SHUTDOWN Block ***\n");
            break;
        }

        default:
            /* Bug in the parser? */
            ERR("Unknown block type %u\n", Type);
            break;
    }

    return BlockBase + Length;
}

static
CODE_SEG("PAGE")
NDIS_STATUS
SRomParseHeader(
    _Inout_ PDC21X4_ADAPTER Adapter,
    _In_ PUCHAR SRom)
{
    ULONG Index, BlockCount;
    USHORT LeafOffset;
    PUCHAR Data;

    PAGED_CODE();

    INFO("SROM Version %u, Controller count %u\n",
         SRom[SROM_VERSION],
         SRom[SROM_CONTROLLER_COUNT]);

    NdisMoveMemory(Adapter->PermanentMacAddress,
                   &SRom[SROM_MAC_ADDRESS],
                   ETH_LENGTH_OF_ADDRESS);

    if (SRom[SROM_CONTROLLER_COUNT] > 1)
    {
        USHORT DeviceNumber;

        if (!Dc21x4GetAdapterDeviceNumber(Adapter, &DeviceNumber))
            return FALSE;

        for (Index = 0; Index < SRom[SROM_CONTROLLER_COUNT]; ++Index)
        {
            if (SRom[SROM_DEVICE_NUMBER(Index)] == DeviceNumber)
                break;
        }
        if (Index == SRom[SROM_CONTROLLER_COUNT])
        {
            ERR("Controller %u was not found in the SROM\n", DeviceNumber);
            return NDIS_STATUS_ADAPTER_NOT_FOUND;
        }

        Adapter->PermanentMacAddress[5] += Index;
    }
    else
    {
        Index = 0;
    }

    LeafOffset = SRomRetrieveWordByOffset(SRom, SROM_LEAF_OFFSET(Index));
    Data = &SRom[LeafOffset];

    Adapter->Media = SRomRetrieveWord(Data);
    Data += sizeof(USHORT);

    if (Adapter->ChipType == DC21140)
    {
        Adapter->GpioDirection = *Data++;
    }

    INFO("GPIO Direction: %x\n", Adapter->GpioDirection);
    INFO("Adapter->Media: %x\n", Adapter->Media);
    INFO("Block Count: %u\n", *Data);
    INFO("LeafOffset: %u\n", LeafOffset);

    BlockCount = *Data++;
    ++BlockCount; // FIXME

    for (Index = 0; Index < BlockCount; ++Index)
    {
        if (Adapter->ChipType == DC21041)
        {
            Data = SRomParse21041Block(Adapter, Data);
        }
        else
        {
            if (SRomIsBlockExtended(Data))
                Data = SRomParseExtendedBlock(Adapter, Data);
            else
                Data = SRomParseCompactBlock(Adapter, Data);
        }
    }

    return NDIS_STATUS_SUCCESS;
}

static
CODE_SEG("PAGE")
USHORT
SRomChecksum(
    _In_reads_bytes_(Size) PUCHAR Data,
    _In_ ULONG Size)
{
    ULONG i, j, Crc;

    PAGED_CODE();

    Crc = 0xFFFFFFFF;
    for (i = 0; i < Size; ++i)
    {
        Crc ^= Data[i];
        for (j = 8; j > 0; j--)
        {
            Crc = (Crc >> 1) ^ (-(LONG)(Crc & 1) & 0xEDB88320);
        }
    }

    return (USHORT)(~Crc);
}

static
CODE_SEG("PAGE")
BOOLEAN
SRomChecksumValid(
    _In_ PUCHAR SRom)
{
    USHORT Checksum;

    PAGED_CODE();

    Checksum = SRomChecksum(SRom, SROM_CHECKSUM_V1);
    if (Checksum == SRomRetrieveWordByOffset(SRom, SROM_CHECKSUM_V1))
        return TRUE;

    Checksum = SRomChecksum(SRom, SROM_CHECKSUM_V2);
    if (Checksum == SRomRetrieveWordByOffset(SRom, SROM_CHECKSUM_V2))
        return TRUE;

    return FALSE;
}

static
CODE_SEG("PAGE")
BOOLEAN
SRomGetAddressBusWidth(
    _In_ PDC21X4_ADAPTER Adapter,
    _Out_ PULONG BusWidth)
{
    ULONG i, Csr, Sequence;

    PAGED_CODE();

    /* Assume the SROM is a 1kB ROM */
    Sequence = (EEPROM_CMD_READ << 6);

    /* Send the command and address */
    for (i = EEPROM_CMD_LENGTH + 6; i--; )
    {
        Csr = ((Sequence >> i) & 1) << 2;

        NdisRawWritePortUlong(Adapter->CSR9, Csr | EE_RD | EE_SR | EE_CS);
        NdisStallExecution(10);
        NdisRawWritePortUlong(Adapter->CSR9, Csr | EE_RD | EE_SR | EE_CS | EE_SK);
        NdisStallExecution(10);
        NdisRawWritePortUlong(Adapter->CSR9, Csr | EE_RD | EE_SR | EE_CS);
        NdisStallExecution(10);
    }

    /* Check the preceding dummy zero bit */
    NdisRawReadPortUlong(Adapter->CSR9, &Csr);
    if (Csr & EE_DO)
    {
        /* 4kB EEPROM */
        *BusWidth = 8;

        /* Send the remaining part of the address */
        for (i = 0; i < (8 - 6); ++i)
        {
            NdisRawWritePortUlong(Adapter->CSR9, Csr | EE_RD | EE_SR | EE_CS);
            NdisStallExecution(10);
            NdisRawWritePortUlong(Adapter->CSR9, Csr | EE_RD | EE_SR | EE_CS | EE_SK);
            NdisStallExecution(10);
            NdisRawWritePortUlong(Adapter->CSR9, Csr | EE_RD | EE_SR | EE_CS);
            NdisStallExecution(10);
        }

        /* The preceding dummy bit must be zero */
        NdisRawReadPortUlong(Adapter->CSR9, &Csr);
        if (Csr & EE_DO)
        {
            return FALSE;
        }
    }
    else
    {
        /* 1kB EEPROM */
        *BusWidth = 6;
    }

    /* Skip the output data */
    for (i = 0; i < 16; ++i)
    {
        NdisRawWritePortUlong(Adapter->CSR9, Csr | EE_RD | EE_SR | EE_CS | EE_SK);
        NdisStallExecution(10);
        NdisRawReadPortUlong(Adapter->CSR9, &Csr);
        NdisStallExecution(10);
        NdisRawWritePortUlong(Adapter->CSR9, Csr | EE_RD | EE_SR | EE_CS);
        NdisStallExecution(10);
    }

    NdisRawWritePortUlong(Adapter->CSR9, Csr | EE_RD | EE_SR);

    return TRUE;
}

static
CODE_SEG("PAGE")
BOOLEAN
ReadSRom(
    _In_ PDC21X4_ADAPTER Adapter,
    _Out_writes_all_(EE_SIZE) PUCHAR SRom)
{
    ULONG i, BusWidth, Address, Csr;

    PAGED_CODE();

    NdisRawWritePortUlong(Adapter->CSR9, EE_RD | EE_SR);
    NdisStallExecution(10);
    NdisRawWritePortUlong(Adapter->CSR9, EE_RD | EE_SR | EE_CS);
    NdisStallExecution(10);

    if (!SRomGetAddressBusWidth(Adapter, &BusWidth))
    {
        return FALSE;
    }

    /* Read the SROM contents once */
    for (Address = 0; Address < EE_SIZE / sizeof(USHORT); ++Address)
    {
        USHORT SerialData = 0;
        ULONG Sequence = (EEPROM_CMD_READ << BusWidth) | Address;

        for (i = BusWidth + EEPROM_CMD_LENGTH; i--; )
        {
            Csr = ((Sequence >> i) & 1) << 2;

            NdisRawWritePortUlong(Adapter->CSR9, Csr | EE_RD | EE_SR | EE_CS);
            NdisStallExecution(10);
            NdisRawWritePortUlong(Adapter->CSR9, Csr | EE_RD | EE_SR | EE_CS | EE_SK);
            NdisStallExecution(10);
            NdisRawWritePortUlong(Adapter->CSR9, Csr | EE_RD | EE_SR | EE_CS);
            NdisStallExecution(10);
        }

        for (i = 0; i < 16; ++i)
        {
            NdisRawWritePortUlong(Adapter->CSR9, EE_RD | EE_SR | EE_CS | EE_SK);
            NdisStallExecution(10);
            NdisRawReadPortUlong(Adapter->CSR9, &Csr);
            SerialData = (SerialData << 1) | ((Csr >> 3) & 1);
            NdisStallExecution(10);
            NdisRawWritePortUlong(Adapter->CSR9, EE_RD | EE_SR | EE_CS);
            NdisStallExecution(10);
        }

        SRom[Address * sizeof(USHORT)] = SerialData & 0xFF;
        SRom[Address * sizeof(USHORT) + 1] = SerialData >> 8;

        NdisRawWritePortUlong(Adapter->CSR9, EE_RD | EE_SR);
        NdisStallExecution(10);
    }

    NdisRawWritePortUlong(Adapter->CSR9, 0);

    return TRUE;
}

static
CODE_SEG("PAGE")
NDIS_STATUS
ParseSRom(
    _Inout_ PDC21X4_ADAPTER Adapter)
{
    PVOID SRom;
    NDIS_STATUS Status;

    PAGED_CODE();

    Status = NdisAllocateMemoryWithTag(&SRom, EE_SIZE, DC21X4_TAG);
    if (Status != NDIS_STATUS_SUCCESS)
    {
        Status = NDIS_STATUS_RESOURCES;
        goto Cleanup;
    }

    if (!ReadSRom(Adapter, SRom))
    {
        ERR("Failed to retrieve the SROM contents\n");
        Status = NDIS_STATUS_FAILURE;
        goto Cleanup;
    }

    if (!SRomChecksumValid(SRom))
    {
        ERR("SROM has an invalid checksum\n");
        Status = NDIS_STATUS_FAILURE;
        goto Cleanup;
    }

    Status = SRomParseHeader(Adapter, SRom);
    if (Status != NDIS_STATUS_SUCCESS)
    {
        ERR("Failed to parse SROM\n");
        goto Cleanup;
    }

Cleanup:
    NdisFreeMemory(SRom, EE_SIZE, 0);
    return Status;
}

static
CODE_SEG("PAGE")
BOOLEAN
AddressRomChecksumValid(
    _In_reads_bytes_(EAR_SIZE) PUCHAR Octet)
{
    ULONG Checksum;

    PAGED_CODE();

    Checksum = (Octet[0] << 10) + (Octet[2] << 9) + (Octet[4] << 8) +
               (Octet[1] << 2) + (Octet[3] << 1) + Octet[5];
    Checksum %= 0xFFFF;

    return (Checksum == ((Octet[6] << 8) | Octet[7]));
}

static
CODE_SEG("PAGE")
BOOLEAN
ReadAddressRom(
    _In_ PDC21X4_ADAPTER Adapter,
    _Out_writes_all_(EAR_SIZE) PUCHAR AddressRom)
{
    ULONG i, j;

    PAGED_CODE();

    /* Reset the ROM pointer */
    NdisRawWritePortUlong(Adapter->CSR9, 0);

    for (i = 0; i < EAR_SIZE; ++i)
    {
        for (j = 100; j > 0; j--)
        {
            ULONG Data;

            NdisMSleep(1000);
            NdisRawReadPortUlong(Adapter->CSR9, &Data);
            if (!(Data & EAR_DN))
            {
                AddressRom[i] = Data & EAR_DT;
                break;
            }
        }
        if (j == 0)
        {
            return FALSE;
        }
    }

    return TRUE;
}

static
CODE_SEG("PAGE")
NDIS_STATUS
ParseAddressRom(
    _Inout_ PDC21X4_ADAPTER Adapter)
{
    UCHAR AddressRom[EAR_SIZE];

    PAGED_CODE();

    if (!ReadAddressRom(Adapter, AddressRom))
    {
        ERR("Failed to retrieve the EAR contents\n");
        return NDIS_STATUS_FAILURE;
    }

    if (!AddressRomChecksumValid(AddressRom))
    {
        ERR("EAR has an invalid checksum\n");
        return NDIS_STATUS_FAILURE;
    }

    NdisMoveMemory(Adapter->PermanentMacAddress, AddressRom, ETH_LENGTH_OF_ADDRESS);

    return NDIS_STATUS_SUCCESS;
}

CODE_SEG("PAGE")
VOID
Dc21x4ReadEeprom(
    _In_ PDC21X4_ADAPTER Adapter)
{
    PAGED_CODE();

    if (Adapter->ChipType == DC21040)
    {
        /* Ethernet Address ROM */
        ParseAddressRom(Adapter);
    }
    else
    {
        /* MicroWire Compatible Serial EEPROM */
        ParseSRom(Adapter);
    }

    INFO("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
         Adapter->PermanentMacAddress[0],
         Adapter->PermanentMacAddress[1],
         Adapter->PermanentMacAddress[2],
         Adapter->PermanentMacAddress[3],
         Adapter->PermanentMacAddress[4],
         Adapter->PermanentMacAddress[5]);

    __debugbreak();
}
