/*
 * COPYRIGHT:       See COPYING.ARM in the top level directory
 * PROJECT:         ReactOS UEFI Boot Library
 * FILE:            boot/environ/include/bltypes.h
 * PURPOSE:         Boot Library Definitions
 * PROGRAMMER:      Alex Ionescu (alex.ionescu@reactos.org)
 */

/* DEFINES *******************************************************************/

DEFINE_GUID(BadMemoryGuid, 0x54B8275B, 0xD431, 0x473F, 0xAC, 0xFB, 0xE5, 0x36, 0xA0, 0x84, 0x94, 0xA3);

#define BL_APPLICATION_FLAG_CONVERTED_FROM_EFI          0x01

#define BL_APP_ENTRY_SIGNATURE                          "BTAPENT"

#define BOOT_APPLICATION_SIGNATURE_1                    'TOOB'
#define BOOT_APPLICATION_SIGNATURE_2                    ' PPA'

#define BOOT_MEMORY_TRANSLATION_TYPE_PHYSICAL           0
#define BOOT_MEMORY_TRANSLATION_TYPE_VIRTUAL            1

#define BOOT_APPLICATION_VERSION                        2
#define BL_MEMORY_DATA_VERSION                          1
#define BL_RETURN_ARGUMENTS_VERSION                     1
#define BL_FIRMWARE_DESCRIPTOR_VERSION                  2

#define BL_RETURN_ARGUMENTS_NO_PAE_FLAG                 0x40

#define BL_APPLICATION_ENTRY_FLAG_NO_GUID               0x01
#define BL_APPLICATION_ENTRY_BCD_OPTIONS_INTERNAL       0x02
#define BL_APPLICATION_ENTRY_WINLOAD                    0x04
#define BL_APPLICATION_ENTRY_STARTUP                    0x08
#define BL_APPLICATION_ENTRY_REBOOT_ON_ERROR            0x20
#define BL_APPLICATION_ENTRY_NTLDR                      0x40
#define BL_APPLICATION_ENTRY_BCD_OPTIONS_EXTERNAL       0x80
#define BL_APPLICATION_ENTRY_WINRESUME                  0x100
#define BL_APPLICATION_ENTRY_SETUPLDR                   0x200
#define BL_APPLICATION_ENTRY_BOOTSECTOR                 0x400
#define BL_APPLICATION_ENTRY_BOOTMGR                    0x1000
#define BL_APPLICATION_ENTRY_DISPLAY_ORDER              0x800000
#define BL_APPLICATION_ENTRY_FIXED_SEQUENCE             0x20000000
#define BL_APPLICATION_ENTRY_RECOVERY                   0x40000000

#define BL_CONTEXT_PAGING_ON                            1
#define BL_CONTEXT_INTERRUPTS_ON                        2

#define BL_MM_FLAG_USE_FIRMWARE_FOR_MEMORY_MAP_BUFFERS  0x01
#define BL_MM_FLAG_REQUEST_COALESCING                   0x02

#define BL_MM_ADD_DESCRIPTOR_COALESCE_FLAG              0x01
#define BL_MM_ADD_DESCRIPTOR_TRUNCATE_FLAG              0x02
#define BL_MM_ADD_DESCRIPTOR_NEVER_COALESCE_FLAG        0x10
#define BL_MM_ADD_DESCRIPTOR_NEVER_TRUNCATE_FLAG        0x20
#define BL_MM_ADD_DESCRIPTOR_ALLOCATE_FLAG              0x1000
#define BL_MM_ADD_DESCRIPTOR_UPDATE_LIST_POINTER_FLAG   0x2000

#define BL_MM_INCLUDE_MAPPED_ALLOCATED                  0x01
#define BL_MM_INCLUDE_MAPPED_UNALLOCATED                0x02
#define BL_MM_INCLUDE_UNMAPPED_ALLOCATED                0x04
#define BL_MM_INCLUDE_UNMAPPED_UNALLOCATED              0x08
#define BL_MM_INCLUDE_RESERVED_ALLOCATED                0x10
#define BL_MM_INCLUDE_BAD_MEMORY                        0x20
#define BL_MM_INCLUDE_FIRMWARE_MEMORY                   0x40
#define BL_MM_INCLUDE_TRUNCATED_MEMORY                  0x80
#define BL_MM_INCLUDE_PERSISTENT_MEMORY                 0x100
#define BL_MM_INCLUDE_FIRMWARE_MEMORY_2                 0x200

#define BL_MM_INCLUDE_NO_FIRMWARE_MEMORY                (BL_MM_INCLUDE_PERSISTENT_MEMORY | \
                                                         BL_MM_INCLUDE_TRUNCATED_MEMORY | \
                                                         BL_MM_INCLUDE_BAD_MEMORY | \
                                                         BL_MM_INCLUDE_RESERVED_ALLOCATED | \
                                                         BL_MM_INCLUDE_UNMAPPED_UNALLOCATED | \
                                                         BL_MM_INCLUDE_UNMAPPED_ALLOCATED | \
                                                         BL_MM_INCLUDE_MAPPED_UNALLOCATED | \
                                                         BL_MM_INCLUDE_MAPPED_ALLOCATED)
C_ASSERT(BL_MM_INCLUDE_NO_FIRMWARE_MEMORY == 0x1BF);

#define BL_MM_INCLUDE_ONLY_FIRMWARE_MEMORY              (BL_MM_INCLUDE_FIRMWARE_MEMORY_2 | \
                                                         BL_MM_INCLUDE_FIRMWARE_MEMORY)
C_ASSERT(BL_MM_INCLUDE_ONLY_FIRMWARE_MEMORY == 0x240);

#define BL_MM_REQUEST_DEFAULT_TYPE                      1
#define BL_MM_REQUEST_TOP_DOWN_TYPE                     2

#define BL_MM_REMOVE_PHYSICAL_REGION_FLAG               0x40000000
#define BL_MM_REMOVE_VIRTUAL_REGION_FLAG                0x80000000

