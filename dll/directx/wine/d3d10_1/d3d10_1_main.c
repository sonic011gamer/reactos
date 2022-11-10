/*
 * Copyright 2014 Henri Verbeet for CodeWeavers
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

#include "wine/debug.h"

#define COBJMACROS
#include "wine/dxgi.h"
#include "wine/winedxgi.h"
#include "d3d10_1.h"
WINE_DEFAULT_DEBUG_CHANNEL(d3d10);



DEFINE_GUID(IID_IDXGIObject,0xaec22fb8,0x76f3,0x4639,0x9b,0xe0,0x28,0xeb,0x43,0xa6,0x7a,0x2e);
DEFINE_GUID(IID_IDXGIDeviceSubObject,0x3d3e0379,0xf9de,0x4d58,0xbb,0x6c,0x18,0xd6,0x29,0x92,0xf1,0xa6);
DEFINE_GUID(IID_IDXGIResource,0x035f3ab4,0x482e,0x4e50,0xb4,0x1f,0x8a,0x7f,0x8b,0xd8,0x96,0x0b);
DEFINE_GUID(IID_IDXGIKeyedMutex,0x9d8e1289,0xd7b3,0x465f,0x81,0x26,0x25,0x0e,0x34,0x9a,0xf8,0x5d);
DEFINE_GUID(IID_IDXGISurface,0xcafcb56c,0x6ac3,0x4889,0xbf,0x47,0x9e,0x23,0xbb,0xd2,0x60,0xec);
DEFINE_GUID(IID_IDXGISurface1,0x4AE63092,0x6327,0x4c1b,0x80,0xAE,0xBF,0xE1,0x2E,0xA3,0x2B,0x86);
DEFINE_GUID(IID_IDXGIAdapter,0x2411e7e1,0x12ac,0x4ccf,0xbd,0x14,0x97,0x98,0xe8,0x53,0x4d,0xc0);
DEFINE_GUID(IID_IDXGIOutput,0xae02eedb,0xc735,0x4690,0x8d,0x52,0x5a,0x8d,0xc2,0x02,0x13,0xaa);
DEFINE_GUID(IID_IDXGISwapChain,0x310d36a0,0xd2e7,0x4c0a,0xaa,0x04,0x6a,0x9d,0x23,0xb8,0x88,0x6a);
DEFINE_GUID(IID_IDXGIFactory,0x7b7166ec,0x21c7,0x44ae,0xb2,0x1a,0xc9,0xae,0x32,0x1a,0xe3,0x69);
DEFINE_GUID(IID_IDXGIDevice,0x54ec77fa,0x1377,0x44e6,0x8c,0x32,0x88,0xfd,0x5f,0x44,0xc8,0x4c);
DEFINE_GUID(IID_IDXGIFactory1,0x770aae78,0xf26f,0x4dba,0xa8,0x29,0x25,0x3c,0x83,0xd1,0xb3,0x87);
DEFINE_GUID(IID_IDXGIAdapter1,0x29038f61,0x3839,0x4626,0x91,0xfd,0x08,0x68,0x79,0x01,0x1a,0x05);
DEFINE_GUID(IID_IDXGIDevice1,0x77db970f,0x6276,0x48ba,0xba,0x28,0x07,0x01,0x43,0xb4,0x39,0x2c);

    

HRESULT WINAPI D3D10CoreCreateDevice(IDXGIFactory *factory, IDXGIAdapter *adapter,
        unsigned int flags, D3D_FEATURE_LEVEL feature_level, ID3D10Device **device);

#define WINE_D3D10_TO_STR(x) case x: return #x

static const char *debug_d3d10_driver_type(D3D10_DRIVER_TYPE driver_type)
{
    switch (driver_type)
    {
        WINE_D3D10_TO_STR(D3D10_DRIVER_TYPE_HARDWARE);
        WINE_D3D10_TO_STR(D3D10_DRIVER_TYPE_REFERENCE);
        WINE_D3D10_TO_STR(D3D10_DRIVER_TYPE_NULL);
        WINE_D3D10_TO_STR(D3D10_DRIVER_TYPE_SOFTWARE);
        WINE_D3D10_TO_STR(D3D10_DRIVER_TYPE_WARP);
        default:
            FIXME("Unrecognized D3D10_DRIVER_TYPE %#x.\n", driver_type);
            return "unrecognized";
    }
}

static const char *debug_d3d10_feature_level(D3D10_FEATURE_LEVEL1 feature_level)
{
    switch (feature_level)
    {
        WINE_D3D10_TO_STR(D3D10_FEATURE_LEVEL_10_0);
        WINE_D3D10_TO_STR(D3D10_FEATURE_LEVEL_10_1);
        WINE_D3D10_TO_STR(D3D10_FEATURE_LEVEL_9_1);
        WINE_D3D10_TO_STR(D3D10_FEATURE_LEVEL_9_2);
        WINE_D3D10_TO_STR(D3D10_FEATURE_LEVEL_9_3);
        default:
            FIXME("Unrecognized D3D10_FEATURE_LEVEL1 %#x.\n", feature_level);
            return "unrecognized";
    }
}

#undef WINE_D3D10_TO_STR

static D3D_FEATURE_LEVEL d3d_feature_level_from_d3d10_1(D3D10_FEATURE_LEVEL1 level)
{
    return (D3D_FEATURE_LEVEL)level;
}

static HRESULT d3d10_create_device1(IDXGIAdapter *adapter, D3D10_DRIVER_TYPE driver_type, HMODULE swrast,
        UINT flags, D3D10_FEATURE_LEVEL1 hw_level, UINT sdk_version, ID3D10Device1 **device)
{
    IDXGIFactory *factory;
    HRESULT hr;

    TRACE("adapter %p, driver_type %s, swrast %p, flags %#x, hw_level %s, sdk_version %d, device %p.\n",
            adapter, debug_d3d10_driver_type(driver_type), swrast, flags,
            debug_d3d10_feature_level(hw_level), sdk_version, device);

    if (!device)
        return E_INVALIDARG;

    *device = NULL;

    if (!hw_level)
        return E_INVALIDARG;

    if (adapter)
    {
        #if 0
        IDXGIAdapter_AddRef(adapter);
        if (FAILED(hr = IDXGIAdapter_GetParent(adapter, &IID_IDXGIFactory, (void **)&factory)))
        {
            WARN("Failed to get dxgi factory, hr %#x.\n", hr);
            return hr;
        }
        #endif
    }
    else
    {
        #if 0
        if (FAILED(hr = CreateDXGIFactory(&IID_IDXGIFactory, (void **)&factory)))
        {
            WARN("Failed to create dxgi factory, hr %#x.\n", hr);
            return hr;
        }
#endif      
        factory = NULL;
        switch (driver_type)
        {
            case D3D10_DRIVER_TYPE_WARP:
                FIXME("WARP driver not implemented, falling back to hardware.\n");
            case D3D10_DRIVER_TYPE_HARDWARE:
            {
                if (FAILED(hr = IDXGIFactory_EnumAdapters(factory, 0, &adapter)))
                {
                    WARN("No adapters found, hr %#x.\n", hr);
                    IDXGIFactory_Release(factory);
                    return hr;
                }
                break;
            }

            case D3D10_DRIVER_TYPE_NULL:
                FIXME("NULL device not implemented, falling back to refrast.\n");
                /* Fall through, for now. */
            case D3D10_DRIVER_TYPE_REFERENCE:
            {
                HMODULE refrast;

                if (!(refrast = LoadLibraryA("d3d10ref.dll")))
                {
                    WARN("Failed to load refrast, returning E_FAIL.\n");
                    IDXGIFactory_Release(factory);
                    return E_FAIL;
                }
                hr = IDXGIFactory_CreateSoftwareAdapter(factory, refrast, &adapter);
                FreeLibrary(refrast);
                if (FAILED(hr))
                {
                    WARN("Failed to create a software adapter, hr %#x.\n", hr);
                    IDXGIFactory_Release(factory);
                    return hr;
                }
                break;
            }

            case D3D10_DRIVER_TYPE_SOFTWARE:
            {
                if (!swrast)
                {
                    WARN("Software device requested, but NULL swrast passed, returning E_FAIL.\n");
                    IDXGIFactory_Release(factory);
                    return E_FAIL;
                }
                if (FAILED(hr = IDXGIFactory_CreateSoftwareAdapter(factory, swrast, &adapter)))
                {
                    WARN("Failed to create a software adapter, hr %#x.\n", hr);
                    IDXGIFactory_Release(factory);
                    return hr;
                }
                break;
            }

            default:
                FIXME("Unhandled driver type %#x.\n", driver_type);
                IDXGIFactory_Release(factory);
                return E_FAIL;
        }
    }

    hr = D3D10CoreCreateDevice(factory, adapter, flags,
            d3d_feature_level_from_d3d10_1(hw_level), (ID3D10Device **)device);
    IDXGIAdapter_Release(adapter);
    IDXGIFactory_Release(factory);
    if (FAILED(hr))
    {
        WARN("Failed to create a device, hr %#x.\n", hr);
        return hr;
    }

    TRACE("Created device %p.\n", *device);

    return hr;
}

