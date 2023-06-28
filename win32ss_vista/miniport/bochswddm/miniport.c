#include "bochswddm.h"
#include <debug.h>

static const BOCHS_SIZE BochsAvailableResolutions[] = {
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

CODE_SEG("PAGE")
static VOID
BochsFreeResources(
    _Inout_ PBOCHS_DEVICE_EXTENSION DeviceExtension)
{
    if (DeviceExtension->AvailableModeInfo)
    {
        VideoPortFreePool(DeviceExtension, DeviceExtension->AvailableModeInfo);
        DeviceExtension->AvailableModeInfo = NULL;
    }
}

CODE_SEG("PAGE")
static VOID
BochsWriteDispI(
    _In_ PBOCHS_DEVICE_EXTENSION DeviceExtension,
    _In_ ULONG Index,
    _In_ USHORT Value)
{
    if (DeviceExtension->IoPorts.RangeInIoSpace)
    {
        VideoPortWritePortUshort((PUSHORT)(DeviceExtension->IoPorts.Mapped - VBE_DISPI_IOPORT_INDEX + VBE_DISPI_IOPORT_INDEX), Index);
        VideoPortWritePortUshort((PUSHORT)(DeviceExtension->IoPorts.Mapped - VBE_DISPI_IOPORT_INDEX + VBE_DISPI_IOPORT_DATA), Value);
    }
    else
    {
        VideoPortWriteRegisterUshort((PUSHORT)(DeviceExtension->IoPorts.Mapped + 0x500 + Index * 2), Value);
    }
}

CODE_SEG("PAGE")
static VOID
BochsGetModeInfo(
    _In_ PBOCHS_SIZE AvailableModeInfo,
    _Out_ PVIDEO_MODE_INFORMATION ModeInfo,
    _In_ ULONG Index)
{
    //VideoDebugPrint((Info, "Bochs: Filling details of mode #%d\n", Index));

    ModeInfo->Length = sizeof(*ModeInfo);
    ModeInfo->ModeIndex = Index;
    ModeInfo->VisScreenWidth = AvailableModeInfo->XResolution;
    ModeInfo->VisScreenHeight = AvailableModeInfo->YResolution;
    ModeInfo->ScreenStride = AvailableModeInfo->XResolution * 4;
    ModeInfo->NumberOfPlanes = 1;
    ModeInfo->BitsPerPlane = 32;
    ModeInfo->Frequency = 60;

    /* 960 DPI appears to be common */
    ModeInfo->XMillimeter = AvailableModeInfo->XResolution * 254 / 960;
    ModeInfo->YMillimeter = AvailableModeInfo->YResolution * 254 / 960;
    ModeInfo->NumberRedBits = 8;
    ModeInfo->NumberGreenBits = 8;
    ModeInfo->NumberBlueBits = 8;
    ModeInfo->RedMask = 0xff0000;
    ModeInfo->GreenMask = 0x00ff00;
    ModeInfo->BlueMask = 0x0000ff;

    ModeInfo->AttributeFlags = VIDEO_MODE_GRAPHICS | VIDEO_MODE_COLOR | VIDEO_MODE_NO_OFF_SCREEN;
    ModeInfo->VideoMemoryBitmapWidth = AvailableModeInfo->XResolution;
    ModeInfo->VideoMemoryBitmapHeight = AvailableModeInfo->YResolution;
}


CODE_SEG("PAGE")
static USHORT
BochsReadDispI(
    _In_ PBOCHS_DEVICE_EXTENSION DeviceExtension,
    _In_ ULONG Index)
{
    if (DeviceExtension->IoPorts.RangeInIoSpace)
    {
        VideoPortWritePortUshort((PUSHORT)(DeviceExtension->IoPorts.Mapped - VBE_DISPI_IOPORT_INDEX + VBE_DISPI_IOPORT_INDEX), Index);
        return VideoPortReadPortUshort((PUSHORT)(DeviceExtension->IoPorts.Mapped - VBE_DISPI_IOPORT_INDEX + VBE_DISPI_IOPORT_DATA));
    }
    else
    {
        return VideoPortReadRegisterUshort((PUSHORT)(DeviceExtension->IoPorts.Mapped + 0x500 + Index * 2));
    }
}

CODE_SEG("PAGE")
static BOOLEAN
BochsWriteDispIAndCheck(
    _In_ PBOCHS_DEVICE_EXTENSION DeviceExtension,
    _In_ ULONG Index,
    _In_ USHORT Value)
{
    BochsWriteDispI(DeviceExtension, Index, Value);
    return BochsReadDispI(DeviceExtension, Index) == Value;
}

CODE_SEG("PAGE")
static BOOLEAN
BochsInitializeSuitableModeInfo(
    _In_ PBOCHS_DEVICE_EXTENSION DeviceExtension,
    _In_ ULONG PotentialModeCount)
{
    ULONG i, ModeCount = 0;

    for (i = 0; i < ARRAYSIZE(BochsAvailableResolutions) && ModeCount < PotentialModeCount; i++)
    {
        if (BochsAvailableResolutions[i].XResolution > DeviceExtension->MaxXResolution)
            continue;
        if (BochsAvailableResolutions[i].YResolution > DeviceExtension->MaxYResolution)
            continue;
        if (BochsAvailableResolutions[i].XResolution * BochsAvailableResolutions[i].YResolution * 4 > DeviceExtension->VramSize64K * 64 * 1024)
            continue;
        DeviceExtension->AvailableModeInfo[ModeCount++] = BochsAvailableResolutions[i];
    }

    if (ModeCount == 0)
    {
        DPRINT1("Bochs: no suitable modes available!\n");
        return FALSE;
    }

    DeviceExtension->AvailableModeCount = ModeCount;
    return TRUE;
}

CODE_SEG("PAGE")
NTSTATUS
NTAPI
BochsFindAdapter(
    _In_ PVOID HwDeviceExtension,
    _In_ PVOID HwContext,
    _In_ PWSTR ArgumentString,
    _In_ PVIDEO_PORT_CONFIG_INFO ConfigInfo,
    _In_ PUCHAR Again)
{
    PBOCHS_DEVICE_EXTENSION DeviceExtension = HwDeviceExtension;
    VIDEO_ACCESS_RANGE AccessRanges[2] = {0};

    if (ConfigInfo->Length < sizeof(*ConfigInfo))
        return ERROR_INVALID_PARAMETER;

    if (VideoPortGetAccessRanges(DeviceExtension, 0, NULL, ARRAYSIZE(AccessRanges), AccessRanges, NULL, NULL, NULL) != NO_ERROR)
    {
        DPRINT1("Bochs: failed to get access ranges\n");
        return ERROR_DEV_NOT_EXIST;
    }

    /* Framebuffer */
    DeviceExtension->FrameBuffer.RangeStart = AccessRanges[0].RangeStart;
    DeviceExtension->FrameBuffer.RangeLength = AccessRanges[0].RangeLength;
    DeviceExtension->FrameBuffer.RangeInIoSpace = AccessRanges[0].RangeInIoSpace;

    /* I/O ports */
    if (AccessRanges[1].RangeLength == 0)
    {
        /* Set default values */
        AccessRanges[1].RangeStart.LowPart = VBE_DISPI_IOPORT_INDEX;
        AccessRanges[1].RangeLength = 2;
        AccessRanges[1].RangeInIoSpace = TRUE;
        if (VideoPortVerifyAccessRanges(DeviceExtension, 1, &AccessRanges[1]) != NO_ERROR)
        {
            DPRINT1("Bochs: failed to claim I/O range 0x%x-0x%x\n",
                            VBE_DISPI_IOPORT_INDEX,
                            VBE_DISPI_IOPORT_INDEX + 1);
            return ERROR_DEV_NOT_EXIST;
        }
    }
    else if (AccessRanges[1].RangeLength != 0x1000)
    {
        DPRINT1("Bochs: invalid access ranges (size 0x%x)\n", AccessRanges[1].RangeLength);
        return ERROR_DEV_NOT_EXIST;
    }
    DeviceExtension->IoPorts.RangeStart = AccessRanges[1].RangeStart;
    DeviceExtension->IoPorts.RangeLength = AccessRanges[1].RangeLength;
    DeviceExtension->IoPorts.RangeInIoSpace = AccessRanges[1].RangeInIoSpace;

    DeviceExtension->IoPorts.Mapped = VideoPortGetDeviceBase(DeviceExtension,
                                                             DeviceExtension->IoPorts.RangeStart,
                                                             DeviceExtension->IoPorts.RangeLength,
                                                             DeviceExtension->IoPorts.RangeInIoSpace
                                                                 ? VIDEO_MEMORY_SPACE_IO
                                                                 : VIDEO_MEMORY_SPACE_MEMORY);
    if (!DeviceExtension->IoPorts.Mapped)
    {
        DPRINT1("Bochs: failed to map dispi interface\n");
        return ERROR_DEV_NOT_EXIST;
    }
    DPRINT1("Bochs: address 0x%x mapped to 0x%p\n",
                     DeviceExtension->IoPorts.RangeStart.LowPart,
                     DeviceExtension->IoPorts.Mapped);

    return NO_ERROR;
}

CODE_SEG("PAGE")
static BOOLEAN
BochsGetControllerInfo(
    _Inout_ PBOCHS_DEVICE_EXTENSION DeviceExtension)
{
    USHORT Version;
    WCHAR ChipType[5];
    ULONG SizeInBytes;

    /* Detect DISPI version */
    for (Version = VBE_DISPI_ID5; Version >= VBE_DISPI_ID0; Version--)
    {
        if (BochsWriteDispIAndCheck(DeviceExtension, VBE_DISPI_INDEX_ID, Version))
            break;
    }
    if (Version < VBE_DISPI_ID0)
    {
        DPRINT1("Bochs: VBE extension signature incorrect\n");
        return FALSE;
    }
    DPRINT1("Bochs: detected version 0x%04x\n", Version);
    if (Version < VBE_DISPI_ID2)
    {
        /* Too old (no 32 bpp support, no linear frame buffer) */
        DPRINT1("Bochs: VBE extension too old (0x%04x)\n", Version);
        return FALSE;
    }

    if (Version <= VBE_DISPI_ID2)
    {
        DeviceExtension->MaxXResolution = 1024;
        DeviceExtension->MaxYResolution = 768;
    }
    else
    {
        BochsWriteDispI(DeviceExtension, VBE_DISPI_INDEX_ENABLE, VBE_DISPI_GETCAPS);
        DeviceExtension->MaxXResolution = BochsReadDispI(DeviceExtension, VBE_DISPI_INDEX_XRES);
        DeviceExtension->MaxYResolution = BochsReadDispI(DeviceExtension, VBE_DISPI_INDEX_YRES);
        BochsWriteDispI(DeviceExtension, VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
        /* Workaround bug in QEMU bochs-display */
        if (DeviceExtension->MaxXResolution == 0 && DeviceExtension->MaxYResolution == 0)
        {
            DeviceExtension->MaxXResolution = 1024;
            DeviceExtension->MaxYResolution = 768;
        }
    }
    if (Version < VBE_DISPI_ID4)
    {
        DeviceExtension->VramSize64K = 4 * 1024 / 64; /* 4 MB */
    }
    else if (Version == VBE_DISPI_ID4)
    {
        DeviceExtension->VramSize64K = 8 * 1024 / 64; /* 8 MB */
    }
    else
    {
        DeviceExtension->VramSize64K = BochsReadDispI(DeviceExtension, VBE_DISPI_INDEX_VIDEO_MEMORY_64K);
    }
    DPRINT1("Bochs: capabilities %dx%d (%d MB)\n",
                     DeviceExtension->MaxXResolution,
                     DeviceExtension->MaxYResolution,
                     DeviceExtension->VramSize64K * 64 / 1024);

    /* Store information in registry */
#define HEX(c) (((c) >= 0 && (c) <= 9) ? (c) + L'0' : (c) - 10 + L'A')
    ChipType[0] = HEX((Version >> 12) & 0xf);
    ChipType[1] = HEX((Version >> 8) & 0xf);
    ChipType[2] = HEX((Version >> 4) & 0xf);
    ChipType[3] = HEX(Version & 0xf);
    ChipType[4] = UNICODE_NULL;
    VideoPortSetRegistryParameters(DeviceExtension, L"HardwareInformation.ChipType", ChipType, sizeof(ChipType));
    SizeInBytes = DeviceExtension->VramSize64K * 64 * 1024;
    VideoPortSetRegistryParameters(DeviceExtension, L"HardwareInformation.MemorySize", &SizeInBytes, sizeof(SizeInBytes));
    return TRUE;
}


CODE_SEG("PAGE")
BOOLEAN NTAPI
BochsInitialize(
    _In_ PVOID HwDeviceExtension)
{
    ULONG PotentialModeCount = 0;
    PBOCHS_DEVICE_EXTENSION DeviceExtension = HwDeviceExtension;

    DPRINT1("Bochs: BochsInitialize\n");

    if (!BochsGetControllerInfo(DeviceExtension))
    {
       // BochsFreeResources(DeviceExtension);
        return FALSE;
    }

    PotentialModeCount = ARRAYSIZE(BochsAvailableResolutions);
    DeviceExtension->AvailableModeInfo = VideoPortAllocatePool(HwDeviceExtension,
                                                               VpPagedPool,
                                                               PotentialModeCount * sizeof(BOCHS_SIZE),
                                                               BOCHS_TAG);
    if (!DeviceExtension->AvailableModeInfo)
    {
       // VideoDebugPrint((Error, "Bochs: insufficient resources\n"));
        BochsFreeResources(DeviceExtension);
        return FALSE;
    }

    if (!BochsInitializeSuitableModeInfo(DeviceExtension, PotentialModeCount))
    {
        BochsFreeResources(DeviceExtension);
        return FALSE;
    }

    return TRUE;
}


CODE_SEG("PAGE")
static BOOLEAN
BochsMapVideoMemory(
    _In_ PBOCHS_DEVICE_EXTENSION DeviceExtension,
    _In_ PVIDEO_MEMORY RequestedAddress,
    _Out_ PVIDEO_MEMORY_INFORMATION MapInformation,
    _Out_ PSTATUS_BLOCK StatusBlock)
{
    VP_STATUS Status;
    PHYSICAL_ADDRESS VideoMemory;
    ULONG MemSpace = VIDEO_MEMORY_SPACE_MEMORY;

    //VideoDebugPrint((Info, "Bochs: BochsMapVideoMemory Entry\n"));

    VideoMemory = DeviceExtension->FrameBuffer.RangeStart;
    MapInformation->VideoRamBase = RequestedAddress->RequestedVirtualAddress;
    MapInformation->VideoRamLength = 4 *
        DeviceExtension->AvailableModeInfo[DeviceExtension->CurrentMode].XResolution *
        DeviceExtension->AvailableModeInfo[DeviceExtension->CurrentMode].YResolution;

    Status = VideoPortMapMemory(DeviceExtension,
                                VideoMemory,
                                &MapInformation->VideoRamLength,
                                &MemSpace,
                                &MapInformation->VideoRamBase);
    if (Status != NO_ERROR)
    {
        DPRINT1("BochsMapVideoMemory - VideoPortMapMemory failed status:%x\n", Status);
        StatusBlock->Status = Status;
        return FALSE;
    }

    MapInformation->FrameBufferBase = MapInformation->VideoRamBase;
    MapInformation->FrameBufferLength = MapInformation->VideoRamLength;
    StatusBlock->Information = sizeof(*MapInformation);
    StatusBlock->Status = NO_ERROR;

    DPRINT1("Bochs:BochsMapVideoMemory Exit VideoRamBase: %p VideoRamLength: 0x%x PhysBasePtr: 0x%x\n",
                     MapInformation->VideoRamBase, MapInformation->VideoRamLength, (ULONG)VideoMemory.QuadPart);
    return TRUE;
}

CODE_SEG("PAGE")
static BOOLEAN NTAPI
BochsUnmapVideoMemory(
    _In_ PBOCHS_DEVICE_EXTENSION DeviceExtension,
    _In_ PVIDEO_MEMORY VideoMemory,
    _Out_ PSTATUS_BLOCK StatusBlock)
{
    VP_STATUS Status;

    Status = VideoPortUnmapMemory(DeviceExtension, VideoMemory->RequestedVirtualAddress, NULL);
    if (Status != NO_ERROR)
    {

    }

    StatusBlock->Status = Status;

    DPRINT1("Bochs: BochsUnmapVideoMemory Exit status:%x\n", Status);
    return (Status == NO_ERROR);
}

CODE_SEG("PAGE")
static BOOLEAN
BochsQueryNumAvailableModes(
    _In_ PBOCHS_DEVICE_EXTENSION DeviceExtension,
    _Out_ PVIDEO_NUM_MODES AvailableModes,
    _Out_ PSTATUS_BLOCK StatusBlock)
{
    AvailableModes->NumModes = DeviceExtension->AvailableModeCount;
    AvailableModes->ModeInformationLength = sizeof(VIDEO_MODE_INFORMATION);

    StatusBlock->Information = sizeof(*AvailableModes);
    StatusBlock->Status = NO_ERROR;
    return TRUE;
}

CODE_SEG("PAGE")
static BOOLEAN
BochsQueryAvailableModes(
    _In_ PBOCHS_DEVICE_EXTENSION DeviceExtension,
    _Out_ PVIDEO_MODE_INFORMATION ReturnedModes,
    _Out_ PSTATUS_BLOCK StatusBlock)
{
    ULONG Count;
    PBOCHS_SIZE AvailableModeInfo;
    PVIDEO_MODE_INFORMATION ModeInfo;

    for (Count = 0, AvailableModeInfo = DeviceExtension->AvailableModeInfo, ModeInfo = ReturnedModes;
        Count < DeviceExtension->AvailableModeCount;
        Count++, AvailableModeInfo++, ModeInfo++)
    {
        VideoPortZeroMemory(ModeInfo, sizeof(*ModeInfo));
        BochsGetModeInfo(AvailableModeInfo, ModeInfo, Count);
    }

    StatusBlock->Information = sizeof(VIDEO_MODE_INFORMATION) * DeviceExtension->AvailableModeCount;
    StatusBlock->Status = NO_ERROR;

    return TRUE;
}



CODE_SEG("PAGE")
static BOOLEAN
BochsSetCurrentMode(
    _In_ PBOCHS_DEVICE_EXTENSION DeviceExtension,
    _In_ PVIDEO_MODE RequestedMode,
    _Out_ PSTATUS_BLOCK StatusBlock)
{
    PBOCHS_SIZE AvailableModeInfo;
    /* Mask the two high-order bits, which can be set to request special behavior */
    ULONG ModeRequested = RequestedMode->RequestedMode & 0x3fffffff;
    BOOLEAN Ret;

   // VideoDebugPrint((Info, "Bochs:BochsSetCurrentMode Entry\n"));

    if (ModeRequested >= DeviceExtension->AvailableModeCount)
    {
      //  VideoDebugPrint((Error, "Bochs: set current mode - invalid parameter\n"));
        StatusBlock->Status = ERROR_INVALID_PARAMETER;
        return FALSE;
    }

    AvailableModeInfo = &DeviceExtension->AvailableModeInfo[ModeRequested];

    /* Set the mode characteristics */
    BochsWriteDispI(DeviceExtension, VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
    Ret = BochsWriteDispIAndCheck(DeviceExtension, VBE_DISPI_INDEX_XRES, AvailableModeInfo->XResolution) &&
          BochsWriteDispIAndCheck(DeviceExtension, VBE_DISPI_INDEX_YRES, AvailableModeInfo->YResolution) &&
          BochsWriteDispIAndCheck(DeviceExtension, VBE_DISPI_INDEX_BPP, 32);
    /* Always enable screen, even if display settings change failed */
    BochsWriteDispI(DeviceExtension, VBE_DISPI_INDEX_ENABLE, VBE_DISPI_LFB_ENABLED | VBE_DISPI_ENABLED);
    if (!Ret)
    {
       // VideoDebugPrint((Error, "Bochs: failed to change mode\n"));
        return FALSE;
    }

    /* Enable VGA (QEMU secondary-vga disables it by default) */
    if (!DeviceExtension->IoPorts.RangeInIoSpace)
    {
        /* Discard AR flip-flip */
        (VOID)VideoPortReadRegisterUshort((PUSHORT)(DeviceExtension->IoPorts.Mapped + 0x41A));
        /* Enable display */
        VideoPortWriteRegisterUshort((PUSHORT)(DeviceExtension->IoPorts.Mapped + 0x400), 0x20);
    }

    DeviceExtension->CurrentMode = (USHORT)ModeRequested;
    StatusBlock->Status = NO_ERROR;

    DPRINT1("Bochs:BochsSetCurrentMode Exit Mode:%d\n", ModeRequested);
    return TRUE;
}

CODE_SEG("PAGE")
static BOOLEAN
BochsQueryCurrentMode(
    _In_ PBOCHS_DEVICE_EXTENSION DeviceExtension,
    _Out_ PVIDEO_MODE_INFORMATION VideoModeInfo,
    _Out_ PSTATUS_BLOCK StatusBlock)
{
    PBOCHS_SIZE AvailableModeInfo;

    if (DeviceExtension->CurrentMode > DeviceExtension->AvailableModeCount)
    {
        StatusBlock->Status = ERROR_INVALID_PARAMETER;
        return FALSE;
    }

    AvailableModeInfo = &DeviceExtension->AvailableModeInfo[DeviceExtension->CurrentMode];
    VideoPortZeroMemory(VideoModeInfo, sizeof(*VideoModeInfo));
    BochsGetModeInfo(AvailableModeInfo, VideoModeInfo, DeviceExtension->CurrentMode);

    StatusBlock->Information = sizeof(*VideoModeInfo);
    StatusBlock->Status = NO_ERROR;
    return TRUE;
}

CODE_SEG("PAGE")
static BOOLEAN
BochsResetDevice(
    _In_ PBOCHS_DEVICE_EXTENSION DeviceExtension,
    _Out_ PSTATUS_BLOCK StatusBlock)
{
  //  VideoDebugPrint((Info, "Bochs:BochsResetDevice Entry\n"));

    StatusBlock->Status = NO_ERROR;

  //  VideoDebugPrint((Info, "Bochs:BochsResetDevice Exit\n"));
    return TRUE;
}

CODE_SEG("PAGE")
static BOOLEAN
BochsGetChildState(
    _In_ PBOCHS_DEVICE_EXTENSION DeviceExtension,
    _Out_ PULONG pChildState,
    _Out_ PSTATUS_BLOCK StatusBlock)
{
    *pChildState = VIDEO_CHILD_ACTIVE;

    StatusBlock->Information = sizeof(*pChildState);
    StatusBlock->Status = NO_ERROR;
    return TRUE;
}


CODE_SEG("PAGE")
BOOLEAN NTAPI
BochsStartIO(
    _In_ PVOID HwDeviceExtension,
    _Inout_ PVIDEO_REQUEST_PACKET RequestPacket)
{
    PBOCHS_DEVICE_EXTENSION DeviceExtension = HwDeviceExtension;

    DPRINT1("Bochs: BochsStartIO\n");
    RequestPacket->StatusBlock->Status = ERROR_INVALID_FUNCTION;

    switch (RequestPacket->IoControlCode)
    {
        case IOCTL_VIDEO_MAP_VIDEO_MEMORY:
        {
            DPRINT1("BochsStartIO - Map video memory\n");
            if (RequestPacket->InputBufferLength < sizeof(VIDEO_MEMORY))
            {
                DPRINT1("BochsStartIO - invalid input parameter\n");
                RequestPacket->StatusBlock->Status = ERROR_INSUFFICIENT_BUFFER;
                return FALSE;
            }
            if (RequestPacket->OutputBufferLength < sizeof(VIDEO_MEMORY_INFORMATION))
            {
                DPRINT1("BochsStartIO - Insufficent output buffer\n");
                RequestPacket->StatusBlock->Status = ERROR_INSUFFICIENT_BUFFER;
                return FALSE;
            }
            return BochsMapVideoMemory(DeviceExtension,
                                       (PVIDEO_MEMORY)RequestPacket->InputBuffer,
                                       (PVIDEO_MEMORY_INFORMATION)RequestPacket->OutputBuffer,
                                       RequestPacket->StatusBlock);
        }

        case IOCTL_VIDEO_UNMAP_VIDEO_MEMORY:
        {
            DPRINT1("BochsStartIO - Unmap video memory\n");
            if (RequestPacket->InputBufferLength < sizeof(VIDEO_MEMORY))
            {
                RequestPacket->StatusBlock->Status = ERROR_INSUFFICIENT_BUFFER;
                return FALSE;
            }
            return BochsUnmapVideoMemory(DeviceExtension,
                                         (PVIDEO_MEMORY)RequestPacket->InputBuffer,
                                         RequestPacket->StatusBlock);
        }

        case IOCTL_VIDEO_QUERY_NUM_AVAIL_MODES:
        {
            DPRINT1("BochsStartIO - Query num available modes\n");
            if (RequestPacket->OutputBufferLength < sizeof(VIDEO_NUM_MODES))
            {
                RequestPacket->StatusBlock->Status = ERROR_INSUFFICIENT_BUFFER;
                return FALSE;
            }
            return BochsQueryNumAvailableModes(DeviceExtension,
                                               (PVIDEO_NUM_MODES)RequestPacket->OutputBuffer,
                                               RequestPacket->StatusBlock);
        }

        case IOCTL_VIDEO_QUERY_AVAIL_MODES:
        {
            DPRINT1("BochsStartIO - Query available modes\n");
            if (RequestPacket->OutputBufferLength < DeviceExtension->AvailableModeCount * sizeof(VIDEO_MODE_INFORMATION))
            {
                RequestPacket->StatusBlock->Status = ERROR_INSUFFICIENT_BUFFER;
                return FALSE;
            }
            return BochsQueryAvailableModes(DeviceExtension,
                                            (PVIDEO_MODE_INFORMATION)RequestPacket->OutputBuffer,
                                            RequestPacket->StatusBlock);
        }

        case IOCTL_VIDEO_SET_CURRENT_MODE:
        {
            DPRINT1("BochsStartIO - Set current mode\n");
            if (RequestPacket->InputBufferLength < sizeof(VIDEO_MODE))
            {
                DPRINT1("Bochs: set current mode - invalid parameter\n");
                RequestPacket->StatusBlock->Status = ERROR_INSUFFICIENT_BUFFER;
                return FALSE;
            }
            return BochsSetCurrentMode(DeviceExtension,
                                       (PVIDEO_MODE)RequestPacket->InputBuffer,
                                       RequestPacket->StatusBlock);
        }

        case IOCTL_VIDEO_QUERY_CURRENT_MODE:
        {
            DPRINT1("BochsStartIO - Query current mode\n");
            if (RequestPacket->OutputBufferLength < sizeof(VIDEO_MODE_INFORMATION))
            {
                RequestPacket->StatusBlock->Status = ERROR_INSUFFICIENT_BUFFER;
                return FALSE;
            }
            return BochsQueryCurrentMode(DeviceExtension,
                                         (PVIDEO_MODE_INFORMATION)RequestPacket->OutputBuffer,
                                         RequestPacket->StatusBlock);
        }

        case IOCTL_VIDEO_RESET_DEVICE:
        {
            DPRINT1("BochsStartIO - Reset device\n");
            return BochsResetDevice(DeviceExtension,
                                    RequestPacket->StatusBlock);
        }

        case IOCTL_VIDEO_GET_CHILD_STATE:
        {
            DPRINT1("BochsStartIO - Get child state\n");
            if (RequestPacket->OutputBufferLength < sizeof(ULONG))
            {
                RequestPacket->StatusBlock->Status = ERROR_INSUFFICIENT_BUFFER;
                return FALSE;
            }
            return BochsGetChildState(DeviceExtension,
                                      (PULONG)RequestPacket->OutputBuffer,
                                      RequestPacket->StatusBlock);
        }

        default:
        {
            DPRINT1( "BochsStartIO - Unknown IOCTL - 0x%08x\n",
                             RequestPacket->IoControlCode);
            break;
        }
    }

    return FALSE;
}

/* WDDM SHIT *************************************/
/*
 * The First WDDM Miniport call initiated by DXGKNRL itself
 * This routine internally sets up the device as a valid device.
 */
NTSTATUS
BochsWDDM_AddDevice(IN_CONST_PDEVICE_OBJECT PhysicalDeviceObject,
                    OUT PVOID *MiniportDeviceContext)
{
    PAGED_CODE();

    DPRINT1("BochsWDDM_AddDevice: Entry\n");
    if (! ARGUMENT_PRESENT(PhysicalDeviceObject) ||
        ! ARGUMENT_PRESENT(MiniportDeviceContext))
    {
        return STATUS_INVALID_PARAMETER;
    }

    /* Allocate boch device extension */
    *MiniportDeviceContext = (PVOID)ExAllocatePoolWithTag(NonPagedPool,
                                                          sizeof(BOCHS_DEVICE_EXTENSION),
                                                          BOCHS_TAG);

    return STATUS_SUCCESS;
}

NTSTATUS
BochsWDDM_StartDevice(_In_  PVOID MiniportDeviceContext,
                      _In_  PDXGK_START_INFO DxgkStartInfo,
                      _In_  PDXGKRNL_INTERFACE DxgkInterface,
                      _Out_ PULONG NumberOfViews,
                      _Out_ PULONG NumberOfChildren)
{
    DXGK_DEVICE_INFO DeviceInfo;
    VIDEO_PORT_CONFIG_INFO LegacyConfigInfo = {'\0'};

    NTSTATUS Status = STATUS_SUCCESS;
    PBOCHS_DEVICE_EXTENSION DeviceExtension = MiniportDeviceContext;
    VIDEO_ACCESS_RANGE AccessRanges[2] = {0};
    UCHAR Again;

    PAGED_CODE();

    DPRINT1("BochsWDDM_StartDevice: Entry\n");
    if (! ARGUMENT_PRESENT(MiniportDeviceContext) ||
        ! ARGUMENT_PRESENT(DxgkInterface) ||
        ! ARGUMENT_PRESENT(NumberOfViews))
    {
        return STATUS_INVALID_PARAMETER;
    }

    /* Call DXGKNRL GetDeviceInformation routine via a funciton pointer */
    Status = DxgkInterface->DxgkCbGetDeviceInformation(DxgkInterface->DeviceHandle, &DeviceInfo);
    if (Status != STATUS_SUCCESS)
    {
        DPRINT1("DxgkCbGetDeviceInformation: Has failed with status %X", Status);
    }

    /* Okay! We have one single view */
    *NumberOfViews = 1;
    DeviceExtension->ddiCallback = *DxgkInterface;

    /* Okay let's figure out what we're working */

    LegacyConfigInfo.Length = sizeof(VIDEO_PORT_CONFIG_INFO);
    LegacyConfigInfo.SystemIoBusNumber = DeviceInfo.TranslatedResourceList->List[0].BusNumber;
    LegacyConfigInfo.AdapterInterfaceType = DeviceInfo.TranslatedResourceList->List[0].InterfaceType;
    LegacyConfigInfo.SystemMemorySize = DeviceInfo.SystemMemorySize.QuadPart;
    LegacyConfigInfo.DriverRegistryPath = DeviceInfo.DeviceRegistryPath.Buffer;

    Status = BochsFindAdapter(DeviceContext, NULL, NULL, &LegacyConfigInfo, &Again);
    if (Status != STATUS_SUCCESS)
    {
        DPRINT1("BochsFindAdapter: Has failed with status %X", Status);
    }

    BochsInitialize(MiniportDeviceContext);
    *NumberOfChildren = 0;
}



BOOLEAN
BochsWDDM_InterruptRoutine(
    PVOID MiniportDeviceContext,
    ULONG MessageNumber
    )
{

    UNREFERENCED_PARAMETER(MessageNumber);

    if (! ARGUMENT_PRESENT(MiniportDeviceContext))
    {
        return FALSE;
    }

    return FALSE;
}

NTSTATUS
BochsWDDM_StopDevice(
    IN PVOID MiniportDeviceContext)
{
    DPRINT1("BochsWDDM_StopDevice: Entry\n");
    return 0;
}

NTSTATUS
BochsWDDM_RemoveDevice(
    IN PVOID MiniportDeviceContext)
{
    DPRINT1("Removing bochs\n");
    PAGED_CODE();

    //Free miniport context
    ExFreePool(MiniportDeviceContext);

    return STATUS_SUCCESS;
}

NTSTATUS
BochsWDDM_DispatchIoRequest(
    IN PVOID MiniportDeviceContext,
    IN ULONG ViewIndex,
    IN PVIDEO_REQUEST_PACKET VideoRequestPacket)
{
    PBOCHS_DEVICE_EXTENSION DeviceExtension = MiniportDeviceContext;
    PAGED_CODE();

    if (!ARGUMENT_PRESENT(MiniportDeviceContext) ||
        !ARGUMENT_PRESENT(VideoRequestPacket) ||
        ViewIndex > MAX_VIEWS)
    {
        return STATUS_INVALID_PARAMETER;
    }
    if (BochsStartIO(HwDeviceExtension, VideoRequestPacket) != TRUE)
    {
        return STATUS_UNSUCCESSFUL;
    }

    return STATUS_SUCCESS;
}

VOID
BochsWDDM_ResetDevice(
    IN PVOID MiniportDeviceContext
    )
{
    //nothing to do here for now
}

VOID
BochsWDDM_Unload(
    VOID
    )
{
    PAGED_CODE();

}

NTSTATUS
DriverEntry(
    _In_  DRIVER_OBJECT*  pDriverObject,
    _In_  UNICODE_STRING* pRegistryPath)
{
    NTSTATUS Status;
    DRIVER_INITIALIZATION_DATA DriverInitializationData = {0};
    DPRINT1("BOCHS WDDM ADAPTER\n");
    __debugbreak();
    DriverInitializationData.Version = DXGKDDI_INTERFACE_VERSION_VISTA_SP1;
    DriverInitializationData.DxgkDdiAddDevice = BochsWDDM_AddDevice;
    DriverInitializationData.DxgkDdiStartDevice = BochsWDDM_AddDevice;
    DriverInitializationData.DxgkDdiStopDevice = BochsWDDM_StopDevice;
    DriverInitializationData.DxgkDdiRemoveDevice = BochsWDDM_RemoveDevice;
    DriverInitializationData.DxgkDdiDispatchIoRequest = BochsWDDM_DispatchIoRequest;
    DriverInitializationData.DxgkDdiSetPowerState = BochsWDDM_SetPowerState;
    DriverInitializationData.DxgkDdiResetDevice = BochsWDDM_ResetDevice;
    DriverInitializationData.DxgkDdiUnload = BochsWDDM_Unload;
    DriverInitializationData.DxgkDdiQueryInterface = BochsWDDM_QueryInterface;
   // DriverInitializationData.DxgkDdiInterruptRoutine = BochsWDDM_InterruptRoutine;
   // DriverInitializationData.DxgkDdiDpcRoutine = BochsWDDM_DpcRoutine;
   // DriverInitializationData.DxgkDdiQueryChildRelations = BochsWDDM_QueryChildRelations;
   // DriverInitializationData.DxgkDdiQueryChildStatus = BochsWDDM_QueryChildStatus;
   // DriverInitializationData.DxgkDdiQueryDeviceDescriptor = BochsWDDM_QueryDevceDescriptor;
  //  DriverInitializationData.DxgkDdiNotifyAcpiEvent = BochsWDDM_NotifyAcpiEvent;
    #if 0
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
#endif
    if (!pDriverObject ||
        !pRegistryPath)
    {
        return STATUS_INVALID_PARAMETER;
    }

    Status = DxgkInitialize(pDriverObject, pRegistryPath, &DriverInitializationData);
    if (!NT_SUCCESS(Status))
    {
        return Status;
    }


    return Status;
}