#define BL_LIBRARY_FLAG_NO_DISPLAY                      0x01
#define BL_LIBRARY_FLAG_REINITIALIZE                    0x02
#define BL_LIBRARY_FLAG_REINITIALIZE_ALL                0x04
#define BL_LIBRARY_FLAG_ZERO_HEAP_ALLOCATIONS_ON_FREE   0x10
#define BL_LIBRARY_FLAG_INITIALIZATION_COMPLETED        0x20
#define BL_LIBRARY_FLAG_NO_GRAPHICS_CONSOLE             0x800

#define BL_DISPLAY_GRAPHICS_FORCED_VIDEO_MODE_FLAG      0x01
#define BL_DISPLAY_GRAPHICS_FORCED_HIGH_RES_MODE_FLAG   0x02

#define BL_HT_VALUE_IS_INLINE                           0x01

#define BL_FS_REGISTER_AT_HEAD_FLAG                     1

#define BL_BLOCK_DEVICE_REMOVABLE_FLAG                  0x01
#define BL_BLOCK_DEVICE_PRESENT_FLAG                    0x02
#define BL_BLOCK_DEVICE_VIRTUAL_FLAG                    0x04

#define BL_MEMORY_CLASS_SHIFT                           28

#define BL_FILE_READ_ACCESS                             0x01
#define BL_FILE_WRITE_ACCESS                            0x02
#define BL_DIRECTORY_ACCESS                             0x04
#define BL_UNKNOWN_ACCESS                               0x10

#define BL_DEVICE_READ_ACCESS                           0x01
#define BL_DEVICE_WRITE_ACCESS                          0x02

#define BL_DEVICE_ENTRY_OPENED                          0x01
#define BL_DEVICE_ENTRY_READ_ACCESS                     0x02
#define BL_DEVICE_ENTRY_WRITE_ACCESS                    0x04

#define BL_FILE_ENTRY_OPENED                            0x01
#define BL_FILE_ENTRY_READ_ACCESS                       0x02
#define BL_FILE_ENTRY_WRITE_ACCESS                      0x04
#define BL_FILE_ENTRY_UNKNOWN_ACCESS                    0x10
#define BL_FILE_ENTRY_DIRECTORY                         0x10000

#define BL_ETFS_FILE_ENTRY_DIRECTORY                    0x01

#define BL_IMG_VALID_FILE                               0x01
#define BL_IMG_MEMORY_FILE                              0x02
#define BL_IMG_REMOTE_FILE                              0x04

#define BL_LOAD_IMG_VIRTUAL_BUFFER                      0x01
#define BL_LOAD_IMG_EXISTING_BUFFER                     0x04
#define BL_LOAD_IMG_UNKNOWN_BUFFER_FLAG                 0x08
#define BL_LOAD_IMG_COMPUTE_SIGNATURE                   0x10
#define BL_LOAD_IMG_COMPUTE_HASH                        0x40000

#define BL_LOAD_PE_IMG_VIRTUAL_BUFFER                   BL_LOAD_IMG_VIRTUAL_BUFFER
#define BL_LOAD_PE_IMG_CHECK_MACHINE                    0x02
#define BL_LOAD_PE_IMG_EXISTING_BUFFER                  BL_LOAD_IMG_EXISTING_BUFFER
#define BL_LOAD_PE_IMG_COMPUTE_HASH                     0x10
#define BL_LOAD_PE_IMG_CHECK_SUBSYSTEM                  0x80
#define BL_LOAD_PE_IMG_SKIP_RELOCATIONS                 0x100
#define BL_LOAD_PE_IMG_CHECK_FORCED_INTEGRITY           0x200
#define BL_LOAD_PE_IMG_IGNORE_CHECKSUM_MISMATCH         0x10000
#define BL_LOAD_PE_IMG_VALIDATE_ORIGINAL_FILENAME       0x400000

#define BL_UTL_CHECKSUM_COMPLEMENT                      0x10000
#define BL_UTL_CHECKSUM_ROTATE                          0x20000
#define BL_UTL_CHECKSUM_NEGATE                          0x40000
#define BL_UTL_CHECKSUM_UCHAR_BUFFER                    0x01
#define BL_UTL_CHECKSUM_USHORT_BUFFER                   0x02

/* ENUMERATIONS **************************************************************/

typedef enum _BL_COLOR
{
    Black,
    Blue,
    Green,
    Cyan,
    Red,
    Magenta,
    Brown,
    LtGray,
    Gray,
    LtBlue,
    LtGreen,
    LtCyan,
    LtRed,
    LtMagenta,
    Yellow,
    White
} BL_COLOR, *PBL_COLOR;

typedef enum _BL_MENU_POLICY
{
    MenuPolicyLegacy = 0,
    MenuPolicyStandard = 1
} BL_MENU_POLICY;

typedef enum _BL_MEMORY_DESCRIPTOR_TYPE
{
    BlMdPhysical,
    BlMdVirtual,
    BlMdTracker
} BL_MEMORY_DESCRIPTOR_TYPE;

typedef enum _BL_TRANSLATION_TYPE
{
    BlNone,
    BlVirtual,
    BlPae,
    BlMax
} BL_TRANSLATION_TYPE;

typedef enum _BL_ARCH_MODE
{
    BlProtectedMode,
    BlRealMode
} BL_ARCH_MODE;

//
// Boot Device Types
//
typedef enum _BL_DEVICE_TYPE
{
    DiskDevice = 0,
    LegacyPartitionDevice = 2,
    SerialDevice = 3,
    UdpDevice = 4,
    BootDevice = 5,
    PartitionDevice = 6,
    LocateDevice = 8,
} BL_DEVICE_TYPE;

//
// Local Device Types
//
typedef enum _BL_LOCAL_DEVICE_TYPE
{
    LocalDevice = 0,
    FloppyDevice = 1,
    CdRomDevice = 2,
    RamDiskDevice = 3,
    FileDevice = 5,
    VirtualDiskDevice = 6
} BL_LOCAL_DEVICE_TYPE;

//
// Partition types
//
typedef enum _BL_PARTITION_TYPE
{
    GptPartition,
    MbrPartition,
    RawPartition,
} BL_PARTITION_TYPE;

