/*
 * PROJECT:     ReactOS WDDM Alternative Headers
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     dispmprt minimal header acqusition
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */
#pragma once


//
// Old includes which are only kept for now for compatibility
//

#ifndef _NTOSP_
#define _NTOSP_

//
// Structures used by the kernel drivers to describe which ports must be
// hooked out directly from the V86 emulator to the driver.
//

typedef enum _EMULATOR_PORT_ACCESS_TYPE {
    Uchar,
    Ushort,
    Ulong
} EMULATOR_PORT_ACCESS_TYPE, *PEMULATOR_PORT_ACCESS_TYPE;

typedef struct _EMULATOR_ACCESS_ENTRY {
    ULONG BasePort;
    ULONG NumConsecutivePorts;
    EMULATOR_PORT_ACCESS_TYPE AccessType;
    UCHAR AccessMode;
    UCHAR StringSupport;
    PVOID Routine;
} EMULATOR_ACCESS_ENTRY, *PEMULATOR_ACCESS_ENTRY;

#endif

//
// Graphics support routines.
//

typedef
VOID
(*PBANKED_SECTION_ROUTINE) (
    _In_ ULONG ReadBank,
    _In_ ULONG WriteBank,
    _In_ PVOID Context
    );

//
// Exclude some items from video.h
//

#define _NTOSDEF_

#include <ntddvdeo.h>
#include <video.h>

//
// Define types used in d3dukmdt.h, included via d3dkmdt.h that are not defined in this scope.
//

typedef unsigned int   UINT;
typedef unsigned long  DWORD;
typedef unsigned char  BYTE;

#include <windef.h>
#include <d3dkmddi.h>
#include <d3dkmdt.h>

#define DlDebugPrintEx(arg) DlDebugPrint arg

//
// ** New definitions *********************************************************
//


//
// Define ACPI event IDs
//

#define ACPI_NOTIFY_DOCK_EVENT              0x77
#define ACPI_NOTIFY_PANEL_SWITCH            0x80
#define ACPI_NOTIFY_DEVICE_HOTPLUG          0x81
#define ACPI_NOTIFY_CYCLE_DISPLAY_HOTKEY    0x82
#define ACPI_NOTIFY_NEXT_DISPLAY_HOTKEY     0x83
#define ACPI_NOTIFY_PREV_DISPLAY_HOTKEY     0x84
#define ACPI_NOTIFY_CYCLE_BRIGHTNESS_HOTKEY 0x85
#define ACPI_NOTIFY_INC_BRIGHTNESS_HOTKEY   0x86
#define ACPI_NOTIFY_DEC_BRIGHTNESS_HOTKEY   0x87
#define ACPI_NOTIFY_ZERO_BRIGHTNESS_HOTKEY  0x88
#define ACPI_NOTIFY_VIDEO_WAKEUP            0x90

//
// ACPI argument definitions
//

#define ACPI_ARG_ENABLE_SWITCH_EVENT         0x0
#define ACPI_ARG_ENABLE_AUTO_SWITCH          0x1
#define ACPI_ARG_DISABLE_SWITCH_EVENT        0x2
#define ACPI_ARG_ENABLE_AUTO_LCD_BRIGHTNESS  0x0
#define ACPI_ARG_DISABLE_AUTO_LCD_BRIGHTNESS 0x4

//
// ACPI methods for the adapter
//

#define ACPI_METHOD_DISPLAY_DOS (ULONG)('SOD_')
#define ACPI_METHOD_DISPLAY_DOD (ULONG)('DOD_')
#define ACPI_METHOD_DISPLAY_ROM (ULONG)('MOR_')
#define ACPI_METHOD_DISPLAY_GPD (ULONG)('DPG_')
#define ACPI_METHOD_DISPLAY_SPD (ULONG)('DPS_')
#define ACPI_METHOD_DISPLAY_VPO (ULONG)('OPV_')

#define ACPI_METHOD_HARDWARE_ID  (ULONG)('DIH_')
#define ACPI_METHOD_SUBSYSTEM_ID (ULONG)('BUS_')
#define ACPI_METHOD_REVISION_ID  (ULONG)('VRH_')

//
// ACPI methods for children
//

#define ACPI_METHOD_OUTPUT_ADR (ULONG)('RDA_')
#define ACPI_METHOD_OUTPUT_BCL (ULONG)('LCB_')
#define ACPI_METHOD_OUTPUT_BCM (ULONG)('MCB_')
#define ACPI_METHOD_OUTPUT_DDC (ULONG)('CDD_')
#define ACPI_METHOD_OUTPUT_DCS (ULONG)('SCD_')
#define ACPI_METHOD_OUTPUT_DGS (ULONG)('SGD_')
#define ACPI_METHOD_OUTPUT_DSS (ULONG)('SSD_')

//
// ACPI Flags
//

#define DXGK_ACPI_POLL_DISPLAY_CHILDREN   0x00000001
#define DXGK_ACPI_CHANGE_DISPLAY_MODE     0x00000002
#define DXGK_ACPI_CHANGE_DISPLAY_TOPOLOGY 0x00000004
#define DXGK_ACPI_CHAIN_NOT_HANDLED       0x00000008

//
// Exclude adapter access flags.
//

#define DXGK_EXCLUDE_EVICT_ALL          0x00000001
#define DXGK_EXCLUDE_CALL_SYNCHRONOUS   0x00000002
#define DXGK_EXCLUDE_BRIDGE_ACCESS      0x00000004

#define DXGK_EXCLUDE_EVICT_STANDBY        0x00000008
#define DXGK_EXCLUDE_EVICT_HIBERNATE      0x00000010
#define DXGK_EXCLUDE_EVICT_SHUTDOWN       0x00000020
#define DXGK_EXCLUDE_D3_STATE_TRANSITION  0x00000040
#define DXGK_EXCLUDE_EVICT_DFX_STANDBY    0x00000080

//
// Max of 50 characters per string.
//

#define DXGK_MAX_STRING_LEN 50
#define DXGK_MAX_REG_SZ_LEN DXGK_MAX_STRING_LEN + 1

//
// Supported device space types.
//

#define DXGK_WHICHSPACE_CONFIG       PCI_WHICHSPACE_CONFIG
#define DXGK_WHICHSPACE_ROM          PCI_WHICHSPACE_ROM
#define DXGK_WHICHSPACE_MCH          0x80000000
#define DXGK_WHICHSPACE_BRIDGE       0x80000001

//
// The below is to allow drivers to use the updated annotations if they wish,
// but not yet require all drivers to update their annotations.
//

#ifdef ENABLE_DXGK_SAL
#define _Function_class_DXGK_(param)    _Function_class_(param)
#define _IRQL_requires_DXGK_(param)     _IRQL_requires_(param)
#define _Field_size_bytes_DXGK_(param)  _Field_size_bytes_(param)
#else
#define _Function_class_DXGK_(param)
#define _IRQL_requires_DXGK_(param)
#define _Field_size_bytes_DXGK_(param)
#endif


//
// Linked display adapter support.
//

typedef struct _LINKED_DEVICE {
    ULONG ChainUid;
    ULONG NumberOfLinksInChain;
    BOOLEAN LeadLink;
} LINKED_DEVICE, *PLINKED_DEVICE;


typedef struct _DXGK_VIDEO_OUTPUT_CAPABILITIES {
    D3DKMDT_VIDEO_OUTPUT_TECHNOLOGY InterfaceTechnology;
    D3DKMDT_MONITOR_ORIENTATION_AWARENESS MonitorOrientationAwareness;
    BOOLEAN SupportsSdtvModes;
} DXGK_VIDEO_OUTPUT_CAPABILITIES, *PDXGK_VIDEO_OUTPUT_CAPABILITIES;

//
// Type of ACPI notification event.
//

