/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     WDDM 1.0-1.1 initilization
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include <dxgkrnl.h>
#define NDEBUG
#include <debug.h>

typedef
NTSTATUS
(APIENTRY *TEST_WIN32K_INTERFACE)();

NTSTATUS
APIENTRY
TestWin32kInterface()
{
    DPRINT1("TestWin32kInterface\n");
    __debugbreak();
    return 0;
}

VOID
RDDM_CreateWin32kInterface(PDXGKWIN32K_INTERFACE Interface, ULONG *Size)
{
    TEST_WIN32K_INTERFACE InterfaceTest = TestWin32kInterface;
    if (sizeof(DXGKWIN32K_INTERFACE) != 254)
    {
        DPRINT1("size mismatch, size is exactlly %d\n", sizeof(DXGKWIN32K_INTERFACE));
    }
    else
    {
        DPRINT1("Congrats the size fits\n");
    }
    Interface->DxgkCddInterfaceReference = (ULONG_PTR)InterfaceTest;
    Interface->DxgkCddInterfaceReferenceTwo = (ULONG_PTR)InterfaceTest;
    Interface->DxgkProcessCallout = (ULONG_PTR)InterfaceTest;
    Interface->DxgkOpenAdapter = (ULONG_PTR)InterfaceTest;
    Interface->DxgkCloseAdapter = (ULONG_PTR)InterfaceTest;
    Interface->DxgkCreateAllocation = (ULONG_PTR)InterfaceTest;
    Interface->DxgkQueryResourceInfo = (ULONG_PTR)InterfaceTest;
    Interface->DxgkOpenResource = (ULONG_PTR)InterfaceTest;
    Interface->DxgkDestroyAllocation = (ULONG_PTR)InterfaceTest;
    Interface->DxgkSetAllocationPriority = (ULONG_PTR)InterfaceTest;
    Interface->DxgkQueryAllocationResidency = (ULONG_PTR)InterfaceTest;
    Interface->DxgkCreateDevice = (ULONG_PTR)InterfaceTest;
    Interface->DxgkDestroyDevice = (ULONG_PTR)InterfaceTest;
    Interface->DxgkCreateContext = (ULONG_PTR)InterfaceTest;
    Interface->DxgkDestroyContext = (ULONG_PTR)InterfaceTest;
    Interface->DxgkCreateSynchronizationObject = (ULONG_PTR)InterfaceTest;
    Interface->DxgkDestroySynchronizationObject = (ULONG_PTR)InterfaceTest;
    Interface->DxgkWaitForSynchronizationObject = (ULONG_PTR)InterfaceTest;
    Interface->DxgkSignalSynchronizationObject = (ULONG_PTR)InterfaceTest;
    Interface->DxgkLock = (ULONG_PTR)InterfaceTest;
    Interface->DxgkUnlock = (ULONG_PTR)InterfaceTest;
    Interface->DxgkRender = (ULONG_PTR)InterfaceTest;
    Interface->DxgkGetRuntimeData = (ULONG_PTR)InterfaceTest;
    Interface->DxgkQueryAdapterInfo = (ULONG_PTR)InterfaceTest;
    Interface->DxgkEscape = (ULONG_PTR)InterfaceTest;
    Interface->DxgkQueryStatistics = (ULONG_PTR)InterfaceTest;
    Interface->DxgkGetDisplayModeList = (ULONG_PTR)InterfaceTest;
    Interface->DxgkSetDisplayMode = (ULONG_PTR)InterfaceTest;
    Interface->DxgkGetMultisampleMethodList = (ULONG_PTR)InterfaceTest;
    Interface->DxgkPresent = (ULONG_PTR)InterfaceTest;
    Interface->DxgkGetSharedPrimaryHandle = (ULONG_PTR)InterfaceTest;
    Interface->DxgkCreateOverlay = (ULONG_PTR)InterfaceTest;
    Interface->DxgkUpdateOverlay = (ULONG_PTR)InterfaceTest;
    Interface->DxgkFlipOverlay = (ULONG_PTR)InterfaceTest;
    Interface->DxgkDestroyOverlay = (ULONG_PTR)InterfaceTest;
    Interface->DxgkWaitForVerticalBlankEvent = (ULONG_PTR)InterfaceTest;
    Interface->DxgkGetPresentHistory = (ULONG_PTR)InterfaceTest;
    Interface->DxgkSetGammaRamp = (ULONG_PTR)InterfaceTest;
    Interface->DxgkSetVidPnSourceOwner = (ULONG_PTR)InterfaceTest;
    Interface->DxgkGetDeviceState = (ULONG_PTR)InterfaceTest;
    Interface->DxgkSetContextSchedulingPriority = (ULONG_PTR)InterfaceTest;
    Interface->DxgkGetContextSchedulingPriority = (ULONG_PTR)InterfaceTest;
    Interface->DxgkSetProcessSchedulingPriorityClass = (ULONG_PTR)InterfaceTest;
    Interface->DxgkGetProcessSchedulingPriorityClass = (ULONG_PTR)InterfaceTest;
    Interface->DxgkReleaseProcessVidPnSourceOwners = (ULONG_PTR)InterfaceTest;
    Interface->DxgkGetScanLine = (ULONG_PTR)InterfaceTest;
    Interface->DxgkSetQueuedLimit = (ULONG_PTR)InterfaceTest;
    Interface->DxgkPollDisplayChildren = (ULONG_PTR)InterfaceTest;
    Interface->DxgkInvalidateActiveVidPn = (ULONG_PTR)InterfaceTest;
    Interface->DxgkCheckOcclusion = (ULONG_PTR)InterfaceTest;
    Interface->DxgkWaitForIdle = (ULONG_PTR)InterfaceTest;
    Interface->DxgkCheckMonitorPowerState = (ULONG_PTR)InterfaceTest;
    Interface->DxgkCheckExclusiveOwnership = (ULONG_PTR)InterfaceTest;
    Interface->DxgkSetDisplayPrivateDriverFormat = (ULONG_PTR)InterfaceTest;
    *Size = 254; /* Fuck you. (With Respect)*/
    __debugbreak();
}
