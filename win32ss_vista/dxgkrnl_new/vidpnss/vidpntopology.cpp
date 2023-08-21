/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     VidPn Subsystem Topology
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <dxgkrnl.h>
//#define NDEBUG
#include <debug.h>


NTSTATUS
APIENTRY
DxgkDdiVidPnTopologyGetNumPaths(IN_CONST_D3DKMDT_HVIDPNTOPOLOGY  hVidPnTopology,
                                OUT_PSIZE_T                      pNumPaths)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DxgkDdiVidPnTopologyGetNumPathsFromSource(IN_CONST_D3DKMDT_HVIDPNTOPOLOGY            hVidPnTopology,
                                          IN_CONST_D3DDDI_VIDEO_PRESENT_SOURCE_ID    VidPnSourceId,
                                          OUT_PSIZE_T                                pNumPathsFromSource)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DxgkDdiVidPnTopologyEnumPathTargetsFromSources(
    IN_CONST_D3DKMDT_HVIDPNTOPOLOGY            hVidPnTopology,
    IN_CONST_D3DDDI_VIDEO_PRESENT_SOURCE_ID    VidPnSourceId,
    IN_CONST_D3DKMDT_VIDPN_PRESENT_PATH_INDEX  VidPnPresentPathIndex,
    OUT_PD3DDDI_VIDEO_PRESENT_TARGET_ID        pVidPnTargetId)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DxgkDdiVidPnTopologyGetPathSourceFromTargets(
    IN_CONST_D3DKMDT_HVIDPNTOPOLOGY            hVidTopology,
    IN_CONST_D3DDDI_VIDEO_PRESENT_TARGET_ID    VidPnTargetId,
    OUT_PD3DDDI_VIDEO_PRESENT_SOURCE_ID        pVidPnSourceId)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DxgkDdiVidPnTopologyAcquirePathInfo(
    IN_CONST_D3DKMDT_HVIDPNTOPOLOGY              hVidPnTopology,
    IN_CONST_D3DDDI_VIDEO_PRESENT_SOURCE_ID      VidPnSourceId,
    IN_CONST_D3DDDI_VIDEO_PRESENT_TARGET_ID      VidPnTargetId,
    DEREF_OUT_CONST_PPD3DKMDT_VIDPN_PRESENT_PATH ppVidPnPresentPathInfo)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DxgkDdiVidPnTopologyAcquireFirstPathInfo(
    IN_CONST_D3DKMDT_HVIDPNTOPOLOGY              hVidPnTopology,
    DEREF_OUT_CONST_PPD3DKMDT_VIDPN_PRESENT_PATH ppFirstVidPnPresentPathInfo)
{
    PD3DKMDT_VIDPN_PRESENT_PATH VidPnPresentPathInfo = ( PD3DKMDT_VIDPN_PRESENT_PATH )*ppFirstVidPnPresentPathInfo;
    VidPnPresentPathInfo->VidPnSourceId = 0;
    VidPnPresentPathInfo->VidPnTargetId = 0;
    VidPnPresentPathInfo->NumPathModes = 1;
    VidPnPresentPathInfo->ContentTransformation.Scaling = D3DKMDT_VPPS_IDENTITY;
    VidPnPresentPathInfo->ContentTransformation.Rotation = D3DKMDT_VPPR_IDENTITY;
    VidPnPresentPathInfo->VidPnTargetColorBasis = D3DKMDT_CB_SCRGB;
    VidPnPresentPathInfo->VidPnTargetColorCoeffDynamicRanges.FirstChannel = 0;
    VidPnPresentPathInfo->VidPnTargetColorCoeffDynamicRanges.SecondChannel = 0;
    VidPnPresentPathInfo->VidPnTargetColorCoeffDynamicRanges.ThirdChannel = 0;
    VidPnPresentPathInfo->VidPnTargetColorCoeffDynamicRanges.FourthChannel = 0;
    VidPnPresentPathInfo->VidPnTargetColorCoeffDynamicRanges.FifthChannel = 0;
    VidPnPresentPathInfo->VidPnTargetColorCoeffDynamicRanges.SixthChannel = 0;
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}


