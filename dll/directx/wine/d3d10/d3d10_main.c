/*
 * Direct3D 10
 *
 * Copyright 2007 Andras Kovacs
 * Copyright 2009 Henri Verbeet for CodeWeavers
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

#include "config.h"
#include "wine/port.h"

#include "d3d10_private.h"
#include <debug.h>
WINE_DEFAULT_DEBUG_CHANNEL(d3d10);

/* At process attach */
BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpv)
{
    TRACE("fdwReason=%d\n", fdwReason);
    switch(fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls( hInstDLL );
        break;
    }
    return TRUE;
}



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

    
    
HRESULT WINAPI D3D10CreateDevice(IDXGIAdapter *adapter, D3D10_DRIVER_TYPE driver_type,
        HMODULE swrast, UINT flags, UINT sdk_version, ID3D10Device **device)
{
    IDXGIFactory *factory;
    HRESULT hr;
    DPRINT1("D3D10CreateDevice: Function Entry");
    __debugbreak();

    if (adapter)
    {
        factory = NULL;
        #if 0
        IDXGIAdapter_AddRef(adapter);
        hr = IDXGIAdapter_GetParent(adapter, &IID_IDXGIFactory, (void **)&factory);
        if (FAILED(hr))
        {
            WARN("Failed to get dxgi factory, returning %#x\n", hr);
            return hr;
        }
        #endif
    }
    else
    {
        hr = 0;
        #if 0
        hr = CreateDXGIFactory(&IID_IDXGIFactory, (void **)&factory);
        if (FAILED(hr))
        {
            WARN("Failed to create dxgi factory, returning %#x\n", hr);
            return hr;
        }
#endif
        factory = NULL;
        switch(driver_type)
        {
            case D3D10_DRIVER_TYPE_HARDWARE:
            {
                hr = IDXGIFactory_EnumAdapters(factory, 0, &adapter);
                if (FAILED(hr))
                {
                    WARN("No adapters found, returning %#x\n", hr);
                    IDXGIFactory_Release(factory);
                    return hr;
                }
                break;
            }

            case D3D10_DRIVER_TYPE_NULL:
                FIXME("NULL device not implemented, falling back to refrast\n");
                /* fall through, for now */
            case D3D10_DRIVER_TYPE_REFERENCE:
            {
                HMODULE refrast = LoadLibraryA("d3d10ref.dll");
                if (!refrast)
                {
                    WARN("Failed to load refrast, returning E_FAIL\n");
                    IDXGIFactory_Release(factory);
                    return E_FAIL;
                }
                hr = IDXGIFactory_CreateSoftwareAdapter(factory, refrast, &adapter);
                FreeLibrary(refrast);
                if (FAILED(hr))
                {
                    WARN("Failed to create a software adapter, returning %#x\n", hr);
                    IDXGIFactory_Release(factory);
                    return hr;
                }
                break;
            }

            case D3D10_DRIVER_TYPE_SOFTWARE:
            {
                if (!swrast)
                {
                    WARN("Software device requested, but NULL swrast passed, returning E_FAIL\n");
                    IDXGIFactory_Release(factory);
                    return E_FAIL;
                }
                hr = IDXGIFactory_CreateSoftwareAdapter(factory, swrast, &adapter);
                if (FAILED(hr))
                {
                    WARN("Failed to create a software adapter, returning %#x\n", hr);
                    IDXGIFactory_Release(factory);
                    return hr;
                }
                break;
            }
        }
    }

    hr = (HRESULT)D3D10CoreCreateDevice(factory, adapter, flags, 0, device);
    IDXGIAdapter_Release(adapter);
    IDXGIFactory_Release(factory);
    if (FAILED(hr))
    {
        WARN("Failed to create a device, returning %#x\n", hr);
        return hr;
    }

    TRACE("Created ID3D10Device %p\n", *device);

    return hr;
}

