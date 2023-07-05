/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DXGKRNL Undocumented Vista
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include <dxgkrnl.h>
//#define NDEBUG
#include <debug.h>
extern DXGKRNL_INTERFACE DxgkrnlInterface;
/*
 *  I can't really find much information on these exports...
 *  These are going to be made VIA guesses from me inserting our lovely little
 *  RDDM into vista.
 */

#if 0
//TODO: Turn into custom definitions
_DxgkSqmAddToStream(long long ptr)
_DxgkSqmCommonGeneric(long long ptr long)
_DxgkSqmCreateDwordStreamEntry(ptr long)
_DxgkSqmCreateStringStreamEntry(ptr ptr)
_DxgkSqmGenericDword64(ptr)
_DxgkSqmGenericDword(ptr)
DpiGetDriverVersion(long)
DpiGetDxgAdapter(long)
DpiGetSchedulerCallbackState(long)
DpiSetSchedulerCallbackState(long ptr)
DpSynchronizeExecution(long long ptr long ptr)
TdrCompleteRecoveryContext(ptr long)
TdrCreateRecoveryContext()
TdrIsRecoveryRequired(ptr)
TdrIsTimeoutForcedFlip()
TdrResetFromTimeout(ptr)
TraceDxgkBlockThread(long)
TraceDxgkContext(long ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr)
TraceDxgkDevice(long ptr ptr ptr ptr)
TraceDxgkFunctionProfiler(long long)
TraceDxgkPerformanceWarning(long)
TraceDxgkPresentHistory(long long ptr)
#endif

#if 0
DXGKDDI_START_DEVICE(_In_ PVOID                MiniportDeviceContext,
                     _In_ PDXGK_START_INFO     DxgkStartInfo,
                     _In_ PDXGKRNL_INTERFACE   DxgkInterface,
                     _Out_ PULONG              NumberOfVideoPresentSources,
                     _Out_ PULONG              NumberOfChildren);
#endif
extern PDXGKRNL_PRIVATE_EXTENSION Extension;
NTSTATUS
NTAPI
RDDM_StartMiniport(DEVICE_OBJECT *DeviceObject)
{
    NTSTATUS Status = 0;
    DXGK_START_INFO     DxgkStartInfo = {0};

    ULONG              NumberOfVideoPresentSources;
    ULONG              NumberOfChildren;
    PAGED_CODE();

    DPRINT1("RDDM_StartMiniport: calling miniport DxgkDdiStartDevice\n");
    /* Call the miniport Routine */
    Status = Extension->DxgkDdiStartDevice(Extension->MiniportContext, &DxgkStartInfo,
                        &DxgkrnlInterface, &NumberOfVideoPresentSources, &NumberOfChildren);
    if(Status != STATUS_SUCCESS)
    {
        DPRINT1("RDDM_MiniportAddDevice: DxgkDdiStartDevice Miniport call failed with status %X\n", Status);
    }
    else{
        DPRINT1("RDDM_MiniportAddDevice: DxgkDdiStartDevice Miniport call has continued with success\n");
    }

    return 0;
}
