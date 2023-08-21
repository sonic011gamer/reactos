/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     VidPn Subsystem Interface
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <dxgkrnl.h>
//#define NDEBUG
#include <debug.h>

extern PDXGKRNL_PRIVATE_EXTENSION DxgkpExtension;


NTSTATUS
APIENTRY
DxgkVidPnGetTopology(IN_CONST_D3DKMDT_HVIDPN                        hVidPn,
                     OUT_PD3DKMDT_HVIDPNTOPOLOGY                    phVidPnTopology,
                     DEREF_OUT_CONST_PPDXGK_VIDPNTOPOLOGY_INTERFACE ppVidPnTopologyInterface);

NTSTATUS
APIENTRY
DxgkVidPnAcquireSourceModeSet(IN_CONST_D3DKMDT_HVIDPN                                hVidPn,
                              IN_CONST_D3DDDI_VIDEO_PRESENT_SOURCE_ID                VidPnSourceId,
                              OUT_PD3DKMDT_HVIDPNSOURCEMODESET                       phVidPnSourceModeSet,
                              DEREF_OUT_CONST_PPDXGK_VIDPNSOURCEMODESET_INTERFACE    ppVidPnSourceModeSetInterface)
{
    DPRINT1("DxgkVidPnAcquireSourceModeSet: Entry\n");
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DxgkVidPnReleaseSourceModeSet(IN_CONST_D3DKMDT_HVIDPN                hVidPn,
                              IN_CONST_D3DKMDT_HVIDPNSOURCEMODESET   hVidPnSourceModeSet)
{
    DPRINT1("DxgkVidPnReleaseSourceModeSet: Entry\n");
    __debugbreak();
    return 0;
}


NTSTATUS
APIENTRY
DxgkVidPnCreateNewSourceModeSet(IN_CONST_D3DKMDT_HVIDPN hVidPn,
                                IN_CONST_D3DDDI_VIDEO_PRESENT_SOURCE_ID                VidPnSourceId,
                                OUT_PD3DKMDT_HVIDPNSOURCEMODESET                       phNewVidPnSourceModeSet,
                                DEREF_OUT_CONST_PPDXGK_VIDPNSOURCEMODESET_INTERFACE    ppVidPnSourceModeSetInterface)
{
    DPRINT1("DxgkVidPnReleaseSourceModeSet: Entry\n");
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DxgkVidPnAssignSourceModeSet(IN_D3DKMDT_HVIDPN                       hVidPn,
                                  IN_CONST_D3DDDI_VIDEO_PRESENT_SOURCE_ID VidPnSourceId,
                                  IN_CONST_D3DKMDT_HVIDPNSOURCEMODESET    hVidPnSourceModeSet)
{
    DPRINT1("DxgkVidPnAssignSourceModeSet: Entry\n");
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DxgkVidPnAssignMultisamplingMethodSet(IN_D3DKMDT_HVIDPN                        hVidPn,
                                      IN_CONST_D3DDDI_VIDEO_PRESENT_SOURCE_ID  VidPnSourceId,
                                      IN_CONST_SIZE_T                          NumMethods,
                                      CONST D3DDDI_MULTISAMPLINGMETHOD* pSupportedMethodSet)

{
    DPRINT1("DxgkVidPnAssignMultisamplingMethodSet: Entry\n");
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DxgkVidPnAcquireTargetModeSet(IN_CONST_D3DKMDT_HVIDPN                                  hVidPn,
                              IN_CONST_D3DDDI_VIDEO_PRESENT_TARGET_ID                  VidPnTargetId,
                              OUT_PD3DKMDT_HVIDPNTARGETMODESET                         phVidPnTargetModeSet,
                              DEREF_OUT_CONST_PPDXGK_VIDPNTARGETMODESET_INTERFACE      ppVidPnTargetModeSetInterface)
{
    DPRINT1("DxgkVidPnAcquireTargetModeSet: Entry\n");
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DxgkVidPnReleaseTargetModeSet(
    IN_CONST_D3DKMDT_HVIDPN                                  hVidPn,
    IN_CONST_D3DKMDT_HVIDPNTARGETMODESET                     hVidPnTargetModeSet)
{
    DPRINT1("DxgkVidPnReleaseTargetModeSet: Entry\n");
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DxgkVidPnCreateNewTargetModeSet(IN_CONST_D3DKMDT_HVIDPN                               hVidPn,
                                IN_CONST_D3DDDI_VIDEO_PRESENT_TARGET_ID               VidPnTargetId,
                                OUT_PD3DKMDT_HVIDPNTARGETMODESET                      phNewVidPnTargetModeSet,
                                DEREF_OUT_CONST_PPDXGK_VIDPNTARGETMODESET_INTERFACE   ppVidPnTargetModeSetInterace)
{
    DPRINT1("DxgkVidPnCreateNewTargetModeSet: Entry\n");
    __debugbreak();
    return 0;
}

NTSTATUS
APIENTRY
DxgkVidPnAssignTargetModeSet(IN_D3DKMDT_HVIDPN                        hVidPn,
                             IN_CONST_D3DDDI_VIDEO_PRESENT_TARGET_ID  VidPnTargetId,
                             IN_CONST_D3DKMDT_HVIDPNTARGETMODESET     hVidPnTargetModeSet)
{
    DPRINT1("DxgkVidPnAssignTargetModeSet: Entry\n");
    __debugbreak();
    return 0;
}

/*
 *  Just provide the VidPn Interface to the KMD miniport driver
 */
NTSTATUS
APIENTRY
CALLBACK
RDDM_DxgkCbQueryVidPnInterface(IN_CONST_D3DKMDT_HVIDPN                hVidPn,
                               IN_CONST_DXGK_VIDPN_INTERFACE_VERSION  VidPnInterfaceVersion,
                               DEREF_OUT_CONST_PPDXGK_VIDPN_INTERFACE ppVidPnInterface)
{
    if (!ppVidPnInterface)
        return STATUS_INVALID_PARAMETER;

    DXGK_VIDPN_INTERFACE* VidPnInterface = ( DXGK_VIDPN_INTERFACE* )*ppVidPnInterface;

    /* For now we're targetting WDDM version 2.0 */
    VidPnInterface->Version = DXGK_VIDPN_INTERFACE_VERSION_V1;
    VidPnInterface->pfnGetTopology = DxgkVidPnGetTopology;
    VidPnInterface->pfnAcquireSourceModeSet = DxgkVidPnAcquireSourceModeSet;
    VidPnInterface->pfnReleaseSourceModeSet =  DxgkVidPnReleaseSourceModeSet;
    VidPnInterface->pfnCreateNewSourceModeSet =  DxgkVidPnCreateNewSourceModeSet;
    VidPnInterface->pfnAssignSourceModeSet =  DxgkVidPnAssignSourceModeSet;
    VidPnInterface->pfnAssignMultisamplingMethodSet = (DXGKDDI_VIDPN_ASSIGNMULTISAMPLINGMETHODSET)DxgkVidPnAssignMultisamplingMethodSet;
    VidPnInterface->pfnAcquireTargetModeSet =  DxgkVidPnAcquireTargetModeSet;
    VidPnInterface->pfnReleaseTargetModeSet =  DxgkVidPnReleaseTargetModeSet;
    VidPnInterface->pfnCreateNewTargetModeSet =  DxgkVidPnCreateNewTargetModeSet;
    VidPnInterface->pfnAssignTargetModeSet =  DxgkVidPnAssignTargetModeSet;

    return STATUS_SUCCESS;
}