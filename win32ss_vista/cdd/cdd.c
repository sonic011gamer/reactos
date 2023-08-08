
#include "cdd.h"
#include <debug.h>

VOID APIENTRY
DrvDisableSurface(
   IN DHPDEV dhpdev)
{
    UNIMPLEMENTED;
    __debugbreak();
}

HSURF APIENTRY
DrvEnableSurface(
   IN DHPDEV dhpdev)
{
   UNIMPLEMENTED;
    __debugbreak();
    return NULL;
}

VOID APIENTRY
DrvMovePointer(
   IN SURFOBJ *pso,
   IN LONG x,
   IN LONG y,
   IN RECTL *prcl)
{
   UNIMPLEMENTED;
   __debugbreak();
}

BOOL
APIENTRY
DrvIcmSetDeviceGammaRamp(DHPDEV  dhpdev,
                        ULONG   iFormat,
                        LPVOID  lpRamp)
{
   return 0;
}

ULONG APIENTRY
DrvSetPointerShape(
   IN SURFOBJ *pso,
   IN SURFOBJ *psoMask,
   IN SURFOBJ *psoColor,
   IN XLATEOBJ *pxlo,
   IN LONG xHot,
   IN LONG yHot,
   IN LONG x,
   IN LONG y,
   IN RECTL *prcl,
   IN FLONG fl)
{
   UNIMPLEMENTED;
   __debugbreak();
   return EngSetPointerShape(pso, psoMask, psoColor, pxlo, xHot, yHot, x, y, prcl, fl);
}

ULONG
APIENTRY
DrvGetModes(_In_ HANDLE hDriver,
            _In_ ULONG cjSize,
            _Out_ DEVMODEW *pdm)
{
   UNIMPLEMENTED;
   __debugbreak();
   return 1;
}


BOOL
APIENTRY
DrvSetPalette(
   IN DHPDEV dhpdev,
   IN PALOBJ *ppalo,
   IN FLONG fl,
   IN ULONG iStart,
   IN ULONG cColors)
{
       UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

DHPDEV
APIENTRY
DrvEnablePDEV(
   IN DEVMODEW *pdm,
   IN LPWSTR pwszLogAddress,
   IN ULONG cPat,
   OUT HSURF *phsurfPatterns,
   IN ULONG cjCaps,
   OUT ULONG *pdevcaps,
   IN ULONG cjDevInfo,
   OUT DEVINFO *pdi,
   IN HDEV hdev,
   IN LPWSTR pwszDeviceName,
   IN HANDLE hDriver)
{
   UNIMPLEMENTED;
    __debugbreak();
   return NULL;
}


/*
 * @implemented
 */
VOID APIENTRY
DrvCompletePDEV(
   IN DHPDEV dhpdev,
   IN HDEV hdev)
{
   ((PPDEV)dhpdev)->hDevEng = hdev;
}

/*
 * @implemented
 */
VOID APIENTRY
DrvDisablePDEV(
   IN DHPDEV dhpdev)
{
   if (((PPDEV)dhpdev)->DefaultPalette)
   {
      EngDeletePalette(((PPDEV)dhpdev)->DefaultPalette);
   }

   if (((PPDEV)dhpdev)->PaletteEntries != NULL)
   {
      EngFreeMem(((PPDEV)dhpdev)->PaletteEntries);
   }

   EngFreeMem(dhpdev);
}
