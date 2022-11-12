#include <stdarg.h>

#define WIN32_NO_STATUS

#include <windef.h>
#include <winbase.h>
#include <winreg.h>
#include <winuser.h>
#include <winwlx.h>
#include <ndk/rtltypes.h>
#include <ndk/umfuncs.h>
#include <ndk/ldrfuncs.h>
#define NDEBUG
#include <debug.h>

VOID
RtlpInitializeKeyedEvent(VOID);

VOID
RtlpCloseKeyedEvent(VOID);

#define DONT_RESOLVE_DLL_REFERENCES         0x00000001
#define LOAD_LIBRARY_AS_DATAFILE            0x00000002
 #define LOAD_WITH_ALTERED_SEARCH_PATH       0x00000008
 #define LOAD_IGNORE_CODE_AUTHZ_LEVEL        0x00000010
#define LOAD_LIBRARY_AS_IMAGE_RESOURCE      0x00000020
 #define LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE  0x00000040
 #define LOAD_LIBRARY_REQUIRE_SIGNED_TARGET  0x00000080
#define LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR    0x00000100
#define LOAD_LIBRARY_SEARCH_APPLICATION_DIR 0x00000200
#define LOAD_LIBRARY_SEARCH_USER_DIRS       0x00000400
 #define LOAD_LIBRARY_SEARCH_SYSTEM32        0x00000800
 #define LOAD_LIBRARY_SEARCH_DEFAULT_DIRS    0x00001000

BOOL
WINAPI
DllMain(HANDLE hDll,
        DWORD dwReason,
        LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        LdrDisableThreadCalloutsForDll(hDll);
        RtlpInitializeKeyedEvent();
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        RtlpCloseKeyedEvent();
    }
    return TRUE;
}
static DWORD default_search_flags;  /* default flags set by LdrSetDefaultDllDirectories */

NTSTATUS
NTAPI 
LdrSetDefaultDllDirectories( ULONG flags )
{
   /* LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR doesn't make sense in default dirs */
 const ULONG load_library_search_flags = (LOAD_LIBRARY_SEARCH_APPLICATION_DIR |
                                             LOAD_LIBRARY_SEARCH_USER_DIRS |
                                            LOAD_LIBRARY_SEARCH_SYSTEM32 |
                                            LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
        if (!flags || (flags & ~load_library_search_flags)) return STATUS_INVALID_PARAMETER;
    default_search_flags = flags;
  return STATUS_SUCCESS;
 }