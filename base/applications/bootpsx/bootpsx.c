#include "bootpsx.h"
#include <debug.h>

/**********************************************************************
 * PsxCheckSubSystem/1
 */
NTSTATUS
PsxCheckSubSystem(VOID)
{
	NTSTATUS          Status = STATUS_SUCCESS;
	UNICODE_STRING    DirectoryName = {0, 0, NULL};
	OBJECT_ATTRIBUTES DirectoryAttributes = {0};
	HANDLE            hDir = (HANDLE) 0;

	RtlInitUnicodeString (& DirectoryName, L"\\POSIX");
	InitializeObjectAttributes (& DirectoryAttributes,
				    & DirectoryName,
				    0,0,0);
	Status = NtOpenDirectoryObject (& hDir,
					DIRECTORY_TRAVERSE,
					& DirectoryAttributes);
	if(NT_SUCCESS(Status))
	{
		NtClose (hDir);
	}

	return Status;
}

/**********************************************************************
 * PsxBootstrap/1
 */
NTSTATUS
PsxBootstrap(VOID)
{
	NTSTATUS       Status = STATUS_SUCCESS;
	UNICODE_STRING Program = {0, 0, NULL};
	HANDLE         SmApiPort = (HANDLE) 0;


	DPRINT1("PsxBootstrap: Connecting to the SM: ");
	Status = SmConnectApiPort (NULL,
				   (HANDLE) 0,
				   IMAGE_SUBSYSTEM_POSIX_CUI,
				   & SmApiPort);
	if(!NT_SUCCESS(Status))
	{
		DPRINT1("PsxBootstrap: SmConnectApiPort failed with %X\n", Status);
		return Status;
	}
	RtlInitUnicodeString (& Program, L"POSIX");
	Status = SmExecuteProgram (SmApiPort, & Program);
	if(STATUS_SUCCESS != Status)
	{
		DPRINT1("PsxBootstrap: SmExecuteProgram = %X\n", Status);
	}
	NtClose (SmApiPort);
	return Status;
}

/**********************************************************************
 *
 *	ENTRY POINT					PUBLIC
 *
 *********************************************************************/
int main (int argc, char * argv [])
{
	DPRINT1("Attempting to start the POSIX Subsystem\n");
	NTSTATUS Status = STATUS_SUCCESS;
	INT      RetryCount = RETRY_COUNT;

	while(RetryCount > 0)
	{
		Status = PsxCheckSubSystem();
		if(STATUS_SUCCESS == Status)
		{
			if (RETRY_COUNT == RetryCount)
			{
				DPRINT1("POSIX already booted.\n");
			}else{
				DPRINT1("POSIX booted.\n");
			}
			break;
		}else{
			Status = PsxBootstrap ();
		}
		-- RetryCount;
	}
	return NT_SUCCESS(Status) ? EXIT_SUCCESS : EXIT_FAILURE;
}
/* EOF */
