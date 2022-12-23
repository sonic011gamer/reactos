
#define WIN32_NO_STATUS
#define WIN32_LEAN_AND_MEAN

#include <dshow.h>
//#i
#include <devioctl.h>
#include <stdio.h>
#include <stdarg.h>
#include <setupapi.h>
#include <assert.h>
#include <windef.h>
#include <winbase.h>
#include <ndk/rtlfuncs.h>
#include <wingdi.h>
#include <winreg.h>
#include <d3dkmthk.h>
//#include <winddi.h>
#include <devguid.h>
typedef UINT D3DKMT_HANDLE;

typedef struct _D3DKMT_OPENADAPTERFROMLUID
{
    LUID            AdapterLuid;
    D3DKMT_HANDLE   hAdapter;
} D3DKMT_OPENADAPTERFROMLUID;
__kernel_entry
DWORD
APIENTRY
NtGdiDdDDICreateDCFromMemory(
    _Inout_ D3DKMT_CREATEDCFROMMEMORY *desc);

__kernel_entry
DWORD
APIENTRY
NtGdiDdDDIDestroyDCFromMemory(
    _In_ CONST D3DKMT_DESTROYDCFROMMEMORY *desc);
NTSTATUS WINAPI NtGdiDdDDIOpenAdapterFromLuid( D3DKMT_OPENADAPTERFROMLUID *desc );

DEFINE_GUID(DEVPROPKEY_GPU_LUID, 0x60b193cb, 0x5276, 0x4d0f, 0x96, 0xfc, 0xf1, 0x73, 0xab, 0xad, 0x3e, 0xc6);
/***********************************************************************
 *           D3DKMTCreateDCFromMemory    (GDI32.@)
 */
DWORD WINAPI D3DKMTCreateDCFromMemory( D3DKMT_CREATEDCFROMMEMORY *desc )
{
    return NtGdiDdDDICreateDCFromMemory( desc );
}

DWORD WINAPI D3DKMTDestroyDCFromMemory( const D3DKMT_DESTROYDCFROMMEMORY *desc )
{
    return NtGdiDdDDIDestroyDCFromMemory( desc );
}

typedef UINT D3DKMT_HANDLE;
typedef UINT  D3DDDI_VIDEO_PRESENT_SOURCE_ID;
typedef struct _D3DKMT_OPENADAPTERFROMGDIDISPLAYNAME
{
    WCHAR                           DeviceName[32]; // in:  Name of GDI device from which to open an adapter instance
    D3DKMT_HANDLE                   hAdapter;       // out: adapter handle
    LUID                            AdapterLuid;    // out: adapter LUID
    D3DDDI_VIDEO_PRESENT_SOURCE_ID  VidPnSourceId;  // out: VidPN source ID for that particular display
} D3DKMT_OPENADAPTERFROMGDIDISPLAYNAME;

typedef PVOID HINF;
typedef PVOID HDSKSPC;
typedef PVOID HDEVINFO;
typedef PVOID HSPFILEQ;
typedef PVOID HSPFILELOG;
typedef UINT DI_FUNCTION;

typedef GUID  DEVPROPGUID, *PDEVPROPGUID;
typedef ULONG DEVPROPID,   *PDEVPROPID;
typedef struct _DEVPROPKEY {
    DEVPROPGUID fmtid;
    DEVPROPID   pid;
} DEVPROPKEY, *PDEVPROPKEY;


typedef ULONG DEVPROPTYPE, *PDEVPROPTYPE;

typedef unsigned char BYTE;
BOOL WINAPI SetupDiGetDevicePropertyW(HDEVINFO devinfo, PSP_DEVINFO_DATA device_data,
                const DEVPROPKEY *prop_key, DEVPROPTYPE *prop_type, BYTE *prop_buff,
                DWORD prop_buff_size, DWORD *required_size, DWORD flags);