NTSTATUS
APIENTRY
DxgkDdiVidPnTopologyAcquireNextPathInfo(
    IN_CONST_D3DKMDT_HVIDPNTOPOLOGY              hVidPnTopology,
    IN_CONST_PD3DKMDT_VIDPN_PRESENT_PATH_CONST   pVidPnPresentPathInfo,
    DEREF_OUT_CONST_PPD3DKMDT_VIDPN_PRESENT_PATH ppNextVidPnPresentPathInfo)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DxgkDdiVidPnTopologyUpdatePathSupportInfo(
     IN_CONST_D3DKMDT_HVIDPNTOPOLOGY              i_hVidPnTopology,
     IN_CONST_PD3DKMDT_VIDPN_PRESENT_PATH         i_pVidPnPresentPathInfo)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DxgkDdiVidPnTopologyReleasePathInfo(
     IN_CONST_D3DKMDT_HVIDPNTOPOLOGY             hVidPnTopology,
     IN_CONST_PD3DKMDT_VIDPN_PRESENT_PATH_CONST  pVidPnPresentPathInfo)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DxgkDdiVidPnTopologyCreateNewPathInfo(IN_CONST_D3DKMDT_HVIDPNTOPOLOGY             hVidPnTopology,
                                        DEREF_OUT_PPD3DKMDT_VIDPN_PRESENT_PATH      ppNewVidPnPresentPathInfo)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DxgkDdiVidPnTopologyAddPath(IN_D3DKMDT_HVIDPNTOPOLOGY                   hVidPnTopology,
                            IN_PD3DKMDT_VIDPN_PRESENT_PATH              pVidPnPresentPath)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DxgkDdiVidPnTopologyRemovePath(IN_CONST_D3DKMDT_HVIDPNTOPOLOGY           hVidPnTopology,
                                 IN_CONST_D3DDDI_VIDEO_PRESENT_SOURCE_ID  VidPnSourceId,
                                 IN_CONST_D3DDDI_VIDEO_PRESENT_TARGET_ID  VidPnTargetId)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DxgkVidPnGetTopology(IN_CONST_D3DKMDT_HVIDPN                        hVidPn,
                     OUT_PD3DKMDT_HVIDPNTOPOLOGY                    phVidPnTopology,
                     DEREF_OUT_CONST_PPDXGK_VIDPNTOPOLOGY_INTERFACE ppVidPnTopologyInterface)
{
    DPRINT1("DxgkVidPnGetTopology: Entry\n");
    if (!ppVidPnTopologyInterface)
        return STATUS_INVALID_PARAMETER;

    REACTOS_VIDPN_TOPOLOGY_HANDLE VidPnTopologyHandle = {0};
    DXGK_VIDPNTOPOLOGY_INTERFACE* VidPnTopologyInterface = ( DXGK_VIDPNTOPOLOGY_INTERFACE* )*ppVidPnTopologyInterface;

    /*
     * This looks weird... Lemme explain
     * 1) phVidPnTopology is technically a double pointer, essentially a pointer to
     * a handle
     * 2) We dereference this so we can assign the actual HANDLE (D3DKMDT_HVIDPNTOPOLOGY)
     * 3) We assign it to a reference of an internal structure known as REACTOS_VIDPN_TOPOLOGY_HANDLE for now.
     * all of this stuff is internal and unused by anything else, unless they're hacky.
     * as this internal structure changes between versions..
     */
    *phVidPnTopology = (D3DKMDT_HVIDPNTOPOLOGY)&VidPnTopologyHandle
    VidPnTopologyHandle.IsActive = TRUE; /* Doesn't do much yet */

    /* For now we're targetting WDDM version 2.0 */
    VidPnTopologyInterface.Version = DXGK_VIDPN_INTERFACE_VERSION_V1;
    VidPnTopologyInterface.pfnAcquireFirstPathInfo = DxgkDdiVidPnTopologyAcquireFirstPathInfo;
    VidPnTopologyInterface.pfnAcquireNextPathInfo = DxgkDdiVidPnTopologyAcquireNextPathInfo;
    VidPnTopologyInterface.pfnAcquirePathInfo = DxgkDdiVidPnTopologyAcquirePathInfo;
    VidPnTopologyInterface.pfnAddPath = DxgkDdiVidPnTopologyAddPath;
    VidPnTopologyInterface.pfnCreateNewPathInfo = DxgkDdiVidPnTopologyCreateNewPathInfo;
    VidPnTopologyInterface.pfnEnumPathTargetsFromSource = DxgkDdiVidPnTopologyEnumPathTargetsFromSources;
    VidPnTopologyInterface.pfnGetNumPaths = DxgkDdiVidPnTopologyGetNumPaths;
    VidPnTopologyInterface.pfnGetNumPathsFromSource = DxgkDdiVidPnTopologyGetNumPathsFromSource;
    VidPnTopologyInterface.pfnGetPathSourceFromTarget = DxgkDdiVidPnTopologyGetPathSourceFromTargets;
    VidPnTopologyInterface.pfnReleasePathInfo = DxgkDdiVidPnTopologyReleasePathInfo;
    VidPnTopologyInterface.pfnRemovePath = DxgkDdiVidPnTopologyRemovePath;
    VidPnTopologyInterface.pfnUpdatePathSupportInfo = DxgkDdiVidPnTopologyUpdatePathSupportInfo;


    __debugbreak();
    return 0;
}