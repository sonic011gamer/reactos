
NTSTATUS
NTAPI
OpenWDDM_AddDevice(
     _In_ PDEVICE_OBJECT     PhysicalDeviceObject,
    _Out_ PVOID*                  MiniportDeviceContext);
NTSTATUS
NTAPI
OpenWDDM_StartDevice(
     _In_  PVOID          MiniportDeviceContext,
     _In_ PDXGK_START_INFO     DxgkStartInfo,
     _In_ PDXGKRNL_INTERFACE   DxgkInterface,
     _Out_ PULONG              NumberOfVideoPresentSources,
     _Out_ PULONG              NumberOfChildren);

NTSTATUS
NTAPI
OpenWDDM_StopDevice(
    IN_CONST_PVOID  MiniportDeviceContext);

NTSTATUS
NTAPI
OpenWDDM_RemoveDevice(
    IN_CONST_PVOID  MiniportDeviceContext);

NTSTATUS
NTAPI
OpenWDDM_Dispatch_IoRequest(
    IN_CONST_PVOID              MiniportDeviceContext,
    IN_ULONG                    VidPnSourceId,
    IN_PVIDEO_REQUEST_PACKET    VideoRequestPacket);