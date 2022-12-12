/*
 * PROJECT:     Freeldr UEFI Extension
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     UEFI stubs
 * COPYRIGHT:   Copyright 2022 Justin Miller <justinmiller100@gmail.com>
 */

#include <uefildr.h>
#include <internal/arm/intrin_i.h>
#include <debug.h>

ULONG FirstLevelDcacheSize;
ULONG FirstLevelDcacheFillSize;
ULONG FirstLevelIcacheSize;
ULONG FirstLevelIcacheFillSize;
ULONG SecondLevelDcacheSize;
ULONG SecondLevelDcacheFillSize;
ULONG SecondLevelIcacheSize;
ULONG SecondLevelIcacheFillSize;

ULONG SizeBits[] =
{
    -1,      // INVALID
    -1,      // INVALID
    1 << 12, // 4KB
    1 << 13, // 8KB
    1 << 14, // 16KB
    1 << 15, // 32KB
    1 << 16, // 64KB
    1 << 17  // 128KB
};

ULONG AssocBits[] =
{
    -1,      // INVALID
    -1,      // INVALID
    4        // 4-way associative
};

ULONG LenBits[] =
{
    -1,      // INVALID
    -1,      // INVALID
    8        // 8 words per line (32 bytes)
};

VOID
UefiArchSpecificSetup()
{
    ARM_CACHE_REGISTER CacheReg;
        /* Get cache information */
    CacheReg = KeArmCacheRegisterGet();
    FirstLevelDcacheSize = SizeBits[CacheReg.DSize];
    FirstLevelDcacheFillSize = LenBits[CacheReg.DLength];
    FirstLevelDcacheFillSize <<= 2;
    FirstLevelIcacheSize = SizeBits[CacheReg.ISize];
    FirstLevelIcacheFillSize = LenBits[CacheReg.ILength];
    FirstLevelIcacheFillSize <<= 2;
    SecondLevelDcacheSize =
    SecondLevelDcacheFillSize =
    SecondLevelIcacheSize =
    SecondLevelIcacheFillSize = 0;
}