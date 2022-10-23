#pragma once

/* C Headers */
#define _CTYPE_DISABLE_MACROS
#define _CRT_SECURE_NO_DEPRECATE
#define _INC_SWPRINTF_INL_
#include <stdio.h>


#define WIN32_NO_STATUS
#include <windef.h>
#include <winbase.h>
#include <winreg.h>
#include <winuser.h>
#include <dpfilter.h>

#define NTOS_MODE_USER
#include <ndk/cmfuncs.h>
#include <ndk/exfuncs.h>
#include <ndk/iofuncs.h>
#include <ndk/kefuncs.h>
#include <ndk/ldrfuncs.h>
#include <ndk/mmfuncs.h>
#include <ndk/obfuncs.h>
#include <ndk/psfuncs.h>
#include <ndk/rtlfuncs.h>
#include <ndk/umfuncs.h>

/* CSRSS Headers */
#include <csr/csr.h>

#undef STDCALL
#define STDCALL __stdcall