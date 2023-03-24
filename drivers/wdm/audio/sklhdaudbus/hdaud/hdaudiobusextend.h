#pragma once



#define ACPI_EVAL_INPUT_BUFFER_COMPLEX_SIGNATURE_V1_EX         'FieA'
#define MAXULONG64  ((ULONG64)~((ULONG64)0))
#define MAXLONG64   ((LONG64)(MAXULONG64 >> 1))
#define MINLONG64   ((LONG64)~MAXLONG64)

#define MAXUINT64   ((UINT64)~((UINT64)0))
#define MAXINT64    ((INT64)(MAXUINT64 >> 1))
#define MININT64    ((INT64)~MAXINT64)

DEFINE_GUID (GUID_HDAUDIO_BUS_INTERFACE_V3,
0xdc25da12, 0xbcd1, 0x4c7f, 0x84, 0xe1, 0x02, 0xad, 0xb6, 0xdc, 0xdc, 0xbf);


typedef VOID (*PHDAUDIO_DMA_NOTIFICATION_CALLBACK)(PVOID, LARGE_INTEGER);
typedef _Must_inspect_result_ NTSTATUS (*PREGISTER_NOTIFICATION_CALLBACK) (_In_ PVOID _context,
                                                                           _In_ HANDLE Handle,
                                                                           _In_ PDEVICE_OBJECT Fdo,
                                                                           _In_ PHDAUDIO_DMA_NOTIFICATION_CALLBACK NotificationCallback,
                                                                           _In_ PVOID CallbackContext);


typedef _Must_inspect_result_ NTSTATUS (*PUNREGISTER_NOTIFICATION_CALLBACK) (_In_ PVOID _context,
                                                                             _In_ HANDLE Handle,
                                                                             _In_ PHDAUDIO_DMA_NOTIFICATION_CALLBACK NotificationCallback,
                                                                             _In_ PVOID CallbackContext);
typedef struct _HDAUDIO_BUS_INTERFACE_V3
{
    //
    // First we define the standard INTERFACE structure ...
    //
    USHORT                    Size;
    USHORT                    Version;
    PVOID                     Context;
    PINTERFACE_REFERENCE      InterfaceReference;
    PINTERFACE_DEREFERENCE    InterfaceDereference;

    //
    // Then we expand the structure with the HDAUDIO_BUS_INTERFACE_PING_PONG stuff.
    // Many functions are identical (and derived) from the HDAUDIO_BUS_INTERFACE
    // interface.

    PTRANSFER_CODEC_VERBS           TransferCodecVerbs;
    PALLOCATE_CAPTURE_DMA_ENGINE    AllocateCaptureDmaEngine;
    PALLOCATE_RENDER_DMA_ENGINE     AllocateRenderDmaEngine;
    PCHANGE_BANDWIDTH_ALLOCATION    ChangeBandwidthAllocation;
    PALLOCATE_DMA_BUFFER            AllocateDmaBuffer;
    PFREE_DMA_BUFFER                FreeDmaBuffer;
    PFREE_DMA_ENGINE                FreeDmaEngine;
    PSET_DMA_ENGINE_STATE           SetDmaEngineState;
    PGET_WALL_CLOCK_REGISTER        GetWallClockRegister;
    PGET_LINK_POSITION_REGISTER     GetLinkPositionRegister;
    PREGISTER_EVENT_CALLBACK        RegisterEventCallback;
    PUNREGISTER_EVENT_CALLBACK      UnregisterEventCallback;
    PGET_DEVICE_INFORMATION         GetDeviceInformation;
    PGET_RESOURCE_INFORMATION       GetResourceInformation;
    PALLOCATE_DMA_BUFFER_WITH_NOTIFICATION AllocateDmaBufferWithNotification;
    PFREE_DMA_BUFFER_WITH_NOTIFICATION FreeDmaBufferWithNotification;
    PREGISTER_NOTIFICATION_EVENT    RegisterNotificationEvent;
    PUNREGISTER_NOTIFICATION_EVENT  UnregisterNotificationEvent;
    PREGISTER_NOTIFICATION_CALLBACK RegisterNotificationCallback;
    PUNREGISTER_NOTIFICATION_CALLBACK UnregisterNotificationCallback;
} HDAUDIO_BUS_INTERFACE_V3, *PHDAUDIO_BUS_INTERFACE_V3;

//
// HDAudio device information structure
//
typedef struct _HDAUDIO_DEVICE_INFORMATION_V2
{
  USHORT  Size;                 // size of this structure
  USHORT  DeviceVersion;        // maj.min (maj is high byte, min is low byte)
  USHORT  DriverVersion;        // maj.min (maj is high byte, min is low byte)
  USHORT  CodecsDetected;       // mask of codecs present. Bit number == SDI line number
  BOOLEAN IsStripingSupported;  // TRUE if striping (2 SDO lines) is supported
  UCHAR   CtrlRevision;         // HDA controller revision.
  USHORT  CtrlVendorId;         // HDA controller vendor ID.
  USHORT  CtrlDeviceId;         // HDA controller device ID.
} HDAUDIO_DEVICE_INFORMATION_V2, *PHDAUDIO_DEVICE_INFORMATION_V2;

