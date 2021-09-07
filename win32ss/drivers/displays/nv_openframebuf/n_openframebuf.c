#include "nv_openframebuf.h"

static DRVFN DrvFunctionTable[] =
{
   {INDEX_DrvEnablePDEV, (PFN)DrvEnablePDEV},
   {INDEX_DrvCompletePDEV, (PFN)DrvCompletePDEV},
   {INDEX_DrvDisablePDEV, (PFN)DrvDisablePDEV},
   {INDEX_DrvEnableSurface, (PFN)DrvEnableSurface},
   {INDEX_DrvDisableSurface, (PFN)DrvDisableSurface},
   {INDEX_DrvAssertMode, (PFN)DrvAssertMode},
   {INDEX_DrvGetModes, (PFN)DrvGetModes},
   {INDEX_DrvSetPalette, (PFN)DrvSetPalette},
   {INDEX_DrvSetPointerShape, (PFN)DrvSetPointerShape},
   {INDEX_DrvMovePointer, (PFN)DrvMovePointer}
};

/*
 * DrvEnableDriver
 *
 * Initial driver entry point exported by the driver DLL. It fills in a
 * DRVENABLEDATA structure with the driver's DDI version number and the
 * calling addresses of all DDI functions supported by the driver.
 *
 * Status
 *    @unimplemented
 */

BOOL
APIENTRY
DrvEnableDriver(
   ULONG iEngineVersion,
   ULONG cj,
   PDRVENABLEDATA pded)
{
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
