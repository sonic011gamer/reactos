/*
 * PROJECT:     ReactOS Xbox miniport video driver
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Simple framebuffer driver for NVIDIA NV2A XGPU
 * COPYRIGHT:   Copyright 2021 Justin Miller <justinmiller100@gmail.com>
 *
 * TODO:
 * - Check input parameters everywhere.
 * - Call VideoPortVerifyAccessRanges to reserve the memory we're about
 *   to map.
 */

/* INCLUDES *******************************************************************/

#include "open_nvminiport.h"

#include <debug.h>
#include <dpfilter.h>

#include <drivers/nvidia/xgpu.h>

/* PUBLIC AND PRIVATE FUNCTIONS ***********************************************/

ULONG
NTAPI
DriverEntry(
    IN PVOID Context1,
    IN PVOID Context2)
{
    VIDEO_HW_INITIALIZATION_DATA InitData;
    VP_STATUS vpStatus;

    VideoPortZeroMemory(&InitData, sizeof(InitData));
    InitData.AdapterInterfaceType = PCIBus;
    InitData.HwInitDataSize = sizeof(VIDEO_HW_INITIALIZATION_DATA);
    InitData.HwFindAdapter             = OpenNVIDIA_FindAdapter;
    InitData.HwInitialize              = OpenNVIDIA_Initialize;
    InitData.HwStartIO                 = OpenNVIDIA_StartIO;
    InitData.HwResetHw                 = OpenNVIDIA_ResetHW;
    InitData.HwInterrupt               = OpenNVIDIA_VidInterrupt;
    InitData.HwGetPowerState           = OpenNVIDIA_GetPowerState;
    InitData.HwSetPowerState           = OpenNVIDIA_SetPowerState;
    InitData.HwDeviceExtensionSize = sizeof(NVIDIAMP_DEVICE_EXTENSION);
    InitData.HwGetVideoChildDescriptor = 
                                    OpenNVIDIA_GetChildDescriptor;
    vpStatus = VideoPortInitialize(Context1, Context2, &InitData, NULL);
    return vpStatus;
}

/**********************************************************************
 * 
 *  FakeGfxCard_ResetHW
 *
 *     This routine would reset the hardware when a soft reboot is
 *     performed. Returning FALSE from this routine would force
 *     the HAL to perform an INT 10h and set Mode 3 (Text). 
 *
 *     We are not real hardware so we will just return TRUE so the HAL
 *     does nothing.
 *
 **********************************************************************/
BOOLEAN
NTAPI
OpenNVIDIA_ResetHW(PVOID HwDeviceExtension, 
                                 ULONG Columns, ULONG Rows)
{
   return TRUE;
}

/**********************************************************************
 * 
 *  FakeGfxCard_VidInterrupt
 *
 *     Checks if it's adapter generated an interrupt and dismisses it
 *     or returns FALSE if it did not.
 *
 **********************************************************************/
BOOLEAN
NTAPI
OpenNVIDIA_VidInterrupt(PVOID HwDeviceExtension)
{
   return FALSE;
}

/**********************************************************************
 * 
 *  FakeGfxCard_GetPowerState
 *
 *         Queries if the device can support the requested power state.
 *
 **********************************************************************/
VP_STATUS
NTAPI
OpenNVIDIA_GetPowerState(PVOID HwDeviceExtension, 
          ULONG HwId, PVIDEO_POWER_MANAGEMENT VideoPowerControl)
{
   return NO_ERROR;
}

/**********************************************************************
 * 
 *  FakeGfxCard_SetPowerState
 *
 *         Sets the power state.
 *
 **********************************************************************/
VP_STATUS
NTAPI
OpenNVIDIA_SetPowerState(PVOID HwDeviceExtension, 
          ULONG HwId, PVIDEO_POWER_MANAGEMENT VideoPowerControl)
{
   return NO_ERROR;
}

/**********************************************************************
 * 
 *  FakeGfxCard_GetChildDescriptor
 *
 *        Returns an identifer for any child device supported 
 *        by the miniport.
 *
 **********************************************************************/
ULONG
NTAPI
OpenNVIDIA_GetChildDescriptor (PVOID HwDeviceExtension, 
      PVIDEO_CHILD_ENUM_INFO ChildEnumInfo, PVIDEO_CHILD_TYPE pChildType, 
      PVOID pChildDescriptor, PULONG pUId, PULONG pUnused)
{
   return ERROR_NO_MORE_DEVICES;
}

/**********************************************************************
 * 
 *  FakeGfxCard_FindAdapter
 *
 *        This function performs initialization specific to devices 
 *        maintained by this miniport driver.
 *
 **********************************************************************/
VP_STATUS
NTAPI
OpenNVIDIA_FindAdapter(PVOID HwDeviceExtension, 
            PVOID HwContext, PWSTR ArgumentString, 
            PVIDEO_PORT_CONFIG_INFO ConfigInfo, PUCHAR Again)
{
   return NO_ERROR;
}

/**********************************************************************
 * 
 *  FakeGfxCard_Initialize
 *
 *      This initializes the device.
 *
 **********************************************************************/
BOOLEAN
NTAPI
OpenNVIDIA_Initialize(PVOID HwDeviceExtension)
{
   return TRUE;
}

/**********************************************************************
 * 
 *  FakeGfxCard_StartIO
 *
 *      This routine executes requests on behalf of the GDI Driver 
 *      and the system. The GDI driver is allowed to issue IOCTLs 
 *      which would then be sent to this routine to be performed 
 *      on it's behalf.
 *
 *      We can add our own proprietary IOCTLs here to be processed 
 *      from the GDI driver. 
 *
 **********************************************************************/
BOOLEAN
NTAPI
OpenNVIDIA_StartIO(PVOID HwDeviceExtension, 
                PVIDEO_REQUEST_PACKET RequestPacket)
{
   RequestPacket->StatusBlock->Status      = 0;
   RequestPacket->StatusBlock->Information = 0;
   return TRUE;
}
