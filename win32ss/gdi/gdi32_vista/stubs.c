#define WIN32_NO_STATUS
#define _INC_WINDOWS
#define COM_NO_WINDOWS_H
#define NTOS_MODE_USER
#include <stdarg.h>
#include <windef.h>
#include <winbase.h>
#include <ndk/rtlfuncs.h>
#include <wingdi.h>
#include <winddi.h>
#include <prntfont.h>
#include <ntgdityp.h>
#include <ntgdi.h>

#include <debug.h>

BOOL
WINAPI
GetFontFileData(DWORD instance_id,
                DWORD unknown,
                UINT64 offset,
                PVOID buff,
                DWORD buff_size)
{
    return TRUE;
}

BOOL
WINAPI
GetFontFileInfo(DWORD instance_id,
                DWORD unknown,
                PVOID *info,
                SIZE_T size,
                SIZE_T *needed)
{
    return TRUE;
}

BOOL
WINAPI
GetFontRealizationInfo(HDC hdc,
                       PVOID *info)
{
    return TRUE;
}

NTSTATUS
WINAPI
D3DKMTCloseAdapter(const PVOID *Arg1) // D3DKMT_CLOSEADAPTER
{
    //UNIMPLEMENTED;
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
WINAPI
D3DKMTGetPresentHistory(PVOID *Arg1) // D3DKMT_GETPRESENTHISTORY
{
    //UNIMPLEMENTED;
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
WINAPI
D3DKMTGetPresentQueueEvent(PVOID hAdapter, // D3DKMT_HANDLE
                           HANDLE *Handle)
{
    //UNIMPLEMENTED;
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
WINAPI
D3DKMTOpenAdapterFromGdiDisplayName(PVOID *Arg1) // D3DKMT_OPENADAPTERFROMGDIDISPLAYNAME
{
    //UNIMPLEMENTED;
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
WINAPI
D3DKMTOpenAdapterFromHdc(PVOID *Arg1) // D3DKMT_OPENADAPTERFROMHDC
{
    //UNIMPLEMENTED;
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
WINAPI
D3DKMTQueryAdapterInfo(const PVOID *Arg1) // D3DKMT_QUERYADAPTERINFO
{
    //UNIMPLEMENTED;
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
WINAPI
D3DKMTSetProcessSchedulingPriorityClass(HANDLE ProcessHandle,
                                        PVOID PriorityClass) // D3DKMT_SCHEDULINGPRIORITYCLASS
{
    //UNIMPLEMENTED;
    return STATUS_NOT_IMPLEMENTED;
}

VOID
WINAPI
CreateBitmapFromDxSurface(VOID)
{
    //UNIMPLEMENTED;
}

VOID
WINAPI
CreateBitmapFromDxSurface2(VOID)
{
    //UNIMPLEMENTED;
}

VOID
WINAPI
BeginGdiRendering(VOID)
{
    //UNIMPLEMENTED;
}

VOID
WINAPI
EndGdiRendering(VOID)
{
    //UNIMPLEMENTED;
}