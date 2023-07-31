#include <win32k.h>

#include <debug.h>

PEPROCESS GlobalDwmProcess;
PVOID GlobalDwmApiPort;

/* 
 * NtUserDwmGetDxRgn
 * @ UNIMPLEMENTED
 * UNDOCUMENTED
 */
UINT32
APIENTRY
NtUserDwmGetDxRgn(PVOID x1, PVOID x2, PVOID x3)
{
    DPRINT1("NtUserDwmGetDxRgn: Has been called - NO idea what's this for\n");
    DPRINT1("Please stop and recoup in the ReactOS Longhorn discord!\n");
    DPRINT1("Param 1: 0x%X\nParam 2: 0x%X\nParam 3: 0x%x\n", x1, x2 ,x3);
    __debugbreak();
    return 0;
}

/*
 * NtUserDwmGetDxRgn
 * @ UNIMPLEMENTED
 * UNDOCUMENTED
 */
UINT32
APIENTRY
NtUserDwmHintDxUpdate(PVOID x1, PVOID x2)
{
    DPRINT1("NtUserDwmHintDxUpdate: Has been called - NO idea what's this for\n");
    DPRINT1("Please stop and recoup in the ReactOS Longhorn discord!\n");
    DPRINT1("Param 1: 0x%X\nParam 2: 0x%X\n", x1, x2);
    __debugbreak();
    return 0;
}

/*
 * Ugh the history of this function is fruturating.
 * Acording to PDBs:
 * -> Longhorn 5048: DwmStartup
 * -> Vista RTM:     DwmStartRedirection
 *
 * @ UNIMPLEMENTED
 * UNDOCUMENTED
 * HOWEVER, instead of uDWM triggering the start it's done by dwmapi? i guess?
 *
 * Param 1 i just made match 5048 for now, i don't think it's right.
 */
BOOLEAN
APIENTRY
NtUserDwmStartRedirection(HANDLE Handle)
{
    NTSTATUS Status;
    BOOLEAN IsRedirectionEnabled;

    DPRINT1("NtUserDwmStartRedirection: Has been called\n");
    GlobalDwmProcess = PsGetCurrentProcess();
    IsRedirectionEnabled = TRUE;


    /* Some kind of internal call here? */
    Status = 0;//IntDwmStartRedirection(Handle)

    if (Status != STATUS_SUCCESS)
    {
        IsRedirectionEnabled = FALSE;
        DPRINT1("NtUserDwmStartRedirection: Failed to enable redirection Status: 0x%X\n", Status);
    }

    return IsRedirectionEnabled;
}

/*
 * Acording to PDBs:
 * -> Longhorn 5048: DwmStop
 * -> Vista RTM:     DwmStopRedirection
 *
 * @ UNIMPLEMENTED
 * UNDOCUMENTED
 */
BOOLEAN
APIENTRY
NtUserDwmStopRedirection(VOID)
{
    NTSTATUS Status;
    BOOLEAN IsRedirectionStopped;

    DPRINT1("NtUserDwmStopRedirection: Has been called\n");
    IsRedirectionStopped = TRUE;


    /* Some kind of internal call here? */
    Status = 1;//IntDwmStopRedirection()

    if (Status != STATUS_SUCCESS)
    {
        IsRedirectionStopped = FALSE;
        DPRINT1("NtUserDwmStopRedirection: Failed to enable redirection Status: 0x%X\n", Status);
    }

    return IsRedirectionStopped;
}
/*
 *  Setup and connect the global DWM port
 *  @ UNIMPLEMENTED
 */
UINT32
APIENTRY
NtUserRegisterSessionPort(HANDLE Handle)
{
    NTSTATUS Status;

    Status = 0;
    if (GlobalDwmProcess)
    {
        DPRINT1("Already Initialized Session Port\n");
        return STATUS_ALREADY_INITIALIZED;
    }

    Status = ObReferenceObjectByHandle(Handle, 1, LpcPortObjectType, 1, &Handle, NULL);
    if (Status != STATUS_SUCCESS)
    {
        DPRINT1("NtUserRegisterSessionPort: ObReferenceObjectByHandle failed with Status: %d\n", Status);
        return Status;
    }
    GlobalDwmApiPort = (PVOID)Handle;
    GlobalDwmProcess = PsGetCurrentProcess();

    Status = ObReferenceObjectByPointer(GlobalDwmProcess, 0x1FFFFFu, (POBJECT_TYPE)PsProcessType, 0);
    if (Status != STATUS_SUCCESS)
    {
        DPRINT1("NtUserRegisterSessionPort: ObReferenceObjectByHandle failed with Status: %d\n", Status);
        return Status;
    }
    __debugbreak();
    return Status;
}