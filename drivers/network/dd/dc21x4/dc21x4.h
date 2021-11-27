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

#endif /* _DC21X4_PCH_ */