//
// File Path Types
//
typedef enum _BL_PATH_TYPE
{
    InternalPath = 3,
    EfiPath = 4
} BL_PATH_TYPE;

//
// Classes of Memory
//
typedef enum _BL_MEMORY_CLASS
{
    BlLoaderClass = 0xD,
    BlApplicationClass,
    BlSystemClass
} BL_MEMORY_CLASS;

//
// Types of Memory
//
typedef enum _BL_MEMORY_TYPE
{
    //
    // Loader Memory
    //
    BlLoaderMemory = 0xD0000002,
    BlLoaderDeviceMemory = 0xD0000004,
    BlLoaderHeap = 0xD0000005,
    BlLoaderPageDirectory = 0xD0000006,
    BlLoaderReferencePage = 0xD0000007,
    BlLoaderRamDisk = 0xD0000008,
    BlLoaderArchData = 0xD0000009,
    BlLoaderData = 0xD000000A,
    BlLoaderRegistry = 0xD000000B,
    BlLoaderBlockMemory = 0xD000000C,
    BlLoaderSelfMap = 0xD000000F,

    //
    // Application Memory
    //
    BlApplicationReserved = 0xE0000001,
    BlApplicationData = 0xE0000004,

    //
    // System Memory
    //
    BlConventionalMemory = 0xF0000001,
    BlUnusableMemory = 0xF0000002,
    BlReservedMemory = 0xF0000003,
    BlEfiBootMemory = 0xF0000004,
    BlConventionalZeroedMemory = 0xF000005,
    BlEfiRuntimeCodeMemory = 0xF0000006,
    BlAcpiReclaimMemory = 0xF0000008,
    BlAcpiNvsMemory = 0xF0000009,
    BlDeviceIoMemory = 0xF000000A,
    BlDevicePortMemory = 0xF000000B,
    BlPalMemory = 0xF000000C,
    BlEfiRuntimeDataMemory = 0xF000000E,
} BL_MEMORY_TYPE;

typedef enum _BL_MEMORY_ATTR
{
    //
    // Memory Caching Attributes
    //
    BlMemoryUncached =          0x00000001,
    BlMemoryWriteCombined =     0x00000002,
    BlMemoryWriteThrough =      0x00000004,
    BlMemoryWriteBack =         0x00000008,
    BlMemoryUncachedExported =  0x00000010,
    BlMemoryValidCacheAttributes            = BlMemoryUncached | BlMemoryWriteCombined | BlMemoryWriteThrough | BlMemoryWriteBack | BlMemoryUncachedExported,
    BlMemoryValidCacheAttributeMask         = 0x000000FF,

    //
    // Memory Protection Attributes
    //
    BlMemoryWriteProtected =    0x00000100,
    BlMemoryReadProtected =     0x00000200,
    BlMemoryExecuteProtected =  0x00000400,
    BlMemoryValidProtectionAttributes       = BlMemoryWriteProtected | BlMemoryReadProtected | BlMemoryExecuteProtected,
    BlMemoryValidProtectionAttributeMask    = 0x0000FF00,

    //
    // Memory Allocation Attributes
    //
    BlMemoryLargePages =        0x00010000,
    BlMemoryKernelRange =       0x00020000,
    BlMemoryFixed =             0x00040000,
    BlMemoryBelow1MB =          0x00080000,
    BlMemoryValidAllocationAttributes       = BlMemoryKernelRange | BlMemoryFixed | BlMemoryBelow1MB | BlMemoryLargePages,
    BlMemoryValidAllocationAttributeMask    = 0x00FF0000,

    //
    // Memory Type Attributes
    //
    BlMemoryRuntime =           0x01000000,
    BlMemoryCoalesced =         0x02000000,
    BlMemoryUpdate =            0x04000000,
    BlMemoryNonFirmware =       0x08000000,
    BlMemoryPersistent =        0x10000000,
    BlMemorySpecial =           0x20000000,
    BlMemoryFirmware =          0x80000000,
    BlMemoryValidTypeAttributes             = BlMemoryRuntime | BlMemoryCoalesced | BlMemoryUpdate | BlMemoryNonFirmware | BlMemoryPersistent | BlMemorySpecial | BlMemoryFirmware,
    BlMemoryValidTypeAttributeMask          = 0xFF000000,
} BL_MEMORY_ATTR;

/* CALLBACKS *****************************************************************/

struct _BL_FILE_ENTRY;
struct _BL_FILE_INFORMATION;
typedef
NTSTATUS
(*PBL_FILE_OPEN) (
    _In_ struct _BL_FILE_ENTRY* Directory,
    _In_ PWCHAR FileName,
    _In_ ULONG Flags,
    _Out_ struct _BL_FILE_ENTRY** FileEntry
    );

typedef
NTSTATUS
(*PBL_FILE_CLOSE) (
    _In_ struct _BL_FILE_ENTRY* FileEntry
    );

typedef
NTSTATUS
(*PBL_FILE_READ) (
    _In_ struct _BL_FILE_ENTRY* FileEntry,
    _In_ PVOID Buffer,
    _In_ ULONG Size,
    _Out_opt_ PULONG BytesRead
    );

typedef
NTSTATUS
(*PBL_FILE_WRITE) (
    VOID
    );

typedef
NTSTATUS
(*PBL_FILE_GET_NEXT) (
    VOID
    );

typedef
NTSTATUS
(*PBL_FILE_GET_INFO) (
    _In_ struct _BL_FILE_ENTRY* FileEntry,
    _Out_ struct _BL_FILE_INFORMATION* FileInfo
    );

typedef
NTSTATUS
(*PBL_FILE_SET_INFO) (
    _In_ struct _BL_FILE_ENTRY* FileEntry,
    _In_ struct _BL_FILE_INFORMATION* FileInfo
    );

typedef
NTSTATUS
(*PBL_FS_INIT_CALLBACK) (
    VOID
    );

typedef
NTSTATUS
(*PBL_FS_DESTROY_CALLBACK) (
    VOID
    );

