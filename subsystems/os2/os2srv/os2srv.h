#ifndef _OS2SRV_H_INCLUDED_
#define _OS2SRV_H_INCLUDED_

/* PSDK/NDK Headers */
#include <stdio.h>
//#include <windows.h>

#define NTOS_MODE_USER
#include <ndk/setypes.h>
#include <ndk/sefuncs.h>
#include <ndk/exfuncs.h>
#include <ndk/obfuncs.h>
#include <ndk/lpcfuncs.h>
#include <ndk/umfuncs.h>
#include <ndk/psfuncs.h>
#include <ndk/rtlfuncs.h>
#include <ndk/lpctypes.h>

#include <csr/csrsrv.h>
#include <csr/csr.h>
#include <csr/csrmsg.h>

#endif /* ndef _OS2SSRV_H_INCLUDED_ */
