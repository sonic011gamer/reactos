/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DISPLIB static library
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include <ntddk.h>
#include <windef.h>
#include <winerror.h>
#include <stdio.h>
#include <locd3d.h>
#include <wdm.h>


NTSTATUS
RDDM_LoadDxgkrnl(FILE_OBJECT **FileObject, DEVICE_OBJECT **DeviceObject);

VOID
RDDM_SetupIoControlReq(DEVICE_OBJECT *DeviceObject);