typedef enum _DXGK_EVENT_TYPE {
    DxgkUndefinedEvent,
    DxgkAcpiEvent,
    DxgkPowerStateEvent,
    DxgkDockingEvent,
    DxgkChainedAcpiEvent
} DXGK_EVENT_TYPE, *PDXGK_EVENT_TYPE;

typedef struct _DXGK_INTEGRATED_DISPLAY_CHILD {
    D3DKMDT_VIDEO_OUTPUT_TECHNOLOGY InterfaceTechnology;
    USHORT                          DescriptorLength;
} DXGK_INTEGRATED_DISPLAY_CHILD, *PDXGK_INTEGRATED_DISPLAY_CHILD;


typedef struct _DXGK_CHILD_CAPABILITIES {

    union
    {
        //
        // If (CHILD_DESCRIPTOR::ChildDeviceType == TypeVideoOutput)
        //

        DXGK_VIDEO_OUTPUT_CAPABILITIES  VideoOutput;

        //
        // If (CHILD_DESCRIPTOR::ChildDeviceType == TypeOther)
        //

        struct
        {
            UINT MustBeZero;
        }
        Other;

        //
        // If (CHILD_DESCRIPTOR::ChildDeviceType == TypeIntegratedDisplay)
        //

        DXGK_INTEGRATED_DISPLAY_CHILD   IntegratedDisplayChild;
    } Type;

    DXGK_CHILD_DEVICE_HPD_AWARENESS HpdAwareness;
} DXGK_CHILD_CAPABILITIES, *PDXGK_CHILD_CAPABILITIES;

// We don't want to add anything in the Type union which would increase the
// size beyond the original DXGK_VIDEO_OUTPUT_CAPABILITIES unexpectedly so assert it.
//static_assert( FIELD_OFFSET( DXGK_CHILD_CAPABILITIES, HpdAwareness ) == 12, "Type field has changed size" );

typedef enum _DXGK_CHILD_DEVICE_TYPE {
   TypeUninitialized,
   TypeVideoOutput,
   TypeOther,
   TypeIntegratedDisplay
} DXGK_CHILD_DEVICE_TYPE, *PDXGK_CHILD_DEVICE_TYPE;

//
// Child descriptor structure returned to us from the miniport
//
// NOTE: If (ChildDeviceType==TypeVideoOutput) then (ChildUid == video present target ID)
//

typedef struct _DXGK_CHILD_DESCRIPTOR {
   DXGK_CHILD_DEVICE_TYPE ChildDeviceType;
   DXGK_CHILD_CAPABILITIES ChildCapabilities;
   ULONG AcpiUid;
   ULONG ChildUid;
} DXGK_CHILD_DESCRIPTOR, *PDXGK_CHILD_DESCRIPTOR;

typedef struct _DXGK_DEVICE_DESCRIPTOR {
   ULONG DescriptorOffset;
   ULONG DescriptorLength;
   _Field_size_bytes_DXGK_(DescriptorLength) PVOID DescriptorBuffer;
} DXGK_DEVICE_DESCRIPTOR, *PDXGK_DEVICE_DESCRIPTOR;

typedef struct _DXGK_GENERIC_DESCRIPTOR {
   WCHAR HardwareId[DXGK_MAX_REG_SZ_LEN];
   WCHAR InstanceId[DXGK_MAX_REG_SZ_LEN];
   WCHAR CompatibleId[DXGK_MAX_REG_SZ_LEN];
   WCHAR DeviceText[DXGK_MAX_REG_SZ_LEN];
} DXGK_GENERIC_DESCRIPTOR, *PDXGK_GENERIC_DESCRIPTOR;

typedef enum _DXGK_CHILD_STATUS_TYPE{
   StatusUninitialized,
   StatusConnection,
   StatusRotation,
#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM1_3)
   StatusMiracastConnection,
#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM1_3)

} DXGK_CHILD_STATUS_TYPE, *PDXGK_CHILD_STATUS_TYPE;

typedef struct _DXGK_CHILD_STATUS {
   DXGK_CHILD_STATUS_TYPE Type;
   ULONG ChildUid;
   union {
      struct {
         BOOLEAN Connected;
      } HotPlug;
      struct {
         UCHAR Angle;
      } Rotation;
#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM1_3)
      struct {
         BOOLEAN Connected;
         D3DKMDT_VIDEO_OUTPUT_TECHNOLOGY MiracastMonitorType;
      } Miracast;
#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM1_3)
   };
} DXGK_CHILD_STATUS, *PDXGK_CHILD_STATUS;

typedef struct _DXGK_START_INFO {
    ULONG                          RequiredDmaQueueEntry;
    GUID                           AdapterGuid;

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WIN8)

    LUID                           AdapterLuid;

#endif // DXGKDDI_INTERFACE_VERSION

} DXGK_START_INFO, *PDXGK_START_INFO;


typedef enum
{
    DxgkServicesAgp,
    DxgkServicesDebugReport,
    DxgkServicesTimedOperation,
    DxgkServicesSPB,
    DxgkServicesBDD,
    DxgkServicesFirmwareTable,
    DxgkServicesIDD,
} DXGK_SERVICES;

typedef struct _ACPI_EVAL_INPUT_BUFFER_V1 ACPI_EVAL_INPUT_BUFFER, *PACPI_EVAL_INPUT_BUFFER;

typedef struct _ACPI_EVAL_INPUT_BUFFER_SIMPLE_INTEGER_V1
    ACPI_EVAL_INPUT_BUFFER_SIMPLE_INTEGER, *PACPI_EVAL_INPUT_BUFFER_SIMPLE_INTEGER;

typedef struct _ACPI_EVAL_INPUT_BUFFER_SIMPLE_STRING_V1
     ACPI_EVAL_INPUT_BUFFER_SIMPLE_STRING, *PACPI_EVAL_INPUT_BUFFER_SIMPLE_STRING;

typedef struct _ACPI_EVAL_INPUT_BUFFER_COMPLEX_V1
     ACPI_EVAL_INPUT_BUFFER_COMPLEX, *PACPI_EVAL_INPUT_BUFFER_COMPLEX;

typedef struct _ACPI_EVAL_INPUT_BUFFER_V1_EX ACPI_EVAL_INPUT_BUFFER_EX, *PACPI_EVAL_INPUT_BUFFER_EX;

typedef struct _ACPI_EVAL_INPUT_BUFFER_SIMPLE_INTEGER_V1_EX
    ACPI_EVAL_INPUT_BUFFER_SIMPLE_INTEGER_EX, *PACPI_EVAL_INPUT_BUFFER_SIMPLE_INTEGER_EX;

typedef struct _ACPI_EVAL_INPUT_BUFFER_SIMPLE_STRING_V1_EX
     ACPI_EVAL_INPUT_BUFFER_SIMPLE_STRING_EX, *PACPI_EVAL_INPUT_BUFFER_SIMPLE_STRING_EX;

typedef struct _ACPI_EVAL_INPUT_BUFFER_COMPLEX_V1_EX
     ACPI_EVAL_INPUT_BUFFER_COMPLEX_EX, *PACPI_EVAL_INPUT_BUFFER_COMPLEX_EX;

//
// Define ACPI_EVAL_OUTPUT_BUFFER structure as V1 for DDKVERSION < NTDDI_WIN10_RS2
//

typedef struct _ACPI_EVAL_OUTPUT_BUFFER_V1 ACPI_EVAL_OUTPUT_BUFFER;
typedef struct _ACPI_EVAL_OUTPUT_BUFFER_V1 UNALIGNED *PACPI_EVAL_OUTPUT_BUFFER;


