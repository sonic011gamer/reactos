
#include <win32k.h>
#include <debug.h>

/* Global Autism */
PEPROCESS GlobalDwmProcessPtr;
HANDLE GlobalDwmApiPort;
BOOLEAN IsCompositionOn;
extern HSEMAPHORE ghsemGraphicsDeviceList;

#define ALLOC_TAG	'MWD'

#ifndef LOBYTE
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))
#endif

BOOLEAN
InternalGraphicsDwmStartup(HDEV hsurf)
{
    EngAcquireSemaphore(ghsemGraphicsDeviceList);
    EngAcquireSemaphore(*((HSEMAPHORE *)hsurf + 10));

    EngReleaseSemaphore(ghsemGraphicsDeviceList);
    EngReleaseSemaphore(*((HSEMAPHORE *)hsurf + 10));
    return TRUE;
}

NTSTATUS
InternalDwmStartup()
{
    DPRINT1("Starting DWM Internal\n");
    if (IsCompositionOn)
        return STATUS_ALREADY_INITIALIZED;
    if (InternalGraphicsDwmStartup(*(HDEV *)gpPrimaryGraphicsDevice))
    {
      IsCompositionOn = 1;
    //  StartCompositionOfLeDesktop(ptiRawInput, 1);
    }
    else
    {
        return STATUS_UNSUCCESSFUL;
    }

    __debugbreak();
    return 0;
}


BOOLEAN
NTAPI
NtUserDwmStartup(HANDLE Handle)
{
    NTSTATUS Status;
    if ( GlobalDwmProcessPtr )
        return TRUE;

    /* Set the state and reference the handle */
    Status = ObReferenceObjectByHandle(Handle, 0, LpcPortObjectType, 1, &Handle, 0);
    GlobalDwmApiPort = Handle;

    if (Status == STATUS_SUCCESS)
    {
        GlobalDwmProcessPtr = PsGetCurrentProcess();
        Status = InternalDwmStartup();

        if (Status == STATUS_SUCCESS)
            return TRUE;

        /* We failed, let's clean up */
        if (GlobalDwmApiPort)
          ObfDereferenceObject(GlobalDwmApiPort);

        GlobalDwmApiPort = 0;
        GlobalDwmProcessPtr = (PEPROCESS)NULL;
    }

    /* Dwm is not on :( */
    return FALSE;
}