/*
 * PROJECT:     VMS Enviroment Subsystem Server
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * ReactOS VMS+ Environment Subsystem Server source file
 * COPYRIGHT:
 */

#include "vmssrv.h"
//#define NDEBUG
#include <debug.h>

HANDLE VmsApiPort = NULL;

/**********************************************************************
 * NAME							PRIVATE
 * 	VmsApiNull/2
 */
NTSTATUS NTAPI VmsApiNull (IN OUT PCSR_API_MESSAGE ApiMessage,
			   IN OUT PULONG Reply)
{
	DPRINT("VMSSRV: %s called\n", __FUNCTION__);

	*Reply = 0;
	return STATUS_SUCCESS;
}

PCSR_API_ROUTINE VmsServerApiDispatchTable[1] =
{
    (PCSR_API_ROUTINE)VmsApiNull,
};


BOOLEAN VmsServerApiValidTable [1] =
{
    TRUE
};

PCHAR VmsServerApiNameTable [1] =
{
    "Null",
};

/*=====================================================================
 * 	PUBLIC API
 *===================================================================*/

NTSTATUS NTAPI ServerDllInitialization (PCSR_SERVER_DLL LoadedServerDll)
{
	NTSTATUS Status = STATUS_SUCCESS;

	DPRINT("VMSSRV: %s called\n", __FUNCTION__);

	// Get the listening port from csrsrv.dll
	VmsApiPort = CsrQueryApiPort ();
	if (NULL == VmsApiPort)
	{
		Status = STATUS_UNSUCCESSFUL;
	} else {
		// Set CSR information
		LoadedServerDll->ApiBase             = 0;
		LoadedServerDll->HighestApiSupported = 0;
		LoadedServerDll->DispatchTable       = VmsServerApiDispatchTable;
		LoadedServerDll->ValidTable          = VmsServerApiValidTable;
		LoadedServerDll->NameTable           = VmsServerApiNameTable;
		LoadedServerDll->SizeOfProcessData   = 0;
		LoadedServerDll->ConnectCallback     = NULL;
		LoadedServerDll->DisconnectCallback  = NULL;
	}
	return Status;
}

/* EOF */