typedef
_Function_class_DXGK_(DXGKCB_EVAL_ACPI_METHOD)
_IRQL_requires_(PASSIVE_LEVEL)
NTSTATUS
(APIENTRY *DXGKCB_EVAL_ACPI_METHOD)(
    _In_ HANDLE DeviceHandle,
    _In_ ULONG DeviceUid,
    _In_reads_bytes_(AcpiInputSize) PACPI_EVAL_INPUT_BUFFER_COMPLEX AcpiInputBuffer,
    _In_range_(>=, sizeof(ACPI_EVAL_INPUT_BUFFER_COMPLEX)) ULONG AcpiInputSize,
    _Out_writes_bytes_(AcpiOutputSize) PACPI_EVAL_OUTPUT_BUFFER AcpiOutputBuffer,
    _In_range_(>=, sizeof(ACPI_EVAL_OUTPUT_BUFFER)) ULONG AcpiOutputSize
    );

typedef enum {
    DockStateUnsupported = 0,
    DockStateUnDocked    = 1,
    DockStateDocked      = 2,
    DockStateUnknown     = 3,
} DOCKING_STATE;

typedef struct _DXGK_DEVICE_INFO {
    PVOID MiniportDeviceContext;
    PDEVICE_OBJECT PhysicalDeviceObject;
    UNICODE_STRING DeviceRegistryPath;
    PCM_RESOURCE_LIST TranslatedResourceList;
    LARGE_INTEGER SystemMemorySize;
    PHYSICAL_ADDRESS HighestPhysicalAddress;
    PHYSICAL_ADDRESS AgpApertureBase;
    SIZE_T AgpApertureSize;
    DOCKING_STATE DockingState;
} DXGK_DEVICE_INFO, *PDXGK_DEVICE_INFO;

typedef
_Function_class_DXGK_(DXGKCB_GET_DEVICE_INFORMATION)
_IRQL_requires_(PASSIVE_LEVEL)
NTSTATUS
(APIENTRY *DXGKCB_GET_DEVICE_INFORMATION)(
    _In_ HANDLE DeviceHandle,
    _Out_ PDXGK_DEVICE_INFO DeviceInfo
    );

typedef
_Function_class_DXGK_(DXGKCB_INDICATE_CHILD_STATUS)
_IRQL_requires_max_(DISPATCH_LEVEL)
NTSTATUS
(APIENTRY *DXGKCB_INDICATE_CHILD_STATUS)(
    _In_ HANDLE DeviceHandle,
    _In_ PDXGK_CHILD_STATUS ChildStatus
    );

typedef
_Function_class_DXGK_(DXGKCB_MAP_MEMORY)
_IRQL_requires_(PASSIVE_LEVEL)
NTSTATUS
(APIENTRY *DXGKCB_MAP_MEMORY)(
    _In_ HANDLE DeviceHandle,
    _In_ PHYSICAL_ADDRESS TranslatedAddress,
    _In_ ULONG Length,
    _In_ BOOLEAN InIoSpace,
    _In_ BOOLEAN MapToUserMode,
    _In_ MEMORY_CACHING_TYPE CacheType,
    _Outptr_ PVOID *VirtualAddress
    );

typedef
_Function_class_DXGK_(DXGKCB_QUERY_SERVICES)
_IRQL_requires_(PASSIVE_LEVEL)
NTSTATUS
(APIENTRY *DXGKCB_QUERY_SERVICES)(
    _In_ HANDLE DeviceHandle,
    _In_ DXGK_SERVICES ServicesType,
    _Inout_ PINTERFACE Interface
    );

typedef
_Function_class_DXGK_(DXGKCB_QUEUE_DPC)
_Success_(return != 0)
BOOLEAN
(APIENTRY *DXGKCB_QUEUE_DPC)(
    _In_ HANDLE DeviceHandle
    );

typedef
_Function_class_DXGK_(DXGKCB_READ_DEVICE_SPACE)
_IRQL_requires_DXGK_(PASSIVE_LEVEL)
NTSTATUS
(APIENTRY *DXGKCB_READ_DEVICE_SPACE)(
    _In_ HANDLE DeviceHandle,
    _In_ ULONG DataType,
    _Out_writes_bytes_to_(Length, *BytesRead) PVOID Buffer,
    _In_ ULONG Offset,
    _In_ ULONG Length,
    _Out_ PULONG BytesRead
    );

typedef
_Function_class_DXGK_(DXGKCB_SYNCHRONIZE_EXECUTION)
_IRQL_requires_max_(DISPATCH_LEVEL)
NTSTATUS
(APIENTRY *DXGKCB_SYNCHRONIZE_EXECUTION)(
    _In_ HANDLE DeviceHandle,
    _In_ PKSYNCHRONIZE_ROUTINE SynchronizeRoutine,
    _In_ PVOID Context,
    _In_ ULONG MessageNumber,
    _Out_ PBOOLEAN ReturnValue
    );

typedef
_Function_class_DXGK_(DXGKCB_UNMAP_MEMORY)
_IRQL_requires_(PASSIVE_LEVEL)
NTSTATUS
(APIENTRY *DXGKCB_UNMAP_MEMORY)(
    _In_ HANDLE DeviceHandle,
    _In_ PVOID VirtualAddress
    );

typedef
_Function_class_DXGK_(DXGKCB_WRITE_DEVICE_SPACE)
_IRQL_requires_DXGK_(PASSIVE_LEVEL)
NTSTATUS
(APIENTRY *DXGKCB_WRITE_DEVICE_SPACE)(
    _In_ HANDLE DeviceHandle,
    _In_ ULONG DataType,
    _In_reads_bytes_(Length) PVOID Buffer,
    _In_ ULONG Offset,
    _In_ ULONG Length,
    _Out_ _Out_range_(<=, Length) PULONG BytesWritten
    );

typedef
_Function_class_DXGK_(DXGKCB_IS_DEVICE_PRESENT)
_IRQL_requires_(PASSIVE_LEVEL)
NTSTATUS
(APIENTRY *DXGKCB_IS_DEVICE_PRESENT)(
    _In_ HANDLE DeviceHandle,
    _In_ PPCI_DEVICE_PRESENCE_PARAMETERS DevicePresenceParameters,
    _Out_ PBOOLEAN DevicePresent
    );

typedef
_Function_class_DXGK_(DXGKCB_LOG_ETW_EVENT)
_When_(EventBufferSize > 256, _IRQL_requires_DXGK_(PASSIVE_LEVEL))
VOID
(APIENTRY *DXGKCB_LOG_ETW_EVENT)(
    _In_ CONST LPCGUID EventGuid,
    _In_ UCHAR Type,
    _In_ USHORT EventBufferSize,
    _In_reads_bytes_(EventBufferSize) PVOID EventBuffer
    );

#if 0
typedef
_Function_class_DXGK_(DXGKCB_EXCLUDE_ADAPTER_ACCESS)
_IRQL_requires_(PASSIVE_LEVEL)
NTSTATUS
(APIENTRY *DXGKCB_EXCLUDE_ADAPTER_ACCESS)(
    _In_ HANDLE DeviceHandle,
    _In_ ULONG Attributes,
    _In_ DXGKDDI_PROTECTED_CALLBACK DxgkProtectedCallback,
    _In_ PVOID ProtectedCallbackContext
    );

