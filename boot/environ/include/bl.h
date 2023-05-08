/*
 * COPYRIGHT:       See COPYING.ARM in the top level directory
 * PROJECT:         ReactOS UEFI Boot Library
 * FILE:            boot/environ/include/bl.h
 * PURPOSE:         Main Boot Library Header
 * PROGRAMMER:      Alex Ionescu (alex.ionescu@reactos.org)
 */

#ifndef _BL_H
#define _BL_H

/* INCLUDES ******************************************************************/

/* C Headers */
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

/* NT Base Headers */
#include <ntifs.h>

/* NDK Headers */
#include <ntndk.h>

/* NT SafeInt Header */
#include <ntintsafe.h>

/* PE Headers */
#include <ntimage.h>

/* ACPI Headers */
#include <drivers/acpi/acpi.h>

/* UEFI Headers */
#include <Uefi.h>
#include <DevicePath.h>
#include <LoadedImage.h>
#include <GraphicsOutput.h>
#include <UgaDraw.h>
#include <BlockIo.h>
#include <Acpi.h>
#include <GlobalVariable.h>

/* Registry Headers */
#include <cmlib.h>

/* Boot Library Definitions Header */
#include "bltypes.h"

/* INLINE ROUTINES ***********************************************************/

FORCEINLINE
VOID
BlSetupDefaultParameters (
    _Out_ PBL_LIBRARY_PARAMETERS LibraryParameters
    )
{
    BL_LIBRARY_PARAMETERS DefaultParameters =
    {
        0x20,
        BlVirtual,
        1024,
        2 * 1024 * 1024,
        0,
        NULL,
        0,
        NULL
    };

    /* Copy the defaults */
    RtlCopyMemory(LibraryParameters, &DefaultParameters, sizeof(*LibraryParameters));
}

FORCEINLINE
VOID
MmMdInitializeListHead (
    _In_ PBL_MEMORY_DESCRIPTOR_LIST List
    )
{
    /* Initialize the list */
    InitializeListHead(&List->ListHead);
    List->First = &List->ListHead;
    List->This = NULL;
    List->Type = 0;
}

FORCEINLINE
PVOID
PhysicalAddressToPtr (
    _In_ PHYSICAL_ADDRESS PhysicalAddress)
{
    return (PVOID)(ULONG_PTR)PhysicalAddress.QuadPart;
}

/* INITIALIZATION ROUTINES ***************************************************/

NTSTATUS
BlInitializeLibrary(
    _In_ PBOOT_APPLICATION_PARAMETER_BLOCK BootAppParameters,
    _In_ PBL_LIBRARY_PARAMETERS LibraryParameters
    );

NTSTATUS
BlpArchInitialize (
    _In_ ULONG Phase
    );

NTSTATUS
BlpFwInitialize (
    _In_ ULONG Phase,
    _In_ PBL_FIRMWARE_DESCRIPTOR FirmwareParameters
    );

NTSTATUS
BlpMmInitialize (
    _In_ PBL_MEMORY_DATA MemoryData,
    _In_ BL_TRANSLATION_TYPE TranslationType,
    _In_ PBL_LIBRARY_PARAMETERS LibraryParameters
    );

NTSTATUS
MmBaInitialize (
    VOID
    );

NTSTATUS
MmPaInitialize (
    _In_ PBL_MEMORY_DATA MemoryData,
    _In_ ULONG MinimumPages
    );

NTSTATUS
MmArchInitialize (
    _In_ ULONG Phase,
    _In_ PBL_MEMORY_DATA MemoryData,
    _In_ BL_TRANSLATION_TYPE TranslationType,
    _In_ BL_TRANSLATION_TYPE LibraryTranslationType
    );

NTSTATUS
MmHaInitialize (
    _In_ ULONG HeapSize,
    _In_ ULONG HeapAttributes
    );

VOID
MmMdInitialize (
    _In_ ULONG Phase,
    _In_ PBL_LIBRARY_PARAMETERS LibraryParameters
    );

NTSTATUS
BlpDeviceInitialize (
    VOID
    );

NTSTATUS
BlpIoInitialize (
    VOID
    );

NTSTATUS
BlpFileInitialize (
    VOID
    );

NTSTATUS
BlpDisplayInitialize (
    _In_ ULONG Flags
    );

NTSTATUS
BlpDisplayReinitialize (
    VOID
    );

VOID
BlDestroyLibrary (
    VOID
    );

NTSTATUS
BcInitialize (
    VOID
    );

/* FIRMWARE ROUTINES *********************************************************/

VOID
EfiPrintf (
    _In_ PWCHAR Format,
    ...
    );

NTSTATUS
BlFwGetParameters(
    _In_ PBL_FIRMWARE_DESCRIPTOR Parameters
    );

NTSTATUS
BlFwEnumerateDevice (
    _In_ PBL_DEVICE_DESCRIPTOR Device
    );

NTSTATUS
EfiAllocatePages (
    _In_ ULONG Type,
    _In_ ULONG Pages,
    _Inout_ EFI_PHYSICAL_ADDRESS* Memory
    );

NTSTATUS
EfiStall (
    _In_ ULONG StallTime
    );

NTSTATUS
EfiConInExReset (
    VOID
    );

NTSTATUS
EfiConInReset (
    VOID
    );

NTSTATUS
EfiConOutOutputString (
    _In_ SIMPLE_TEXT_OUTPUT_INTERFACE *TextInterface,
    _In_ PWCHAR String
    );

NTSTATUS
EfiConOutQueryMode (
    _In_ SIMPLE_TEXT_OUTPUT_INTERFACE *TextInterface,
    _In_ ULONG Mode,
    _In_ UINTN* Columns,
    _In_ UINTN* Rows
    );

