/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     Core Header
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#pragma once

#include <ntddk.h>
#include <windef.h>
#include <ntstatus.h>

#include <stdio.h>
#include <locd3d.h>

#include "pdxgkrnl.h"

/* Create an IO request to fill out the function pointer list */
#define IOCTL_VIDEO_DDI_FUNC_REGISTER \
	CTL_CODE( FILE_DEVICE_VIDEO, 0xF, METHOD_NEITHER, FILE_ANY_ACCESS  )

#define IOCTL_VIDEO_I_AM_REACTOS \
	CTL_CODE( FILE_DEVICE_VIDEO, 0xB, METHOD_NEITHER, FILE_ANY_ACCESS  )

#define IOCTL_VIDEO_DDI_FUNC_WIN8_REGISTER \
	CTL_CODE( FILE_DEVICE_VIDEO, 0xA, METHOD_NEITHER, FILE_ANY_ACCESS  )

/* FIXME: This needs some real investigation - this is this stupid  */
typedef struct _DXGKRNL_PRIVATE_EXTENSION
{
	PDRIVER_INITIALIZATION_DATA DriverInitData;
	PDRIVER_OBJECT DriverObject;
	PDXGKDDI_ADD_DEVICE                       DxgkDdiAddDevice;
    PDXGKDDI_START_DEVICE                     DxgkDdiStartDevice;
    PDXGKDDI_STOP_DEVICE                      DxgkDdiStopDevice;
    PDXGKDDI_REMOVE_DEVICE                    DxgkDdiRemoveDevice;
    PDXGKDDI_DISPATCH_IO_REQUEST              DxgkDdiDispatchIoRequest;
    PDXGKDDI_INTERRUPT_ROUTINE                DxgkDdiInterruptRoutine;
    PDXGKDDI_DPC_ROUTINE                      DxgkDdiDpcRoutine;
    PDXGKDDI_QUERY_CHILD_RELATIONS            DxgkDdiQueryChildRelations;
    PDXGKDDI_QUERY_CHILD_STATUS               DxgkDdiQueryChildStatus;
    PDXGKDDI_QUERY_DEVICE_DESCRIPTOR          DxgkDdiQueryDeviceDescriptor;
    PDXGKDDI_SET_POWER_STATE                  DxgkDdiSetPowerState;
    PDXGKDDI_NOTIFY_ACPI_EVENT                DxgkDdiNotifyAcpiEvent;
    PDXGKDDI_RESET_DEVICE                     DxgkDdiResetDevice;
    PDXGKDDI_UNLOAD                           DxgkDdiUnload;
    PDXGKDDI_QUERY_INTERFACE                  DxgkDdiQueryInterface;
    PDXGKDDI_CONTROL_ETW_LOGGING              DxgkDdiControlEtwLogging;
    PDXGKDDI_QUERYADAPTERINFO                 DxgkDdiQueryAdapterInfo;
    PDXGKDDI_CREATEDEVICE                     DxgkDdiCreateDevice;
    PDXGKDDI_CREATEALLOCATION                 DxgkDdiCreateAllocation;
    PDXGKDDI_DESTROYALLOCATION                DxgkDdiDestroyAllocation;
    PDXGKDDI_DESCRIBEALLOCATION               DxgkDdiDescribeAllocation;
    PDXGKDDI_GETSTANDARDALLOCATIONDRIVERDATA  DxgkDdiGetStandardAllocationDriverData;
    PDXGKDDI_ACQUIRESWIZZLINGRANGE            DxgkDdiAcquireSwizzlingRange;
    PDXGKDDI_RELEASESWIZZLINGRANGE            DxgkDdiReleaseSwizzlingRange;
    PDXGKDDI_PATCH                            DxgkDdiPatch;
    PDXGKDDI_SUBMITCOMMAND                    DxgkDdiSubmitCommand;
    PDXGKDDI_PREEMPTCOMMAND                   DxgkDdiPreemptCommand;
    PDXGKDDI_BUILDPAGINGBUFFER                DxgkDdiBuildPagingBuffer;
    PDXGKDDI_SETPALETTE                       DxgkDdiSetPalette;
    PDXGKDDI_SETPOINTERPOSITION               DxgkDdiSetPointerPosition;
    PDXGKDDI_SETPOINTERSHAPE                  DxgkDdiSetPointerShape;
    PDXGKDDI_RESETFROMTIMEOUT                 DxgkDdiResetFromTimeout;
    PDXGKDDI_RESTARTFROMTIMEOUT               DxgkDdiRestartFromTimeout;
    PDXGKDDI_ESCAPE                           DxgkDdiEscape;
    PDXGKDDI_COLLECTDBGINFO                   DxgkDdiCollectDbgInfo;
    PDXGKDDI_QUERYCURRENTFENCE                DxgkDdiQueryCurrentFence;
    PDXGKDDI_ISSUPPORTEDVIDPN                 DxgkDdiIsSupportedVidPn;
    PDXGKDDI_RECOMMENDFUNCTIONALVIDPN         DxgkDdiRecommendFunctionalVidPn;
    PDXGKDDI_ENUMVIDPNCOFUNCMODALITY          DxgkDdiEnumVidPnCofuncModality;
    PDXGKDDI_SETVIDPNSOURCEADDRESS            DxgkDdiSetVidPnSourceAddress;
    PDXGKDDI_SETVIDPNSOURCEVISIBILITY         DxgkDdiSetVidPnSourceVisibility;
    PDXGKDDI_COMMITVIDPN                      DxgkDdiCommitVidPn;
    PDXGKDDI_UPDATEACTIVEVIDPNPRESENTPATH     DxgkDdiUpdateActiveVidPnPresentPath;
    PDXGKDDI_RECOMMENDMONITORMODES            DxgkDdiRecommendMonitorModes;
    PDXGKDDI_RECOMMENDVIDPNTOPOLOGY           DxgkDdiRecommendVidPnTopology;
    PDXGKDDI_GETSCANLINE                      DxgkDdiGetScanLine;
    PDXGKDDI_STOPCAPTURE                      DxgkDdiStopCapture;
    PDXGKDDI_CONTROLINTERRUPT                 DxgkDdiControlInterrupt;
    PDXGKDDI_CREATEOVERLAY                    DxgkDdiCreateOverlay;
    PDXGKDDI_DESTROYDEVICE                    DxgkDdiDestroyDevice;
    PDXGKDDI_OPENALLOCATIONINFO               DxgkDdiOpenAllocation;
    PDXGKDDI_CLOSEALLOCATION                  DxgkDdiCloseAllocation;
    PDXGKDDI_RENDER                           DxgkDdiRender;
    PDXGKDDI_PRESENT                          DxgkDdiPresent;
    PDXGKDDI_UPDATEOVERLAY                    DxgkDdiUpdateOverlay;
    PDXGKDDI_FLIPOVERLAY                      DxgkDdiFlipOverlay;
    PDXGKDDI_DESTROYOVERLAY                   DxgkDdiDestroyOverlay;
    PDXGKDDI_CREATECONTEXT                    DxgkDdiCreateContext;
    PDXGKDDI_DESTROYCONTEXT                   DxgkDdiDestroyContext;
    PDXGKDDI_LINK_DEVICE                      DxgkDdiLinkDevice;
    PDXGKDDI_SETDISPLAYPRIVATEDRIVERFORMAT    DxgkDdiSetDisplayPrivateDriverFormat;
    PDEVICE_OBJECT MiniportFdo;
    PDEVICE_OBJECT MiniportPdo;
    PVOID MiniportContext;
} DXGKRNL_PRIVATE_EXTENSION, *PDXGKRNL_PRIVATE_EXTENSION;