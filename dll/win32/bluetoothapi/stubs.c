/*
 * PROJECT:     ReactOS DWM Compatibility Layer
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Stub file for bluetoothapis
 * COPYRIGHT:   Copyright 2022 Justin Miller <justinmiller100@gmail.com>
 */

#include "bluetoothapis_private.h"
//#define NDEBUG
#include <debug.h>

HRESULT BluetoothGATTGetServices(
  HANDLE               hDevice,
  USHORT               ServicesBufferCount,
  ULONG_PTR ServicesBuffer,
  USHORT               *ServicesBufferActual,
  ULONG                Flags
)
{
    return 0;
}