NTSTATUS
EfiConOutSetMode (
    _In_ SIMPLE_TEXT_OUTPUT_INTERFACE *TextInterface,
    _In_ ULONG Mode
    );

VOID
EfiConOutReadCurrentMode (
    _In_ SIMPLE_TEXT_OUTPUT_INTERFACE *TextInterface,
    _Out_ EFI_SIMPLE_TEXT_OUTPUT_MODE* Mode
    );

NTSTATUS
EfiConOutSetAttribute (
    _In_ SIMPLE_TEXT_OUTPUT_INTERFACE *TextInterface,
    _In_ ULONG Attribute
    );

NTSTATUS
EfiConOutSetCursorPosition (
    _In_ SIMPLE_TEXT_OUTPUT_INTERFACE *TextInterface,
    _In_ ULONG Column,
    _In_ ULONG Row
    );

NTSTATUS
EfiConOutEnableCursor (
    _In_ SIMPLE_TEXT_OUTPUT_INTERFACE *TextInterface,
    _In_ BOOLEAN Visible
    );

NTSTATUS
EfiLocateHandleBuffer (
    _In_ EFI_LOCATE_SEARCH_TYPE SearchType,
    _In_ EFI_GUID *Protocol,
    _Inout_ PULONG HandleCount,
    _Inout_ EFI_HANDLE** Buffer
    );

NTSTATUS
EfiOpenProtocol (
    _In_ EFI_HANDLE Handle,
    _In_ EFI_GUID *Protocol,
    _Out_ PVOID* Interface
    );

NTSTATUS
EfiCloseProtocol (
    _In_ EFI_HANDLE Handle,
    _In_ EFI_GUID *Protocol
    );

NTSTATUS
EfiGopGetCurrentMode (
    _In_ EFI_GRAPHICS_OUTPUT_PROTOCOL *GopInterface,
    _Out_ UINTN* Mode,
    _Out_ EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Information
    );

NTSTATUS
EfiGopSetMode (
    _In_ EFI_GRAPHICS_OUTPUT_PROTOCOL *GopInterface,
    _In_ ULONG Mode
    );

VOID
EfiGopGetFrameBuffer (
    _In_ EFI_GRAPHICS_OUTPUT_PROTOCOL *GopInterface,
    _Out_ PHYSICAL_ADDRESS* FrameBuffer,
    _Out_ UINTN *FrameBufferSize
    );

VOID
EfiResetSystem (
    _In_ EFI_RESET_TYPE ResetType
    );

EFI_DEVICE_PATH*
EfiGetLeafNode (
    _In_ EFI_DEVICE_PATH *DevicePath
    );

EFI_DEVICE_PATH *
EfiIsDevicePathParent (
    _In_ EFI_DEVICE_PATH *DevicePath1,
    _In_ EFI_DEVICE_PATH *DevicePath2
    );

NTSTATUS
EfipGetRsdt (
    _Out_ PPHYSICAL_ADDRESS FoundRsdt
    );

NTSTATUS
EfiFreePages (
    _In_ ULONG Pages,
    _In_ EFI_PHYSICAL_ADDRESS PhysicalAddress
    );

/* PLATFORM TIMER ROUTINES ***************************************************/

NTSTATUS
BlpTimeCalibratePerformanceCounter (
    VOID
    );

ULONGLONG
BlTimeQueryPerformanceCounter (
    _Out_opt_ PLARGE_INTEGER Frequency
    );

ULONGLONG
BlArchGetPerformanceCounter (
    VOID
    );

/* RESOURCE LOCALE INTERNATIONALIZATION ROUTINES *****************************/

NTSTATUS
BlpDisplayRegisterLocale (
    _In_ PWCHAR Locale
    );

/* FONT ROUTINES *************************************************************/

VOID
BfiFreeDeferredFontFile (
    _In_ PBL_DEFERRED_FONT_FILE DeferredFontFile
    );

NTSTATUS
BfLoadFontFile (
    _In_ PBL_DEVICE_DESCRIPTOR Device,
    _In_ PWCHAR FontPath
    );

NTSTATUS
BfLoadDeferredFontFiles (
    VOID
    );

NTSTATUS
BfClearScreen  (
    _In_ PBL_GRAPHICS_CONSOLE Console
    );

NTSTATUS
BfClearToEndOfLine (
    _In_ PBL_GRAPHICS_CONSOLE Console
    );

/* FILESYSTEM ROUTINES *******************************************************/

NTSTATUS
FatInitialize (
    VOID
    );

NTSTATUS
FatMount (
    _In_ ULONG DeviceId,
    _In_ ULONG Unknown,
    _Out_ PBL_FILE_ENTRY* FileEntry
    );

NTSTATUS
EtfsInitialize (
    VOID
    );

NTSTATUS
EtfsMount (
    _In_ ULONG DeviceId,
    _In_ ULONG Unknown,
    _Out_ PBL_FILE_ENTRY* FileEntry
    );

/* DEBUG ROUTINES ************************************************************/

BOOLEAN
BlBdDebuggerEnabled (
    VOID
    );

NTSTATUS
BlBdPullRemoteFile (
    _In_ PWCHAR FilePath,
    _Out_ PVOID BaseAddress,
    _Out_ PULONGLONG FileSize
    );

VOID
BlStatusPrint (
    _In_ PCWCH Format,
    ...
    );

VOID
BlStatusError (
    _In_ ULONG ErrorCode,
    _In_ ULONG Parameter1,
    _In_ ULONG_PTR Parameter2,
    _In_ ULONG_PTR Parameter3,
    _In_ ULONG_PTR Parameter4
    );

