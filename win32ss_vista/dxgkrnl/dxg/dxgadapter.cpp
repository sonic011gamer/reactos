
#include <include/dxgkrnl.h>
#include <debug.h>


DXGADAPTER::DXGADAPTER()
{

}

DXGADAPTER::~DXGADAPTER()
{

}

NTSTATUS
DXGADAPTER::Initialize(PDEVICE_OBJECT AdapterDeviceObject)
{
    if (!AdapterDeviceObject)
        return STATUS_INVALID_PARAMETER;

    UNIMPLEMENTED;
    __debugbreak();


    /*
     * What a chonker.
     * A BUNCH of classes are initialized here it seems
     * let's go over what's important.
     *
     * 1)
     */
    return STATUS_UNSUCCESSFUL;
}


BOOLEAN
DXGADAPTER::IsOverlayEnabled(VOID)
{
    return 0;
}

NTSTATUS
DXGADAPTER::FlushScheduler(BOOLEAN EnableFlush)
{
    return 0;
}

VOID
DXGADAPTER::UpdateVidPnTargetList(VOID)
{

}

VOID
DXGADAPTER::UpdateOrientation(UINT32 UndecidedName1)
{

}

BOOLEAN
DXGADAPTER::AcquireDdiSync(UINT32 AdapterDdiSynchLevel)
{
    return 0;
}

VOID
DXGADAPTER::ReleaseDdiSync(VOID)
{

}

VOID
DXGADAPTER::AcquireReference(VOID)
{

}

VOID
DXGADAPTER::AcquireCoreResourceShared(VOID)
{

}

VOID
DXGADAPTER::AcquireCoreResourceExclusive(VOID)
{

}

VOID
DXGADAPTER::PurgeSegments(BOOLEAN PreserveContent)
{

}

VOID
DXGADAPTER::RestoreFromPurgeSegments(VOID)
{

}

NTSTATUS
DXGADAPTER::DdiSetVidPnSourceAddress(DXGKARG_SETVIDPNSOURCEADDRESS* SetVidPnSourceAddressArgs)
{
    return 0;
}

DXGKARG_SUBMITCOMMAND*
DXGADAPTER::DdiSubmitCommand(DXGKARG_SUBMITCOMMAND* SubmitCommandArgs)
{
    return SubmitCommandArgs;
}

DXGKARG_PREEMPTCOMMAND*
DXGADAPTER::DdiPreemptCommand(DXGKARG_PREEMPTCOMMAND* PremptCommandArgs)
{
    return PremptCommandArgs;
}

NTSTATUS
DXGADAPTER::DdiQueryCurrentFence(DXGKARG_QUERYCURRENTFENCE* QueryCurrentFenceArgs)
{
    return 0;
}

PVOID
DXGADAPTER::GetDisplayModeInfo(VOID)
{
 return NULL;
}

NTSTATUS
DXGADAPTER::IsPartOfDesktop(UINT32 DxgAdapter)
{
    return 0;
}

BOOLEAN
DXGADAPTER::IsVidPnSourceOwner(DXGDEVICE* DxgDevice, UINT32 DxgAdapter)
{
    return 0;
}

PVOID
DXGADAPTER::GetCddInterface(UINT32 DxgAdapter)
{
 return NULL;
}
