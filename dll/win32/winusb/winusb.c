/*
 * COPYRIGHT:   See COPYING in the top level directory
 * LICENSE:     See LGPL.txt in the top level directory
 * PROJECT:     ReactOS system libraries
 * FILE:        reactos/lib/winusb/misc/win32.c
 * PURPOSE:     Win32 interfaces for winusb
 * PROGRAMMER:  KJK::Hyperion <noog@libero.it>
 *              Thomas Weidenmueller <w3seek@reactos.com>
 *              Pierre Schweitzer <pierre@reactos.org>
 * UPDATE HISTORY:
 *              10/06/2002: Created
 */

#include <stdarg.h>

#define WIN32_NO_STATUS
#include <windef.h>
#include <winbase.h>
#include <winnls.h>
/* PUBLIC *********************************************************************/

/*
 * @implemented
 */
BOOLEAN
WINAPI
DllMain(HINSTANCE hDllHandle,
        DWORD nReason,
        LPVOID Reserved)
{
  return TRUE;
}

