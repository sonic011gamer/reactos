#pragma once
#include <wine/config.h>

#include <ntstatus.h>
#define WIN32_NO_STATUS

#include <wine/debug.h>

#define WIN32_NO_STATUS
#define _INC_WINDOWS
#define COM_NO_WINDOWS_H
#include <stdarg.h>
#include <windef.h>
#include <winbase.h>
#include <winreg.h>
#include <wingdi.h>
#include <winnls.h>
#include <winuser.h>
#include <setupapi.h>
#include <ddraw.h>
#include <initguid.h>
#include <devguid.h>
#include <strsafe.h>
#include <ntsecapi.h>
#include <bcrypt.h>
#include <math.h>
#include <d3dkmthk.h>
#include <limits.h>
#include <rtlfuncs.h>
#include <devpropdef.h>

typedef UINT D3DDDI_VIDEO_PRESENT_SOURCE_ID;

typedef UINT D3DKMT_HANDLE;

typedef struct _D3DKMT_CHECKMONITORPOWERSTATE {
  D3DKMT_HANDLE                  hAdapter;
  D3DDDI_VIDEO_PRESENT_SOURCE_ID VidPnSourceId;
} D3DKMT_CHECKMONITORPOWERSTATE;


typedef struct _D3DKMT_OPENADAPTERFROMLUID {
        LUID          AdapterLuid;
		D3DKMT_HANDLE hAdapter;
} D3DKMT_OPENADAPTERFROMLUID;

typedef enum _D3DKMT_MEMORY_SEGMENT_GROUP {
  D3DKMT_MEMORY_SEGMENT_GROUP_LOCAL,
  D3DKMT_MEMORY_SEGMENT_GROUP_NON_LOCAL
} D3DKMT_MEMORY_SEGMENT_GROUP;

typedef struct _D3DKMT_QUERYVIDEOMEMORYINFO {
  PVOID D3DKMT_PTR;
  D3DKMT_HANDLE               hAdapter;
  D3DKMT_MEMORY_SEGMENT_GROUP MemorySegmentGroup;
  UINT64       Budget;
  UINT64       CurrentUsage;
  UINT64       CurrentReservation;
  UINT64       AvailableForReservation;
  UINT                        PhysicalAdapterIndex;
} D3DKMT_QUERYVIDEOMEMORYINFO;