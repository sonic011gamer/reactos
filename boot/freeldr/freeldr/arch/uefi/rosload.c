#include <uefildr.h>

#include "../../../../environ/include/bltypes.h"
#include <debug.h>

EFI_HANDLE GlobalImageHandle;
EFI_SYSTEM_TABLE *GlobalSystemTable;


/*
 *
 * Both of these para   meters to EFI_ENTRY are pointers.
 * awkwardly, so is BlAppicationEntry.
 * So behold the birth of the weirdest fucking idea Illen has ever given me
 * Freeldr.epic
 */
NTSTATUS
NTAPI
OslMain(_In_ PBOOT_APPLICATION_PARAMETER_BLOCK BootAppParameters,
	    _In_ PBL_LIBRARY_PARAMETERS LibraryParameters)
{
    PBL_LIBRARY_PARAMETERS	LibraryParams;
	PBL_FIRMWARE_DESCRIPTOR FirmwareDescriptor;
    UINTN ParamPointer;

	if (!BootAppParameters)
	{
	    // Invalid parameter
	    return STATUS_INVALID_PARAMETER;
	}

	LibraryParams = LibraryParameters;
	ParamPointer = (UINTN) BootAppParameters;
	FirmwareDescriptor = (PBL_FIRMWARE_DESCRIPTOR) (ParamPointer + BootAppParameters->FirmwareParametersOffset);

	// Do what ever you want now
    if (FirmwareDescriptor->SystemTable)
	{
            GlobalImageHandle = FirmwareDescriptor->ImageHandle;
			GlobalSystemTable = FirmwareDescriptor->SystemTable;
	}
    /* Some idiot decided to boot freeldr as a bootmgr app*/
    GlobalSystemTable->ConOut->OutputString(GlobalSystemTable->ConOut,L"Starting Freeldr from bootmgr\n");

    BootMain(NULL);
    return 0;
}