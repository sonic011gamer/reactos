/*
 * PROJECT:     ReactOS WDDM Alternative Headers
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     WDDM Headers
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */
#pragma once

#include "d3dkmddi.h"
#include "d3dkmthk.h"
#include "dispmprt.h"

#define STATUS_GRAPHICS_PRESENT_OCCLUDED ((NTSTATUS)0xC01E0006L)

typedef struct _D3DKMT_CHECKMONITORPOWERSTATE {
  D3DKMT_HANDLE                  hAdapter;
  D3DDDI_VIDEO_PRESENT_SOURCE_ID VidPnSourceId;
} D3DKMT_CHECKMONITORPOWERSTATE;

typedef struct _D3DKMT_CHECKOCCLUSION {
  void D3DKMT_PTR(
    HWND    unnamedParam1,
    hWindow unnamedParam2
  );
} D3DKMT_CHECKOCCLUSION;

NTSTATUS
APIENTRY
NtGdiDdDDICheckMonitorPowerState(_In_ D3DKMT_CHECKMONITORPOWERSTATE *unnamedParam1);

NTSTATUS
APIENTRY
NtGdiDdDDICheckOcclusion(_In_ D3DKMT_CHECKOCCLUSION *unnamedParam1);

NTSTATUS
APIENTRY
NtGdiDdDDICheckVidPnExclusiveOwnership(_In_ D3DKMT_CHECKVIDPNEXCLUSIVEOWNERSHIP *unnamedParam1);

NTSTATUS
APIENTRY
NtGdiDdDDICloseAdapter(_In_ D3DKMT_CLOSEADAPTER *unnamedParam1);

NTSTATUS
APIENTRY
NtGdiDdDDICreateAllocation(_Inout_ D3DKMT_CREATEALLOCATION *unnamedParam1);

NTSTATUS
APIENTRY
NtGdiDdDDICreateContext(D3DKMT_CREATECONTEXT *unnamedParam1);

NTSTATUS
APIENTRY
NtGdiDdDDICreateDCFromMemory(D3DKMT_CREATEDCFROMMEMORY *desc);

NTSTATUS
APIENTRY
NtGdiDdDDICreateDevice(D3DKMT_CREATEDEVICE *unnamedParam1);

NTSTATUS
APIENTRY
NtGdiDdDDICreateOverlay(D3DKMT_CREATEOVERLAY *unnamedParam1);

NTSTATUS
APIENTRY
NtGdiDdDDICreateSynchronizationObject(D3DKMT_CREATESYNCHRONIZATIONOBJECT *unnamedParam1);

NTSTATUS
APIENTRY
NtGdiDdDDIDestroyAllocation(_In_ D3DKMT_DESTROYALLOCATION *unnamedParam1);

NTSTATUS
APIENTRY
NtGdiDdDDIDestroyContext(_In_ D3DKMT_DESTROYCONTEXT *unnamedParam1);

NTSTATUS
APIENTRY
NtGdiDdDDIDestroyDevice(_In_ D3DKMT_DESTROYDEVICE *unnamedParam1);

NTSTATUS
APIENTRY
NtGdiDdDDIDestroyAllocation(_In_ D3DKMT_DESTROYALLOCATION *unnamedParam1);

NTSTATUS
APIENTRY
NtGdiDdDDIDestroyContext(_In_ D3DKMT_DESTROYCONTEXT *unnamedParam1);

NTSTATUS
APIENTRY
NtGdiDdDDIEscape(_In_ D3DKMT_ESCAPE *unnamedParam1);
