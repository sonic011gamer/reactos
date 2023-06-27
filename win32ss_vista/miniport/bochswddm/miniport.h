#pragma once

NTSTATUS
BochsWDDM_AddDevice(IN_CONST_PDEVICE_OBJECT PhysicalDeviceObject,
                    OUT PVOID *MiniportDeviceContext);

NTSTATUS
BochsWDDM_StartDevice(_In_  PVOID MiniportDeviceContext,
                      _In_  PDXGK_START_INFO DxgkStartInfo,
                      _In_  PDXGKRNL_INTERFACE DxgkInterface,
                      _Out_ PULONG NumberOfViews,
                      _Out_ PULONG NumberOfChildren);
