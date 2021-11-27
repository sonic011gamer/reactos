/*
 * PROJECT:     ReactOS DC21x4 Driver
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Main header file
 * COPYRIGHT:   Copyright 2021 Dmitry Borisov <di.sean@protonmail.com>
 */

#ifndef _DC21X4_PCH_
#define _DC21X4_PCH_

#include <ndis.h>
#include <section_attribs.h>

#include "nic.h"

#define DC21X4_TAG '4x12'

typedef enum _DC_CHIP_TYPE
{
    DC21040,
    DC21140,
    DC21041,
    DC21142,
    DC21143
} DC_CHIP_TYPE;

typedef struct _DC21X4_ADAPTER
{
    PULONG CSR0;
    PULONG CSR1;
    PULONG CSR2;
    PULONG CSR3;
    PULONG CSR4;
    PULONG CSR5;
    PULONG CSR6;
    PULONG CSR7;
    PULONG CSR8;
    PULONG CSR9;
    PULONG CSR10;
    PULONG CSR11;
    PULONG CSR12;
    PULONG CSR13;
    PULONG CSR14;
    PULONG CSR15;

    NDIS_HANDLE AdapterHandle;
    NDIS_HANDLE WrapperConfigurationHandle;

    UCHAR PermanentMacAddress[ETH_LENGTH_OF_ADDRESS];
    UCHAR CurrentMacAddress[ETH_LENGTH_OF_ADDRESS];

    DC_CHIP_TYPE ChipType;

    USHORT DeviceId;
    UCHAR RevisionId;

    USHORT Media;
    UCHAR GpioDirection;

    BOOLEAN HasPmFeatures;
} DC21X4_ADAPTER, *PDC21X4_ADAPTER;

/*
 * Various offsets in the SROM
 */
#define SROM_VERSION             18
#define SROM_CONTROLLER_COUNT    19
#define SROM_MAC_ADDRESS         20
#define SROM_DEVICE_NUMBER(n)    (26 + ((n) * 3))
#define SROM_LEAF_OFFSET(n)      (27 + ((n) * 3))
#define SROM_CHECKSUM_V1         126
#define SROM_CHECKSUM_V2         94
#define SROM_MAGIC_PACKET_BLOCK  96

/*
 * Extended format types
 */
#define SROM_BLOCK_NON_MII       0
#define SROM_BLOCK_MII_1         1
#define SROM_BLOCK_SIA           2
#define SROM_BLOCK_MII_2         3
#define SROM_BLOCK_SYM           4
#define SROM_BLOCK_RESET         5
#define SROM_BLOCK_PHY_SHUTDOWN  6

/*
 * CSR 9
 */
#define EE_CS     0x0001
#define EE_SK     0x0002
#define EE_DI     0x0004
#define EE_DO     0x0008
#define EE_SR     0x0800
#define EE_RD     0x4000
#define MII_MDC   0x10000
#define MII_MDO   0x20000
#define MII_READ  0x40000
#define MII_MDI   0x80000
#define EAR_DN   0x80000000
#define EAR_DT   0x000000FF

#define EEPROM_CMD_WRITE    5
#define EEPROM_CMD_READ     6
#define EEPROM_CMD_ERASE    7

#define EEPROM_CMD_LENGTH   3

#define EAR_SIZE    8

/* We don't care about Magic Info in 21143+ */
#define EE_SIZE     128

CODE_SEG("PAGE")
VOID
Dc21x4ReadEeprom(
    _In_ PDC21X4_ADAPTER Adapter);

BOOLEAN
MiiWrite(
    _In_ PDC21X4_ADAPTER Adapter,
    _In_ ULONG PhyAddress,
    _In_ ULONG RegAddress,
    _In_ USHORT Data);

BOOLEAN
MiiRead(
    _In_ PDC21X4_ADAPTER Adapter,
    _In_ ULONG PhyAddress,
    _In_ ULONG RegAddress,
    _Out_ PUSHORT Data);

#endif /* _DC21X4_PCH_ */
