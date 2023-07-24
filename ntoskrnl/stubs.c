 
// Vista+ stubs 

#include <ntoskrnl.h>

// io
_IRQL_requires_max_(PASSIVE_LEVEL)
_Must_inspect_result_
NTSTATUS
NTAPI
IoSetDevicePropertyData(
    _In_ PDEVICE_OBJECT Pdo,
    _In_ CONST DEVPROPKEY *PropertyKey,
    _In_ LCID Lcid,
    _In_ ULONG Flags,
    _In_ DEVPROPTYPE Type,
    _In_ ULONG Size,
    _In_opt_ PVOID Data)
{
    return STATUS_NOT_IMPLEMENTED;
}

_IRQL_requires_max_(PASSIVE_LEVEL)
_Must_inspect_result_
NTSTATUS
NTAPI
IoGetDevicePropertyData(
    _In_ PDEVICE_OBJECT Pdo,
    _In_ CONST DEVPROPKEY *PropertyKey,
    _In_ LCID Lcid,
    _Reserved_ ULONG Flags,
    _In_ ULONG Size,
    _Out_ PVOID Data,
    _Out_ PULONG RequiredSize,
    _Out_ PDEVPROPTYPE Type)
{
    return STATUS_NOT_IMPLEMENTED;
}

_IRQL_requires_max_(PASSIVE_LEVEL)
_Must_inspect_result_
NTSTATUS
IoSetDeviceInterfacePropertyData(
    _In_ PUNICODE_STRING SymbolicLinkName,
    _In_ CONST DEVPROPKEY *PropertyKey,
    _In_ LCID Lcid,
    _In_ ULONG Flags,
    _In_ DEVPROPTYPE Type,
    _In_ ULONG Size,
    _In_reads_bytes_opt_(Size) PVOID Data)
{
    return STATUS_NOT_IMPLEMENTED;
}

IO_PRIORITY_HINT
NTAPI
IoGetIoPriorityHint(
    _In_ PIRP Irp)
{
    return IoPriorityNormal;
}

//ke
ULONG
NTAPI
KeQueryActiveProcessorCount(OUT PKAFFINITY ActiveProcessors OPTIONAL)
{
    RTL_BITMAP Bitmap;
    KAFFINITY ActiveMap = KeQueryActiveProcessors();

    if (ActiveProcessors != NULL)
    {
        *ActiveProcessors = ActiveMap;
    }

    RtlInitializeBitMap(&Bitmap, (PULONG)&ActiveMap,  sizeof(ActiveMap) * 8);
    return RtlNumberOfSetBits(&Bitmap);
}

USHORT
NTAPI
KeQueryHighestNodeNumber()
{
	return 0;
}

USHORT
NTAPI
KeGetCurrentNodeNumber()
{
	return 0;
}

//po
NTSTATUS
NTAPI
PoRegisterPowerSettingCallback(
    _In_opt_ PDEVICE_OBJECT DeviceObject,
    _In_ LPCGUID SettingGuid,
    _In_ PPOWER_SETTING_CALLBACK Callback,
    _In_opt_ PVOID Context,
    _Outptr_opt_ PVOID *Handle)
{
    return STATUS_NOT_IMPLEMENTED;
}

_IRQL_requires_max_(APC_LEVEL)
NTSTATUS
NTAPI
PoUnregisterPowerSettingCallback(
    _Inout_ PVOID Handle)
{
    return STATUS_NOT_IMPLEMENTED;
}

_IRQL_requires_max_(DISPATCH_LEVEL)
BOOLEAN
NTAPI
PoQueryWatchdogTime(
    _In_ PDEVICE_OBJECT Pdo,
    _Out_ PULONG SecondsRemaining)
{
    return FALSE;
}

_IRQL_requires_max_(DISPATCH_LEVEL)
VOID
NTAPI
PoSetSystemWake(
    _Inout_ struct _IRP *Irp)
{

}

_IRQL_requires_max_(DISPATCH_LEVEL)
BOOLEAN
NTAPI
PoGetSystemWake(
    _In_ struct _IRP *Irp)
{
    return FALSE;
}
