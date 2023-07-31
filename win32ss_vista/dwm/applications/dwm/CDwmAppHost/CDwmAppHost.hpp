#pragma once

#include <dwm.hpp>

class CDwmAppHost {
public:
    UINT32  WINAPI Initialize(HINSTANCE hInstance);
    UINT32  WINAPI Run();
    HRESULT WINAPI WaitForAndProcessEvent();
    VOID    WINAPI Cleanup();
    VOID    WINAPI OnClose();

    BOOLEAN IsCompositionEnabled;
private:
};