#endif
typedef struct _DXGKRNL_INTERFACE {
    ULONG                                   Size;
    ULONG                                   Version;
    HANDLE                                  DeviceHandle;

    DXGKCB_EVAL_ACPI_METHOD                 DxgkCbEvalAcpiMethod;
    DXGKCB_GET_DEVICE_INFORMATION           DxgkCbGetDeviceInformation;
    DXGKCB_INDICATE_CHILD_STATUS            DxgkCbIndicateChildStatus;
    DXGKCB_MAP_MEMORY                       DxgkCbMapMemory;
    DXGKCB_QUEUE_DPC                        DxgkCbQueueDpc;
    DXGKCB_QUERY_SERVICES                   DxgkCbQueryServices;
    DXGKCB_READ_DEVICE_SPACE                DxgkCbReadDeviceSpace;
    DXGKCB_SYNCHRONIZE_EXECUTION            DxgkCbSynchronizeExecution;
    DXGKCB_UNMAP_MEMORY                     DxgkCbUnmapMemory;
    DXGKCB_WRITE_DEVICE_SPACE               DxgkCbWriteDeviceSpace;
    DXGKCB_IS_DEVICE_PRESENT                DxgkCbIsDevicePresent;

    DXGKCB_GETHANDLEDATA                    DxgkCbGetHandleData;
    DXGKCB_GETHANDLEPARENT                  DxgkCbGetHandleParent;
    DXGKCB_ENUMHANDLECHILDREN               DxgkCbEnumHandleChildren;
    DXGKCB_NOTIFY_INTERRUPT                 DxgkCbNotifyInterrupt;
    DXGKCB_NOTIFY_DPC                       DxgkCbNotifyDpc;
    DXGKCB_QUERYVIDPNINTERFACE              DxgkCbQueryVidPnInterface;
    DXGKCB_QUERYMONITORINTERFACE            DxgkCbQueryMonitorInterface;
    DXGKCB_GETCAPTUREADDRESS                DxgkCbGetCaptureAddress;

    DXGKCB_LOG_ETW_EVENT                    DxgkCbLogEtwEvent;

    //DXGKCB_EXCLUDE_ADAPTER_ACCESS           DxgkCbExcludeAdapterAccess;

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WIN8)

    DXGKCB_CREATECONTEXTALLOCATION          DxgkCbCreateContextAllocation;
    DXGKCB_DESTROYCONTEXTALLOCATION         DxgkCbDestroyContextAllocation;
    DXGKCB_SETPOWERCOMPONENTACTIVE          DxgkCbSetPowerComponentActive;
    DXGKCB_SETPOWERCOMPONENTIDLE            DxgkCbSetPowerComponentIdle;
   // DXGKCB_ACQUIRE_POST_DISPLAY_OWNERSHIP   DxgkCbAcquirePostDisplayOwnership;
    DXGKCB_POWERRUNTIMECONTROLREQUEST       DxgkCbPowerRuntimeControlRequest;
    DXGKCB_SETPOWERCOMPONENTLATENCY         DxgkCbSetPowerComponentLatency;
    DXGKCB_SETPOWERCOMPONENTRESIDENCY       DxgkCbSetPowerComponentResidency;
    DXGKCB_COMPLETEFSTATETRANSITION         DxgkCbCompleteFStateTransition;

#endif // DXGKDDI_INTERFACE_VERSION

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM1_3)

    DXGKCB_COMPLETEPSTATETRANSITION         DxgkCbCompletePStateTransition;

#endif // DXGKDDI_INTERFACE_VERSION_WDDM1_3

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_0)

    DXGKCB_MAPCONTEXTALLOCATION             DxgkCbMapContextAllocation;
    DXGKCB_UPDATECONTEXTALLOCATION          DxgkCbUpdateContextAllocation;
    DXGKCB_RESERVEGPUVIRTUALADDRESSRANGE    DxgkCbReserveGpuVirtualAddressRange;
    DXGKCB_ACQUIREHANDLEDATA                DxgkCbAcquireHandleData;
    DXGKCB_RELEASEHANDLEDATA                DxgkCbReleaseHandleData;
    DXGKCB_HARDWARECONTENTPROTECTIONTEARDOWN DxgkCbHardwareContentProtectionTeardown;

#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_0)

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_1)

    DXGKCB_MULTIPLANEOVERLAYDISABLED        DxgkCbMultiPlaneOverlayDisabled;
    DXGKCB_DXGKCB_MITIGATEDRANGEUPDATE      DxgkCbMitigatedRangeUpdate;

#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_1)

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_2)

    DXGKCB_INVALIDATEHWCONTEXT              DxgkCbInvalidateHwContext;
    DXGKCB_INDICATE_CONNECTOR_CHANGE        DxgkCbIndicateConnectorChange;
    DXGKCB_UNBLOCKUEFIFRAMEBUFFERRANGES     DxgkCbUnblockUEFIFrameBufferRanges;
   // DXGKCB_ACQUIRE_POST_DISPLAY_OWNERSHIP2  DxgkCbAcquirePostDisplayOwnership2;

#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_2)

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_3)

    DXGKCB_SETPROTECTEDSESSIONSTATUS        DxgkCbSetProtectedSessionStatus;

#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_3)

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_4)
    DXGKCB_ALLOCATECONTIGUOUSMEMORY         DxgkCbAllocateContiguousMemory;
    DXGKCB_FREECONTIGUOUSMEMORY             DxgkCbFreeContiguousMemory;
    DXGKCB_ALLOCATEPAGESFORMDL              DxgkCbAllocatePagesForMdl;
    DXGKCB_FREEPAGESFROMMDL                 DxgkCbFreePagesFromMdl;
    DXGKCB_PINFRAMEBUFFERFORSAVE            DxgkCbPinFrameBufferForSave;
    DXGKCB_UNPINFRAMEBUFFERFORSAVE          DxgkCbUnpinFrameBufferForSave;
    DXGKCB_MAPFRAMEBUFFERPOINTER            DxgkCbMapFrameBufferPointer;
    DXGKCB_UNMAPFRAMEBUFFERPOINTER          DxgkCbUnmapFrameBufferPointer;
    DXGKCB_MAPMDLTOIOMMU                    DxgkCbMapMdlToIoMmu;
    DXGKCB_UNMAPMDLFROMIOMMU                DxgkCbUnmapMdlFromIoMmu;
   // DXGKCB_REPORT_DIAGNOSTIC                DxgkCbReportDiagnostic;
#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_4)

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_5)
    DXGKCB_SIGNALEVENT                      DxgkCbSignalEvent;
#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_5)

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_6)
    DXGKCB_ISFEATUREENABLED                 DxgkCbIsFeatureEnabled;
    DXGKCB_SAVEMEMORYFORHOTUPDATE           DxgkCbSaveMemoryForHotUpdate;
#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_6)

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_8)
    DXGKCB_NOTIFYCURSORSUPPORTCHANGE        DxgkCbNotifyCursorSupportChange;
#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_8)

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_9)
    DXGKCB_QUERYFEATURESUPPORT              DxgkCbQueryFeatureSupport;
    DXGKCB_CREATEPHYSICALMEMORYOBJECT       DxgkCbCreatePhysicalMemoryObject;
    DXGKCB_DESTROYPHYSICALMEMORYOBJECT      DxgkCbDestroyPhysicalMemoryObject;
    DXGKCB_MAPPHYSICALMEMORY                DxgkCbMapPhysicalMemory;
    DXGKCB_UNMAPPHYSICALMEMORY              DxgkCbUnmapPhysicalMemory;
    DXGKCB_ALLOCATEADL                      DxgkCbAllocateAdl;
    DXGKCB_FREEADL                          DxgkCbFreeAdl;
    DXGKCB_OPENPHYSICALMEMORYOBJECT         DxgkCbOpenPhysicalMemoryObject;
    DXGKCB_CLOSEPHYSICALMEMORYOBJECT        DxgkCbClosePhysicalMemoryObject;
    DXGKCB_PINFRAMEBUFFERFORSAVE2           DxgkCbPinFrameBufferForSave2;
#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_9)

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM3_1)
    DXGKCB_DISCONNECTDOORBELL               DxgkCbDisconnectDoorbell;
#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM3_1)

} DXGKRNL_INTERFACE, *PDXGKRNL_INTERFACE;

