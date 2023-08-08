#include "cdd.h"
#include <debug.h>


VOID APIENTRY DrvSynchronizeSurface(
    SURFOBJ *pso,
    RECTL   *prcl,
    FLONG    fl
)
{
    UNIMPLEMENTED;
    __debugbreak();
}

BOOL APIENTRY DrvStrokePath(
    _Inout_ SURFOBJ   *pso,
    _In_ PATHOBJ   *ppo,
    _In_ CLIPOBJ   *pco,
    _In_opt_ XFORMOBJ  *pxo,
    _In_ BRUSHOBJ  *pbo,
    _In_ POINTL    *pptlBrushOrg,
    _In_ LINEATTRS *plineattrs,
    _In_ MIX        mix
    )
{
        UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

BOOL APIENTRY DrvTransparentBlt(
    _Inout_ SURFOBJ    *psoDst,
    _In_ SURFOBJ    *psoSrc,
    _In_ CLIPOBJ    *pco,
    _In_opt_ XLATEOBJ   *pxlo,
    _In_ RECTL      *prclDst,
    _In_ RECTL      *prclSrc,
    _In_ ULONG      iTransColor,
    _In_ ULONG      ulReserved
)
{
        UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

BOOL APIENTRY DrvBitBlt(
    _Inout_ SURFOBJ  *psoTrg,
    _In_opt_ SURFOBJ  *psoSrc,
    _In_opt_ SURFOBJ  *psoMask,
    _In_ CLIPOBJ  *pco,
    _In_opt_ XLATEOBJ *pxlo,
    _In_ RECTL    *prclTrg,
    _In_opt_ POINTL   *pptlSrc,
    _In_opt_ POINTL   *pptlMask,
    _In_opt_ BRUSHOBJ *pbo,
    _In_opt_ POINTL   *pptlBrush,
    _In_ ROP4      rop4
    )
{
        UNIMPLEMENTED;
    __debugbreak();
    return 0;
}


BOOL APIENTRY
DrvCopyBits(
    OUT SURFOBJ*  DestObj,
    IN  SURFOBJ*  SourceObj,
    IN  CLIPOBJ*  ClipObj,
    IN  XLATEOBJ* XLateObj,
    IN  RECTL*    DestRectL,
    IN  POINTL*   SrcPointL)
{
        UNIMPLEMENTED;
    __debugbreak();
    return 0;
}


BOOL APIENTRY DrvTextOut(
    SURFOBJ  *pso,
    STROBJ   *pstro,
    FONTOBJ  *pfo,
    CLIPOBJ  *pco,
    RECTL    *prclExtra,        // Obsolete, always NULL
    RECTL    *prclOpaque,
    BRUSHOBJ *pboFore,
    BRUSHOBJ *pboOpaque,
    POINTL   *pptlOrg,
    MIX       mix
    )
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

BOOL APIENTRY
DrvLineTo(
    IN SURFOBJ *DestObj,
    IN CLIPOBJ *Clip,
    IN BRUSHOBJ *Brush,
    IN LONG x1,
    IN LONG y1,
    IN LONG x2,
    IN LONG y2,
    IN RECTL *RectBounds,
    IN MIX mix)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

BOOL APIENTRY DrvFillPath(
    _Inout_ SURFOBJ  *pso,
    _In_ PATHOBJ  *ppo,
    _In_ CLIPOBJ  *pco,
    _In_ BRUSHOBJ *pbo,
    _In_ POINTL   *pptlBrushOrg,
    _In_ MIX       mix,
    _In_ FLONG     flOptions
    )
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

BOOL APIENTRY DrvStrokeAndFillPath(
    _Inout_ SURFOBJ   *pso,
    _Inout_ PATHOBJ   *ppo,
    _In_ CLIPOBJ   *pco,
    _In_opt_ XFORMOBJ  *pxo,
    _In_ BRUSHOBJ  *pboStroke,
    _In_ LINEATTRS *plineattrs,
    _In_ BRUSHOBJ  *pboFill,
    _In_ POINTL    *pptlBrushOrg,
    _In_ MIX        mixFill,
    _In_ FLONG      flOptions
    )
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

BOOL APIENTRY DrvStretchBltROP(
    _Inout_ SURFOBJ         *psoDest,
    _Inout_ SURFOBJ         *psoSrc,
    _In_opt_ SURFOBJ         *psoMask,
    _In_ CLIPOBJ         *pco,
    _In_opt_ XLATEOBJ        *pxlo,
    _In_opt_ COLORADJUSTMENT *pca,
    _In_ POINTL          *pptlHTOrg,
    _In_ RECTL           *prclDest,
    _In_ RECTL           *prclSrc,
    _In_opt_ POINTL          *pptlMask,
    _In_ ULONG            iMode,
    _In_ BRUSHOBJ        *pbo,
    _In_ DWORD            rop4
    )
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}


BOOL APIENTRY DrvPlgBlt(
    _Inout_ SURFOBJ         *psoTrg,
    _Inout_ SURFOBJ         *psoSrc,
    _In_opt_ SURFOBJ         *psoMsk,
    _In_ CLIPOBJ         *pco,
    _In_opt_ XLATEOBJ        *pxlo,
    _In_opt_ COLORADJUSTMENT *pca,
    _In_opt_ POINTL          *pptlBrushOrg,
    _In_ POINTFIX        *pptfx,
    _In_ RECTL           *prcl,
    _In_opt_ POINTL          *pptl,
    _In_ ULONG            iMode
    )
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

static DRVFN DrvFunctionTable[] =
{
   {INDEX_DrvEnablePDEV, (PFN)DrvEnablePDEV},
   {INDEX_DrvCompletePDEV, (PFN)DrvCompletePDEV},
   {INDEX_DrvDisablePDEV, (PFN)DrvDisablePDEV},
   {INDEX_DrvEnableSurface, (PFN)DrvEnableSurface},
   {INDEX_DrvDisableSurface, (PFN)DrvDisableSurface},
   {INDEX_DrvAssertMode, (PFN)DrvAssertMode},
   {INDEX_DrvNotify, (PFN)DrvNotify},
   {INDEX_DrvDisableDriver,(PFN) DrvDisableDriver},
   {INDEX_DrvGetModes, (PFN)DrvGetModes},
   {INDEX_DrvSetPalette, (PFN)DrvSetPalette},
   {INDEX_DrvSetPointerShape, (PFN)DrvSetPointerShape},
   {INDEX_DrvMovePointer, (PFN)DrvMovePointer},
   {INDEX_DrvIcmSetDeviceGammaRamp, (PFN)DrvIcmSetDeviceGammaRamp},
   {INDEX_DrvSynchronizeSurface, (PFN)DrvSynchronizeSurface},
   {INDEX_DrvStrokePath, (PFN)DrvStrokePath},
   {INDEX_DrvTransparentBlt, (PFN)DrvTransparentBlt},
   {INDEX_DrvBitBlt, (PFN)DrvBitBlt},
   {INDEX_DrvCopyBits, (PFN)DrvCopyBits},
   {INDEX_DrvTextOut, (PFN)DrvTextOut},
   {INDEX_DrvLineTo, (PFN)DrvLineTo},
   {INDEX_DrvFillPath, (PFN)DrvFillPath},
   {INDEX_DrvStrokeAndFillPath, (PFN)DrvStrokeAndFillPath},
   {INDEX_DrvStretchBltROP, (PFN)DrvStretchBltROP},
   {INDEX_DrvPlgBlt, (PFN)DrvPlgBlt},
};

/*
 * Canonical Display Driver is a XDDM Framebuf like driver to handle
 * the WDDM Monitor resolution changes and Pointer accleration
 *
 * Even in windows 11 this driver handles these operations.
 * Eventually more stuff got passed to cdd.dll handling more and more so we will
 * eventually build on this but for now.
 *
 * This is the first real rendering requests for WDDM
 */
BOOL
APIENTRY
DrvEnableDriver(ULONG iEngineVersion,
                ULONG cj,
                PDRVENABLEDATA pded)
{
    DPRINT1("ReactOS CDD - ReactOS Display Driver Model\n");
    __debugbreak();
    if (cj >= sizeof(DRVENABLEDATA))
    {
       pded->c = sizeof(DrvFunctionTable) / sizeof(DRVFN);
       pded->pdrvfn = DrvFunctionTable;
       pded->iDriverVersion = DDI_DRIVER_VERSION_NT5;
       return TRUE;
    }
    else
    {
       return FALSE;
    }
}

VOID
APIENTRY
DrvDisableDriver()
{
    /* Nothing to do here as CDD doesn't handle adapter*/
}

VOID
APIENTRY
DrvNotify(SURFOBJ *pso,
          ULONG    iType,
          PVOID    pvData)
{
    UNIMPLEMENTED;
    __debugbreak();
}