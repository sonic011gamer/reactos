/*
 * PROJECT:     ReactOS DC21x4 Driver
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     PHY layer setup and management
 * COPYRIGHT:   Copyright 2021 Dmitry Borisov <di.sean@protonmail.com>
 */

/* INCLUDES *******************************************************************/

#include "dc21x4.h"

#define NDEBUG
#include <debug.h>

/* GLOBALS ********************************************************************/

#define MDIO_START    0x01
#define MDIO_WRITE    0x01
#define MDIO_READ     0x02
#define MDIO_TA       0x02
#define MDIO_PREAMBLE 0xFFFFFFFF

/* FUNCTIONS ******************************************************************/

static
VOID
MdioPacket(
    _In_ PDC21X4_ADAPTER Adapter,
    _In_ ULONG Sequence,
    _In_ ULONG Length)
{
    ULONG i;

    for (i = Length; i--; )
    {
        ULONG Mdo = ((Sequence >> i) & 1) << 17;

        NdisRawWritePortUlong(Adapter->CSR9, Mdo);
        NdisStallExecution(2);
        NdisRawWritePortUlong(Adapter->CSR9, Mdo | MII_MDC);
        NdisStallExecution(2);
    }
}

BOOLEAN
MiiWrite(
    _In_ PDC21X4_ADAPTER Adapter,
    _In_ ULONG PhyAddress,
    _In_ ULONG RegAddress,
    _In_ USHORT Data)
{
    MdioPacket(Adapter, MDIO_PREAMBLE, 32);
    MdioPacket(Adapter,
               (MDIO_START << 30) |
               (MDIO_WRITE << 28) |
               (PhyAddress << 23) |
               (RegAddress << 18) |
               (MDIO_TA << 16) |
               Data,
               32);

    NdisRawWritePortUlong(Adapter->CSR9, MII_READ);
    NdisStallExecution(2);
    NdisRawWritePortUlong(Adapter->CSR9, MII_READ | MII_MDC);
    NdisStallExecution(2);

    return TRUE;
}

BOOLEAN
MiiRead(
    _In_ PDC21X4_ADAPTER Adapter,
    _In_ ULONG PhyAddress,
    _In_ ULONG RegAddress,
    _Out_ PUSHORT Data)
{
    ULONG i, Csr;
    USHORT Result;
    BOOLEAN Success;

    MdioPacket(Adapter, MDIO_PREAMBLE, 32);
    MdioPacket(Adapter,
               (MDIO_START << 30) |
               (MDIO_READ << 28) |
               (PhyAddress << 23) |
               (RegAddress << 18),
               14);

    NdisRawWritePortUlong(Adapter->CSR9, MII_READ);
    NdisStallExecution(2);
    NdisRawWritePortUlong(Adapter->CSR9, MII_READ | MII_MDC);
    NdisStallExecution(2);

    NdisRawReadPortUlong(Adapter->CSR9, &Csr);
    Success = (Csr & MII_MDI) ? FALSE : TRUE;

    Result = 0;
    for (i = 0; i < 16; ++i)
    {
        NdisRawWritePortUlong(Adapter->CSR9, MII_READ);
        NdisStallExecution(2);
        NdisRawWritePortUlong(Adapter->CSR9, MII_READ | MII_MDC);
        NdisStallExecution(2);
        NdisRawReadPortUlong(Adapter->CSR9, &Csr);
        Result = (Result << 1) | ((Csr >> 19) & 1);
        NdisStallExecution(2);
    }
    *Data = Result;

    NdisRawWritePortUlong(Adapter->CSR9, MII_READ);
    NdisStallExecution(2);
    NdisRawWritePortUlong(Adapter->CSR9, MII_READ | MII_MDC);
    NdisStallExecution(2);

    return Success;
}
