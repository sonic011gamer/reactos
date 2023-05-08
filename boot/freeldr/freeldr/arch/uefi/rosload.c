#include <uefildr.h>
#include "loc.h"
#include <debug.h>

extern EFI_HANDLE GlobalImageHandle;
extern EFI_SYSTEM_TABLE *GlobalSystemTable;

volatile unsigned int * UART0DR2 = (unsigned int *) 0x09000000;
/*
 *
 * Both of these para   meters to EFI_ENTRY are pointers.
 * awkwardly, so is BlAppicationEntry.
 * So behold the birth of the weirdest fucking idea Illen has ever given me
 * Freeldr.epic
 */


#include <armintr.h>

#ifndef _ARM_INTRIN_SHORTCUT_
#define _ARM_INTRIN_SHORTCUT_
#define CP15_PMSELR								15, 0,  9, 12, 5         // Event Counter Selection Register
#define CP15_PMXEVCNTR							15, 0,  9, 13, 2         // Event Count Register
#define CP15_TPIDRURW							15, 0, 13,  0, 2         // Software Thread ID Register, User Read/Write
#define CP15_TPIDRURO							15, 0, 13,  0, 3         // Software Thread ID Register, User Read Only
#define CP15_TPIDRPRW							15, 0, 13,  0, 4         // Software Thread ID Register, Privileged Only
#define CP15_TTBR0								15, 0,  2,  0, 0
#define CP15_TLBIALL							15, 0,  8,  7, 0
#define CP15_BPIALL								15, 0,  7,  5, 6
#define CP15_SCTLR								15, 0,  1,  0, 0
#define CP15_VBAR								15, 0, 12,  0, 0
#define ArmInvalidateBTAC()						_MoveToCoprocessor(0, CP15_BPIALL)
#define ArmDataSynchronizationBarrier()			__dsb(_ARM_BARRIER_SY)
#define ArmInstructionSynchronizationBarrier()	__isb(_ARM_BARRIER_SY)
#define DisableInterrupt()						_disable()
#define ArmEnableInterrupt()					_enable()
#define ArmMoveToProcessor						_MoveToCoprocessor
#endif

// Switch to "real" mode and never look back.
// And we do not need boot application context in this application.
void SwitchToRealModeContext(PBL_FIRMWARE_DESCRIPTOR FirmwareDescriptor)
{

	// We are currently in the APPLICATION context.
	// We need to switch to the Firmware context FIRST otherwise things blow up.
	unsigned int InterruptState = FirmwareDescriptor->InterruptState;

	// Disable IRQ
	DisableInterrupt();

	// First up, switch MM state
	unsigned long Value = FirmwareDescriptor->MmState.HardwarePageDirectory |
		FirmwareDescriptor->MmState.TTB_Config;

	ArmMoveToProcessor(Value, CP15_TTBR0);
	ArmInstructionSynchronizationBarrier();

	ArmMoveToProcessor(0, CP15_TLBIALL);
	ArmInvalidateBTAC();
	ArmDataSynchronizationBarrier();
	ArmInstructionSynchronizationBarrier();

	// Next up, set the exception state
	ArmMoveToProcessor(FirmwareDescriptor->ExceptionState.IdSvcRW, CP15_TPIDRPRW);
	ArmDataSynchronizationBarrier();

	ArmMoveToProcessor(FirmwareDescriptor->ExceptionState.Control, CP15_SCTLR);
	ArmInvalidateBTAC();
	ArmDataSynchronizationBarrier();
	ArmInstructionSynchronizationBarrier();

	ArmMoveToProcessor(FirmwareDescriptor->ExceptionState.Vbar, CP15_VBAR);
	ArmInstructionSynchronizationBarrier();

	// Restore IRQ if necessary
	if (InterruptState) ArmEnableInterrupt();

}
EfiEntry(
    _In_ EFI_HANDLE ImageHandle,
    _In_ EFI_SYSTEM_TABLE *SystemTable);
NTSTATUS BlApplicationEntry(
	_In_ PBOOT_APPLICATION_PARAMETER_BLOCK BootAppParameters,
	_In_ PBL_LIBRARY_PARAMETERS LibraryParameters
)
{

	// Get excited now
	PBL_LIBRARY_PARAMETERS	LibraryParams;
	PBL_FIRMWARE_DESCRIPTOR FirmwareDescriptor;
	UINT32 ParamPointer;

	if (!BootAppParameters)
	{
		// Invalid parameter
		return STATUS_INVALID_PARAMETER;
	}

	LibraryParams = LibraryParameters;
	ParamPointer = (UINT32) BootAppParameters;
	FirmwareDescriptor = (PBL_FIRMWARE_DESCRIPTOR) (ParamPointer + BootAppParameters->FirmwareParametersOffset);

	// Switch mode
	SwitchToRealModeContext(FirmwareDescriptor);

	// Do what ever you want now
	if (FirmwareDescriptor->SystemTable)
	{

		#if 1
		EfiEntry(
			FirmwareDescriptor->ImageHandle,
			FirmwareDescriptor->SystemTable
		);
		#endif
		FirmwareDescriptor->SystemTable->ConOut->OutputString(FirmwareDescriptor->SystemTable->ConOut, L"UEFI EntryPoint: Starting freeldr from UEFI");

	}

	// We are done
	return STATUS_SUCCESS;

}