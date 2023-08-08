

#ifndef _CDD_PCH_
#define _CDD_PCH_

#include <stdarg.h>
#include <windef.h>
#include <wingdi.h>
#include <winddi.h>
#include <winioctl.h>
#include <ntddvdeo.h>
//#include "../include/locd3d.h"

typedef struct _PDEV
{
   HANDLE hDriver;
   HDEV hDevEng;
   HSURF hSurfEng;
   ULONG ModeIndex;
   ULONG ScreenWidth;
   ULONG ScreenHeight;
   ULONG ScreenDelta;
   BYTE BitsPerPixel;
   ULONG RedMask; //TODO: Grab me from WDDM
   ULONG GreenMask; //TODO: Grab me from WDDM
   ULONG BlueMask; //TODO: Grab me from WDDM
   BYTE PaletteShift;
   PVOID ScreenPtr;
   HPALETTE DefaultPalette;
   PALETTEENTRY *PaletteEntries;

#ifdef EXPERIMENTAL_MOUSE_CURSOR_SUPPORT
   VIDEO_POINTER_ATTRIBUTES PointerAttributes;
   XLATEOBJ *PointerXlateObject;
   HSURF PointerColorSurface;
   HSURF PointerMaskSurface;
   HSURF PointerSaveSurface;
   POINTL PointerHotSpot;
#endif

   /* DirectX Support */
   DWORD iDitherFormat;
   ULONG MemHeight;
   ULONG MemWidth;
   DWORD dwHeap;
   VIDEOMEMORY* pvmList;
   BOOL bDDInitialized;
   DDPIXELFORMAT ddpfDisplay;
} PDEV, *PPDEV;

#define DEVICE_NAME	L"cdd"
#define ALLOC_TAG	'DDC'


BOOL APIENTRY
DrvEnableDirectDraw(
    DHPDEV dhpdev,
    DD_CALLBACKS *pCallbacks,
    DD_SURFACECALLBACKS *pSurfaceCallbacks,
    DD_PALETTECALLBACKS *pPaletteCallbacks);

VOID APIENTRY
DrvDisableDirectDraw(
    DHPDEV dhpdev);

DHPDEV APIENTRY
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
   IN HANDLE hDriver);

VOID APIENTRY
DrvCompletePDEV(
   IN DHPDEV dhpdev,
   IN HDEV hdev);

VOID
APIENTRY
DrvDisablePDEV(IN DHPDEV dhpdev);

HSURF
APIENTRY
DrvEnableSurface(IN DHPDEV dhpdev);

VOID
APIENTRY
DrvDisableSurface(IN DHPDEV dhpdev);

BOOL
APIENTRY
DrvAssertMode(_In_ DHPDEV dhpdev,
              _In_ BOOL bEnable);

ULONG
APIENTRY
DrvGetModes(_In_ HANDLE hDriver,
            _In_ ULONG cjSize,
            _Out_ DEVMODEW *pdm);

BOOL APIENTRY
DrvSetPalette(
   IN DHPDEV dhpdev,
   IN PALOBJ *ppalo,
   IN FLONG fl,
   IN ULONG iStart,
   IN ULONG cColors);

ULONG
APIENTRY
DrvSetPointerShape(IN SURFOBJ *pso,
                   IN SURFOBJ *psoMask,
                   IN SURFOBJ *psoColor,
                   IN XLATEOBJ *pxlo,
                   IN LONG xHot,
                   IN LONG yHot,
                   IN LONG x,
                   IN LONG y,
                   IN RECTL *prcl,
                   IN FLONG fl);

VOID
APIENTRY
DrvMovePointer(
   IN SURFOBJ *pso,
   IN LONG x,
   IN LONG y,
   IN RECTL *prcl);

BOOL
IntInitScreenInfo(
   PPDEV ppdev,
   LPDEVMODEW pDevMode,
   PGDIINFO pGdiInfo,
   PDEVINFO pDevInfo);

BOOL
IntInitDefaultPalette(
   PPDEV ppdev,
   PDEVINFO pDevInfo);

BOOL APIENTRY
IntSetPalette(
   IN DHPDEV dhpdev,
   IN PPALETTEENTRY ppalent,
   IN ULONG iStart,
   IN ULONG cColors);

VOID
APIENTRY
DrvDisableDriver();

VOID
APIENTRY
DrvNotify(_In_ SURFOBJ *pso,
          _In_ ULONG    iType,
          _In_ PVOID    pvData);
BOOL
APIENTRY
DrvIcmSetDeviceGammaRamp(DHPDEV  dhpdev,
                         ULONG   iFormat,
                         LPVOID  lpRamp);
VOID
APIENTRY
DrvSynchronizeSurface(
    SURFOBJ *pso,
    RECTL   *prcl,
    FLONG    fl);

BOOL
APIENTRY
DrvStrokePath(_Inout_ SURFOBJ   *pso,
              _In_ PATHOBJ   *ppo,
              _In_ CLIPOBJ   *pco,
              _In_opt_ XFORMOBJ  *pxo,
              _In_ BRUSHOBJ  *pbo,
              _In_ POINTL    *pptlBrushOrg,
              _In_ LINEATTRS *plineattrs,
              _In_ MIX        mix);

