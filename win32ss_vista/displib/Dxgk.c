/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DISPLIB static library
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include "pdisplib.h"
//#define NDEBUG
#include <debug.h>

/*
 * This should probably be inside of the SDK/LIB/DRIVERS, but I'm keeping it here
 * due to the need of the LDDM headers :).
 */

/*
 * Some information:
 * First off, unless this library is totally implemented 100% stable, WDDM drivers
 * compiled with the ReactOS toolchain will never start. This is because
 * as far as i can tell the managment of DXGKRNL is mostly done within this library
 * Starting the driver, prepping function call backs to pass to dxgkrnl are all done here.
 *
 * this also means that DXGKRNL as a service doesn't start unless a driver has invoked it
 * as of this recent commit this behavior is now true on ReactOS as well :).
 */

/*
 * HALF-IMPLEMENTED;
 */
NTSTATUS
NTAPI
DxgkInitialize(
  _In_ PDRIVER_OBJECT              DriverObject,
  _In_ PUNICODE_STRING             RegistryPath,
  _In_ PDRIVER_INITIALIZATION_DATA DriverInitializationData
)
{
    DPRINT1("Entering DxgkInitialize\n");

    NTSTATUS Status;
    FILE_OBJECT *FileObject;
    DEVICE_OBJECT *DeviceObject;
    Status = RDDM_LoadDxgkrnl(&FileObject, &DeviceObject);
    RDDM_SetupIoControlReq(DeviceObject);
    DPRINT1("RDDM_SetupIoControlReq called");
    return Status;
}