/* UTILITY ROUTINES **********************************************************/

VOID
BlArchCpuId (
    _In_ ULONG Function,
    _In_ ULONG SubFunction,
    _Out_ PCPU_INFO Result
    );

CPU_VENDORS
BlArchGetCpuVendor (
    VOID
    );

BOOLEAN
BlArchIsCpuIdFunctionSupported (
    _In_ ULONG Function
    );

VOID
BlUtlUpdateProgress (
    _In_ ULONG Percentage,
    _Out_opt_ PBOOLEAN Completed
    );

NTSTATUS
BlUtlGetAcpiTable (
    _Out_ PVOID* TableAddress,
    _In_ ULONG Signature
    );

NTSTATUS
BlUtlInitialize (
    VOID
    );

NTSTATUS
BlUtlRegisterProgressRoutine (
    VOID
    );

ULONG
BlUtlCheckSum (
    _In_ ULONG PartialSum,
    _In_ PUCHAR Buffer,
    _In_ ULONG Length,
    _In_ ULONG Flags
    );

NTSTATUS
BlGetApplicationBaseAndSize (
    _Out_ PVOID* ImageBase,
    _Out_ PULONG ImageSize
    );

VOID
BlDestroyBootEntry (
    _In_ PBL_LOADED_APPLICATION_ENTRY AppEntry
    );

NTSTATUS
BlPdQueryData (
    _In_ const GUID* DataGuid,
    _In_ PVOID Unknown,
    _Inout_ PBL_PD_DATA_BLOB DataBlob
    );

/* FIRMWARE UTILITY ROUTINES *************************************************/

EFI_STATUS
EfiGetEfiStatusCode(
    _In_ NTSTATUS Status
    );

NTSTATUS
EfiGetNtStatusCode (
    _In_ EFI_STATUS EfiStatus
    );

VOID
BlFwReboot (
    VOID
    );

NTSTATUS
MmFwFreePages (
    _In_ ULONG BasePage,
    _In_ ULONG PageCount
    );

PGUID
BlGetApplicationIdentifier (
    VOID
    );

NTSTATUS
BlpSecureBootEFIIsEnabled (
    VOID
    );

NTSTATUS
BlSecureBootIsEnabled (
    _Out_ PBOOLEAN SecureBootEnabled
    );

NTSTATUS
BlSecureBootCheckForFactoryReset (
    VOID
    );

/* RESOURCE ROUTINES *********************************************************/

PWCHAR
BlResourceFindMessage (
    _In_ ULONG MsgId
    );

PWCHAR
BlResourceFindHtml (
    VOID
    );

NTSTATUS
BlpResourceInitialize (
    VOID
    );

/* TABLE ROUTINES ************************************************************/

NTSTATUS
BlTblMap (
    _In_ PVOID *Table,
    _In_ ULONG Count,
    _In_ PBL_TBL_MAP_ROUTINE MapCallback
    );

PVOID
BlTblFindEntry (
    _In_ PVOID *Table,
    _In_ ULONG Count,
    _Out_ PULONG EntryIndex,
    _In_ PBL_TBL_LOOKUP_ROUTINE Callback,
    _In_ PVOID Argument1,
    _In_ PVOID Argument2,
    _In_ PVOID Argument3,
    _In_ PVOID Argument4
    );

NTSTATUS
BlTblSetEntry (
    _Inout_ PVOID** Table,
    _Inout_ PULONG Count,
    _In_ PVOID Entry,
    _Out_ PULONG EntryIndex,
    _In_ PBL_TBL_SET_ROUTINE Callback
    );

NTSTATUS
TblDoNotPurgeEntry (
    _In_ PVOID Entry
    );

/* HASH TABLE ROUTINES *******************************************************/

NTSTATUS
BlHtStore (
    _In_ ULONG TableId,
    _In_ PBL_HASH_ENTRY Entry,
    _In_ PVOID Data,
    _In_ ULONG DataSize
    );

NTSTATUS
BlHtDelete (
    _In_ ULONG TableId,
    _In_ PBL_HASH_ENTRY Entry
    );

NTSTATUS
BlHtLookup (
    _In_ ULONG TableId,
    _In_ PBL_HASH_ENTRY Entry,
    _Out_ PBL_HASH_VALUE *Value
    );

NTSTATUS
BlHtCreate (
    _In_ ULONG Size,
    _In_ PBL_HASH_TABLE_HASH_FUNCTION HashFunction,
    _In_ PBL_HASH_TABLE_COMPARE_FUNCTION CompareFunction,
    _Out_ PULONG Id
    );

/* BCD OPTION ROUTINES *******************************************************/

PBL_BCD_OPTION
MiscGetBootOption (
    _In_ PBL_BCD_OPTION List,
    _In_ ULONG Type
    );

ULONG
BlGetBootOptionListSize (
    _In_ PBL_BCD_OPTION BcdOption
    );

ULONG
BlGetBootOptionSize (
    _In_ PBL_BCD_OPTION BcdOption
    );

NTSTATUS
BlGetBootOptionString (
    _In_ PBL_BCD_OPTION List,
    _In_ ULONG Type,
    _Out_ PWCHAR* Value
    );

NTSTATUS
BlGetBootOptionInteger (
    _In_ PBL_BCD_OPTION List,
    _In_ ULONG Type,
    _Out_ PULONGLONG Value
    );

NTSTATUS
BlGetBootOptionBoolean (
    _In_ PBL_BCD_OPTION List,
    _In_ ULONG Type,
    _Out_ PBOOLEAN Value
    );

