#ifndef _VMSSS_H_INCLUDED_
#define _VMSSS_H_INCLUDED_

/* PSDK/NDK Headers */
#include <stdio.h>
#define WIN32_NO_STATUS
#include <windef.h>
#include <winbase.h>
#define NTOS_MODE_USER
#include <ndk/setypes.h>
#include <ndk/sefuncs.h>
#include <ndk/exfuncs.h>
#include <ndk/obfuncs.h>
#include <ndk/lpcfuncs.h>
#include <ndk/umfuncs.h>
#include <ndk/psfuncs.h>
#include <ndk/rtlfuncs.h>

/* CSR Header */
#include <csr/csrsrv.h>

/* Subsystem Manager Header */
#include <sm/smmsg.h>

/* VMSSS Headers */
#include "vmsmsg.h"

/* Defines */
#define SM_REG_KEY \
    L"\\Registry\\Machine\\System\\CurrentControlSet\\Control\\Session Manager"

#define SESSION_ROOT        L"\\Sessions"
#define GLOBAL_ROOT         L"\\GLOBAL??"
#define SYMLINK_NAME        L"SymbolicLink"
#define SB_PORT_NAME        L"SbApiPort"
#define CSR_PORT_NAME       L"ApiPort"
#define UNICODE_PATH_SEP    L"\\"

/*
 * Server DLL structure
 */
typedef
NTSTATUS
(NTAPI *TEST)(
    IN OUT PCSR_API_MESSAGE ApiMessage,
    IN OUT PCSR_REPLY_CODE  ReplyCode OPTIONAL
);

#endif /* ndef _VMSSS_H_INCLUDED_ */