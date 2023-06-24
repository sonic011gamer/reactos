Missing for win7 dlls:

User32:
CheckWindowThreadDesktop
VTagOutput
VRipOutput

NTDLL:
wcsncpy_s
wcsncay_s
sscanf_s
strcpy_s
wcscpy_s
wcscat_S
swprintf_s
_snprintf_s
strncpy_s
WinSqmIncrementDWORD
WinSqmAddToStreamEx

comctrl:
ordinal 381

ntoskrnl:
vsprintf_s
EtwRegisterClassProvider

Powrprof:
PowerSetActiveScheme
PowerWriteDCValueIndex
PowerWriteACValueIndex

cfgmgr32:
CM_MapCrToWin32Err

HAL:
x86BiosCall


Kernel32:
RegQueryValueExW
RegOpenKeyExW
RegEnumValueW
RegQueryInfoExW
RegSetValueExW
RegCloseKey
RegCreateKeyExW

user32:
GetWindowRgnEx
GetWindowMinimizeRect
SetWindowRgnEx
N/A - ordinal 1553
DwmStopRedirection
DwmStartRedirection
UnregisterSessionPort
RegisterSessionPort
CheckDesktopByThreadId

729 SfmDxReleaseSwapChain
731 SfmDxSetSwapChainBindingStatus
728 SfmDxQuerySwapChainBindingStatus
730- SfmDxReportPendingBindingsToDwm
732 SfmSetSwapChainStats
727 SfmDxOpenSwapChain

UXTheme:
GetThemeStream

gdi32:
GdiLoadType1Fonts
BeginGdiRendering
EndGdiRendering
CreateBitmapFromDxSurface
D3DKMTGetPresentQueueEvent
D3DKMTGetPresentHistory
D3DKMTSetprocessSchedulingPriorityClass

Ordinal 104 -> 11
MSCTF:
basically entire dll