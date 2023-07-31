#include <dwm.hpp>
#include <debug.h>

/*
 *
 */
UINT32
WINAPI
CDwmAppHost::Initialize(HINSTANCE hInstance)
{
    return 0;
}

HRESULT
WINAPI
CDwmAppHost::WaitForAndProcessEvent()
{
    return S_OK;
}

UINT32
WINAPI
CDwmAppHost::Run()
{
    MSG Msg;
    UINT ExitCode;
    HRESULT HResult;
    ExitCode = 0;
    DPRINT1("DWM: Starting the DWM service\n");
    while (1)
    {
        Msg.hwnd = 0;
        Msg.message = 0;
        Msg.wParam = 0;
        Msg.lParam = 0;
        Msg.time = 0;
        Msg.pt.x = 0;
        Msg.pt.y = 0;
        while (PeekMessageW(&Msg, 0, 0, 0, 1u) && Msg.message != 18)
        {
            TranslateMessage(&Msg);
            DispatchMessageW(&Msg);
        }
        if (Msg.message == 18)
            break;
        HResult = CDwmAppHost::WaitForAndProcessEvent();
        if (HResult != S_OK)
        {
            ExitCode = HResult;
            goto StartShutdown;
        }
    }
    // CDwmAppHost::OnClose();
StartShutdown:
    DPRINT1("Shutting down the DWM with return code: %d\n", ExitCode);
    return ExitCode;
}

/* On the event of Dwm Closing*/
VOID
WINAPI
CDwmAppHost::OnClose()
{
    //TODO: We should clean up all globals
}

/* Preparing to exit service */
VOID
WINAPI
CDwmAppHost::Cleanup()
{
    //TODO: We should clean up all globals
}
