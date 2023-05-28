#ifdef __cplusplus
extern "C" {
#endif

#include <ntdll.h>
#include <ndk/ldrtypes.h>
#include <psdk/winreg.h>
extern PCONTEXT LdrpProcessInitContextRecord;

//extern PLDR_DATA_TABLE_ENTRY LdrpLoadedDllHandleCache;
#ifdef __cplusplus
}
#endif
extern LDRP_DEBUG_FLAGS LdrpDebugFlags;

NTSTATUS
NTAPI
LdrpRunInitializeRoutine(IN PLDR_DATA_TABLE_ENTRY Module)
{
    NTSTATUS Status = STATUS_SUCCESS;

    const PPEB Peb = NtCurrentPeb();
    RTL_CALLER_ALLOCATED_ACTIVATION_CONTEXT_STACK_FRAME_EXTENDED ActCtx;
    ULONG BreakOnDllLoad;
    BOOLEAN DllStatus = TRUE;

    /* Are we being debugged? */
    BreakOnDllLoad = 0;
    if (Peb->BeingDebugged || Peb->ReadImageFileExecOptions)
    {
        /* Check if we should break on load */
        Status = LdrQueryImageFileExecutionOptions(&Module->BaseDllName,
                                                   L"BreakOnDllLoad",
                                                   REG_DWORD,
                                                   &BreakOnDllLoad,
                                                   sizeof(ULONG),
                                                   NULL);
        if (!NT_SUCCESS(Status))
            BreakOnDllLoad = 0;

        /* Reset status back to STATUS_SUCCESS */
        Status = STATUS_SUCCESS;
    }

    /* Break if asked */
    if (BreakOnDllLoad)
    {


        /* Break in debugger */
        DbgBreakPoint();
    }



    /* Save the old Dll Initializer and write the current one */
    const PLDR_DATA_TABLE_ENTRY OldInitializer = LdrpCurrentDllInitializer;
    LdrpCurrentDllInitializer = Module;

    /* Set up the Act Ctx */
    ActCtx.Size = sizeof(ActCtx);
    ActCtx.Format = 1;
    RtlZeroMemory(&ActCtx.Frame, sizeof(RTL_ACTIVATION_CONTEXT_STACK_FRAME));

    /* Activate the ActCtx */
    RtlActivateActivationContextUnsafeFast(&ActCtx,
                                           Module->EntryPointActivationContext);


        /* Check if it has TLS */
        if (Module->TlsIndex)
        {
            /* Call TLS */
            LdrpCallTlsInitializers(Module, DLL_PROCESS_ATTACH);
        }


        /* Make sure we have an entrypoint */
        if (Module->EntryPoint)
        {
            PCONTEXT Context = Module->ProcessStaticImport
                                         ? LdrpProcessInitContextRecord
                                         : NULL;



            DllStatus = LdrpCallInitRoutine((PDLL_INIT_ROUTINE)Module->EntryPoint,
                                            Module->DllBase,
                                            DLL_PROCESS_ATTACH,
                                            Context);
        }


    /* Deactivate the ActCtx */
    RtlDeactivateActivationContextUnsafeFast(&ActCtx);

    /* Save the Current DLL Initializer */
    LdrpCurrentDllInitializer = OldInitializer;

    /* Mark the entry as processed */
    if (Module->EntryPoint)
        Module->ProcessAttachCalled = TRUE;

    /* Fail if DLL init failed */
    if (!DllStatus)
    {


        if (LdrpDebugFlags.BreakInDebugger)
            __debugbreak();

        Module->ProcessAttachFailed = TRUE;
        Status = STATUS_DLL_INIT_FAILED;
    }

Quickie:
    /* Return to caller */
    return Status;
}