HRESULT WINAPI D3D10CreateDevice1(IDXGIAdapter *adapter, D3D10_DRIVER_TYPE driver_type, HMODULE swrast,
        UINT flags, D3D10_FEATURE_LEVEL1 hw_level, UINT sdk_version, ID3D10Device1 **device)
{
    return d3d10_create_device1(adapter, driver_type, swrast, flags, hw_level, sdk_version, device);
}

HRESULT WINAPI D3D10CreateDeviceAndSwapChain1(IDXGIAdapter *adapter, D3D10_DRIVER_TYPE driver_type,
        HMODULE swrast, UINT flags, D3D10_FEATURE_LEVEL1 feature_level, UINT sdk_version,
        DXGI_SWAP_CHAIN_DESC *swapchain_desc, IDXGISwapChain **swapchain, ID3D10Device1 **device)
{
    IDXGIDevice *dxgi_device;
    IDXGIFactory *factory;
    HRESULT hr;

    TRACE("adapter %p, driver_type %s, swrast %p, flags %#x, "
            "feature_level %s, sdk_version %d, swapchain_desc %p, swapchain %p, device %p.\n",
            adapter, debug_d3d10_driver_type(driver_type), swrast, flags,
            debug_d3d10_feature_level(feature_level), sdk_version, swapchain_desc, swapchain, device);

    if (swapchain)
        *swapchain = NULL;

    if (!device)
        return E_INVALIDARG;

    /* Avoid forwarding to D3D10CreateDevice1(), since it breaks applications
     * hooking these entry-points. */
    if (FAILED(hr = d3d10_create_device1(adapter, driver_type, swrast, flags, feature_level, sdk_version, device)))
    {
        WARN("Failed to create a device, returning %#x.\n", hr);
        *device = NULL;
        return hr;
    }

    if (swapchain)
    {
        dxgi_device = NULL;
        #if 0
        if (FAILED(hr = ID3D10Device1_QueryInterface(*device, &IID_IDXGIDevice, (void **)&dxgi_device)))
        {
            ERR("Failed to get a dxgi device from the d3d10 device, returning %#x.\n", hr);
            goto cleanup;
        }
        #endif

        hr = IDXGIDevice_GetAdapter(dxgi_device, &adapter);
        IDXGIDevice_Release(dxgi_device);
        if (FAILED(hr))
        {
            ERR("Failed to get the device adapter, returning %#x.\n", hr);
            goto cleanup;
        }
        factory = NULL;
#if 0
        hr = IDXGIAdapter_GetParent(adapter, &IID_IDXGIFactory, (void **)&factory);
        IDXGIAdapter_Release(adapter);
        if (FAILED(hr))
        {
            ERR("Failed to get the adapter factory, returning %#x.\n", hr);
            goto cleanup;
        }
#endif
        hr = IDXGIFactory_CreateSwapChain(factory, (IUnknown *)*device, swapchain_desc, swapchain);
        IDXGIFactory_Release(factory);
        if (FAILED(hr))
        {
            WARN("Failed to create a swapchain, returning %#x.\n", hr);
            goto cleanup;
        }

        TRACE("Created IDXGISwapChain %p.\n", *swapchain);
    }

    return S_OK;

cleanup:
    ID3D10Device1_Release(*device);
    *device = NULL;
    return hr;
}
