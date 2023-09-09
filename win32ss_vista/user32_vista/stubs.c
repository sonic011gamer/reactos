#include "include/user32_vista.h"
#include <debug.h>

/*
 * @unimplemented
 */
BOOL
WINAPI
SetProcessDPIAware()
{
    return TRUE;
}

/*
 * @unimplemented
 */
BOOL
WINAPI
LogicalToPhysicalPoint( HWND hWnd, LPPOINT lpPoint )
{
    UNIMPLEMENTED;
    return TRUE;
}

/*
 * @unimplemented
 */
BOOL
WINAPI
ChangeWindowMessageFilter(
    UINT  message,
    DWORD dwFlag)
{
    UNIMPLEMENTED;
    return TRUE;
}

/*
 * @unimplemented
 */
BOOL WINAPI
ChangeWindowMessageFilterEx(HWND hwnd,
                            UINT message,
                            DWORD action,
                            PVOID pChangeFilterStruct)
{
    UNIMPLEMENTED;
    return TRUE;
}

BOOL
WINAPI
ShutdownBlockReasonCreate(HWND hWnd, LPCWSTR pwszReason)
{
    UNIMPLEMENTED;
    return 1;
}

BOOL
WINAPI
ShutdownBlockReasonQuery( HWND   hWnd,
  LPWSTR pwszBuff,
  DWORD  *pcchBuff)
{
    UNIMPLEMENTED;
    return 1;
}

BOOL
WINAPI
ShutdownBlockReasonDestroy(HWND hWnd)
{
    UNIMPLEMENTED;
    return 1;
}

BOOL
WINAPI
CalculatePopupWindowPosition(const POINT *anchorPoint,
                             const SIZE *windowSize,
                             UINT flags,
                             RECT *excludeRect,
                             RECT *popupWindowPosition)
{
    UNIMPLEMENTED;
    return TRUE;
}

BOOL
WINAPI
DwmGetDxSharedSurface(HWND hwnd,
                      HANDLE* phSurface,
                      LUID* pAdapterLuid,
                      ULONG* pFmtWindow,
                      ULONG* pPresentFlags,
                      ULONGLONG* pWin32kUpdateId)
{
    UNIMPLEMENTED;
    return TRUE;
}

HWND
WINAPI
GhostWindowFromHungWindow(HWND hwndGhost)
{
    UNIMPLEMENTED;
    return NULL;
}

HWND
WINAPI
HungWindowFromGhostWindow(HWND hwndGhost)
{
    UNIMPLEMENTED;
    return NULL;
}

BOOL
WINAPI
IsProcessDPIAware(VOID)
{
    UNIMPLEMENTED;
    return TRUE;
}

BOOL
WINAPI
IsTouchWindow(HWND hwnd,
              PULONG pulFlags)
{
    UNIMPLEMENTED;
    return TRUE;
}

BOOL
WINAPI
IsThreadDesktopComposited(VOID)
{
    UNIMPLEMENTED;
    return TRUE;
} 

BOOL
WINAPI
IsTopLevelWindow(IN HWND hWnd)
{
    UNIMPLEMENTED;
    return TRUE;
}

BOOL
WINAPI
IsWindowRedirectedForPrint(IN HWND hWnd)
{
    UNIMPLEMENTED;
    return TRUE;
}

BOOL
WINAPI
GetWindowCompositionAttribute(HWND hwnd,
                              PVOID pAttrData) // WINCOMPATTRDATA
{
    UNIMPLEMENTED;
    return TRUE;
}

HICON
WINAPI
InternalGetWindowIcon(HWND hwnd, UINT iconType)
{
    UNIMPLEMENTED;
    return NULL;
}

BOOL
WINAPI
RegisterTouchWindow(HWND  hwnd,
                    ULONG ulFlags)
{
    UNIMPLEMENTED;
    return TRUE;
}

PVOID // HPOWERNOTIFY
WINAPI
RegisterPowerSettingNotification(HANDLE hRecipient,
                                 LPCGUID PowerSettingGuid,
                                 DWORD Flags)
{
    UNIMPLEMENTED;
    return NULL;
}

BOOL
WINAPI
SetWindowCompositionAttribute(HWND hwnd,
                              PVOID pAttrData) // WINCOMPATTRDATA
{
    UNIMPLEMENTED;
    return TRUE;
}

BOOL
WINAPI
UnregisterPowerSettingNotification(HANDLE Handle) // HPOWERNOTIFY
{
    UNIMPLEMENTED;
    return TRUE;
}

LONG 
WINAPI
DisplayConfigGetDeviceInfo(
  PVOID *requestPacket
)
{
    return 0;
}

BOOL 
WINAPI
CloseTouchInputHandle(
  PVOID hTouchInput
)
{
    return 1;
}

BOOL
WINAPI
AddClipboardFormatListener(
    HWND hwnd)
{
    return 0;
}

BOOL
WINAPI
RemoveClipboardFormatListener(
    HWND hwnd)
{
    return 0;
}

BOOL
WINAPI
GetProcessDpiAwarenessInternal(HANDLE handle,DPI_AWARENESS* dpi )
{
    return FALSE;
}
BOOL
WINAPI
SetProcessDpiAwarenessInternal(DPI_AWARENESS dpi)
{
    return FALSE;
}
BOOL
WINAPI
GetDpiForMonitorInternal(HMONITOR monitor ,UINT one,UINT* two,UINT* three)
{
    return FALSE;
}

UINT32
WINAPI
DwmGetDxRgn(PVOID x1, PVOID x2, PVOID x3)
{
    DPRINT1("DwmGetDxRgn Entry\n");
    __debugbreak();
    return 0;
}

/*
 * NtUserDwmGetDxRgn
 * @ UNIMPLEMENTED
 * UNDOCUMENTED
 */
UINT32
WINAPI
DwmHintDxUpdate(PVOID x1, PVOID x2)
{
    DPRINT1("DwmHintDxUpdate Entry\n");
    __debugbreak();
    return 0;
}

BOOLEAN
APIENTRY
NtUserDwmStartRedirection(HANDLE Handle);

BOOLEAN
WINAPI
DwmStartRedirection(HANDLE Handle)
{
    return NtUserDwmStartRedirection(Handle);
}

BOOLEAN
WINAPI
DwmStopRedirection(VOID)
{
    DPRINT1("It's been requested to kill DWM, Forcing success\n");
    return TRUE;
}

UINT32
APIENTRY
NtUserRegisterSessionPort(HANDLE Handle);

NTSTATUS
WINAPI
RegisterSessionPort(UINT32 x1)
{

    return 1;
   // return NtUserRegisterSessionPort((HANDLE)x1);
}

NTSTATUS
WINAPI
UnregisterSessionPort()
{
    DPRINT1("UnregisterSessionPort\n");
    __debugbreak();
    return STATUS_SUCCESS;
}