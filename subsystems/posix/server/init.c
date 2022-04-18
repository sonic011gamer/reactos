#include "include/psxss.h"
//#define NDEBUG
#include <debug.h>

/* DLL entry point */

HANDLE PsxSrvDllHandle = 0;

/* FUNCTIONS *****************************************************************/
#if 0
BOOL
WINAPI
DllMain(HANDLE hDll,
	DWORD dwReason,
	LPVOID lpReserved)
{
  if (DLL_PROCESS_ATTACH == dwReason)
    {
      PsxSrvDllHandle = hDll;
    }

  return TRUE;
}
#endif
/**********************************************************************
 *	PsxServerInitialization/2
 *
 * DESCRIPTION
 *	Initialize the PSX server process.
 */
NTSTATUS
NTAPI
PsxServerInitialization (
    IN ULONG ArgumentCount,
    IN PWSTR *ArgumentArray
    )
{
	NTSTATUS Status;

    Status = 0;
#if 0
	/* Get our heap */
        PdxInitializeHeap ();
	/* Initialize POSIX+ and Sessions */
	Status = PdxInitializeDirectories ();
	if (!NT_SUCCESS(Status)) return Status;
	/* LPCs dispatchers */
	Status = PdxInitializeListeners ();
	if (!NT_SUCCESS(Status)) return Status;
	/* Terminal manager */
	Status = PsxInitializeSessions ();
	if (!NT_SUCCESS(Status)) return Status;
	/* Process manager */
	Status = PsxInitializeProcesses ();
	if (!NT_SUCCESS(Status)) return Status;
	/* OK. Run! */
	Status = PdxRunServer ();
#endif
	/* Done */
	return Status;
}
/* EOF */