typedef _In_    CONST PDEVICE_OBJECT     IN_CONST_PDEVICE_OBJECT;
typedef _Inout_ PLINKED_DEVICE           INOUT_PLINKED_DEVICE;
typedef _Inout_ PDXGK_CHILD_DESCRIPTOR   INOUT_PDXGK_CHILD_DESCRIPTOR;
typedef _In_    PDXGK_CHILD_STATUS       IN_PDXGK_CHILD_STATUS;
typedef _Inout_ PDXGK_CHILD_STATUS       INOUT_PDXGK_CHILD_STATUS;
typedef _Inout_ PDXGK_DEVICE_DESCRIPTOR  INOUT_PDXGK_DEVICE_DESCRIPTOR;
typedef _In_    DXGK_EVENT_TYPE          IN_DXGK_EVENT_TYPE;
typedef _In_    PDXGK_START_INFO         IN_PDXGK_START_INFO;
typedef _In_    PDXGKRNL_INTERFACE       IN_PDXGKRNL_INTERFACE;
typedef _In_    PQUERY_INTERFACE         IN_PQUERY_INTERFACE;
typedef _In_    PVIDEO_REQUEST_PACKET    IN_PVIDEO_REQUEST_PACKET;

typedef
    _Check_return_
_Function_class_DXGK_(DXGKDDI_ADD_DEVICE)
_IRQL_requires_DXGK_(PASSIVE_LEVEL)
NTSTATUS
DXGKDDI_ADD_DEVICE(
    IN_CONST_PDEVICE_OBJECT     PhysicalDeviceObject,
    OUT_PPVOID                  MiniportDeviceContext
    );

typedef
    _Check_return_
_Function_class_DXGK_(DXGKDDI_START_DEVICE)
_IRQL_requires_DXGK_(PASSIVE_LEVEL)
NTSTATUS
DXGKDDI_START_DEVICE(
    IN_CONST_PVOID          MiniportDeviceContext,
    IN_PDXGK_START_INFO     DxgkStartInfo,
    IN_PDXGKRNL_INTERFACE   DxgkInterface,
    OUT_PULONG              NumberOfVideoPresentSources,
    OUT_PULONG              NumberOfChildren
    );

typedef
    _Check_return_
_Function_class_DXGK_(DXGKDDI_STOP_DEVICE)
_IRQL_requires_DXGK_(PASSIVE_LEVEL)
NTSTATUS
DXGKDDI_STOP_DEVICE(
    IN_CONST_PVOID  MiniportDeviceContext
    );

typedef
    _Check_return_
_Function_class_DXGK_(DXGKDDI_REMOVE_DEVICE)
_IRQL_requires_DXGK_(PASSIVE_LEVEL)
NTSTATUS
DXGKDDI_REMOVE_DEVICE(
    IN_CONST_PVOID  MiniportDeviceContext
    );

typedef
    _Check_return_
_Function_class_DXGK_(DXGKDDI_DISPATCH_IO_REQUEST)
_IRQL_requires_DXGK_(PASSIVE_LEVEL)
NTSTATUS
DXGKDDI_DISPATCH_IO_REQUEST(
    IN_CONST_PVOID              MiniportDeviceContext,
    IN_ULONG                    VidPnSourceId,
    IN_PVIDEO_REQUEST_PACKET    VideoRequestPacket
    );

typedef
    _Check_return_
_Function_class_DXGK_(DXGKDDI_QUERY_CHILD_RELATIONS)
_IRQL_requires_DXGK_(PASSIVE_LEVEL)
NTSTATUS
DXGKDDI_QUERY_CHILD_RELATIONS(
    IN_CONST_PVOID                                                    MiniportDeviceContext,
    _Inout_updates_bytes_(ChildRelationsSize) PDXGK_CHILD_DESCRIPTOR  ChildRelations,
    _In_ ULONG                                                        ChildRelationsSize
    );

typedef
    _Check_return_
_Function_class_DXGK_(DXGKDDI_QUERY_CHILD_STATUS)
_IRQL_requires_DXGK_(PASSIVE_LEVEL)
NTSTATUS
DXGKDDI_QUERY_CHILD_STATUS(
    IN_CONST_PVOID              MiniportDeviceContext,
    INOUT_PDXGK_CHILD_STATUS    ChildStatus,
    IN_BOOLEAN                  NonDestructiveOnly
    );


typedef
    _Check_return_
_Function_class_DXGK_(DXGKDDI_INTERRUPT_ROUTINE)
_IRQL_requires_DXGK_(HIGH_LEVEL) // HIGH_LEVEL is best approximation of DIRQL
BOOLEAN
DXGKDDI_INTERRUPT_ROUTINE(
    IN_CONST_PVOID  MiniportDeviceContext,
    IN_ULONG        MessageNumber
    );

typedef
_Function_class_DXGK_(DXGKDDI_DPC_ROUTINE)
_IRQL_requires_DXGK_(DISPATCH_LEVEL)
VOID
DXGKDDI_DPC_ROUTINE(
    IN_CONST_PVOID  MiniportDeviceContext
    );

typedef
    _Check_return_
_Function_class_DXGK_(DXGKDDI_QUERY_DEVICE_DESCRIPTOR)
_IRQL_requires_DXGK_(PASSIVE_LEVEL)
NTSTATUS
DXGKDDI_QUERY_DEVICE_DESCRIPTOR(
    IN_CONST_PVOID                  MiniportDeviceContext,
    IN_ULONG                        ChildUid,
    INOUT_PDXGK_DEVICE_DESCRIPTOR   DeviceDescriptor
    );

typedef
    _Check_return_
_Function_class_DXGK_(DXGKDDI_SET_POWER_STATE)
_IRQL_requires_DXGK_(PASSIVE_LEVEL)
NTSTATUS
DXGKDDI_SET_POWER_STATE(
    IN_CONST_PVOID          MiniportDeviceContext,
    IN_ULONG                DeviceUid,
    IN_DEVICE_POWER_STATE   DevicePowerState,
    IN_POWER_ACTION         ActionType
    );

typedef
    _Check_return_
_Function_class_DXGK_(DXGKDDI_NOTIFY_ACPI_EVENT)
_IRQL_requires_DXGK_(PASSIVE_LEVEL)
NTSTATUS
DXGKDDI_NOTIFY_ACPI_EVENT(
    IN_CONST_PVOID      MiniportDeviceContext,
    IN_DXGK_EVENT_TYPE  EventType,
    IN_ULONG            Event,
    IN_PVOID            Argument,
    OUT_PULONG          AcpiFlags
    );

typedef
_Function_class_DXGK_(DXGKDDI_RESET_DEVICE)
VOID
DXGKDDI_RESET_DEVICE(
    IN_CONST_PVOID  MiniportDeviceContext
    );

typedef
_Function_class_DXGK_(DXGKDDI_UNLOAD)
_IRQL_requires_DXGK_(PASSIVE_LEVEL)
VOID
DXGKDDI_UNLOAD(
    VOID
    );

typedef
    _Check_return_
_Function_class_DXGK_(DXGKDDI_QUERY_INTERFACE)
_IRQL_requires_DXGK_(PASSIVE_LEVEL)
NTSTATUS
DXGKDDI_QUERY_INTERFACE(
    IN_CONST_PVOID          MiniportDeviceContext,
    IN_PQUERY_INTERFACE     QueryInterface
    );

typedef
_Function_class_DXGK_(DXGKDDI_CONTROL_ETW_LOGGING)
_IRQL_requires_DXGK_(PASSIVE_LEVEL)
VOID
DXGKDDI_CONTROL_ETW_LOGGING(
    IN_BOOLEAN  Enable,
    IN_ULONG    Flags,
    IN_UCHAR    Level
    );

typedef
    _Check_return_
_Function_class_DXGK_(DXGKDDI_LINK_DEVICE)
_IRQL_requires_DXGK_(PASSIVE_LEVEL)
NTSTATUS
DXGKDDI_LINK_DEVICE(
    IN_CONST_PDEVICE_OBJECT   PhysicalDeviceObject,
    IN_CONST_PVOID            MiniportDeviceContext,
    INOUT_PLINKED_DEVICE      LinkedDevice
    );