typedef
NTSTATUS
(*PBL_FS_MOUNT_CALLBACK) (
    _In_ ULONG DeviceId,
    _In_ ULONG Unknown,
    _Out_ struct _BL_FILE_ENTRY** FileEntry
    );

typedef
NTSTATUS
(*PBL_FS_PURGE_CALLBACK) (
    VOID
    );

typedef
NTSTATUS
(*PBL_FILE_DESTROY_CALLBACK) (
    _In_ PVOID Entry
    );

struct _BL_TEXT_CONSOLE;
struct _BL_DISPLAY_STATE;
struct _BL_DISPLAY_MODE;
struct _BL_INPUT_CONSOLE;
struct _BL_REMOTE_CONSOLE;
struct _BL_GRAPHICS_CONSOLE;
typedef
VOID
(*PCONSOLE_DESTRUCT) (
    _In_ struct _BL_TEXT_CONSOLE* Console
    );

typedef
NTSTATUS
(*PCONSOLE_REINITIALIZE) (
    _In_ struct _BL_TEXT_CONSOLE* Console
    );

typedef
NTSTATUS
(*PCONSOLE_GET_TEXT_STATE) (
    _In_ struct _BL_TEXT_CONSOLE* Console,
    _Out_ struct _BL_DISPLAY_STATE* TextState
    );

typedef
NTSTATUS
(*PCONSOLE_SET_TEXT_STATE) (
    _In_ struct _BL_TEXT_CONSOLE* Console,
    _In_ ULONG Flags,
    _In_ struct _BL_DISPLAY_STATE* TextState
    );

typedef
NTSTATUS
(*PCONSOLE_GET_TEXT_RESOLUTION) (
    _In_ struct _BL_TEXT_CONSOLE* Console,
    _Out_ PULONG TextResolution
    );

typedef
NTSTATUS
(*PCONSOLE_SET_TEXT_RESOLUTION) (
    _In_ struct _BL_TEXT_CONSOLE* Console,
    _In_ ULONG NewTextResolution,
    _Out_ PULONG OldTextResolution
    );

typedef
NTSTATUS
(*PCONSOLE_CLEAR_TEXT) (
    _In_ struct _BL_TEXT_CONSOLE* Console,
    _In_ BOOLEAN LineOnly
    );

typedef
BOOLEAN
(*PCONSOLE_IS_ENABLED) (
    _In_ struct _BL_GRAPHICS_CONSOLE* Console
    );

typedef
NTSTATUS
(*PCONSOLE_GET_GRAPHICAL_RESOLUTION) (
    _In_ struct _BL_GRAPHICS_CONSOLE* Console,
    _Out_ struct _BL_DISPLAY_MODE* DisplayMode
    );

typedef
NTSTATUS
(*PCONSOLE_SET_GRAPHICAL_RESOLUTION) (
    _In_ struct _BL_GRAPHICS_CONSOLE* Console,
    _In_ struct _BL_DISPLAY_MODE DisplayMode
    );

typedef
NTSTATUS
(*PCONSOLE_ENABLE) (
    _In_ struct _BL_GRAPHICS_CONSOLE* Console,
    _In_ BOOLEAN Enable
);

typedef
NTSTATUS
(*PCONSOLE_WRITE_TEXT) (
    _In_ struct _BL_TEXT_CONSOLE* Console,
    _In_ PCHAR Text,
    _In_ ULONG Attribute
    );

typedef
BOOLEAN
(*PBL_TBL_LOOKUP_ROUTINE) (
    _In_ PVOID Entry,
    _In_ PVOID Argument1,
    _In_ PVOID Argument2,
    _In_ PVOID Argument3,
    _In_ PVOID Argument4
    );

typedef
NTSTATUS
(*PBL_TBL_MAP_ROUTINE) (
    _In_ PVOID Entry,
    _In_ ULONG EntryIndex
    );

typedef
NTSTATUS
(*PBL_TBL_SET_ROUTINE) (
    _In_ PVOID Entry
    );

typedef
NTSTATUS
(*PBL_IO_DESTROY_ROUTINE) (
    VOID
    );

struct _BL_HASH_ENTRY;
typedef
BOOLEAN
(*PBL_HASH_TABLE_COMPARE_FUNCTION) (
    _In_ struct _BL_HASH_ENTRY* Entry1,
    _In_ struct _BL_HASH_ENTRY* Entry2
    );

typedef
ULONG
(*PBL_HASH_TABLE_HASH_FUNCTION) (
    _In_ struct _BL_HASH_ENTRY* Entry,
    _In_ ULONG TableSize
    );

struct _BL_DEVICE_ENTRY;
struct _BL_DEVICE_DESCRIPTOR;
struct _BL_DEVICE_INFORMATION;

typedef
NTSTATUS
(*PBL_DEVICE_ENUMERATE_DEVICE_CLASS) (
    VOID
    );

typedef
NTSTATUS
(*PBL_DEVICE_OPEN) (
    _In_ struct _BL_DEVICE_DESCRIPTOR* Device,
    _In_ struct _BL_DEVICE_ENTRY* DeviceEntry
    );

typedef
NTSTATUS
(*PBL_DEVICE_CLOSE) (
    _In_ struct _BL_DEVICE_ENTRY* DeviceEntry
    );

typedef
NTSTATUS
(*PBL_DEVICE_READ) (
    _In_ struct _BL_DEVICE_ENTRY* DeviceEntry,
    _In_ PVOID Buffer,
    _In_ ULONG Size,
    _Out_ PULONG BytesRead
    );

typedef
NTSTATUS
(*PBL_DEVICE_WRITE) (
    VOID
    );

typedef
NTSTATUS
(*PBL_DEVICE_GET_INFORMATION) (
    _In_ struct _BL_DEVICE_ENTRY* DeviceEntry,
    _Out_ struct _BL_DEVICE_INFORMATION* DeviceInformation
    );

typedef
NTSTATUS
(*PBL_DEVICE_SET_INFORMATION) (
    _In_ struct _BL_DEVICE_ENTRY* DeviceEntry,
    _In_ struct _BL_DEVICE_INFORMATION* DeviceInformation
    );

