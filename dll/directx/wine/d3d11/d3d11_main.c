/*
 * Direct3D 11
 *
 * Copyright 2013 Austin English
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 *
 */

#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "d3d11.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(d3d11);

static const char *debug_d3d_driver_type(D3D_DRIVER_TYPE driver_type)
{
    switch(driver_type)
    {
#define D3D11_TO_STR(x) case x: return #x
        D3D11_TO_STR(D3D_DRIVER_TYPE_UNKNOWN);
        D3D11_TO_STR(D3D_DRIVER_TYPE_HARDWARE);
        D3D11_TO_STR(D3D_DRIVER_TYPE_REFERENCE);
        D3D11_TO_STR(D3D_DRIVER_TYPE_NULL);
        D3D11_TO_STR(D3D_DRIVER_TYPE_SOFTWARE);
        D3D11_TO_STR(D3D_DRIVER_TYPE_WARP);
#undef D3D11_TO_STR
        default:
            return wine_dbg_sprintf("Unrecognized D3D_DRIVER_TYPE %#x\n", driver_type);
    }
}
HRESULT WINAPI D3D11CreateDevice(
    _In_opt_ IDXGIAdapter* pAdapter,
    D3D_DRIVER_TYPE DriverType,
    HMODULE Software,
    UINT Flags,
    _In_reads_opt_( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
    UINT FeatureLevels,
    UINT SDKVersion,
    _COM_Outptr_opt_ ID3D11Device** ppDevice,
    _Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
    _COM_Outptr_opt_ ID3D11DeviceContext** ppImmediateContext )
{
    return E_OUTOFMEMORY;
}