typedef DXGKDDI_ADD_DEVICE                      *PDXGKDDI_ADD_DEVICE;
typedef DXGKDDI_START_DEVICE                    *PDXGKDDI_START_DEVICE;
typedef DXGKDDI_STOP_DEVICE                     *PDXGKDDI_STOP_DEVICE;
typedef DXGKDDI_REMOVE_DEVICE                   *PDXGKDDI_REMOVE_DEVICE;
typedef DXGKDDI_DISPATCH_IO_REQUEST             *PDXGKDDI_DISPATCH_IO_REQUEST;
typedef DXGKDDI_QUERY_CHILD_RELATIONS           *PDXGKDDI_QUERY_CHILD_RELATIONS;
typedef DXGKDDI_QUERY_CHILD_STATUS              *PDXGKDDI_QUERY_CHILD_STATUS;
typedef DXGKDDI_INTERRUPT_ROUTINE               *PDXGKDDI_INTERRUPT_ROUTINE;
typedef DXGKDDI_DPC_ROUTINE                     *PDXGKDDI_DPC_ROUTINE;
typedef DXGKDDI_QUERY_DEVICE_DESCRIPTOR         *PDXGKDDI_QUERY_DEVICE_DESCRIPTOR;
typedef DXGKDDI_SET_POWER_STATE                 *PDXGKDDI_SET_POWER_STATE;
typedef DXGKDDI_NOTIFY_ACPI_EVENT               *PDXGKDDI_NOTIFY_ACPI_EVENT;
typedef DXGKDDI_RESET_DEVICE                    *PDXGKDDI_RESET_DEVICE;
typedef DXGKDDI_UNLOAD                          *PDXGKDDI_UNLOAD;
typedef DXGKDDI_QUERY_INTERFACE                 *PDXGKDDI_QUERY_INTERFACE;
typedef DXGKDDI_CONTROL_ETW_LOGGING             *PDXGKDDI_CONTROL_ETW_LOGGING;
typedef DXGKDDI_LINK_DEVICE                     *PDXGKDDI_LINK_DEVICE;
#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_2)
typedef DXGKDDI_EXCHANGEPRESTARTINFO            *PDXGKDDI_EXCHANGEPRESTARTINFO;
typedef DXGKDDI_SETTARGETADJUSTEDCOLORIMETRY    *PDXGKDDI_SETTARGETADJUSTEDCOLORIMETRY;
#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_2)
#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_4)
typedef DXGKDDI_QUERYDIAGNOSTICTYPESSUPPORT     *PDXGKDDI_QUERYDIAGNOSTICTYPESSUPPORT;
typedef DXGKDDI_CONTROLDIAGNOSTICREPORTING      *PDXGKDDI_CONTROLDIAGNOSTICREPORTING;
#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_4)
#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_5)
typedef DXGKDDI_SETTARGETADJUSTEDCOLORIMETRY2   *PDXGKDDI_SETTARGETADJUSTEDCOLORIMETRY2;
#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_5)
#if 1
typedef struct _DRIVER_INITIALIZATION_DATA {
    ULONG                                   Version;
    PDXGKDDI_ADD_DEVICE                     DxgkDdiAddDevice;
    PDXGKDDI_START_DEVICE                   DxgkDdiStartDevice;
    PDXGKDDI_STOP_DEVICE                    DxgkDdiStopDevice;
    PDXGKDDI_REMOVE_DEVICE                  DxgkDdiRemoveDevice;
    PDXGKDDI_DISPATCH_IO_REQUEST            DxgkDdiDispatchIoRequest;
    PDXGKDDI_INTERRUPT_ROUTINE              DxgkDdiInterruptRoutine;
    PDXGKDDI_DPC_ROUTINE                    DxgkDdiDpcRoutine;
    PDXGKDDI_QUERY_CHILD_RELATIONS          DxgkDdiQueryChildRelations;
    PDXGKDDI_QUERY_CHILD_STATUS             DxgkDdiQueryChildStatus;
    PDXGKDDI_QUERY_DEVICE_DESCRIPTOR        DxgkDdiQueryDeviceDescriptor;
    PDXGKDDI_SET_POWER_STATE                DxgkDdiSetPowerState;
    PDXGKDDI_NOTIFY_ACPI_EVENT              DxgkDdiNotifyAcpiEvent;
    PDXGKDDI_RESET_DEVICE                   DxgkDdiResetDevice;
    PDXGKDDI_UNLOAD                         DxgkDdiUnload;
    PDXGKDDI_QUERY_INTERFACE                DxgkDdiQueryInterface;
    PDXGKDDI_CONTROL_ETW_LOGGING            DxgkDdiControlEtwLogging;

    PDXGKDDI_QUERYADAPTERINFO               DxgkDdiQueryAdapterInfo;
    PDXGKDDI_CREATEDEVICE                   DxgkDdiCreateDevice;
    PDXGKDDI_CREATEALLOCATION               DxgkDdiCreateAllocation;
    PDXGKDDI_DESTROYALLOCATION              DxgkDdiDestroyAllocation;
    PDXGKDDI_DESCRIBEALLOCATION             DxgkDdiDescribeAllocation;
    PDXGKDDI_GETSTANDARDALLOCATIONDRIVERDATA DxgkDdiGetStandardAllocationDriverData;
    PDXGKDDI_ACQUIRESWIZZLINGRANGE          DxgkDdiAcquireSwizzlingRange;
    PDXGKDDI_RELEASESWIZZLINGRANGE          DxgkDdiReleaseSwizzlingRange;
    PDXGKDDI_PATCH                          DxgkDdiPatch;
    PDXGKDDI_SUBMITCOMMAND                  DxgkDdiSubmitCommand;
    PDXGKDDI_PREEMPTCOMMAND                 DxgkDdiPreemptCommand;
    PDXGKDDI_BUILDPAGINGBUFFER              DxgkDdiBuildPagingBuffer;
    PDXGKDDI_SETPALETTE                     DxgkDdiSetPalette;
    PDXGKDDI_SETPOINTERPOSITION             DxgkDdiSetPointerPosition;
    PDXGKDDI_SETPOINTERSHAPE                DxgkDdiSetPointerShape;
    PDXGKDDI_RESETFROMTIMEOUT               DxgkDdiResetFromTimeout;
    PDXGKDDI_RESTARTFROMTIMEOUT             DxgkDdiRestartFromTimeout;
    PDXGKDDI_ESCAPE                         DxgkDdiEscape;
    PDXGKDDI_COLLECTDBGINFO                 DxgkDdiCollectDbgInfo;
    PDXGKDDI_QUERYCURRENTFENCE              DxgkDdiQueryCurrentFence;
    PDXGKDDI_ISSUPPORTEDVIDPN               DxgkDdiIsSupportedVidPn;
    PDXGKDDI_RECOMMENDFUNCTIONALVIDPN       DxgkDdiRecommendFunctionalVidPn;
    PDXGKDDI_ENUMVIDPNCOFUNCMODALITY        DxgkDdiEnumVidPnCofuncModality;
    PDXGKDDI_SETVIDPNSOURCEADDRESS          DxgkDdiSetVidPnSourceAddress;
    PDXGKDDI_SETVIDPNSOURCEVISIBILITY       DxgkDdiSetVidPnSourceVisibility;
    PDXGKDDI_COMMITVIDPN                    DxgkDdiCommitVidPn;
    PDXGKDDI_UPDATEACTIVEVIDPNPRESENTPATH   DxgkDdiUpdateActiveVidPnPresentPath;
    PDXGKDDI_RECOMMENDMONITORMODES          DxgkDdiRecommendMonitorModes;
    PDXGKDDI_RECOMMENDVIDPNTOPOLOGY         DxgkDdiRecommendVidPnTopology;
    PDXGKDDI_GETSCANLINE                    DxgkDdiGetScanLine;
    PDXGKDDI_STOPCAPTURE                    DxgkDdiStopCapture;
    PDXGKDDI_CONTROLINTERRUPT               DxgkDdiControlInterrupt;
    PDXGKDDI_CREATEOVERLAY                  DxgkDdiCreateOverlay;

    //
    // Device functions
    //

    PDXGKDDI_DESTROYDEVICE                  DxgkDdiDestroyDevice;
    PDXGKDDI_OPENALLOCATIONINFO             DxgkDdiOpenAllocation;
    PDXGKDDI_CLOSEALLOCATION                DxgkDdiCloseAllocation;
    PDXGKDDI_RENDER                         DxgkDdiRender;
    PDXGKDDI_PRESENT                        DxgkDdiPresent;

    //
    // Overlay functions
    //

    PDXGKDDI_UPDATEOVERLAY                  DxgkDdiUpdateOverlay;
    PDXGKDDI_FLIPOVERLAY                    DxgkDdiFlipOverlay;
    PDXGKDDI_DESTROYOVERLAY                 DxgkDdiDestroyOverlay;

    //
    // Context supports.
    //

    PDXGKDDI_CREATECONTEXT                  DxgkDdiCreateContext;
    PDXGKDDI_DESTROYCONTEXT                 DxgkDdiDestroyContext;

    //
    // Linked Display Adapter support.
    //

    PDXGKDDI_LINK_DEVICE                    DxgkDdiLinkDevice;
    PDXGKDDI_SETDISPLAYPRIVATEDRIVERFORMAT  DxgkDdiSetDisplayPrivateDriverFormat;

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WIN7)
    //
    // Extended for WDDM 2.0
    //
    PVOID                                   DxgkDdiDescribePageTable;
    PVOID                                   DxgkDdiUpdatePageTable;
    PVOID                                   DxgkDdiUpdatePageDirectory;
    PVOID                                   DxgkDdiMovePageDirectory;

    PVOID                                   DxgkDdiSubmitRender;
    PVOID                                   DxgkDdiCreateAllocation2;

    //
    // GDI acceleration. Extended for WDDM 1.0
    //
    PDXGKDDI_RENDER                         DxgkDdiRenderKm;

    //
    // New DMM DDIs for CCD support
    //
    VOID*                                   Reserved;
    PDXGKDDI_QUERYVIDPNHWCAPABILITY         DxgkDdiQueryVidPnHWCapability;

