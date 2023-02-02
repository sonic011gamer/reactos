/*
 * PROJECT:     ReactOS Intel HD4000 graphics card driver
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     HD4000 primary file
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include "hd4000.h"
#include <debug.h>
/************************************ Globals *********************************************/
#define KB 1024
#define MB (KB * KB)

static const UINT32 GMS_TO_SIZE[] =
{
      0 * MB,     // GMS_0MB
     32 * MB,     // GMS_32MB_1
     64 * MB,     // GMS_64MB_1
     96 * MB,     // GMS_96MB_1
    128 * MB,     // GMS_128MB_1
     32 * MB,     // GMS_32MB
     48 * MB,     // GMS_48MB
     64 * MB,     // GMS_64MB
    128 * MB,     // GMS_128MB
    256 * MB,     // GMS_256MB
     96 * MB,     // GMS_96MB
    160 * MB,     // GMS_160MB
    224 * MB,     // GMS_224MB
    352 * MB,     // GMS_352MB
    448 * MB,     // GMS_448MB
    480 * MB,     // GMS_480MB
    512 * MB,     // GMS_512MB
};

static const HD4000_SIZE BochsAvailableResolutions[] = {
    { 640, 480 },   // VGA
    { 800, 600 },   // SVGA
    { 1024, 600 },  // WSVGA
    { 1024, 768 },  // XGA
    { 1152, 864 },  // XGA+
    { 1280, 720 },  // WXGA-H
    { 1280, 768 },  // WXGA
    { 1280, 960 },  // SXGA-
    { 1280, 1024 }, // SXGA
    { 1368, 768 },  // HD ready
    { 1400, 1050 }, // SXGA+
    { 1440, 900 },  // WSXGA
    { 1600, 900 },  // HD+
    { 1600, 1200 }, // UXGA
    { 1680, 1050 }, // WSXGA+
    { 1920, 1080 }, // FHD
    { 2048, 1536 }, // QXGA
    { 2560, 1440 }, // WQHD
    { 2560, 1600 }, // WQXGA
    { 2560, 2048 }, // QSXGA
    { 2800, 2100 }, // QSXGA+
    { 3200, 2400 }, // QUXGA
    { 3840, 2160 }, // 4K UHD-1
};

/************************************ Private *********************************************/

/************************************ Public  *********************************************/

CODE_SEG("PAGE")
VP_STATUS NTAPI
HD4000FindAdapter(
    _In_ PVOID HwDeviceExtension,
    _In_ PVOID HwContext,
    _In_ PWSTR ArgumentString,
    _In_ PVIDEO_PORT_CONFIG_INFO ConfigInfo,
    _In_ PUCHAR Again)
{
    VideoDebugPrint((Info, "Intel HD4000: HD4000FindAdapter\n"));
    PHD4000_DEVICE_EXTENSION Hd4000DeviceExtension = HwDeviceExtension;
    VIDEO_ACCESS_RANGE AccessRanges[3];
    VP_STATUS Status;
    PVOID RegsBase;
    //USHORT VendorId = 0x8086; /* Intel Corporation */
    //USHORT DeviceId = 0x0000; /* Some intel GPU.... */


    Status = VideoPortGetAccessRanges(HwDeviceExtension, 0, NULL, 3, AccessRanges,
                                      NULL, NULL, NULL);

    if (Status == NO_ERROR)
    {
        DPRINT1("AccessRange[0].RangeStart: %X", AccessRanges[0].RangeStart);
        DPRINT1("AccessRange[0].RangeStart: %X", AccessRanges[0].RangeLength);
    }

    RegsBase = VideoPortGetDeviceBase(
                     HwDeviceExtension,
                     AccessRanges[0].RangeStart,
                     AccessRanges[0].RangeLength,
                     AccessRanges[0].RangeInIoSpace
                     );

    Hd4000DeviceExtension->IoPorts.Mapped = RegsBase;
    Hd4000DeviceExtension->IoPorts.RangeStart = AccessRanges[0].RangeStart;
    Hd4000DeviceExtension->IoPorts.RangeLength = AccessRanges[0].RangeLength;
    Hd4000DeviceExtension->IoPorts.RangeInIoSpace = AccessRanges[0].RangeInIoSpace;
    if (!Hd4000DeviceExtension->IoPorts.Mapped)
    {
        VideoDebugPrint((Error, "HD4000 Failed to map interface\n"));
        for(;;)
        {

        }
        return ERROR_DEV_NOT_EXIST;
    }
#if 1
    DPRINT1("Hd4000DeviceExtension Addr: %X",  Hd4000DeviceExtension->IoPorts.Mapped);
#endif
for(;;)
{
    
}
    return NO_ERROR;
}

CODE_SEG("PAGE")
BOOLEAN NTAPI
HD4000Initialize(
    _In_ PVOID HwDeviceExtension)
{
    VideoDebugPrint((Info, "Intel HD4000: HD4000Initialize\n"));
    for(;;)
    {
        
    }
    return 0;
}

CODE_SEG("PAGE")
BOOLEAN NTAPI
HD4000StartIO(
    _In_ PVOID HwDeviceExtension,
    _Inout_ PVIDEO_REQUEST_PACKET RequestPacket)
{
    return 0;
}

CODE_SEG("PAGE")
VP_STATUS NTAPI
HD4000SetPowerState(
    _In_ PVOID HwDeviceExtension,
    _In_ ULONG HwId,
    _In_ PVIDEO_POWER_MANAGEMENT VideoPowerControl)
{
    return NO_ERROR;
}

CODE_SEG("PAGE")
VP_STATUS NTAPI
HD4000GetPowerState(
    _In_ PVOID HwDeviceExtension,
    _In_ ULONG HwId,
    _Out_ PVIDEO_POWER_MANAGEMENT VideoPowerControl)
{
    return ERROR_DEVICE_REINITIALIZATION_NEEDED;
}

CODE_SEG("PAGE")
VP_STATUS NTAPI
HD4000GetVideoChildDescriptor(
    _In_ PVOID HwDeviceExtension,
    _In_ PVIDEO_CHILD_ENUM_INFO ChildEnumInfo,
    _Out_ PVIDEO_CHILD_TYPE VideoChildType,
    _Out_ PUCHAR pChildDescriptor,
    _Out_ PULONG UId,
    _Out_ PULONG pUnused)
{
     return NO_ERROR;
}

ULONG NTAPI
DriverEntry(PVOID Context1, PVOID Context2)
{
    VIDEO_HW_INITIALIZATION_DATA VideoInitData;

    VideoDebugPrint((Info, "Intel HD4000: DriverEntry\n"));
    VideoPortZeroMemory(&VideoInitData, sizeof(VideoInitData));
    VideoInitData.HwInitDataSize = sizeof(VideoInitData);
    VideoInitData.HwFindAdapter = HD4000FindAdapter;
    VideoInitData.HwInitialize = HD4000Initialize;
    VideoInitData.HwStartIO = HD4000StartIO;
    VideoInitData.HwDeviceExtensionSize = sizeof(HD4000_DEVICE_EXTENSION);
    VideoInitData.HwSetPowerState = HD4000SetPowerState;
    VideoInitData.HwGetPowerState = HD4000GetPowerState;
    VideoInitData.HwGetVideoChildDescriptor = HD4000GetVideoChildDescriptor;

    return VideoPortInitialize(Context1, Context2, &VideoInitData, NULL);
}
