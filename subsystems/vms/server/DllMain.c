#include "vmssrv.h"

#define NDEBUG
#include <debug.h>

/* DLL entry point */

HANDLE VmsSrvDllHandle = 0;

/* FUNCTIONS *****************************************************************/

BOOL
WINAPI
DllMain(HANDLE hDll,
	DWORD dwReason,
	LPVOID lpReserved)
{
  if (DLL_PROCESS_ATTACH == dwReason)
    {
      VmsSrvDllHandle = hDll;
    }

  return TRUE;
}

/* EOF */