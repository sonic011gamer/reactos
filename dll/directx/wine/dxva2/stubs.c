#define WIN32_NO_STATUS
#define _INC_WINDOWS
#define COM_NO_WINDOWS_H
#include <windef.h>

#define NDEBUG
#include <reactos/debug.h>

HRESULT DXVA2CreateDirect3DDeviceManager9(
  UINT                    *pResetToken,
  PVOID **ppDeviceManager
)
{
    UNIMPLEMENTED;
    return 0;
}

HRESULT DXVA2CreateVideoService(
  ULONG_PTR *pDD,
  REFIID           riid,
  void             **ppService
)
{
  return 0;
}