HRESULT WINAPI D3D10CreateDeviceAndSwapChain(IDXGIAdapter *adapter, D3D10_DRIVER_TYPE driver_type,
        HMODULE swrast, UINT flags, UINT sdk_version, DXGI_SWAP_CHAIN_DESC *swapchain_desc,
        IDXGISwapChain **swapchain, ID3D10Device **device)
{
    IDXGIDevice *dxgi_device;
    IDXGIFactory *factory;
    HRESULT hr;

    hr = D3D10CreateDevice(adapter, driver_type, swrast, flags, sdk_version, device);
    if (FAILED(hr))
    {
        WARN("Failed to create a device, returning %#x\n", hr);
        *device = NULL;
        return hr;
    }

    TRACE("Created ID3D10Device %p\n", *device);
    dxgi_device = NULL;
#if 0
    hr = ID3D10Device_QueryInterface(*device, &IID_IDXGIDevice, (void **)&dxgi_device);
    if (FAILED(hr))
    {
        ERR("Failed to get a dxgi device from the d3d10 device, returning %#x\n", hr);
        ID3D10Device_Release(*device);
        *device = NULL;
        return hr;
    }
#endif
    hr = IDXGIDevice_GetAdapter(dxgi_device, &adapter);
    IDXGIDevice_Release(dxgi_device);
    if (FAILED(hr))
    {
        ERR("Failed to get the device adapter, returning %#x\n", hr);
        ID3D10Device_Release(*device);
        *device = NULL;
        return hr;
    }
    factory = NULL;
    hr = 0;
#if 0
    hr = IDXGIAdapter_GetParent(adapter, &IID_IDXGIFactory, (void **)&factory);
    IDXGIAdapter_Release(adapter);
    if (FAILED(hr))
    {
        ERR("Failed to get the adapter factory, returning %#x\n", hr);
        ID3D10Device_Release(*device);
        *device = NULL;
        return hr;
    }
#endif
    hr = IDXGIFactory_CreateSwapChain(factory, (IUnknown *)*device, swapchain_desc, swapchain);
    IDXGIFactory_Release(factory);
    if (FAILED(hr))
    {
        ID3D10Device_Release(*device);
        *device = NULL;

        WARN("Failed to create a swapchain, returning %#x\n", hr);
        return hr;
    }

    TRACE("Created IDXGISwapChain %p\n", *swapchain);

    return S_OK;
}

HRESULT WINAPI D3D10CreateEffectFromMemory(void *data, SIZE_T data_size, UINT flags,
        ID3D10Device *device, ID3D10EffectPool *effect_pool, ID3D10Effect **effect)
{
    struct d3d10_effect *object;
    HRESULT hr;

    FIXME("data %p, data_size %lu, flags %#x, device %p, effect_pool %p, effect %p stub!\n",
            data, data_size, flags, device, effect_pool, effect);

    object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object));
    if (!object)
    {
        ERR("Failed to allocate D3D10 effect object memory\n");
        return E_OUTOFMEMORY;
    }

    object->ID3D10Effect_iface.lpVtbl = NULL;
    object->refcount = 1;
    ID3D10Device_AddRef(device);
    object->device = device;
    hr = 0;
#if 0
    hr = d3d10_effect_parse(object, data, data_size);
    if (FAILED(hr))
    {
        ERR("Failed to parse effect\n");
        IUnknown_Release(&object->ID3D10Effect_iface);
        return hr;
    }
#endif
    *effect = &object->ID3D10Effect_iface;

    TRACE("Created ID3D10Effect %p\n", object);

    return S_OK;
}

HRESULT WINAPI D3D10CompileEffectFromMemory(void *data, SIZE_T data_size, const char *filename,
        const D3D10_SHADER_MACRO *defines, ID3D10Include *include, UINT hlsl_flags, UINT fx_flags,
        ID3D10Blob **effect, ID3D10Blob **errors)
{
    FIXME("data %p, data_size %lu, filename %s, defines %p, include %p,"
            " hlsl_flags %#x, fx_flags %#x, effect %p, errors %p stub!\n",
            data, data_size, wine_dbgstr_a(filename), defines, include,
            hlsl_flags, fx_flags, effect, errors);

    if (effect)
        *effect = NULL;
    if (errors)
        *errors = NULL;

    return E_NOTIMPL;
}


LPCSTR WINAPI D3D10GetVertexShaderProfile(ID3D10Device *device)
{
    FIXME("device %p stub!\n", device);

    return "vs_4_0";
}

LPCSTR WINAPI D3D10GetGeometryShaderProfile(ID3D10Device *device)
{
    FIXME("device %p stub!\n", device);

    return "gs_4_0";
}

LPCSTR WINAPI D3D10GetPixelShaderProfile(ID3D10Device *device)
{
    FIXME("device %p stub!\n", device);

    return "ps_4_0";
}
HRESULT WINAPI D3D10ReflectShader(const void *data, SIZE_T data_size, ID3D10ShaderReflection **reflector)
{
#if 0
    // struct d3d10_shader_reflection *object;
     
    FIXME("data %p, data_size %lu, reflector %p stub!\n", data, data_size, reflector);

    object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object));
    if (!object)
    {
        ERR("Failed to allocate D3D10 shader reflection object memory\n");
        return E_OUTOFMEMORY;
    }

    object->ID3D10ShaderReflection_iface.lpVtbl = &d3d10_shader_reflection_vtbl;
    object->refcount = 1;

    *reflector = &object->ID3D10ShaderReflection_iface;
#endif
    TRACE("Created ID3D10ShaderReflection");

    return S_OK;
}

