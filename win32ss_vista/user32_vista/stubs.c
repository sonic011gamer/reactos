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
