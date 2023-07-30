
#include <win32k.h>
#include <debug.h>
PEPROCESS GlobalDwmProcessPtr;
HANDLE GlobalDwmApiPort;

NTSTATUS
InternalDwmStartup()
{
    DPRINT1("Starting DWM Internal\n");
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