NTSTATUS
BlpGetBootOptionIntegerList (
    _In_ PBL_BCD_OPTION List,
    _In_ ULONG Type,
    _Out_ PULONGLONG* Value,
    _Out_ PULONGLONG Count,
    _In_ BOOLEAN NoCopy
    );

NTSTATUS
BlGetBootOptionDevice (
    _In_ PBL_BCD_OPTION List,
    _In_ ULONG Type,
    _Out_ PBL_DEVICE_DESCRIPTOR* Value,
    _In_opt_ PBL_BCD_OPTION* ExtraOptions
    );

NTSTATUS
BlGetBootOptionGuid (
    _In_ PBL_BCD_OPTION List,
    _In_ ULONG Type,
    _Out_ PGUID Value
    );

NTSTATUS
BlGetBootOptionGuidList (
    _In_ PBL_BCD_OPTION List,
    _In_ ULONG Type,
    _Out_ PGUID *Value,
    _In_ PULONG Count
    );

NTSTATUS
BlCopyBootOptions (
    _In_ PBL_BCD_OPTION OptionList,
    _Out_ PBL_BCD_OPTION *CopiedOptions
    );

NTSTATUS
BlAppendBootOptionBoolean (
    _In_ PBL_LOADED_APPLICATION_ENTRY AppEntry,
    _In_ ULONG OptionId,
    _In_ BOOLEAN Value
    );

NTSTATUS
BlAppendBootOptionInteger (
    _In_ PBL_LOADED_APPLICATION_ENTRY AppEntry,
    _In_ ULONG OptionId,
    _In_ ULONGLONG Value
    );

NTSTATUS
BlAppendBootOptionString (
    _In_ PBL_LOADED_APPLICATION_ENTRY AppEntry,
    _In_ ULONG OptionId,
    _In_ PWCHAR OptionString
    );

NTSTATUS
BlAppendBootOptions (
    _In_ PBL_LOADED_APPLICATION_ENTRY AppEntry,
    _In_ PBL_BCD_OPTION Options
    );

VOID
BlRemoveBootOption (
    _In_ PBL_BCD_OPTION List,
    _In_ ULONG Type
    );

NTSTATUS
BlReplaceBootOptions (
    _In_ PBL_LOADED_APPLICATION_ENTRY AppEntry,
    _In_ PBL_BCD_OPTION NewOptions
    );

/* BOOT REGISTRY ROUTINES ****************************************************/

VOID
BiCloseKey (
    _In_ HANDLE KeyHandle
    );

NTSTATUS
BiOpenKey(
    _In_ HANDLE ParentHandle,
    _In_ PWCHAR KeyName,
    _Out_ PHANDLE Handle
    );

NTSTATUS
BiLoadHive (
    _In_ PBL_FILE_PATH_DESCRIPTOR FilePath,
    _Out_ PHANDLE HiveHandle
    );

NTSTATUS
BiGetRegistryValue (
    _In_ HANDLE KeyHandle,
    _In_ PWCHAR ValueName,
    _In_ ULONG Type,
    _Out_ PVOID* Buffer,
    _Out_ PULONG ValueLength
    );

NTSTATUS
BiEnumerateSubKeys (
    _In_ HANDLE KeyHandle,
    _Out_ PWCHAR** SubKeyList,
    _Out_ PULONG SubKeyCount
    );

NTSTATUS
BiDeleteKey (
    _In_ HANDLE KeyHandle
    );

VOID
BiDereferenceHive (
    _In_ HANDLE KeyHandle
    );

/* CONTEXT ROUTINES **********************************************************/

VOID
BlpArchSwitchContext (
    _In_ BL_ARCH_MODE NewMode
    );

VOID
BlpArchEnableTranslation (
    VOID
    );

VOID
Archx86TransferTo32BitApplicationAsm (
    VOID
    );

/* MEMORY DESCRIPTOR ROUTINES ************************************************/

VOID
MmMdDbgDumpList (
    _In_ PBL_MEMORY_DESCRIPTOR_LIST DescriptorList,
    _In_opt_ ULONG MaxCount
);

VOID
MmMdInitializeList (
    _In_ PBL_MEMORY_DESCRIPTOR_LIST DescriptorList,
    _In_ ULONG Type,
    _In_ PLIST_ENTRY ListHead
    );

PBL_MEMORY_DESCRIPTOR
MmMdFindDescriptor (
    _In_ ULONG WhichList,
    _In_ ULONG Flags,
    _In_ ULONGLONG Page
    );

PBL_MEMORY_DESCRIPTOR
MmMdFindDescriptorFromMdl (
    _In_ PBL_MEMORY_DESCRIPTOR_LIST MdList,
    _In_ ULONG Flags,
    _In_ ULONGLONG Page
    );

NTSTATUS
MmMdCopyList (
    _In_ PBL_MEMORY_DESCRIPTOR_LIST DestinationList,
    _In_ PBL_MEMORY_DESCRIPTOR_LIST SourceList,
    _In_opt_ PBL_MEMORY_DESCRIPTOR ListDescriptor,
    _Out_ PULONG ActualCount,
    _In_ ULONG Count,
    _In_ ULONG Flags
    );

ULONG
MmMdCountList (
    _In_ PBL_MEMORY_DESCRIPTOR_LIST MdList
    );

VOID
MmMdFreeList(
    _In_ PBL_MEMORY_DESCRIPTOR_LIST MdList
    );

PBL_MEMORY_DESCRIPTOR
MmMdInitByteGranularDescriptor (
    _In_ ULONG Flags,
    _In_ BL_MEMORY_TYPE Type,
    _In_ ULONGLONG BasePage,
    _In_ ULONGLONG VirtualPage,
    _In_ ULONGLONG PageCount
    );

