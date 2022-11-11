/*
 * Copyright 2008 Henri Verbeet for CodeWeavers
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
 */

#ifndef __WINE_DXGI_PRIVATE_H
#define __WINE_DXGI_PRIVATE_H

#include "wine/debug.h"

#define COBJMACROS
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "objbase.h"
#include "winnls.h"
#include "config.h"

#include "wine/dxgi.h"

#ifdef DXGI_INIT_GUID
#include "initguid.h"
#endif
#include "wine/wined3d.h"


#if 1
DEFINE_GUID(IID_IWineDXGIDevice, 0x3e1ff30b, 0xc951, 0x48c3, 0xb0,0x10, 0x0f,0xb4,0x9f,0x3d,0xca,0x71);
DEFINE_GUID(IID_IWineDXGIDeviceParent, 0xf2b918f3, 0x603f, 0x430a, 0x9c,0xcd, 0x55,0x87,0x2b,0x6e,0x85,0xdf);

DEFINE_GUID(IID_ID3D10BlendState1,0xEDAD8D99,0x8A35,0x4d6d,0x85,0x66,0x2E,0xA2,0x76,0xCD,0xE1,0x61);
DEFINE_GUID(IID_ID3D10ShaderResourceView1,0x9B7E4C87,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);
DEFINE_GUID(IID_ID3D10Device1,0x9B7E4C8F,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);
DEFINE_GUID(IID_ID3D10DeviceChild,0x9B7E4C00,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);
DEFINE_GUID(IID_ID3D10DepthStencilState,0x2B4B1CC8,0xA4AD,0x41f8,0x83,0x22,0xCA,0x86,0xFC,0x3E,0xC6,0x75);
DEFINE_GUID(IID_ID3D10BlendState,0xEDAD8D19,0x8A35,0x4d6d,0x85,0x66,0x2E,0xA2,0x76,0xCD,0xE1,0x61);
DEFINE_GUID(IID_ID3D10RasterizerState,0xA2A07292,0x89AF,0x4345,0xBE,0x2E,0xC5,0x3D,0x9F,0xBB,0x6E,0x9F);
DEFINE_GUID(IID_ID3D10Resource,0x9B7E4C01,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);
DEFINE_GUID(IID_ID3D10Buffer,0x9B7E4C02,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);
DEFINE_GUID(IID_ID3D10Texture1D,0x9B7E4C03,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);
DEFINE_GUID(IID_ID3D10Texture2D,0x9B7E4C04,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);
DEFINE_GUID(IID_ID3D10Texture3D,0x9B7E4C05,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);
DEFINE_GUID(IID_ID3D10View,0xC902B03F,0x60A7,0x49BA,0x99,0x36,0x2A,0x3A,0xB3,0x7A,0x7E,0x33);
DEFINE_GUID(IID_ID3D10ShaderResourceView,0x9B7E4C07,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);
DEFINE_GUID(IID_ID3D10RenderTargetView,0x9B7E4C08,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);
DEFINE_GUID(IID_ID3D10DepthStencilView,0x9B7E4C09,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);
DEFINE_GUID(IID_ID3D10VertexShader,0x9B7E4C0A,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);
DEFINE_GUID(IID_ID3D10GeometryShader,0x6316BE88,0x54CD,0x4040,0xAB,0x44,0x20,0x46,0x1B,0xC8,0x1F,0x68);
DEFINE_GUID(IID_ID3D10PixelShader,0x4968B601,0x9D00,0x4cde,0x83,0x46,0x8E,0x7F,0x67,0x58,0x19,0xB6);
DEFINE_GUID(IID_ID3D10InputLayout,0x9B7E4C0B,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);
DEFINE_GUID(IID_ID3D10SamplerState,0x9B7E4C0C,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);
DEFINE_GUID(IID_ID3D10Asynchronous,0x9B7E4C0D,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);
DEFINE_GUID(IID_ID3D10Query,0x9B7E4C0E,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);
DEFINE_GUID(IID_ID3D10Predicate,0x9B7E4C10,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);
DEFINE_GUID(IID_ID3D10Counter,0x9B7E4C11,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);
DEFINE_GUID(IID_ID3D10Device,0x9B7E4C0F,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);
DEFINE_GUID(IID_ID3D10Multithread,0x9B7E4E00,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x04,0xF6,0x89,0xF0);

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


#endif

#define DXGI_ERROR_INVALID_CALL          _HRESULT_TYPEDEF_(0x887A0001L)

