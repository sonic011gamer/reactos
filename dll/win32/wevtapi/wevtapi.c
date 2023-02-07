
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

