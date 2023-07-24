/*
 * PROJECT:     ReactOS Desktop Window manager
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     GDI Specific components
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <precomp.h>
#include "../../../win32ss_vista/include/locd3d.h"
#include <debug.h>
/* Legacy */

/* just ended up getting removed in windows 7 */
PVOID
APIENTRY
DwmGetSurfaceData(VOID)
{
    return NULL;
    #if 0
    return NtGdiDwmGetSurfaceData();
    #endif
}

/* just ended up getting removed in windows 7 */
PVOID
APIENTRY
DwmGetDirtyRgn(VOID)
{
    return NULL;
    #if 0
    return NtGdiDwmGetDirtyRgn();
    #endif
}
