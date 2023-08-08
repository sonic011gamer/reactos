#pragma once

class DXGADAPTER
{
public:
    DXGADAPTER();
    ~DXGADAPTER();

    NTSTATUS                Initialize(PDEVICE_OBJECT AdapterDeviceObject);
    BOOLEAN                 IsOverlayEnabled(VOID);
    NTSTATUS                FlushScheduler(BOOLEAN EnableFlush);
    VOID                    UpdateVidPnTargetList(VOID);
    VOID                    UpdateOrientation(UINT32 UndecidedName1);
    BOOLEAN                 AcquireDdiSync(UINT32 AdapterDdiSynchLevel);
    VOID                    ReleaseDdiSync(VOID);
    VOID                    AcquireReference(VOID);
    VOID                    AcquireCoreResourceShared(VOID);
    VOID                    AcquireCoreResourceExclusive(VOID);
    VOID                    PurgeSegments(BOOLEAN PreserveContent);
    VOID                    RestoreFromPurgeSegments(VOID);
    NTSTATUS                DdiSetVidPnSourceAddress(DXGKARG_SETVIDPNSOURCEADDRESS* SetVidPnSourceAddressArgs);
    DXGKARG_SUBMITCOMMAND*  DdiSubmitCommand(DXGKARG_SUBMITCOMMAND* SubmitCommandArgs);
    DXGKARG_PREEMPTCOMMAND* DdiPreemptCommand(DXGKARG_PREEMPTCOMMAND* PremptCommandArgs);
    NTSTATUS                DdiQueryCurrentFence(DXGKARG_QUERYCURRENTFENCE* QueryCurrentFenceArgs);
    PVOID                   GetDisplayModeInfo(VOID); //FIXME: hmm seems like something passes in ecx here...
    NTSTATUS                IsPartOfDesktop(UINT32 DxgAdapter); //FIXME: Reverse me more
    BOOLEAN                 IsVidPnSourceOwner(DXGDEVICE* DxgDevice, UINT32 DxgAdapter); //FIXME: Reverse me
    PVOID                   GetCddInterface(UINT32 DxgAdapter); //FIXME: Reverse me














#if 0
    DXGADAPTER::InvalidateActiveVidPnDisplayModeListCache(uint)
    DXGADAPTER::CopyIcdInfo(void *,uint,ulong *,ulong *)
    DXGADAPTER::ReleaseReference(void)=
    DXGADAPTER::IsVidPnSourceOwner(DXGDEVICE const *)
    DXGADAPTER::SetDisplayModeInfo(uint,_DXGK_DISPLAYMODE_INFO const * const)
    DXGADAPTER::SetPartOfDesktop(uint,uchar)
    DXGADAPTER::SetLastCddIntegerVSync(uint,uint)
    DXGADAPTER::SetVidPnPrivateDriverFormatAttribute(uint,uint)
    DXGADAPTER::GetVidPnSourceOwnerType(uint)
    DXGADAPTER::AcquireReferenceSafe(void)
    DXGADAPTER::ControlVSyncForLogging(uchar)
    DXGADAPTER::SavePointerInfo(uint,uchar,int,int)
    DXGADAPTER::IsPrimaryVisible(uint)
    DXGADAPTER::MarkCommitVidPnOnModeChange(uint)
    DXGADAPTER::GetLastCddIntegerVSync(uint)
    DXGADAPTER::SignalVerticalBlankEvent(uint)
    DXGADAPTER::MapVidPnTargetToVidPnSource(uint)
    DXGADAPTER::GetCddPrimaryAllocation(uint)
    DXGADAPTER::CallDriverQueryInterface(_GUID const *,ushort,ushort,void *,void *)
    DXGADAPTER::AcquireOverlayId(DXGOVERLAY *)
    DXGADAPTER::ReleaseOverlayId(uint)
    DXGADAPTER::WaitForVerticalBlankEvent(uint)
    DXGADAPTER::UpdateGdiGammaRamp(uint,_D3DDDI_GAMMA_RAMP_RGB256x3x16 *)
    DXGADAPTER::GetEmulatedScanLine(uint,_DXGKARG_GETSCANLINE *)
    DXGADAPTER::DefaultDdiCreateOverlay(void * const,_DXGKARG_CREATEOVERLAY *)
    DXGADAPTER::DefaultDdiUpdateOverlay(void * const,_DXGKARG_UPDATEOVERLAY const *)
    DXGADAPTER::DefaultDdiFlipOverlay(void * const,_DXGKARG_FLIPOVERLAY const *)
    DXGADAPTER::DefaultDdiDestroyOverlay(void * const)
    DXGADAPTER::DefaultDdiAcquireSwizzlingRange(void * const,_DXGKARG_ACQUIRESWIZZLINGRANGE *)
    DXGADAPTER::DefaultDdiReleaseSwizzlingRange(void * const,_DXGKARG_RELEASESWIZZLINGRANGE const *)
    DXGADAPTER::DefaultDdiEscape(void * const,_DXGKARG_ESCAPE const *)
    DXGADAPTER::DefaultDdiStopCapture(void * const,_DXGKARG_STOPCAPTURE const *)
    DXGADAPTER::DefaultDdiRecommendVidPnTopology(void * const,_DXGKARG_RECOMMENDVIDPNTOPOLOGY const * const)
    DXGADAPTER::DefaultDdiSetDisplayPrivateDriverFormat(void * const,_DXGKARG_SETDISPLAYPRIVATEDRIVERFORMAT*)
    DXGADAPTER::GetCddPrimaryHandle(uint)
    DXGADAPTER::SetCddPrimary(uint,DXGSHAREDRESOURCE *,_CDDDXGK_INTERFACE const * const)
    DXGADAPTER::GetDisplayedPrimaryAllocation(uint)
    DXGADAPTER::IsWindowVisible(tagRECT const * const)
    DXGADAPTER::IsCoreSyncAcquired(DXGADAPTERCORESYNC_LEVEL)
    DXGADAPTER::ReleaseCoreSync(DXGADAPTERCORESYNC_LEVEL)
    DXGADAPTER::SuspendMemorySegments(void)
    DXGADAPTER::AcquireOpmI2CSync(void)
    DXGADAPTER::ReleaseOpmI2CSync(void)
    DXGADAPTER::IsOpmI2CMutexOwner(void)
    DXGADAPTER::ReportStatistics(uchar *,uint)
    DXGADAPTER::CollectDbgInfo(TDR_RECOVERY_CONTEXT *,void * *,uint *)
    DXGADAPTER::CopyUmdFileName(D3DKMT_UMDFILENAMEINFO *)
    DXGADAPTER::GetCachedModeList(D3DKMT_GETDISPLAYMODELIST *)
    DXGADAPTER::SetCachedModeList(D3DKMT_GETDISPLAYMODELIST *)
    DXGADAPTER::CreateModeList(D3DKMT_GETDISPLAYMODELIST *)
    DXGADAPTER::OpmCreateHandle(void *,void * *)
    DXGADAPTER::OpmValidateAdapterHandle(void *)
    DXGADAPTER::WaitForEventOrVerticalBlankEvent(uint,_KEVENT *)
    DXGADAPTER::SetGammaRamp(uint)
    DXGADAPTER::HasAnyVidPnSourceOwner(D3DKMT_VIDPNSOURCEOWNER_TYPE *,DXGDEVICECLIENT_TYPE *,uint *)
    DXGADAPTER::DdiQueryAdapterInfo(DXGKARG_QUERYADAPTERINFO const *)
    DXGADAPTER::DdiCreateDevice(DXGKARG_CREATEDEVICE *)
    DXGADAPTER::DdiCreateAllocation(DXGKARG_CREATEALLOCATION *)
    DXGADAPTER::DdiDescribeAllocation(DXGKARG_DESCRIBEALLOCATION *)
    DXGADAPTER::DdiGetStandardAllocationDriverData(DXGKARG_GETSTANDARDALLOCATIONDRIVERDA
    DXGADAPTER::DdiDestroyAllocation(DXGKARG_DESTROYALLOCATION const *)
    DXGADAPTER::DdiAcquireSwizzlingRange(DXGKARG_ACQUIRESWIZZLINGRANGE *)
    DXGADAPTER::DdiReleaseSwizzlingRange(DXGKARG_RELEASESWIZZLINGRANGE const *)
    DXGADAPTER::DdiPatch(DXGKARG_PATCH const *)
    DXGADAPTER::DdiCommitVidPn(DXGKARG_COMMITVIDPN const *)
    DXGADAPTER::DdiSetVidPnSourceVisibility(DXGKARG_SETVIDPNSOURCEVISIBILITY const *)
    DXGADAPTER::DdiUpdateActiveVidPnPresentPath(DXGKARG_UPDATEACTIVEVIDPNPRESENTPATH const *)
    DXGADAPTER::DdiControlInterrupt(DXGK_INTERRUPT_TYPE,uchar)
    DXGADAPTER::DdiBuildPagingBuffer(DXGKARG_BUILDPAGINGBUFFER *)
    DXGADAPTER::DdiSetPalette(DXGKARG_SETPALETTE const *)
    DXGADAPTER::DdiSetPointerShape(DXGKARG_SETPOINTERSHAPE const *)
    DXGADAPTER::DdiSetPointerPosition(DXGKARG_SETPOINTERPOSITION const *)
    DXGADAPTER::DdiResetFromTimeout(void)
    DXGADAPTER::DdiRestartFromTimeout(void)
    DXGADAPTER::DdiEscape(DXGKARG_ESCAPE const *)
    DXGADAPTER::DdiCollectDbgInfo(DXGKARG_COLLECTDBGINFO const *)
    DXGADAPTER::DdiRecommendFunctionalVidPn(DXGKARG_RECOMMENDFUNCTIONALVIDPN *)
    DXGADAPTER::DdiIsSupportedVidPn(DXGKARG_ISSUPPORTEDVIDPN *)
    DXGADAPTER::DdiEnumVidPnCofuncModality(DXGKARG_ENUMVIDPNCOFUNCMODALITY *)
    DXGADAPTER::DdiRecommendMonitorModes(DXGKARG_RECOMMENDMONITORMODES *)
    DXGADAPTER::DdiRecommendVidPnTopology(DXGKARG_RECOMMENDVIDPNTOPOLOGY *)
    DXGADAPTER::DdiGetScanLine(DXGKARG_GETSCANLINE *)
    DXGADAPTER::DdiStopCapture(DXGKARG_STOPCAPTURE *)
    DXGADAPTER::DdiCreateOverlay(_DXGKARG_CREATEOVERLAY *)
    DXGADAPTER::DdiDestroyDevice(void *)
    DXGADAPTER::DdiOpenAllocation(void *,_DXGKARG_OPENALLOCATION const *)
    DXGADAPTER::DdiCloseAllocation(void *,_DXGKARG_CLOSEALLOCATION const *)
    DXGADAPTER::DdiRender(void *,_DXGKARG_RENDER *)
    DXGADAPTER::DdiPresent(void *,_DXGKARG_PRESENT *)
    DXGADAPTER::DdiUpdateOverlay(void *,_DXGKARG_UPDATEOVERLAY *)
    DXGADAPTER::DdiFlipOverlay(void *,_DXGKARG_FLIPOVERLAY *)
    DXGADAPTER::DdiDestroyOverlay(void *)
    DXGADAPTER::DdiCreateContext(void *,_DXGKARG_CREATECONTEXT *)
    DXGADAPTER::DdiDestroyContext(void *)
    DXGADAPTER::DdiSetDisplayPrivateDriverFormat(_DXGKARG_SETDISPLAYPRIVATEDRIVERFORMAT *)
    DXGADAPTER::CreateHandle(uint *)
    DXGADAPTER::DestroyHandle(uint)
    DXGADAPTER::NeedToResetCdd(uchar const *)
    DXGADAPTER::CalculateCddStates(uchar *)
    DXGADAPTER::Destroy(void)
    DXGADAPTER::CreateDevice(DXGDEVICE * *,DXGDEVICECLIENT_TYPE,uchar)
    DXGADAPTER::FindCopyProtection(uint,uint,DXGPROCESS *)
    DXGADAPTER::CalculateApsTrigger(uint)
    DXGADAPTER::PrepareToReset(_TDR_RECOVERY_CONTEXT *)
    DXGADAPTER::DisablePinnedHardware(void)
    DXGADAPTER::ReportState(void)
    DXGADAPTER::DisableOverlays(void)
    DXGADAPTER::OpmTranslateHandle(void *,void * *)
    DXGADAPTER::OpmTranslateAndDestroyHandle(void *,void * *)
    DXGADAPTER::OpmReset(void)
    DXGADAPTER::UpdateOneCddPrimaryPrivateDriverData(uint)
    DXGADAPTER::IsSameProcessAsVidPnSourceOwner(uint,DXGPROCESS const *)
    DXGADAPTER::UpdateModeWithPrimary(DXGADAPTERALLOCATION const *)	PAGE
    DXGADAPTER::CommitVidPn(D3DKMDT_HVIDPN__ * const,uint,DXGADAPTERALLOCATION const *,_DMM_COMMITVIDPNREQUEST_DIAGINFO const * const,uchar *)
    DXGADAPTER::DisableAllPrimaries(void)
    DXGADAPTER::DestroyDevice(DXGDEVICE *)
    DXGADAPTER::SetCopyProtection(uint)
    DXGADAPTER::Reset(_TDR_RECOVERY_CONTEXT *)
    DXGADAPTER::UpdateAllCddPrimariesPrivateDriverData(void)
    DXGADAPTER::ReleaseVidPnSourceOwner(DXGDEVICE *)
    DXGADAPTER::ReleaseAllVidPnSourceOwners(void)
    DXGADAPTER::DisableAllDisplayMode(void)
    DXGADAPTER::AcquireCoreSync(DXGADAPTERCORESYNC_LEVEL)
    DXGADAPTER::CreateCopyProtection(uint,uint,uint *)
    DXGADAPTER::UpdateCopyProtection(uint,uint,uint)
    DXGADAPTER::Stop(VOID);
    DXGADAPTER::AcquireVidPnSourceOwner(DXGDEVICE *,_D3DKMT_VIDPNSOURCEOWNER_TYPE const *,uint const *,uint,uchar *,uchar *)
#endif
private:
};