//
// MessageId: DXGI_ERROR_NOT_FOUND
//
// MessageText:
//
// The object was not found. If calling IDXGIFactory::EnumAdaptes, there is no adapter with the specified ordinal.
//
#define DXGI_ERROR_NOT_FOUND             _HRESULT_TYPEDEF_(0x887A0002L)

//
// MessageId: DXGI_ERROR_MORE_DATA
//
// MessageText:
//
// The caller did not supply a sufficiently large buffer.
//
#define DXGI_ERROR_MORE_DATA             _HRESULT_TYPEDEF_(0x887A0003L)

//
// MessageId: DXGI_ERROR_UNSUPPORTED
//
// MessageText:
//
// The specified device interface or feature level is not supported on this system.
//
#define DXGI_ERROR_UNSUPPORTED           _HRESULT_TYPEDEF_(0x887A0004L)

typedef enum D3D_FEATURE_LEVEL
{
    D3D_FEATURE_LEVEL_9_1 = 0x9100,
    D3D_FEATURE_LEVEL_9_2 = 0x9200,
    D3D_FEATURE_LEVEL_9_3 = 0x9300,
    D3D_FEATURE_LEVEL_10_0 = 0xa000,
    D3D_FEATURE_LEVEL_10_1 = 0xa100,
    D3D_FEATURE_LEVEL_11_0 = 0xb000,
    D3D_FEATURE_LEVEL_11_1 = 0xb100
} D3D_FEATURE_LEVEL;

enum dxgi_frame_latency
{
    DXGI_FRAME_LATENCY_DEFAULT =  3,
    DXGI_FRAME_LATENCY_MAX     = 16,
};

/* Layered device */
enum dxgi_device_layer_id
{
    DXGI_DEVICE_LAYER_DEBUG1        = 0x8,
    DXGI_DEVICE_LAYER_THREAD_SAFE   = 0x10,
    DXGI_DEVICE_LAYER_DEBUG2        = 0x20,
    DXGI_DEVICE_LAYER_SWITCH_TO_REF = 0x30,
    DXGI_DEVICE_LAYER_D3D10_DEVICE  = 0xffffffff,
};

struct layer_get_size_args
{
    DWORD unknown0;
    DWORD unknown1;
    DWORD *unknown2;
    DWORD *unknown3;
    IDXGIAdapter *adapter;
    WORD interface_major;
    WORD interface_minor;
    WORD version_build;
    WORD version_revision;
};

struct dxgi_device_layer
{
    enum dxgi_device_layer_id id;
    HRESULT (WINAPI *init)(enum dxgi_device_layer_id id, DWORD *count, DWORD *values);
    UINT (WINAPI *get_size)(enum dxgi_device_layer_id id, struct layer_get_size_args *args, DWORD unknown0);
    HRESULT (WINAPI *create)(enum dxgi_device_layer_id id, void **layer_base, DWORD unknown0,
            void *device_object, REFIID riid, void **device_layer);
    void (WINAPI *set_feature_level)(enum dxgi_device_layer_id id, void *device,
            D3D_FEATURE_LEVEL feature_level);
};

/* TRACE helper functions */
const char *debug_dxgi_format(DXGI_FORMAT format) DECLSPEC_HIDDEN;
const char *debug_dxgi_mode(const DXGI_MODE_DESC *desc) DECLSPEC_HIDDEN;
void dump_feature_levels(const D3D_FEATURE_LEVEL *feature_levels, unsigned int level_count) DECLSPEC_HIDDEN;

DXGI_FORMAT dxgi_format_from_wined3dformat(enum wined3d_format_id format) DECLSPEC_HIDDEN;
enum wined3d_format_id wined3dformat_from_dxgi_format(DXGI_FORMAT format) DECLSPEC_HIDDEN;
UINT dxgi_rational_to_uint(const DXGI_RATIONAL *rational) DECLSPEC_HIDDEN;
enum wined3d_scanline_ordering wined3d_scanline_ordering_from_dxgi(
        DXGI_MODE_SCANLINE_ORDER scanline_order) DECLSPEC_HIDDEN;
void dxgi_sample_desc_from_wined3d(DXGI_SAMPLE_DESC *desc,
        enum wined3d_multisample_type wined3d_type, unsigned int wined3d_quality) DECLSPEC_HIDDEN;
void wined3d_sample_desc_from_dxgi(enum wined3d_multisample_type *wined3d_type,
        unsigned int *wined3d_quality, const DXGI_SAMPLE_DESC *dxgi_desc) DECLSPEC_HIDDEN;
void wined3d_display_mode_from_dxgi(struct wined3d_display_mode *wined3d_mode,
        const DXGI_MODE_DESC *mode) DECLSPEC_HIDDEN;
