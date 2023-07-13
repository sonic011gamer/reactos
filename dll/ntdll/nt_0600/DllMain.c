#include <stdarg.h>

#define WIN32_NO_STATUS

#include <windef.h>
#include <winbase.h>
#include <winreg.h>
#include <winuser.h>
#include <winwlx.h>
#include <ndk/rtltypes.h>
#include <ndk/umfuncs.h>

#define NDEBUG
#include <debug.h>

VOID
RtlpInitializeKeyedEvent(VOID);

VOID
RtlpCloseKeyedEvent(VOID);
