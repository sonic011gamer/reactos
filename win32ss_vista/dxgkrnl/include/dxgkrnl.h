/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     Core Header
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#pragma once

#include <ntddk.h>
#include <windef.h>
#include <ntstatus.h>

#include <stdio.h>
#include <locd3d.h>

/* Create an IO request to fill out the function pointer list */
#define IOCTL_VIDEO_DDI_FUNC_REGISTER \
	CTL_CODE( FILE_DEVICE_VIDEO, 0xF, METHOD_NEITHER, FILE_ANY_ACCESS  )

//FIXME: ReactOS Specific
#define IOCTL_VIDEO_I_AM_REACTOS \
	CTL_CODE( FILE_DEVICE_VIDEO, 0xB, METHOD_NEITHER, FILE_ANY_ACCESS  )

