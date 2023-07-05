#include "drivertest.h"
#include <debug.h>

NTSTATUS
NTAPI
OpenWDDM_AddDevice(_In_  PDEVICE_OBJECT     PhysicalDeviceObject,
                   _Out_ PVOID*             MiniportDeviceContext)
{
    DPRINT1("OpenWDDM_AddDevice\n");
       PHYSICAL_ADDRESS FrameBuffer;


    /* Reuse framebuffer that was set up by firmware */
    FrameBuffer.QuadPart = 0x80000000; //GetFramebuffer

   ULONG_PTR MappedAddress = (ULONG_PTR)MmMapIoSpace(
         FrameBuffer,
         0x1D4C00,
         MmNonCached);
    for(int i = 0; i< 0x1D4C00; i++)
    {
    *((UINT32*)MappedAddress + i) = 0xFF00FF;
    }
    /* Not much to do here! we aren't a real GPU*/
    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
OpenWDDM_StartDevice(
     _In_  PVOID          MiniportDeviceContext,
     _In_ PDXGK_START_INFO     DxgkStartInfo,
     _In_ PDXGKRNL_INTERFACE   DxgkInterface,
     _Out_ PULONG              NumberOfVideoPresentSources,
     _Out_ PULONG              NumberOfChildren)
{
    DPRINT1("OpenWDDM_StartDevice\n");

    PHYSICAL_ADDRESS FrameBuffer;


    /* Reuse framebuffer that was set up by firmware */
    FrameBuffer.QuadPart = 0x80000000; //GetFramebuffer

   ULONG_PTR MappedAddress = (ULONG_PTR)MmMapIoSpace(
         FrameBuffer,
         0x1D4C00,
         MmNonCached);
    for(int i = 0; i< 0x1D4C00; i++)
    {
    *((UINT32*)MappedAddress + i) = 0xFF00FF;
    }

    __debugbreak();
    return 0;
}

NTSTATUS
NTAPI
OpenWDDM_StopDevice(
     _In_ PVOID  MiniportDeviceContext)
{
    DPRINT1("OpenWDDM_StopDevice\n");
    __debugbreak();
    return 0;
}

NTSTATUS
NTAPI
OpenWDDM_RemoveDevice(
     _In_ PVOID  MiniportDeviceContext)
{
    DPRINT1("OpenWDDM_RemoveDevice\n");
    __debugbreak();
    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
OpenWDDM_Dispatch_IoRequest(
     _In_ PVOID              MiniportDeviceContext,
     _In_ ULONG                    VidPnSourceId,
     _In_ PVIDEO_REQUEST_PACKET    VideoRequestPacket)
{
    DPRINT1("OpenWDDM_Dispatch_IoRequest\n");
    __debugbreak();
    return STATUS_SUCCESS;
}