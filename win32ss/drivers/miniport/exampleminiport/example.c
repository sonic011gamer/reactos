
#include "example.h"

/************************************ Globals *********************************************/

static const example_SIZE BochsAvailableResolutions[] = {
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
exampleFindAdapter(
    _In_ PVOID HwDeviceExtension,
    _In_ PVOID HwContext,
    _In_ PWSTR ArgumentString,
    _In_ PVIDEO_PORT_CONFIG_INFO ConfigInfo,
    _In_ PUCHAR Again)
{
    return ERROR_INVALID_FUNCTION;
}

CODE_SEG("PAGE")
BOOLEAN NTAPI
exampleInitialize(
    _In_ PVOID HwDeviceExtension)
{
    return 0;
}

CODE_SEG("PAGE")
BOOLEAN NTAPI
exampleStartIO(
    _In_ PVOID HwDeviceExtension,
    _Inout_ PVIDEO_REQUEST_PACKET RequestPacket)
{
    return 0;
}

CODE_SEG("PAGE")
VP_STATUS NTAPI
exampleSetPowerState(
    _In_ PVOID HwDeviceExtension,
    _In_ ULONG HwId,
    _In_ PVIDEO_POWER_MANAGEMENT VideoPowerControl)
{
    return NO_ERROR;
}

CODE_SEG("PAGE")
VP_STATUS NTAPI
exampleGetPowerState(
    _In_ PVOID HwDeviceExtension,
    _In_ ULONG HwId,
    _Out_ PVIDEO_POWER_MANAGEMENT VideoPowerControl)
{
    return ERROR_DEVICE_REINITIALIZATION_NEEDED;
}

CODE_SEG("PAGE")
VP_STATUS NTAPI
exampleGetVideoChildDescriptor(
    _In_ PVOID HwDeviceExtension,
    _In_ PVIDEO_CHILD_ENUM_INFO ChildEnumInfo,
    _Out_ PVIDEO_CHILD_TYPE VideoChildType,
    _Out_ PUCHAR pChildDescriptor,
    _Out_ PULONG UId,
    _Out_ PULONG pUnused)
{
    return ERROR_INVALID_FUNCTION;
}

ULONG NTAPI
DriverEntry(PVOID Context1, PVOID Context2)
{
    VIDEO_HW_INITIALIZATION_DATA VideoInitData;

    VideoDebugPrint((Info, "Intel example: DriverEntry\n"));
    VideoPortZeroMemory(&VideoInitData, sizeof(VideoInitData));
    VideoInitData.HwInitDataSize = sizeof(VideoInitData);
    VideoInitData.HwFindAdapter = exampleFindAdapter;
    VideoInitData.HwInitialize = exampleInitialize;
    VideoInitData.HwStartIO = exampleStartIO;
    VideoInitData.HwDeviceExtensionSize = sizeof(example_DEVICE_EXTENSION);
    VideoInitData.HwSetPowerState = exampleSetPowerState;
    VideoInitData.HwGetPowerState = exampleGetPowerState;
    VideoInitData.HwGetVideoChildDescriptor = exampleGetVideoChildDescriptor;

    return VideoPortInitialize(Context1, Context2, &VideoInitData, NULL);
}