BOOL
APIENTRY
DrvBitBlt(_Inout_ SURFOBJ  *psoTrg,
          _In_opt_ SURFOBJ  *psoSrc,
          _In_opt_ SURFOBJ  *psoMask,
          _In_ CLIPOBJ  *pco,
          _In_opt_ XLATEOBJ *pxlo,
          _In_ RECTL    *prclTrg,
          _In_opt_ POINTL   *pptlSrc,
          _In_opt_ POINTL   *pptlMask,
          _In_opt_ BRUSHOBJ *pbo,
          _In_opt_ POINTL   *pptlBrush,
          _In_ ROP4      rop4);


VOID
APIENTRY
DrvSynchronizeSurface(SURFOBJ *pso,
                      RECTL   *prcl,
                      FLONG    fl);

BOOL
APIENTRY
DrvStrokePath(_Inout_  SURFOBJ   *pso,
              _In_     PATHOBJ   *ppo,
              _In_     CLIPOBJ   *pco,
              _In_opt_ XFORMOBJ  *pxo,
              _In_     BRUSHOBJ  *pbo,
              _In_     POINTL    *pptlBrushOrg,
              _In_     LINEATTRS *plineattrs,
              _In_     MIX        mix);

BOOL
APIENTRY
DrvTransparentBlt(_Inout_  SURFOBJ    *psoDst,
                  _In_     SURFOBJ    *psoSrc,
                  _In_     CLIPOBJ    *pco,
                  _In_opt_ XLATEOBJ   *pxlo,
                  _In_     RECTL      *prclDst,
                  _In_     RECTL      *prclSrc,
                  _In_     ULONG      iTransColor,
                  _In_     ULONG      ulReserved);

BOOL
APIENTRY
DrvCopyBits(_Out_ SURFOBJ*  DestObj,
            _In_  SURFOBJ*  SourceObj,
            _In_  CLIPOBJ*  ClipObj,
            _In_  XLATEOBJ* XLateObj,
            _In_  RECTL*    DestRectL,
            _In_  POINTL*   SrcPointL);


BOOL
APIENTRY
DrvTextOut(_In_ SURFOBJ  *pso,
           _In_ STROBJ   *pstro,
           _In_ FONTOBJ  *pfo,
           _In_ CLIPOBJ  *pco,
           _In_ RECTL    *prclExtra,
           _In_ RECTL    *prclOpaque,
           _In_ BRUSHOBJ *pboFore,
           _In_ BRUSHOBJ *pboOpaque,
           _In_ POINTL   *pptlOrg,
           _In_ MIX       mix);

BOOL
APIENTRY
DrvLineTo(_In_ SURFOBJ *DestObj,
          _In_ CLIPOBJ *Clip,
          _In_ BRUSHOBJ *Brush,
          _In_ LONG x1,
          _In_ LONG y1,
          _In_ LONG x2,
          _In_ LONG y2,
          _In_ RECTL *RectBounds,
          _In_ MIX mix);

BOOL
APIENTRY
DrvFillPath(_Inout_ SURFOBJ  *pso,
            _In_    PATHOBJ  *ppo,
            _In_    CLIPOBJ  *pco,
            _In_    BRUSHOBJ *pbo,
            _In_    POINTL   *pptlBrushOrg,
            _In_    MIX       mix,
            _In_    FLONG     flOptions);

BOOL
APIENTRY
DrvStrokeAndFillPath(_Inout_  SURFOBJ   *pso,
                     _Inout_  PATHOBJ   *ppo,
                     _In_     CLIPOBJ   *pco,
                     _In_opt_ XFORMOBJ  *pxo,
                     _In_     BRUSHOBJ  *pboStroke,
                     _In_     LINEATTRS *plineattrs,
                     _In_     BRUSHOBJ  *pboFill,
                     _In_     POINTL    *pptlBrushOrg,
                     _In_     MIX        mixFill,
                     _In_     FLONG      flOptions);

BOOL
APIENTRY
DrvStretchBltROP(_Inout_  SURFOBJ         *psoDest,
                 _Inout_  SURFOBJ         *psoSrc,
                 _In_opt_ SURFOBJ         *psoMask,
                 _In_     CLIPOBJ         *pco,
                 _In_opt_ XLATEOBJ        *pxlo,
                 _In_opt_ COLORADJUSTMENT *pca,
                 _In_     POINTL          *pptlHTOrg,
                 _In_     RECTL           *prclDest,
                 _In_     RECTL           *prclSrc,
                 _In_opt_ POINTL          *pptlMask,
                 _In_     ULONG            iMode,
                 _In_     BRUSHOBJ        *pbo,
                 _In_     DWORD            rop4);

BOOL
APIENTRY
DrvPlgBlt(_Inout_  SURFOBJ         *psoTrg,
          _Inout_  SURFOBJ         *psoSrc,
          _In_opt_ SURFOBJ         *psoMsk,
          _In_     CLIPOBJ         *pco,
          _In_opt_ XLATEOBJ        *pxlo,
          _In_opt_ COLORADJUSTMENT *pca,
          _In_opt_ POINTL          *pptlBrushOrg,
          _In_     POINTFIX        *pptfx,
          _In_     RECTL           *prcl,
          _In_opt_ POINTL          *pptl,
          _In_     ULONG            iMode);

#endif /* _CDD_PCH_ */
