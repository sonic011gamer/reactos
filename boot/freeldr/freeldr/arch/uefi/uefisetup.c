/*
 * PROJECT:     Freeldr UEFI Extension
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     UEFI Mach Setup
 * COPYRIGHT:   Copyright 2022 Justin Miller <justinmiller100@gmail.com>
 */

#include <uefildr.h>

#include <debug.h>
DBG_DEFAULT_CHANNEL(WARNING);

/* GLOBALS ********************************************************************/

extern EFI_SYSTEM_TABLE* GlobalSystemTable;
extern EFI_HANDLE GlobalImageHandle;
BOOLEAN AcpiPresent = FALSE;

/* FUNCTIONS ******************************************************************/

ULONG FirstLevelDcacheSize;
ULONG FirstLevelDcacheFillSize;
ULONG FirstLevelIcacheSize;
ULONG FirstLevelIcacheFillSize;
ULONG SecondLevelDcacheSize;
ULONG SecondLevelDcacheFillSize;
ULONG SecondLevelIcacheSize;
ULONG SecondLevelIcacheFillSize;

extern ULONG reactos_disk_count;

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
MachInit(const char *CmdLine)
{
    MachVtbl.ConsPutChar = UefiConsPutChar;
    MachVtbl.ConsKbHit = UefiConsKbHit;
    MachVtbl.ConsGetCh = UefiConsGetCh;
    MachVtbl.VideoClearScreen = UefiVideoClearScreen;
    MachVtbl.VideoSetDisplayMode = UefiVideoSetDisplayMode;
    MachVtbl.VideoGetDisplaySize = UefiVideoGetDisplaySize;
    MachVtbl.VideoGetBufferSize = UefiVideoGetBufferSize;
    MachVtbl.VideoGetFontsFromFirmware = UefiVideoGetFontsFromFirmware;
    MachVtbl.VideoSetTextCursorPosition = UefiVideoSetTextCursorPosition;
    MachVtbl.VideoHideShowTextCursor = UefiVideoHideShowTextCursor;
    MachVtbl.VideoPutChar = UefiVideoPutChar;
    MachVtbl.VideoCopyOffScreenBufferToVRAM = UefiVideoCopyOffScreenBufferToVRAM;
    MachVtbl.VideoIsPaletteFixed = UefiVideoIsPaletteFixed;
    MachVtbl.VideoSetPaletteColor = UefiVideoSetPaletteColor;
    MachVtbl.VideoGetPaletteColor = UefiVideoGetPaletteColor;
    MachVtbl.VideoSync = UefiVideoSync;
    MachVtbl.Beep = UefiPcBeep;
    MachVtbl.PrepareForReactOS = UefiPrepareForReactOS;
    MachVtbl.GetMemoryMap = UefiMemGetMemoryMap;
    MachVtbl.GetExtendedBIOSData = UefiGetExtendedBIOSData;
    MachVtbl.GetFloppyCount = UefiGetFloppyCount;
    MachVtbl.DiskReadLogicalSectors = UefiDiskReadLogicalSectors;
    MachVtbl.DiskGetDriveGeometry = UefiDiskGetDriveGeometry;
    MachVtbl.DiskGetCacheableBlockCount = UefiDiskGetCacheableBlockCount;
    MachVtbl.GetTime = UefiGetTime;
    MachVtbl.InitializeBootDevices = UefiInitializeBootDevices;
    MachVtbl.HwDetect = UefiHwDetect;
    MachVtbl.HwIdle = UefiHwIdle;

    /* Setup GOP */
    if (UefiInitalizeVideo() != EFI_SUCCESS)
    {
        ERR("Failed to setup GOP\n");
    }
}