typedef
NTSTATUS
(*PBL_DEVICE_RESET) (
    VOID
    );

typedef
NTSTATUS
(*PBL_DEVICE_FLUSH) (
    VOID
    );

typedef
NTSTATUS
(*PBL_DEVICE_CREATE) (
    VOID
    );

typedef VOID
(*PBL_MM_FLUSH_TLB) (
    VOID
    );

typedef VOID
(*PBL_MM_RELOCATE_SELF_MAP) (
    VOID
    );

typedef NTSTATUS
(*PBL_MM_MOVE_VIRTUAL_ADDRESS_RANGE) (
    _In_ PVOID DestinationAddress,
    _In_ PVOID SourceAddress,
    _In_ ULONGLONG Size
    );

typedef NTSTATUS
(*PBL_MM_ZERO_VIRTUAL_ADDRESS_RANGE) (
    _In_ PVOID DestinationAddress,
    _In_ ULONGLONG Size
    );

typedef VOID
(*PBL_MM_DESTROY_SELF_MAP) (
    VOID
    );

typedef VOID
(*PBL_MM_FLUSH_TLB_ENTRY) (
    _In_ PVOID VirtualAddress
    );

typedef VOID
(*PBL_MM_FLUSH_TLB) (
    VOID
    );

typedef NTSTATUS
(*PBL_MM_UNMAP_VIRTUAL_ADDRESS) (
    _In_ PVOID VirtualAddress,
    _In_ ULONG Size
    );

typedef NTSTATUS
(*PBL_MM_REMAP_VIRTUAL_ADDRESS) (
    _In_ PPHYSICAL_ADDRESS PhysicalAddress,
    _Out_ PVOID VirtualAddress,
    _In_ ULONG Size,
    _In_ ULONG CacheAttributes
    );

typedef NTSTATUS
(*PBL_MM_MAP_PHYSICAL_ADDRESS) (
    _In_ PHYSICAL_ADDRESS PhysicalAddress,
    _Out_ PVOID VirtualAddress,
    _In_ ULONG Size,
    _In_ ULONG CacheAttributes
    );

typedef BOOLEAN
(*PBL_MM_TRANSLATE_VIRTUAL_ADDRESS) (
    _In_ PVOID VirtualAddress,
    _Out_ PPHYSICAL_ADDRESS PhysicalAddress,
    _Out_opt_ PULONG CacheAttributes
    );

typedef NTSTATUS
(*PBL_STATUS_ERROR_HANDLER) (
    _In_ ULONG ErrorCode,
    _In_ ULONG Parameter1,
    _In_ ULONG_PTR Parameter2,
    _In_ ULONG_PTR Parameter3,
    _In_ ULONG_PTR Parameter4
    );

/* DATA STRUCTURES ***********************************************************/

typedef struct _BL_LIBRARY_PARAMETERS
{
    ULONG LibraryFlags;
    ULONG TranslationType;
    ULONG MinimumAllocationCount;
    ULONG MinimumHeapSize;
    ULONG HeapAllocationAttributes;
    PWCHAR ApplicationBaseDirectory;
    ULONG DescriptorCount;
    PWCHAR FontBaseDirectory;
} BL_LIBRARY_PARAMETERS, *PBL_LIBRARY_PARAMETERS;

/* This should eventually go into a more public header */
typedef struct _BOOT_APPLICATION_PARAMETER_BLOCK
{
    /* This header tells the library what image we're dealing with */
    ULONG Signature[2];
    ULONG Version;
    ULONG Size;
    ULONG ImageType;
    ULONG MemoryTranslationType;

    /* Where is the image located */
    ULONGLONG ImageBase;
    ULONG ImageSize;

    /* Offset to BL_MEMORY_DATA */
    ULONG MemoryDataOffset;

    /* Offset to BL_APPLICATION_ENTRY */
    ULONG AppEntryOffset;

    /* Offset to BL_DEVICE_DESCRPIPTOR */
    ULONG BootDeviceOffset;

    /* Offset to BL_FIRMWARE_PARAMETERS */
    ULONG FirmwareParametersOffset;

    /* Offset to BL_RETURN_ARGUMENTS */
    ULONG ReturnArgumentsOffset;
} BOOT_APPLICATION_PARAMETER_BLOCK, *PBOOT_APPLICATION_PARAMETER_BLOCK;

typedef struct _BL_MEMORY_DATA
{
    ULONG Version;
    ULONG MdListOffset;
    ULONG DescriptorCount;
    ULONG DescriptorSize;
    ULONG DescriptorOffset;
} BL_MEMORY_DATA, *PBL_MEMORY_DATA;
#if 1
typedef struct _ARM_EXCEPTION_STATE
{
	unsigned int Control;
	unsigned int Vbar;
	unsigned int Reserved;
	unsigned int Reserved2;
	unsigned int IdSvcRW;
} ARM_EXCEPTION_STATE, *PARM_EXCEPTION_STATE;

typedef struct _ARM_MM_STATE
{
	char MpExtensions;
	unsigned int HardwarePageDirectory;
	unsigned int TTB_Config;
	unsigned int SoftwarePageDirectory;
	unsigned int *MappedHardwarePageDirectory;
} ARM_MM_STATE, *PARM_MM_STATE;

typedef struct _BL_FIRMWARE_DESCRIPTOR
{
    ULONG Version;
    ULONG Unknown;
    EFI_HANDLE ImageHandle;
    EFI_SYSTEM_TABLE *SystemTable;
    ARM_EXCEPTION_STATE ExceptionState;
	ARM_MM_STATE MmState;
	unsigned int InterruptState;
} BL_FIRMWARE_DESCRIPTOR, *PBL_FIRMWARE_DESCRIPTOR;
#endif
typedef struct _BL_RETURN_ARGUMENTS
{
    ULONG Version;
    NTSTATUS Status;
    ULONG Flags;
    ULONGLONG DataSize;
    ULONGLONG DataPage;
} BL_RETURN_ARGUMENTS, *PBL_RETURN_ARGUMENTS;