VOID
MmMdFreeGlobalDescriptors (
    VOID
    );

NTSTATUS
MmMdAddDescriptorToList (
    _In_ PBL_MEMORY_DESCRIPTOR_LIST MdList,
    _In_ PBL_MEMORY_DESCRIPTOR MemoryDescriptor,
    _In_ ULONG Flags
    );

NTSTATUS
MmMdTruncateDescriptors (
    _In_ PBL_MEMORY_DESCRIPTOR_LIST MdList,
    _In_ PBL_MEMORY_DESCRIPTOR_LIST NewList,
    _In_ ULONGLONG BasePage
    );

VOID
MmMdRemoveDescriptorFromList (
    _In_ PBL_MEMORY_DESCRIPTOR_LIST MdList,
    _In_ PBL_MEMORY_DESCRIPTOR Entry
    );

BOOLEAN
MmMdFindSatisfyingRegion (
    _In_ PBL_MEMORY_DESCRIPTOR Descriptor,
    _Out_ PBL_MEMORY_DESCRIPTOR NewDescriptor,
    _In_ ULONGLONG Pages,
    _In_ PBL_ADDRESS_RANGE BaseRange,
    _In_ PBL_ADDRESS_RANGE VirtualRange,
    _In_ BOOLEAN TopDown,
    _In_ BL_MEMORY_TYPE MemoryType,
    _In_ ULONG Flags,
    _In_ ULONG Alignment
    );

NTSTATUS
MmMdRemoveRegionFromMdlEx (
    __in PBL_MEMORY_DESCRIPTOR_LIST MdList,
    __in ULONG Flags,
    __in ULONGLONG BasePage,
    __in ULONGLONG PageCount,
    __in PBL_MEMORY_DESCRIPTOR_LIST NewMdList
    );

NTSTATUS
MmMdFreeDescriptor (
    _In_ PBL_MEMORY_DESCRIPTOR MemoryDescriptor
    );

/* PAGE ALLOCATOR ROUTINES ***************************************************/

NTSTATUS
MmPaTruncateMemory (
    _In_ ULONGLONG BasePage
    );

NTSTATUS
BlMmAllocatePhysicalPages(
    _Inout_ PPHYSICAL_ADDRESS Address,
    _In_ BL_MEMORY_TYPE MemoryType,
    _In_ ULONGLONG PageCount,
    _In_ ULONG Attributes,
    _In_ ULONG Alignment
    );

NTSTATUS
MmPapAllocatePhysicalPagesInRange (
    _Inout_ PPHYSICAL_ADDRESS BaseAddress,
    _In_ BL_MEMORY_TYPE MemoryType,
    _In_ ULONGLONG Pages,
    _In_ ULONG Attributes,
    _In_ ULONG Alignment,
    _In_ PBL_MEMORY_DESCRIPTOR_LIST NewList,
    _In_opt_ PBL_ADDRESS_RANGE Range,
    _In_ ULONG RangeType
    );

NTSTATUS
MmPaReleaseSelfMapPages (
    _In_ PHYSICAL_ADDRESS Address
    );

NTSTATUS
MmPaReserveSelfMapPages (
    _Inout_ PPHYSICAL_ADDRESS PhysicalAddress,
    _In_ ULONG Alignment,
    _In_ ULONG PageCount
    );

NTSTATUS
BlMmFreePhysicalPages (
    _In_ PHYSICAL_ADDRESS Address
    );

NTSTATUS
MmPapFreePages (
    _In_ PVOID Address,
    _In_ ULONG WhichList
    );

NTSTATUS
MmPapAllocatePagesInRange (
    _Inout_ PVOID* PhysicalAddress,
    _In_ BL_MEMORY_TYPE MemoryType,
    _In_ ULONGLONG Pages,
    _In_ ULONG Attributes,
    _In_ ULONG Alignment,
    _In_opt_ PBL_ADDRESS_RANGE Range,
    _In_ ULONG Type
    );

NTSTATUS
MmFwGetMemoryMap (
    _Out_ PBL_MEMORY_DESCRIPTOR_LIST MemoryMap,
    _In_ ULONG Flags
    );

NTSTATUS
BlpMmInitializeConstraints (
    VOID
    );

NTSTATUS
BlMmRemoveBadMemory (
    VOID
    );

NTSTATUS
BlMmGetMemoryMap (
    _In_ PLIST_ENTRY MemoryMap,
    _In_ PBL_BUFFER_DESCRIPTOR MemoryParameters,
    _In_ ULONG WhichTypes,
    _In_ ULONG Flags
    );

/* VIRTUAL MEMORY ROUTINES ***************************************************/

NTSTATUS
MmSelectMappingAddress (
    _Out_ PVOID* MappingAddress,
    _In_ PVOID PreferredAddress,
    _In_ ULONGLONG Size,
    _In_ ULONG AllocationAttributes,
    _In_ ULONG Flags,
    _In_ PHYSICAL_ADDRESS PhysicalAddress
    );

NTSTATUS
MmMapPhysicalAddress (
    _Inout_ PPHYSICAL_ADDRESS PhysicalAddress,
    _Out_ PVOID* VirtualAddress,
    _Inout_ PULONGLONG Size,
    _In_ ULONG CacheAttributes
    );

NTSTATUS
MmUnmapVirtualAddress (
    _Inout_ PVOID* VirtualAddress,
    _Inout_ PULONGLONG Size
    );

NTSTATUS
BlMmMapPhysicalAddressEx (
    _In_ PVOID* VirtualAddress,
    _In_ ULONG Attributes,
    _In_ ULONGLONG Size,
    _In_ PHYSICAL_ADDRESS PhysicalAddress
    );

