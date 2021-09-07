/*
 * PROJECT:     ReactOS Xbox miniport video driver
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Simple framebuffer driver for NVIDIA NV2A XGPU
 * COPYRIGHT:   Copyright 2004 Ge van Geldorp
 *              Copyright 2004 Filip Navara
 *              Copyright 2019-2020 Stanislav Motylkov (x86corez@gmail.com)
 */

#pragma once

/* INCLUDES *******************************************************************/

/*
 * FIXME: specify headers properly in the triangle brackets and rearrange them
 * in a way so it would be simpler to add NDK and other headers for debugging.
 */
#include "ntdef.h"
#define PAGE_SIZE 4096
#include "dderror.h"
#include "devioctl.h"
#include "miniport.h"
#include "ioaccess.h"
#include "video.h"

typedef struct
{
    PHYSICAL_ADDRESS PhysControlStart;
    ULONG ControlLength;
    PVOID VirtControlStart;
    PHYSICAL_ADDRESS PhysFrameBufferStart;
} NVIDIAMP_DEVICE_EXTENSION, *PNVIDIAMP_DEVICE_EXTENSION;

BOOLEAN
NTAPI
OpenNVIDIA_ResetHW(PVOID HwDeviceExtension, 
                                 ULONG Columns, ULONG Rows);

BOOLEAN
NTAPI
OpenNVIDIA_VidInterrupt(PVOID HwDeviceExtension);

VP_STATUS
NTAPI
OpenNVIDIA_GetPowerState(PVOID HwDeviceExtension, 
          ULONG HwId, PVIDEO_POWER_MANAGEMENT VideoPowerControl);

VP_STATUS
NTAPI
OpenNVIDIA_SetPowerState(PVOID HwDeviceExtension, 
          ULONG HwId, PVIDEO_POWER_MANAGEMENT VideoPowerControl);

ULONG
NTAPI
OpenNVIDIA_GetChildDescriptor (PVOID HwDeviceExtension, 
      PVIDEO_CHILD_ENUM_INFO ChildEnumInfo, PVIDEO_CHILD_TYPE pChildType, 
      PVOID pChildDescriptor, PULONG pUId, PULONG pUnused);

VP_STATUS
NTAPI
OpenNVIDIA_FindAdapter(PVOID HwDeviceExtension, 
            PVOID HwContext, PWSTR ArgumentString, 
            PVIDEO_PORT_CONFIG_INFO ConfigInfo, PUCHAR Again);

BOOLEAN
NTAPI
OpenNVIDIA_Initialize(PVOID HwDeviceExtension);

BOOLEAN
NTAPI
OpenNVIDIA_StartIO(PVOID HwDeviceExtension, 
                PVIDEO_REQUEST_PACKET RequestPacket);

/* EOF */