unsigned int dxgi_swapchain_flags_from_wined3d(unsigned int wined3d_flags) DECLSPEC_HIDDEN;
unsigned int wined3d_swapchain_flags_from_dxgi(unsigned int flags) DECLSPEC_HIDDEN;
HRESULT dxgi_get_private_data(struct wined3d_private_store *store,
        REFGUID guid, UINT *data_size, void *data) DECLSPEC_HIDDEN;
HRESULT dxgi_set_private_data(struct wined3d_private_store *store,
        REFGUID guid, UINT data_size, const void *data) DECLSPEC_HIDDEN;
HRESULT dxgi_set_private_data_interface(struct wined3d_private_store *store,
        REFGUID guid, const IUnknown *object) DECLSPEC_HIDDEN;

/* IDXGIFactory */
struct dxgi_factory
{
    IDXGIFactory1 IDXGIFactory1_iface;
    LONG refcount;
    struct wined3d_private_store private_store;
    struct wined3d *wined3d;
    BOOL extended;
    HWND device_window;
};

HRESULT dxgi_factory_create(REFIID riid, void **factory, BOOL extended) DECLSPEC_HIDDEN;
HWND dxgi_factory_get_device_window(struct dxgi_factory *factory) DECLSPEC_HIDDEN;
struct dxgi_factory *unsafe_impl_from_IDXGIFactory1(IDXGIFactory1 *iface) DECLSPEC_HIDDEN;

/* IDXGIDevice */
struct dxgi_device
{
    IWineDXGIDevice IWineDXGIDevice_iface;
    IUnknown *child_layer;
    LONG refcount;
    struct wined3d_private_store private_store;
    struct wined3d_device *wined3d_device;
    IDXGIAdapter1 *adapter;
};

HRESULT dxgi_device_init(struct dxgi_device *device, struct dxgi_device_layer *layer,
        IDXGIFactory *factory, IDXGIAdapter *adapter,
        const D3D_FEATURE_LEVEL *feature_levels, unsigned int level_count) DECLSPEC_HIDDEN;

/* IDXGIOutput */
struct dxgi_output
{
    IDXGIOutput IDXGIOutput_iface;
    LONG refcount;
    struct wined3d_private_store private_store;
    struct dxgi_adapter *adapter;
};

HRESULT dxgi_output_create(struct dxgi_adapter *adapter, struct dxgi_output **output) DECLSPEC_HIDDEN;

/* IDXGIAdapter */
struct dxgi_adapter
{
    IDXGIAdapter1 IDXGIAdapter1_iface;
    LONG refcount;
    struct wined3d_private_store private_store;
    UINT ordinal;
    struct dxgi_factory *factory;
};

HRESULT dxgi_adapter_create(struct dxgi_factory *factory, UINT ordinal,
        struct dxgi_adapter **adapter) DECLSPEC_HIDDEN;
struct dxgi_adapter *unsafe_impl_from_IDXGIAdapter1(IDXGIAdapter1 *iface) DECLSPEC_HIDDEN;

/* IDXGISwapChain */
struct dxgi_swapchain
{
    IDXGISwapChain IDXGISwapChain_iface;
    LONG refcount;
    struct wined3d_private_store private_store;
    struct wined3d_swapchain *wined3d_swapchain;
    IWineDXGIDevice *device;
    IDXGIFactory *factory;

    BOOL fullscreen;
    IDXGIOutput *target;
};

HRESULT dxgi_swapchain_init(struct dxgi_swapchain *swapchain, struct dxgi_device *device,
        struct wined3d_swapchain_desc *desc, BOOL implicit) DECLSPEC_HIDDEN;

/* IDXGISurface */
struct dxgi_surface
{
    IDXGISurface1 IDXGISurface1_iface;
    IUnknown IUnknown_iface;
    IUnknown *outer_unknown;
    LONG refcount;
    struct wined3d_private_store private_store;
    IDXGIDevice *device;
    struct wined3d_texture *wined3d_texture;
    HDC dc;
};

HRESULT dxgi_surface_init(struct dxgi_surface *surface, IDXGIDevice *device,
        IUnknown *outer, struct wined3d_texture *wined3d_texture) DECLSPEC_HIDDEN;

D3D_FEATURE_LEVEL dxgi_check_feature_level_support(struct dxgi_factory *factory, struct dxgi_adapter *adapter,
        const D3D_FEATURE_LEVEL *feature_levels, unsigned int level_count) DECLSPEC_HIDDEN;

#endif /* __WINE_DXGI_PRIVATE_H */