NTSTATUS
BlMmUnmapVirtualAddressEx (
    _In_ PVOID VirtualAddress,
    _In_ ULONGLONG Size
    );

BOOLEAN
BlMmTranslateVirtualAddress (
    _In_ PVOID VirtualAddress,
    _Out_ PPHYSICAL_ADDRESS PhysicalAddress
    );

BOOLEAN
MmArchTranslateVirtualAddress (
    _In_ PVOID VirtualAddress,
    _Out_opt_ PPHYSICAL_ADDRESS PhysicalAddress,
    _Out_opt_ PULONG CachingFlags
    );

/* BLOCK ALLOCATOR ROUTINES **************************************************/

NTSTATUS
BlpMmCreateBlockAllocator (
    VOID
    );

/* HEAP ALLOCATOR ROUTINES ***************************************************/

PVOID
BlMmAllocateHeap (
    _In_ SIZE_T Size
    );

NTSTATUS
BlMmFreeHeap (
    _In_ PVOID Buffer
    );

/* DISPLAY ROUTINES **********************************************************/

VOID
BlDisplayGetTextCellResolution (
    _Out_ PULONG TextWidth,
    _Out_ PULONG TextHeight
    );

NTSTATUS
BlDisplaySetScreenResolution (
    VOID
    );

NTSTATUS
BlDisplayGetScreenResolution (
    _Out_ PULONG HRes,
    _Out_ PULONG Vres
    );

VOID
BlDisplayInvalidateOemBitmap (
    VOID
    );

PBITMAP
BlDisplayGetOemBitmap (
    _Out_ PCOORD Offset,
    _Out_opt_ PULONG Flags
    );

BOOLEAN
BlDisplayValidOemBitmap (
    VOID
    );

NTSTATUS
BlDisplayClearScreen (
    VOID
    );

NTSTATUS
BlDisplaySetCursorType (
    _In_ ULONG Type
    );

/* I/O ROUTINES **************************************************************/

NTSTATUS
BlpIoRegisterDestroyRoutine (
    _In_ PBL_IO_DESTROY_ROUTINE DestroyRoutine
    );

NTSTATUS
BlDeviceClose (
    _In_ ULONG DeviceId
    );

BOOLEAN
BlDeviceIsVirtualPartitionDevice (
    _In_ PBL_DEVICE_DESCRIPTOR InputDevice,
    _Outptr_ PBL_DEVICE_DESCRIPTOR* VirtualDevice
    );

NTSTATUS
BlpDeviceOpen (
    _In_ PBL_DEVICE_DESCRIPTOR Device,
    _In_ ULONG Flags,
    _In_ ULONG Unknown,
    _Out_ PULONG DeviceId
    );

NTSTATUS
BlDeviceGetInformation (
    _In_ ULONG DeviceId,
    _Out_ PBL_DEVICE_INFORMATION DeviceInformation
    );

NTSTATUS
BlDeviceSetInformation (
    _In_ ULONG DeviceId,
    _In_ PBL_DEVICE_INFORMATION DeviceInformation
    );

NTSTATUS
BlDeviceReadAtOffset (
    _In_ ULONG DeviceId,
    _In_ ULONG Size,
    _In_ ULONGLONG Offset,
    _In_ PVOID Buffer,
    _Out_ PULONG BytesRead
    );

/* IMAGE ROUTINES ************************************************************/

NTSTATUS
BlImgLoadImageWithProgress2 (
    _In_ ULONG DeviceId,
    _In_ BL_MEMORY_TYPE MemoryType,
    _In_ PWCHAR FileName,
    _Inout_ PVOID* MappedBase,
    _Inout_ PULONG MappedSize,
    _In_ ULONG ImageFlags,
    _In_ BOOLEAN ShowProgress,
    _Out_opt_ PUCHAR* HashBuffer,
    _Out_opt_ PULONG HashSize
    );

PIMAGE_SECTION_HEADER
BlImgFindSection (
    _In_ PVOID ImageBase,
    _In_ ULONG ImageSize
    );

NTSTATUS
BlImgLoadBootApplication (
    _In_ PBL_LOADED_APPLICATION_ENTRY BootEntry,
    _Out_ PULONG AppHandle
    );

NTSTATUS
BlImgStartBootApplication (
    _In_ ULONG AppHandle,
    _Inout_ PBL_RETURN_ARGUMENTS ReturnArguments
    );

NTSTATUS
BlImgUnloadBootApplication (
    _In_ ULONG AppHandle
    );

VOID
BlImgQueryCodeIntegrityBootOptions (
    _In_ PBL_LOADED_APPLICATION_ENTRY ApplicationEntry,
    _Out_ PBOOLEAN IntegrityChecksDisabled,
    _Out_ PBOOLEAN TestSigning
    );

/* FILE I/O ROUTINES *********************************************************/

NTSTATUS
BlFileClose (
    _In_ ULONG FileId
    );

NTSTATUS
BlFileReadAtOffsetEx (
    _In_ ULONG FileId,
    _In_ ULONG Size,
    _In_ ULONGLONG ByteOffset,
    _In_ PVOID Buffer,
    _Out_ PULONG BytesReturned,
    _In_ ULONG Flags
    );

NTSTATUS
BlFileGetInformation (
    _In_ ULONG FileId,
    _In_ PBL_FILE_INFORMATION FileInfo
    );

NTSTATUS
BlFileOpen (
    _In_ ULONG DeviceId,
    _In_ PWCHAR FileName,
    _In_ ULONG Flags,
    _Out_ PULONG FileId
    );