typedef struct _BL_MEMORY_DESCRIPTOR
{
    LIST_ENTRY ListEntry;
    union
    {
        struct
        {
            ULONGLONG BasePage;
            ULONGLONG VirtualPage;
        };
        struct
        {
            ULONGLONG BaseAddress;
            ULONGLONG VirtualAddress;
        };
    };
    ULONGLONG PageCount;
    ULONG Flags;
    BL_MEMORY_TYPE Type;
} BL_MEMORY_DESCRIPTOR, *PBL_MEMORY_DESCRIPTOR;

typedef struct _BL_BCD_OPTION
{
    ULONG Type;
    ULONG DataOffset;
    ULONG DataSize;
    ULONG ListOffset;
    ULONG NextEntryOffset;
    ULONG Empty;
} BL_BCD_OPTION, *PBL_BCD_OPTION;

typedef struct _BL_APPLICATION_ENTRY
{
    CHAR Signature[8];
    ULONG Flags;
    GUID Guid;
    ULONG Unknown[4];
    BL_BCD_OPTION BcdData;
} BL_APPLICATION_ENTRY, *PBL_APPLICATION_ENTRY;

typedef struct _BL_LOADED_APPLICATION_ENTRY
{
    ULONG Flags;
    GUID Guid;
    PBL_BCD_OPTION BcdData;
} BL_LOADED_APPLICATION_ENTRY, *PBL_LOADED_APPLICATION_ENTRY;

typedef struct _BL_MENU_STATUS
{
    union
    {
        struct
        {
            ULONG AnyKey : 1;
            ULONG AdvancedOptions : 1;
            ULONG BootOptions : 1;
            ULONG OemKey : 1;
            ULONG Exit : 1;
            ULONG Reserved : 27;
        };
        ULONG AsULong;
    };
    ULONG BootIndex;
    WCHAR KeyValue;
} BL_MENU_STATUS, *PL_MENU_STATUS;

typedef enum _BL_BOOT_ERROR_STATUS
{
    RebootTwo = 1,
    Recover = 2,
    RecoverOem = 3,
    OsSelection = 4,
    NextOs = 5,
    TryAgain = 6,
    AdvancedOptions = 7,
    BootOptions = 8
} BL_BOOT_ERROR_STATUS;

typedef struct _BL_HARDDISK_DEVICE
{
    ULONG PartitionType;
    union
    {
        struct
        {
            ULONG PartitionSignature;
        } Mbr;

        struct
        {
            GUID PartitionSignature;
        } Gpt;

        struct
        {
            ULONG DiskNumber;
        } Raw;
    };
} BL_HARDDISK_DEVICE;

typedef struct _BL_LOCAL_DEVICE
{
    BL_LOCAL_DEVICE_TYPE Type;
    union
    {
        struct
        {
            ULONG DriveNumber;
        } FloppyDisk;

        BL_HARDDISK_DEVICE HardDisk;

        BL_HARDDISK_DEVICE VirtualHardDisk;

        struct
        {
            PHYSICAL_ADDRESS ImageBase;
            LARGE_INTEGER ImageSize;
            ULONG ImageOffset;
        } RamDisk;

        ULONG File; // unknown for now
    };
} BL_LOCAL_DEVICE, *PBL_LOCAL_DEVICE;

typedef struct _BL_DEVICE_DESCRIPTOR
{
    DEVICE_TYPE DeviceType;
    ULONG Flags;
    ULONG Size;
    ULONG Unknown;
    union
    {
        BL_LOCAL_DEVICE Local;

        struct
        {
            ULONG Unknown;
        } Remote;

        struct
        {
            union
            {
                ULONG PartitionNumber;
            } Mbr;

            union
            {
                GUID PartitionGuid;
            } Gpt;

            BL_LOCAL_DEVICE Disk;
        } Partition;
    };
} BL_DEVICE_DESCRIPTOR, *PBL_DEVICE_DESCRIPTOR;

typedef struct _BL_FILE_PATH_DESCRIPTOR
{
    ULONG Version;
    ULONG Length;
    ULONG PathType;
    UCHAR Path[ANYSIZE_ARRAY];
} BL_FILE_PATH_DESCRIPTOR, *PBL_FILE_PATH_DESCRIPTOR;

typedef struct _BL_WINDOWS_LOAD_OPTIONS
{
    CHAR Signature[8];
    ULONG Version;
    ULONG Length;
    ULONG OsPathOffset;
    WCHAR LoadOptions[ANYSIZE_ARRAY];
} BL_WINDOWS_LOAD_OPTIONS, *PBL_WINDOWS_LOAD_OPTIONS;

typedef struct _BL_ARCH_CONTEXT
{
    BL_ARCH_MODE Mode;
    BL_TRANSLATION_TYPE TranslationType;
    ULONG ContextFlags;
} BL_ARCH_CONTEXT, *PBL_ARCH_CONTEXT;

typedef struct _BL_MEMORY_DESCRIPTOR_LIST
{
    LIST_ENTRY ListHead;
    PLIST_ENTRY First;
    PLIST_ENTRY This;
    ULONG Type;
} BL_MEMORY_DESCRIPTOR_LIST, *PBL_MEMORY_DESCRIPTOR_LIST;

typedef struct _BL_ADDRESS_RANGE
{
    ULONGLONG Minimum;
    ULONGLONG Maximum;
} BL_ADDRESS_RANGE, *PBL_ADDRESS_RANGE;

typedef struct _BL_FILE_INFORMATION
{
    ULONGLONG Size;
    ULONGLONG Offset;
    PWCHAR FsName;
    ULONG Flags;
} BL_FILE_INFORMATION, *PBL_FILE_INFORMATION;

typedef struct _BL_FILE_CALLBACKS
{
    PBL_FILE_OPEN Open;
    PBL_FILE_CLOSE Close;
    PBL_FILE_READ Read;
    PBL_FILE_WRITE Write;
    PBL_FILE_GET_NEXT GetNext;
    PBL_FILE_GET_INFO GetInfo;
    PBL_FILE_SET_INFO SetInfo;
} BL_FILE_CALLBACKS, *PBL_FILE_CALLBACKS;

