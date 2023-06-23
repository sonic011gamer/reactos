#pragma once

/* Internal APIs */
NTSTATUS
NTAPI
RDDM_SetupDxgkrnl(
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath);


#include "miniport_dispatch.h"
#include "dxgkrnl_interface.h"