typedef ULONG DEVPROPTYPE, *PDEVPROPTYPE;

/* Yes because the way wine handles this is truly revolutionary */

static HANDLE get_display_device_init_mutex( void )
{
    HANDLE mutex = CreateMutexW( NULL, FALSE, L"display_device_init" );

    WaitForSingleObject( mutex, INFINITE );
    return mutex;
}
 
 static void release_display_device_init_mutex( HANDLE mutex )
 {
     ReleaseMutex( mutex );
     CloseHandle( mutex );
 }

NTSTATUS WINAPI D3DKMTOpenAdapterFromGdiDisplayName( D3DKMT_OPENADAPTERFROMGDIDISPLAYNAME *desc )
{
    #if 0
    WCHAR *end, key_nameW[MAX_PATH], bufferW[MAX_PATH];
    HDEVINFO devinfo = INVALID_HANDLE_VALUE;
    NTSTATUS status = STATUS_UNSUCCESSFUL;
    D3DKMT_OPENADAPTERFROMLUID luid_desc;
    SP_DEVINFO_DATA device_data;
    DWORD size, state_flags;
    DEVPROPTYPE type;
    HANDLE mutex;
    int index;


    if (!desc)
        return STATUS_UNSUCCESSFUL;

    if (wcsnicmp( desc->DeviceName, L"\\\\.\\DISPLAY", lstrlenW(L"\\\\.\\DISPLAY") ))
        return STATUS_UNSUCCESSFUL;

    index = wcstol( desc->DeviceName + lstrlenW(L"\\\\.\\DISPLAY"), &end, 10 ) - 1;
    if (*end)
        return STATUS_UNSUCCESSFUL;

    /* Get adapter LUID from SetupAPI */
    mutex = get_display_device_init_mutex();

    size = sizeof( bufferW );
    swprintf( key_nameW, (const wchar_t *)MAX_PATH, (const wchar_t *)L"\\Device\\Video%d", index );
    if (RegGetValueW( HKEY_LOCAL_MACHINE, L"HARDWARE\\DEVICEMAP\\VIDEO", key_nameW,
                      RRF_RT_REG_SZ, NULL, bufferW, &size ))
        goto done;

    /* Strip \Registry\Machine\ prefix and retrieve Wine specific data set by the display driver */
    lstrcpyW( key_nameW, bufferW + 18 );
    size = sizeof( state_flags );
    if (RegGetValueW( HKEY_CURRENT_CONFIG, key_nameW, L"StateFlags", RRF_RT_REG_DWORD, NULL,
                      &state_flags, &size ))
        goto done;

    if (!(state_flags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
        goto done;

    size = sizeof( bufferW );
    if (RegGetValueW( HKEY_CURRENT_CONFIG, key_nameW, L"GPUID", RRF_RT_REG_SZ, NULL, bufferW, &size ))
        goto done;

    devinfo = SetupDiCreateDeviceInfoList( &GUID_DEVCLASS_DISPLAY, NULL );
    device_data.cbSize = sizeof( device_data );
    SetupDiOpenDeviceInfoW( devinfo, bufferW, NULL, 0, &device_data );
    if (!SetupDiGetDevicePropertyW( devinfo, &device_data, (const DEVPROPKEY *)&DEVPROPKEY_GPU_LUID, &type,
                                    (BYTE *)&luid_desc.AdapterLuid, sizeof( luid_desc.AdapterLuid ),
                                    NULL, 0))
        goto done;

   // if ((status = NtGdiDdDDIOpenAdapterFromLuid( &luid_desc ))) goto done;

    desc->hAdapter = luid_desc.hAdapter;
    desc->AdapterLuid = luid_desc.AdapterLuid;
    desc->VidPnSourceId = index;

done:
    SetupDiDestroyDeviceInfoList( devinfo );
    release_display_device_init_mutex( mutex );
    return status;
    #endif
    return STATUS_SUCCESS;
}