typedef struct _BL_FILE_ENTRY
{
    PWCHAR FilePath;
    ULONG DeviceId;
    ULONG FileId;
    ULONG Flags;
    ULONG ReferenceCount;
    ULONG Unknown;
    ULONGLONG TotalBytesRead;
    ULONGLONG Unknown2;
    BL_FILE_CALLBACKS Callbacks;
    PVOID FsSpecificData;
} BL_FILE_ENTRY, *PBL_FILE_ENTRY;

typedef struct _BL_FILE_SYSTEM_ENTRY
{
    LIST_ENTRY ListEntry;
    PBL_FS_INIT_CALLBACK InitCallback;
    PBL_FS_DESTROY_CALLBACK DestroyCallback;
    PBL_FS_MOUNT_CALLBACK MountCallback;
    PBL_FS_PURGE_CALLBACK PurgeCallback;
} BL_FILE_SYSTEM_ENTRY, *PBL_FILE_SYSTEM_ENTRY;

typedef struct _BL_FILE_SYSTEM_REGISTRATION_TABLE
{
    PBL_FS_INIT_CALLBACK Init;
    PBL_FS_DESTROY_CALLBACK Destroy;
    PBL_FS_MOUNT_CALLBACK Mount;
    PBL_FS_PURGE_CALLBACK Purge;
} BL_FILE_SYSTEM_REGISTRATION_TABLE;

typedef struct _BL_DISPLAY_STATE
{
    ULONG BgColor;
    ULONG FgColor;
    ULONG XPos;
    ULONG YPos;
    ULONG CursorVisible;
} BL_DISPLAY_STATE, *PBL_DISPLAY_STATE;

typedef struct _BL_DISPLAY_MODE
{
    ULONG HRes;
    ULONG VRes;
    ULONG HRes2;
} BL_DISPLAY_MODE, *PBL_DISPLAY_MODE;

typedef struct _BL_TEXT_CONSOLE_VTABLE
{
    PCONSOLE_DESTRUCT Destruct;
    PCONSOLE_REINITIALIZE Reinitialize;
    PCONSOLE_GET_TEXT_STATE GetTextState;
    PCONSOLE_SET_TEXT_STATE SetTextState;
    PCONSOLE_GET_TEXT_RESOLUTION GetTextResolution;
    PCONSOLE_SET_TEXT_RESOLUTION SetTextResolution;
    PCONSOLE_CLEAR_TEXT ClearText;
    PCONSOLE_WRITE_TEXT WriteText;
} BL_TEXT_CONSOLE_VTABLE, *PBL_TEXT_CONSOLE_VTABLE;

typedef struct _BL_GRAPHICS_CONSOLE_VTABLE
{
    BL_TEXT_CONSOLE_VTABLE Text;
    PCONSOLE_IS_ENABLED IsEnabled;
    PCONSOLE_ENABLE Enable;
    PVOID GetConsoleResolution;
    PCONSOLE_GET_GRAPHICAL_RESOLUTION GetGraphicalResolution;
    PCONSOLE_GET_GRAPHICAL_RESOLUTION GetOriginalResolution;
    PCONSOLE_SET_GRAPHICAL_RESOLUTION SetOriginalResolution;
    /// more for graphics ///
} BL_GRAPHICS_CONSOLE_VTABLE, *PBL_GRAPHICS_CONSOLE_VTABLE;

typedef struct _BL_TEXT_CONSOLE
{
    PBL_TEXT_CONSOLE_VTABLE Callbacks;
    BL_DISPLAY_STATE State;
    BL_DISPLAY_MODE DisplayMode;
    BOOLEAN Active;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* Protocol;
    ULONG Mode;
    EFI_SIMPLE_TEXT_OUTPUT_MODE OldMode;
} BL_TEXT_CONSOLE, *PBL_TEXT_CONSOLE;

typedef struct _BL_INPUT_CONSOLE_VTABLE
{
    PCONSOLE_DESTRUCT Destruct;
    PCONSOLE_REINITIALIZE Reinitialize;
    //PCONSOLE_IS_KEY_PENDING IsKeyPending;
    //PCONSOLE_READ_INPUT ReadInput;
    //PCONSOLE_ERASE_BUFFER EraseBuffer;
    //PCONSOLE_FILL_BUFFER FillBuffer;
} BL_INPUT_CONSOLE_VTABLE, *PBL_INPUT_CONSOLE_VTABLE;

typedef struct _BL_INPUT_CONSOLE
{
    PBL_INPUT_CONSOLE_VTABLE Callbacks;
    PULONG Buffer;
    PULONG DataStart;
    PULONG DataEnd;
    PULONG EndBuffer;
} BL_INPUT_CONSOLE, *PBL_INPUT_CONSOLE;

typedef enum _BL_GRAPHICS_CONSOLE_TYPE
{
    BlGopConsole,
    BlUgaConsole
} BL_GRAPHICS_CONSOLE_TYPE;

typedef struct _BL_GRAPHICS_CONSOLE
{
    BL_TEXT_CONSOLE TextConsole;
    BL_DISPLAY_MODE DisplayMode;
    ULONG PixelDepth;
    ULONG FgColor;
    ULONG BgColor;
    BL_DISPLAY_MODE OldDisplayMode;
    ULONG OldPixelDepth;
    EFI_HANDLE Handle;
    BL_GRAPHICS_CONSOLE_TYPE Type;
    EFI_GRAPHICS_OUTPUT_PROTOCOL* Protocol;
    PVOID FrameBuffer;
    ULONG FrameBufferSize;
    ULONG PixelsPerScanLine;
    ULONG Mode;
    ULONG OldMode;
} BL_GRAPHICS_CONSOLE, *PBL_GRAPHICS_CONSOLE;

typedef struct _BL_REMOTE_CONSOLE
{
    BL_TEXT_CONSOLE TextConsole;
} BL_REMOTE_CONSOLE, *PBL_REMOTE_CONSOLE;

