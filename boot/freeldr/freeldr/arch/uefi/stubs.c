/*
 * PROJECT:     Freeldr UEFI Extension
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     UEFI stubs
 * COPYRIGHT:   Copyright 2022 Justin Miller <justinmiller100@gmail.com>
 */

#include <uefildr.h>

#include <debug.h>
extern EFI_SYSTEM_TABLE * GlobalSystemTable;
extern EFI_HANDLE GlobalImageHandle;


#ifndef _M_ARM
/* TODO: Handle this with custom Disk / partition setup */
UCHAR
DriveMapGetBiosDriveNumber(PCSTR DeviceName)
{
    return 0;
}
#endif

VOID
NTAPI
KeStallExecutionProcessor(ULONG Microseconds)
{
}

USHORT
__cdecl PxeCallApi(USHORT Segment, USHORT Offset, USHORT Service, VOID* Parameter)
{
    return 0;
}

//TODOS

VOID
UefiVideoGetFontsFromFirmware(PULONG RomFontPointers)
{

}

VOID
UefiVideoSync(VOID)
{

}

VOID
UefiGetExtendedBIOSData(PULONG ExtendedBIOSDataArea,
                        PULONG ExtendedBIOSDataSize)
{

}

PCONFIGURATION_COMPONENT_DATA
UefiHwDetect(VOID)
{
    return 0;
}

VOID
UefiPcBeep(VOID)
{
    /* Not possible on UEFI, for now */
}
EFI_INPUT_KEY Key;
BOOLEAN scancode;

/* Read into the public Keystroke handler */
BOOLEAN
UefiConsKbHit(VOID)
{
    Key.UnicodeChar = 0;
    Key.ScanCode = 0;
    GlobalSystemTable->ConIn->ReadKeyStroke(GlobalSystemTable->ConIn, &Key);

    if(Key.UnicodeChar != 0)
    {
        scancode = FALSE;
        return TRUE;

    }
    else if(Key.ScanCode != 0)
    {
        scancode = TRUE;
        return TRUE;
    }
    else
    {
        return FALSE;
    }

    return 0;
}

int
UefiConsGetCh(void)
{
    /* The UEFI Scan codes are an entirely different order then BIOS. */
    if (scancode == TRUE)
    {
        switch(Key.ScanCode)
        {
            case SCAN_UP:
                return KEY_UP;
            case SCAN_DOWN:
                return KEY_DOWN;
            case SCAN_RIGHT:
                return KEY_RIGHT;
            case SCAN_LEFT:
                return KEY_LEFT;
            case SCAN_F1:
                return KEY_F1;
            case SCAN_F2:
                return KEY_F2;
            case SCAN_F3:
                return KEY_F3;
            case SCAN_F4:
                return KEY_F4;
            case SCAN_F5:
                return KEY_F5;
            case SCAN_F6:
                return KEY_F6;
            case SCAN_F7:
                return KEY_F7;
            case SCAN_F8:
                return KEY_F8;
            case SCAN_F9:
                return KEY_F9;
            case SCAN_F10:
                return KEY_F10;
            case SCAN_ESC:
                return KEY_ESC;
        }
        return Key.ScanCode;
    }
    else
    {
        return Key.UnicodeChar;
    }
}

VOID
UefiHwIdle(VOID)
{

}
