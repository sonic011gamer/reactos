
/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     Misc Dxgk APIs
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <include/dxgkrnl.h>
//#define NDEBUG
#include <debug.h>

#if 0
typedef struct _DXGK_VIDPNTOPOLOGY_INTERFACE
{
    DXGKDDI_VIDPNTOPOLOGY_GETNUMPATHS                pfnGetNumPaths;
    DXGKDDI_VIDPNTOPOLOGY_GETNUMPATHSFROMSOURCE      pfnGetNumPathsFromSource;
    DXGKDDI_VIDPNTOPOLOGY_ENUMPATHTARGETSFROMSOURCE  pfnEnumPathTargetsFromSource;
    DXGKDDI_VIDPNTOPOLOGY_GETPATHSOURCEFROMTARGET    pfnGetPathSourceFromTarget;
    DXGKDDI_VIDPNTOPOLOGY_ACQUIREPATHINFO            pfnAcquirePathInfo;
    DXGKDDI_VIDPNTOPOLOGY_ACQUIREFIRSTPATHINFO       pfnAcquireFirstPathInfo;
    DXGKDDI_VIDPNTOPOLOGY_ACQUIRENEXTPATHINFO        pfnAcquireNextPathInfo;
    DXGKDDI_VIDPNTOPOLOGY_UPDATEPATHSUPPORTINFO      pfnUpdatePathSupportInfo;
    DXGKDDI_VIDPNTOPOLOGY_RELEASEPATHINFO            pfnReleasePathInfo;
    DXGKDDI_VIDPNTOPOLOGY_CREATENEWPATHINFO          pfnCreateNewPathInfo;
    DXGKDDI_VIDPNTOPOLOGY_ADDPATH                    pfnAddPath;
    DXGKDDI_VIDPNTOPOLOGY_REMOVEPATH                 pfnRemovePath;
} DXGK_VIDPNTOPOLOGY_INTERFACE;
#endif

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
DxgkDdiVidPnTopologyENUMPATHTARGETSFROMSOURCE(
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
DxgkDdiVidPnTopologyGETPATHSOURCEFROMTARGET(
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
DxgkDdiVidPnTopologyACQUIREPATHINFO(
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
DxgkDdiVidPnTopologyACQUIREFIRSTPATHINFO(
    IN_CONST_D3DKMDT_HVIDPNTOPOLOGY              hVidPnTopology,
    DEREF_OUT_CONST_PPD3DKMDT_VIDPN_PRESENT_PATH ppFirstVidPnPresentPathInfo)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}


NTSTATUS
APIENTRY
DxgkDdiVidPnTopologyACQUIRENEXTPATHINFO(
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
DxgkDdiVidPnTopologyUPDATEPATHSUPPORTINFO(
     IN_CONST_D3DKMDT_HVIDPNTOPOLOGY              i_hVidPnTopology,
     IN_CONST_PD3DKMDT_VIDPN_PRESENT_PATH         i_pVidPnPresentPathInfo)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DxgkDdiVidPnTopologyRELEASEPATHINFO(
     IN_CONST_D3DKMDT_HVIDPNTOPOLOGY             hVidPnTopology,
     IN_CONST_PD3DKMDT_VIDPN_PRESENT_PATH_CONST  pVidPnPresentPathInfo)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DXGKDDI_VIDPNTOPOLOGY_CREATENEWPATHINFO(IN_CONST_D3DKMDT_HVIDPNTOPOLOGY             hVidPnTopology,
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