typedef struct _BL_HASH_TABLE
{
    PLIST_ENTRY HashLinks;
    ULONG Size;
    PBL_HASH_TABLE_COMPARE_FUNCTION CompareFunction;
    PBL_HASH_TABLE_HASH_FUNCTION HashFunction;
} BL_HASH_TABLE, *PBL_HASH_TABLE;

typedef struct _BL_HASH_ENTRY
{
    ULONG Size;
    ULONG Flags;
    PVOID Value;
} BL_HASH_ENTRY, *PBL_HASH_ENTRY;

typedef struct _BL_HASH_VALUE
{
    ULONG DataSize;
    PVOID Data;
} BL_HASH_VALUE, *PBL_HASH_VALUE;

typedef struct _BL_HASH_NODE
{
    LIST_ENTRY ListEntry;
    BL_HASH_ENTRY Entry;
    BL_HASH_VALUE Value;
} BL_HASH_NODE, *PBL_HASH_NODE;

typedef struct _BL_BLOCK_DEVICE_INFORMATION
{
    BL_LOCAL_DEVICE_TYPE Type;
    ULONG DeviceFlags;
    ULONG Unknown;
    BL_PARTITION_TYPE PartitionType;
    ULONG BlockSize;
    ULONG Alignment;
    ULONGLONG LastBlock;
    ULONGLONG Offset;
    ULONG Block;
    struct
    {
        union
        {
            struct
            {
                ULONG Signature;
            } Mbr;
            struct
            {
                GUID Signature;
            } Gpt;
        };
    } Disk;
} BL_BLOCK_DEVICE_INFORMATION, *PBL_BLOCK_DEVICE_INFORMATION;

typedef struct _BL_DEVICE_INFORMATION
{
    BL_DEVICE_TYPE DeviceType;
    union
    {
        BL_BLOCK_DEVICE_INFORMATION BlockDeviceInfo;
    };
} BL_DEVICE_INFORMATION, *PBL_DEVICE_INFORMATION;

typedef struct _BL_BLOCK_DEVICE
{
    BL_BLOCK_DEVICE_INFORMATION;
    ULONGLONG StartOffset;
    EFI_BLOCK_IO* Protocol;
    EFI_HANDLE Handle;
} BL_BLOCK_DEVICE, *PBL_BLOCK_DEVICE;

typedef struct _BL_PROTOCOL_HANDLE
{
    EFI_HANDLE Handle;
    PVOID Interface;
} BL_PROTOCOL_HANDLE, *PBL_PROTOCOL_HANDLE;

typedef struct _BL_DEVICE_CALLBACKS
{
    PBL_DEVICE_ENUMERATE_DEVICE_CLASS EnumerateDeviceClass;
    PBL_DEVICE_OPEN Open;
    PBL_DEVICE_CLOSE Close;
    PBL_DEVICE_READ Read;
    PBL_DEVICE_WRITE Write;
    PBL_DEVICE_GET_INFORMATION GetInformation;
    PBL_DEVICE_SET_INFORMATION SetInformation;
    PBL_DEVICE_RESET Reset;
    PBL_DEVICE_FLUSH Flush;
    PBL_DEVICE_CREATE Create;
} BL_DEVICE_CALLBACKS, *PBL_DEVICE_CALLBACKS;

typedef struct _BL_DEVICE_ENTRY
{
    ULONG DeviceId;
    ULONG Flags;
    ULONG Unknown;
    ULONG ReferenceCount;
    BL_DEVICE_CALLBACKS Callbacks;
    PVOID DeviceSpecificData;
    PBL_DEVICE_DESCRIPTOR DeviceDescriptor;
} BL_DEVICE_ENTRY, *PBL_DEVICE_ENTRY;

typedef struct _BL_IMG_FILE
{
    UCHAR Flags;
    union
    {
        PVOID BaseAddress;
        ULONG FileId;
    };
    ULONG FileSize;
    PWCHAR FileName;
} BL_IMG_FILE, *PBL_IMG_FILE;

typedef struct _BL_IMAGE_APPLICATION_ENTRY
{
    PBL_APPLICATION_ENTRY AppEntry;
    PVOID ImageBase;
    ULONG ImageSize;
} BL_IMAGE_APPLICATION_ENTRY, *PBL_IMAGE_APPLICATION_ENTRY;

typedef struct _BL_BUFFER_DESCRIPTOR
{
    PVOID Buffer;
    ULONG ActualSize;
    ULONG BufferSize;
} BL_BUFFER_DESCRIPTOR, *PBL_BUFFER_DESCRIPTOR;

typedef struct _BL_DEFERRED_FONT_FILE
{
    LIST_ENTRY ListEntry;
    ULONG Flags;
    PBL_DEVICE_DESCRIPTOR Device;
    PWCHAR FontPath;
} BL_DEFERRED_FONT_FILE, *PBL_DEFERRED_FONT_FILE;

#pragma pack(push)
#pragma pack(1)
typedef struct _BMP_HEADER
{
    USHORT Signature;
    ULONG Size;
    USHORT Reserved[2];
    ULONG Offset;
} BMP_HEADER, *PBMP_HEADER;

typedef struct _DIB_HEADER
{
    ULONG Size;
    ULONG Width;
    ULONG Height;
    USHORT Planes;
    USHORT BitCount;
    ULONG Compression;
    ULONG SizeImage;
    ULONG XPelsPerMeter;
    ULONG YPelsPerMEter;
    ULONG ClrUsed;
    ULONG ClrImportant;
} DIB_HEADER, *PDIB_HEADER;

typedef struct _BITMAP
{
    BMP_HEADER BmpHeader;
    DIB_HEADER DibHeader;
} BITMAP, *PBITMAP;
#pragma pack(pop)

typedef struct _COORD
{
    ULONG X;
    ULONG Y;
} COORD, *PCOORD;

typedef struct _BL_PD_DATA_BLOB
{
    PVOID Data;
    ULONG DataSize;
    ULONG BlobSize;
} BL_PD_DATA_BLOB, *PBL_PD_DATA_BLOB;