/* BLOCK I/O ROUTINES *******************************************************/

NTSTATUS
BlockIoEfiCompareDevice (
    _In_ PBL_DEVICE_DESCRIPTOR Device,
    _In_ EFI_HANDLE Handle
    );

/* INPUT CONSOLE ROUTINES ****************************************************/

VOID
ConsoleInputLocalDestruct (
    _In_ struct _BL_INPUT_CONSOLE* Console
    );

NTSTATUS
ConsoleInputBaseReinitialize (
    _In_ struct _BL_INPUT_CONSOLE* Console
    );

NTSTATUS
ConsoleCreateLocalInputCnsole (
    VOID
    );

/* TEXT CONSOLE ROUTINES *****************************************************/

VOID
ConsoleGraphicalDestruct (
    _In_ struct _BL_GRAPHICS_CONSOLE* Console
    );

NTSTATUS
ConsoleGraphicalClearText (
    _In_ PBL_GRAPHICS_CONSOLE Console,
    _In_ BOOLEAN LineOnly
    );

NTSTATUS
ConsoleGraphicalClearPixels  (
    _In_ PBL_GRAPHICS_CONSOLE Console,
    _In_ ULONG Color
    );

NTSTATUS
ConsoleGraphicalReinitialize (
    _In_ struct _BL_GRAPHICS_CONSOLE* Console
    );

NTSTATUS
ConsoleGraphicalSetTextState (
    _In_ PBL_GRAPHICS_CONSOLE Console,
    _In_ ULONG Mask,
    _In_ PBL_DISPLAY_STATE TextState
    );

BOOLEAN
ConsoleGraphicalIsEnabled (
    _In_ struct _BL_GRAPHICS_CONSOLE* Console
    );

NTSTATUS
ConsoleGraphicalGetGraphicalResolution (
    _In_ struct _BL_GRAPHICS_CONSOLE* Console,
    _In_ PBL_DISPLAY_MODE DisplayMode
    );

NTSTATUS
ConsoleGraphicalGetOriginalResolution (
    _In_ struct _BL_GRAPHICS_CONSOLE* Console,
    _In_ PBL_DISPLAY_MODE DisplayMode
    );

NTSTATUS
ConsoleGraphicalEnable (
    _In_ struct _BL_GRAPHICS_CONSOLE* Console,
    _In_ BOOLEAN Enable
    );

VOID
ConsoleTextLocalDestruct (
    _In_ struct _BL_TEXT_CONSOLE* Console
    );

NTSTATUS
ConsoleTextLocalReinitialize (
    _In_ struct _BL_TEXT_CONSOLE* Console
    );

NTSTATUS
ConsoleTextBaseGetTextState (
    _In_ struct _BL_TEXT_CONSOLE* Console,
    _Out_ PBL_DISPLAY_STATE TextState
    );

NTSTATUS
ConsoleTextLocalSetTextState (
    _In_ struct _BL_TEXT_CONSOLE* Console,
    _In_ ULONG Flags,
    _In_ PBL_DISPLAY_STATE TextState
    );

NTSTATUS
ConsoleTextBaseGetTextResolution (
    _In_ struct _BL_TEXT_CONSOLE* Console,
    _Out_ PULONG TextResolution
    );

NTSTATUS
ConsoleTextLocalSetTextResolution (
    _In_ struct _BL_TEXT_CONSOLE* Console,
    _In_ ULONG NewTextResolution,
    _Out_ PULONG OldTextResolution
    );

NTSTATUS
ConsoleTextLocalClearText (
    _In_ struct _BL_TEXT_CONSOLE* Console,
    _In_ BOOLEAN LineOnly
    );

NTSTATUS
ConsoleTextLocalWriteText (
    _In_ struct _BL_TEXT_CONSOLE* Console,
    _In_ PCHAR Text,
    _In_ ULONG Attribute
    );

NTSTATUS
ConsoleTextLocalConstruct (
    _In_ PBL_TEXT_CONSOLE TextConsole,
    _In_ BOOLEAN Activate
    );

BOOLEAN
ConsolepFindResolution (
    _In_ PBL_DISPLAY_MODE Mode,
    _In_ PBL_DISPLAY_MODE List,
    _In_ ULONG MaxIndex
    );

NTSTATUS
ConsoleFirmwareTextClear (
    _In_ PBL_TEXT_CONSOLE Console,
    _In_ BOOLEAN LineOnly
    );

VOID
ConsoleFirmwareTextClose (
    _In_ PBL_TEXT_CONSOLE TextConsole
    );

NTSTATUS
ConsoleFirmwareTextOpen (
    _In_ PBL_TEXT_CONSOLE TextConsole
    );

NTSTATUS
ConsoleFirmwareTextSetState (
    _In_ PBL_TEXT_CONSOLE TextConsole,
    _In_ UCHAR Mask,
    _In_ PBL_DISPLAY_STATE State
    );

NTSTATUS
ConsoleGraphicalConstruct (
    _In_ PBL_GRAPHICS_CONSOLE GraphicsConsole
    );

NTSTATUS
ConsoleCreateRemoteConsole (
    _In_ PBL_TEXT_CONSOLE* TextConsole
    );

NTSTATUS
ConsoleEfiGraphicalOpenProtocol (
    _In_ PBL_GRAPHICS_CONSOLE GraphicsConsole,
    _In_ BL_GRAPHICS_CONSOLE_TYPE Type
    );

VOID
ConsoleFirmwareGraphicalClose (
    _In_ PBL_GRAPHICS_CONSOLE GraphicsConsole
    );