#endif // DXGKDDI_INTERFACE_VERSION

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WIN8)
    PDXGKDDISETPOWERCOMPONENTFSTATE         DxgkDdiSetPowerComponentFState;

    //
    // New DDIs for TDR support.
    //
    PDXGKDDI_QUERYDEPENDENTENGINEGROUP      DxgkDdiQueryDependentEngineGroup;
    PDXGKDDI_QUERYENGINESTATUS              DxgkDdiQueryEngineStatus;
    PDXGKDDI_RESETENGINE                    DxgkDdiResetEngine;

    //
    // New DDIs for PnP stop/start support.
    //
   // PDXGKDDI_STOP_DEVICE_AND_RELEASE_POST_DISPLAY_OWNERSHIP DxgkDdiStopDeviceAndReleasePostDisplayOwnership;

    //
    // New DDIs for system display support.
    //
   // PDXGKDDI_SYSTEM_DISPLAY_ENABLE          DxgkDdiSystemDisplayEnable;
   // PDXGKDDI_SYSTEM_DISPLAY_WRITE           DxgkDdiSystemDisplayWrite;

    PDXGKDDI_CANCELCOMMAND                  DxgkDdiCancelCommand;

    //
    // New DDI for the monitor container ID support.
    //
  //  PDXGKDDI_GET_CHILD_CONTAINER_ID         DxgkDdiGetChildContainerId;

    PDXGKDDIPOWERRUNTIMECONTROLREQUEST      DxgkDdiPowerRuntimeControlRequest;

    //
    // New DDI for multi plane overlay support.
    //
    PDXGKDDI_SETVIDPNSOURCEADDRESSWITHMULTIPLANEOVERLAY DxgkDdiSetVidPnSourceAddressWithMultiPlaneOverlay;

    //
    // New DDI for the surprise removal support.
    //
 //   PDXGKDDI_NOTIFY_SURPRISE_REMOVAL        DxgkDdiNotifySurpriseRemoval;

#endif // DXGKDDI_INTERFACE_VERSION

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM1_3)
    //
    // New DDI for querying node metadata
    //
    PDXGKDDI_GETNODEMETADATA                DxgkDdiGetNodeMetadata;

    //
    // New DDI for power management enhancements
    //
    PDXGKDDISETPOWERPSTATE                  DxgkDdiSetPowerPState;
    PDXGKDDI_CONTROLINTERRUPT2              DxgkDdiControlInterrupt2;

    //
    // New DDI for multiplane overlay support
    //
    PDXGKDDI_CHECKMULTIPLANEOVERLAYSUPPORT  DxgkDdiCheckMultiPlaneOverlaySupport;

    //
    // New DDI for GPU clock calibration
    //
    PDXGKDDI_CALIBRATEGPUCLOCK              DxgkDdiCalibrateGpuClock;

    //
    // New DDI for history buffer formatting
    //
    PDXGKDDI_FORMATHISTORYBUFFER            DxgkDdiFormatHistoryBuffer;

#endif

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_0)

    PDXGKDDI_RENDERGDI                      DxgkDdiRenderGdi;
    PDXGKDDI_SUBMITCOMMANDVIRTUAL           DxgkDdiSubmitCommandVirtual;
    PDXGKDDI_SETROOTPAGETABLE               DxgkDdiSetRootPageTable;
    PDXGKDDI_GETROOTPAGETABLESIZE           DxgkDdiGetRootPageTableSize;
    PDXGKDDI_MAPCPUHOSTAPERTURE             DxgkDdiMapCpuHostAperture;
    PDXGKDDI_UNMAPCPUHOSTAPERTURE           DxgkDdiUnmapCpuHostAperture;
    PDXGKDDI_CHECKMULTIPLANEOVERLAYSUPPORT2 DxgkDdiCheckMultiPlaneOverlaySupport2;
    PDXGKDDI_CREATEPROCESS                  DxgkDdiCreateProcess;
    PDXGKDDI_DESTROYPROCESS                 DxgkDdiDestroyProcess;
    PDXGKDDI_SETVIDPNSOURCEADDRESSWITHMULTIPLANEOVERLAY2    DxgkDdiSetVidPnSourceAddressWithMultiPlaneOverlay2;
    void*                                   Reserved1;
    void*                                   Reserved2;
    PDXGKDDI_POWERRUNTIMESETDEVICEHANDLE    DxgkDdiPowerRuntimeSetDeviceHandle;
    PDXGKDDI_SETSTABLEPOWERSTATE            DxgkDdiSetStablePowerState;
    PDXGKDDI_SETVIDEOPROTECTEDREGION        DxgkDdiSetVideoProtectedRegion;

#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_0)

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_1)

    PDXGKDDI_CHECKMULTIPLANEOVERLAYSUPPORT3 DxgkDdiCheckMultiPlaneOverlaySupport3;
    PDXGKDDI_SETVIDPNSOURCEADDRESSWITHMULTIPLANEOVERLAY3    DxgkDdiSetVidPnSourceAddressWithMultiPlaneOverlay3;
    PDXGKDDI_POSTMULTIPLANEOVERLAYPRESENT   DxgkDdiPostMultiPlaneOverlayPresent;
    PDXGKDDI_VALIDATEUPDATEALLOCATIONPROPERTY       DxgkDdiValidateUpdateAllocationProperty;

    PDXGKDDI_CONTROLMODEBEHAVIOR            DxgkDdiControlModeBehavior;
    PDXGKDDI_UPDATEMONITORLINKINFO          DxgkDdiUpdateMonitorLinkInfo;
