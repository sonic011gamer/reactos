/*
 * PROJECT:     Freeldr UEFI Extension
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     UEFI stubs
 * COPYRIGHT:   Copyright 2022 Justin Miller <justinmiller100@gmail.com>
 */

#include <uefildr.h>

#include <debug.h>
DBG_DEFAULT_CHANNEL(WARNING);
#ifndef _M_ARM
/* TODO: Handle this with custom Disk / partition setup */
UCHAR
DriveMapGetBiosDriveNumber(PCSTR DeviceName)
{
    return 0;
}
#endif
#ifndef _M_ARM
VOID StallExecutionProcessor(ULONG Microseconds)
{

}
#endif
extern EFI_SYSTEM_TABLE * GlobalSystemTable;
extern EFI_HANDLE GlobalImageHandle;
VOID
NTAPI
KeStallExecutionProcessor(ULONG Microseconds)
{
    GlobalSystemTable->BootServices->Stall(Microseconds);
}
USHORT
__cdecl PxeCallApi(USHORT Segment, USHORT Offset, USHORT Service, VOID* Parameter)
{
    return 0;
}

EFI_INPUT_KEY Key;
BOOLEAN scancode;
BOOLEAN
UefiConsKbHit(VOID)
{
    Key.UnicodeChar = 0;
    Key.ScanCode = 0;
    GlobalSystemTable->ConIn->ReadKeyStroke(GlobalSystemTable->ConIn, &Key);
     
     if(Key.UnicodeChar != 0)
      {
        TRACE("keyboard kit %d", Key.UnicodeChar);
        scancode = FALSE;
        return TRUE;

      }
      else if(Key.ScanCode != 0)
      {
        TRACE("keyboard kit %d", Key.ScanCode);
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
    if (scancode == TRUE)
    {
        switch(Key.ScanCode)
        {
            case 18:
                return KEY_F8;
            case 1:
                return KEY_UP;
            case 2:
                return KEY_DOWN;
            case 3:
                return KEY_RIGHT;
            case 4:
                return KEY_LEFT;
            case 23:
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
UefiVideoGetFontsFromFirmware(PULONG RomFontPointers)
{

}

VOID
UefiVideoHideShowTextCursor(BOOLEAN Show)
{

}


BOOLEAN
UefiVideoIsPaletteFixed(VOID)
{
    return 0;
}
VOID
UefiVideoSetPaletteColor(UCHAR Color, UCHAR Red,
                         UCHAR Green, UCHAR Blue)
{

}
VOID
UefiVideoGetPaletteColor(UCHAR Color, UCHAR* Red,
                         UCHAR* Green, UCHAR* Blue)
{

}
VOID
UefiVideoSync(VOID)
{

}
VOID
UefiPcBeep(VOID)
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
UefiHwIdle(VOID)
{

}

#ifndef _M_ARM
VOID
UefiArchSpecificSetup()
{

}
#endif
