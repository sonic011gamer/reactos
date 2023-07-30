#include <user32.h>
#include <ndk/cmfuncs.h>
#include <strsafe.h>
#include <debug.h>

BOOLEAN
NTAPI
NtUserDwmStartup(HANDLE Handle);

BOOL
WINAPI
DwmStartup(PVOID hdev)
{
    DPRINT1("DwmStartup Has been Called with HDEV addr of %X\n", hdev);
    return NtUserDwmStartup(hdev);
}

BOOL
WINAPI
DwmShutdown(PVOID hdev)
{
    DPRINT1("DwmShutdown Has been Called with HDEV addr of %X\n", hdev);
    __debugbreak();
    return FALSE;
}

NTSTATUS
WINAPI
UpdateWindowTransform(int a1, int a2, PVOID P)
{
    DPRINT1("NtUserUpdateWindowTransform Has been Called");
    __debugbreak();
    return STATUS_SUCCESS;
}

HRGN
WINAPI
SetWindowRgnEx(long x1, HRGN x2, long x3)
{
    DPRINT1("SetWindowRgnEx Has been Called");
    __debugbreak();
    return (HRGN)NULL;
}