#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_1)

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_2)

    //
    // Hardware context scheduling support.
    //

    PDXGKDDI_CREATEHWCONTEXT                DxgkDdiCreateHwContext;
    PDXGKDDI_DESTROYHWCONTEXT               DxgkDdiDestroyHwContext;

    PDXGKDDI_CREATEHWQUEUE                  DxgkDdiCreateHwQueue;
    PDXGKDDI_DESTROYHWQUEUE                 DxgkDdiDestroyHwQueue;

    PDXGKDDI_SUBMITCOMMANDTOHWQUEUE         DxgkDdiSubmitCommandToHwQueue;
    PDXGKDDI_SWITCHTOHWCONTEXTLIST          DxgkDdiSwitchToHwContextList;

    PDXGKDDI_RESETHWENGINE                  DxgkDdiResetHwEngine;

    PDXGKDDI_CREATEPERIODICFRAMENOTIFICATION    DxgkDdiCreatePeriodicFrameNotification;
    PDXGKDDI_DESTROYPERIODICFRAMENOTIFICATION   DxgkDdiDestroyPeriodicFrameNotification;

    PDXGKDDI_SETTIMINGSFROMVIDPN            DxgkDdiSetTimingsFromVidPn;

    PDXGKDDI_SETTARGETGAMMA                 DxgkDdiSetTargetGamma;
    PDXGKDDI_SETTARGETCONTENTTYPE           DxgkDdiSetTargetContentType;
    PDXGKDDI_SETTARGETANALOGCOPYPROTECTION  DxgkDdiSetTargetAnalogCopyProtection;
    PDXGKDDI_SETTARGETADJUSTEDCOLORIMETRY   DxgkDdiSetTargetAdjustedColorimetry;

    PDXGKDDI_DISPLAYDETECTCONTROL           DxgkDdiDisplayDetectControl;
    PDXGKDDI_QUERYCONNECTIONCHANGE          DxgkDdiQueryConnectionChange;

    PDXGKDDI_EXCHANGEPRESTARTINFO           DxgkDdiExchangePreStartInfo;

    PDXGKDDI_GETMULTIPLANEOVERLAYCAPS       DxgkDdiGetMultiPlaneOverlayCaps;
    PDXGKDDI_GETPOSTCOMPOSITIONCAPS         DxgkDdiGetPostCompositionCaps;

#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_2)

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_3)

    PDXGKDDI_UPDATEHWCONTEXTSTATE           DxgkDdiUpdateHwContextState;

    PDXGKDDI_CREATEPROTECTEDSESSION         DxgkDdiCreateProtectedSession;
    PDXGKDDI_DESTROYPROTECTEDSESSION        DxgkDdiDestroyProtectedSession;

#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_3)

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_4)

    PDXGKDDI_SETSCHEDULINGLOGBUFFER         DxgkDdiSetSchedulingLogBuffer;
    PDXGKDDI_SETUPPRIORITYBANDS             DxgkDdiSetupPriorityBands;
    PDXGKDDI_NOTIFYFOCUSPRESENT             DxgkDdiNotifyFocusPresent;
    PDXGKDDI_SETCONTEXTSCHEDULINGPROPERTIES DxgkDdiSetContextSchedulingProperties;
    PDXGKDDI_SUSPENDCONTEXT                 DxgkDdiSuspendContext;
    PDXGKDDI_RESUMECONTEXT                  DxgkDdiResumeContext;
    PDXGKDDI_SETVIRTUALMACHINEDATA          DxgkDdiSetVirtualMachineData;
    PDXGKDDI_BEGINEXCLUSIVEACCESS           DxgkDdiBeginExclusiveAccess;
    PDXGKDDI_ENDEXCLUSIVEACCESS             DxgkDdiEndExclusiveAccess;
    PDXGKDDI_QUERYDIAGNOSTICTYPESSUPPORT    DxgkDdiQueryDiagnosticTypesSupport;
    PDXGKDDI_CONTROLDIAGNOSTICREPORTING     DxgkDdiControlDiagnosticReporting;
    PDXGKDDI_RESUMEHWENGINE                 DxgkDdiResumeHwEngine;

#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_4)

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_5)

    PDXGKDDI_SIGNALMONITOREDFENCE           DxgkDdiSignalMonitoredFence;
    PDXGKDDI_PRESENTTOHWQUEUE               DxgkDdiPresentToHwQueue;
    PDXGKDDI_VALIDATESUBMITCOMMAND          DxgkDdiValidateSubmitCommand;
    PDXGKDDI_SETTARGETADJUSTEDCOLORIMETRY2  DxgkDdiSetTargetAdjustedColorimetry2;
    PDXGKDDI_SETTRACKEDWORKLOADPOWERLEVEL   DxgkDdiSetTrackedWorkloadPowerLevel;

#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_5)

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_6)
    PDXGKDDI_SAVEMEMORYFORHOTUPDATE         DxgkDdiSaveMemoryForHotUpdate;
    PDXGKDDI_RESTOREMEMORYFORHOTUPDATE      DxgkDdiRestoreMemoryForHotUpdate;
   // PDXGKDDI_COLLECTDIAGNOSTICINFO          DxgkDdiCollectDiagnosticInfo;
    void*                                   Reserved3;

#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_6)

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_7)
    PDXGKDDI_CONTROLINTERRUPT3              DxgkDdiControlInterrupt3;

#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_7)

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_9)

    PDXGKDDI_SETFLIPQUEUELOGBUFFER          DxgkDdiSetFlipQueueLogBuffer;
    PDXGKDDI_UPDATEFLIPQUEUELOG             DxgkDdiUpdateFlipQueueLog;
    PDXGKDDI_CANCELQUEUEDFLIPS              DxgkDdiCancelQueuedFlips;
    PDXGKDDI_SETINTERRUPTTARGETPRESENTID    DxgkDdiSetInterruptTargetPresentId;

#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_9)

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM3_0)
    PDXGKDDI_SETALLOCATIONBACKINGSTORE      DxgkDdiSetAllocationBackingStore;
    PDXGKDDI_CREATECPUEVENT                 DxgkDdiCreateCpuEvent;
    PDXGKDDI_DESTROYCPUEVENT                DxgkDdiDestroyCpuEvent;
    PDXGKDDI_CANCELFLIPS                    DxgkDdiCancelFlips;
#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM3_0)

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM3_1)
    PDXGKDDI_CREATENATIVEFENCE              DxgkDdiCreateNativeFence;
    PDXGKDDI_DESTROYNATIVEFENCE             DxgkDdiDestroyNativeFence;
    PDXGKDDI_UPDATEMONITOREDVALUES          DxgkDdiUpdateMonitoredValues;
    PDXGKDDI_NOTIFYCURRENTVALUEUPDATES      DxgkDdiNotifyCurrentValueUpdates;
    PDXGKDDI_CREATEDOORBELL                 DxgkDdiCreateDoorbell;
    PDXGKDDI_CONNECTDOORBELL                DxgkDdiConnectDoorbell;
    PDXGKDDI_DISCONNECTDOORBELL             DxgkDdiDisconnectDoorbell;
    PDXGKDDI_DESTROYDOORBELL                DxgkDdiDestroyDoorbell;
    PDXGKDDI_NOTIFYWORKSUBMISSION           DxgkDdiNotifyWorkSubmission;
    PDXGKDDI_FLUSHHWQUEUE                   DxgkDdiFlushHwQueue;
#endif // (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM3_1)

} DRIVER_INITIALIZATION_DATA, *PDRIVER_INITIALIZATION_DATA;

#endif