VOID
ConsoleFirmwareGraphicalDisable (
    _In_ PBL_GRAPHICS_CONSOLE GraphicsConsole
    );

NTSTATUS
ConsoleFirmwareGraphicalClear (
    _In_ PBL_GRAPHICS_CONSOLE Console,
    _In_ ULONG Color
    );

NTSTATUS
ConsoleFirmwareGraphicalEnable (
    _In_ PBL_GRAPHICS_CONSOLE GraphicsConsole
    );

NTSTATUS
ConsoleEfiUgaOpen (
    _In_ PBL_GRAPHICS_CONSOLE GraphicsConsole
    );

VOID
ConsoleEfiUgaClose (
    _In_ PBL_GRAPHICS_CONSOLE GraphicsConsole
    );

VOID
ConsoleEfiGopClose (
    _In_ PBL_GRAPHICS_CONSOLE GraphicsConsole
    );

NTSTATUS
ConsoleEfiGopOpen (
    _In_ PBL_GRAPHICS_CONSOLE GraphicsConsole
    );

NTSTATUS
ConsoleEfiGopEnable (
    _In_ PBL_GRAPHICS_CONSOLE GraphicsConsole
    );

NTSTATUS
ConsoleEfiUgaSetResolution  (
    _In_ PBL_GRAPHICS_CONSOLE GraphicsConsole,
    _In_ PBL_DISPLAY_MODE DisplayMode,
    _In_ ULONG DisplayModeCount
    );

NTSTATUS
ConsoleCreateLocalInputConsole (
    VOID
    );

NTSTATUS
ConsoleInputLocalEraseBuffer (
    _In_ PBL_INPUT_CONSOLE Console,
    _In_opt_ PULONG ValueToFill
    );

VOID
ConsolepClearBuffer (
    _In_ PUCHAR FrameBuffer,
    _In_ ULONG Width,
    _In_ PUCHAR FillColor,
    _In_ ULONG Height,
    _In_ ULONG ScanlineWidth,
    _In_ ULONG PixelDepth
    );

NTSTATUS
ConsolepConvertColorToPixel (
    _In_ BL_COLOR Color,
    _Out_ PUCHAR Pixel
    );

extern ULONG MmDescriptorCallTreeCount;
extern ULONG BlpApplicationFlags;
extern BL_LIBRARY_PARAMETERS BlpLibraryParameters;
extern BL_TRANSLATION_TYPE  MmTranslationType;
extern PBL_ARCH_CONTEXT CurrentExecutionContext;
extern PBL_DEVICE_DESCRIPTOR BlpBootDevice;
extern BL_LOADED_APPLICATION_ENTRY BlpApplicationEntry;
extern EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *EfiConOut;
extern EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *EfiConInEx;
extern EFI_GUID EfiGraphicsOutputProtocol;
extern EFI_GUID EfiUgaDrawProtocol;
extern EFI_GUID EfiLoadedImageProtocol;
extern EFI_GUID EfiDevicePathProtocol;
extern EFI_GUID EfiBlockIoProtocol;
extern EFI_GUID EfiSimpleTextInputExProtocol;
extern EFI_GUID EfiRootAcpiTableGuid;
extern EFI_GUID EfiRootAcpiTable10Guid;
extern EFI_GUID EfiGlobalVariable;
extern ULONG ConsoleGraphicalResolutionListFlags;
extern BL_DISPLAY_MODE ConsoleGraphicalResolutionList[];
extern BL_DISPLAY_MODE ConsoleTextResolutionList[];
extern ULONG ConsoleGraphicalResolutionListSize;
extern PVOID DspRemoteInputConsole;
extern PVOID DspLocalInputConsole;
extern WCHAR BlScratchBuffer[8192];
extern BL_MEMORY_DESCRIPTOR_LIST MmMdlMappedAllocated;
extern BL_MEMORY_DESCRIPTOR_LIST MmMdlMappedUnallocated;
extern BL_MEMORY_DESCRIPTOR_LIST MmMdlFwAllocationTracker;
extern BL_MEMORY_DESCRIPTOR_LIST MmMdlUnmappedAllocated;
extern BL_MEMORY_DESCRIPTOR_LIST MmMdlUnmappedUnallocated;
extern BL_MEMORY_DESCRIPTOR_LIST MmMdlReservedAllocated;
extern BL_MEMORY_DESCRIPTOR_LIST MmMdlBadMemory;
extern BL_MEMORY_DESCRIPTOR_LIST MmMdlTruncatedMemory;
extern BL_MEMORY_DESCRIPTOR_LIST MmMdlPersistentMemory;
extern BL_MEMORY_DESCRIPTOR_LIST MmMdlCompleteBadMemory;
extern BL_MEMORY_DESCRIPTOR_LIST MmMdlFreeVirtual;
extern BL_MEMORY_DESCRIPTOR_LIST MmMdlMappingTrackers;
extern ULONGLONG BlpTimePerformanceFrequency;
extern LIST_ENTRY RegisteredFileSystems;
extern BL_ADDRESS_RANGE MmArchKsegAddressRange;
extern ULONG_PTR MmArchTopOfApplicationAddressSpace;
extern PBL_MM_RELOCATE_SELF_MAP BlMmRelocateSelfMap;
extern PBL_MM_FLUSH_TLB BlMmFlushTlb;
extern PBL_MM_MOVE_VIRTUAL_ADDRESS_RANGE BlMmMoveVirtualAddressRange;
extern PBL_MM_ZERO_VIRTUAL_ADDRESS_RANGE BlMmZeroVirtualAddressRange;
extern PBL_STATUS_ERROR_HANDLER BlpStatusErrorHandler;

#endif
