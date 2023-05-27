/*
 * PROJECT:     ReactOS Posix Subsystem
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     BOOTPSX header
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include <windows.h>
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
#include <sm/helper.h>
#define NTOS_MODE_USER
#define RETRY_COUNT 3

NTSTATUS
PsxCheckSubSystem();

NTSTATUS
PsxBootstrap ();