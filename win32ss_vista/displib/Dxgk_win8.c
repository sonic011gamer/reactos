/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DISPLIB static library windows 8+
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include "pdisplib.h"
//#define NDEBUG
#include <debug.h>

NTSTATUS
NTAPI
RDDM_StartDevice(
    _In_  const PVOID          MiniportDeviceContext,
    _In_  PDXGK_START_INFO     DxgkStartInfo,
    _In_  PDXGKRNL_INTERFACE   DxgkInterface,
    _Out_ PULONG               NumberOfVideoPresentSources,
    _Out_ PULONG               NumberOfChildren);
extern PDXGKDDI_START_DEVICE                   PublicStartDevice;
extern DXGKRNL_INTERFACE                       PublicDxgkrnlInterface;
/*
 * TODO: Implement windows 8+ style initalization for
 * WDDM miniport drivers.
 */

/* Note two: */
/* During the setup of Windows 8+ there's a new IOCTRL that we're not exactly sure what it's name is */
/*
 * This value does a similar goal of IOCTL_VIDEO_DDI_FUNC_REGISTER but i haven't tried to figure out what
 * it's name etc should be yet and behaviors. Will do this when i implement the windows 8 Startup sequence.
 * 0x230047u <-
 * for more information see this webpage
 * https://win8displayarchitecture.blogspot.com/2012/02/win8developerpreviewdisplayarchitecture.html
 */
NTSTATUS
NTAPI
DxgkInitializeDisplayOnlyDriver(
  _In_ PDRIVER_OBJECT              DriverObject,
  _In_ PUNICODE_STRING             RegistryPath,
  _In_ PKMDDOD_INITIALIZATION_DATA KmdDodInitializationData)
{
    DPRINT1("DxgkInitializeDisplayOnlyDriver: Enter\n");
    NTSTATUS (NTAPI *DpiInitialize)(PDRIVER_OBJECT, PUNICODE_STRING, PKMDDOD_INITIALIZATION_DATA);
    PKMDDOD_INITIALIZATION_DATA LocDriverInitializationData;
    UNICODE_STRING DxgkrnlServiceName;
    DEVICE_OBJECT *DxgkrnlDeviceObject;
    IO_STATUS_BLOCK IoStatusBlock;
    UNICODE_STRING DeviceName;
    NTSTATUS Status;
    KEVENT Event;
    IRP *Irp;

    DxgkrnlDeviceObject = 0;
    DpiInitialize = NULL;
    DPRINT("Displib: DxgkInitialize - Starting a RDDM Driver\n");
    if (!DriverObject,
        !RegistryPath)
    {
        DPRINT("DriverObject or RegistryPath is NULL\n");
        return STATUS_INVALID_PARAMETER;
    }

    /* Later in this function we make changes to this structure, so let's keep a copy of the unmodified one */
    LocDriverInitializationData = KmdDodInitializationData;
    if (KmdDodInitializationData)
    {

        /* It appears Windows will actually fail if the Miniport is below a specific version - we don't care */
        DPRINT("Displib: This WDDM Miniport version is %X", KmdDodInitializationData->Version);

        /* First load DXGKrnl itself */
        RtlInitUnicodeString(&DxgkrnlServiceName, L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\DXGKrnl");
        Status = ZwLoadDriver(&DxgkrnlServiceName);
        if (Status == STATUS_SUCCESS)
        {
            /* Okay we suceeded, Go ahead and grab the DxgkrnlDeviceObject */
            RtlInitUnicodeString(&DeviceName, L"\\Device\\DxgKrnl");
            Status = IoGetDeviceObjectPointer(&DeviceName,
                                              FILE_ALL_ACCESS,
                                              (PFILE_OBJECT*)&KmdDodInitializationData,
                                              &DxgkrnlDeviceObject);
            if (Status != STATUS_SUCCESS)
            {
                RDDM_UnloadDxgkrnl(&DxgkrnlServiceName);
                return Status;
            }
            /* Grab a function pointer to DpiInitialize via IOCTRL */
            KeInitializeEvent(&Event, NotificationEvent, 0);
            Irp = IoBuildDeviceIoControlRequest(RDDM_FindIoControlCode(),
                                                DxgkrnlDeviceObject,
                                                NULL,
                                                0,
                                                &DpiInitialize,
                                                sizeof(DpiInitialize),
                                                TRUE,
                                                &Event,
                                                &IoStatusBlock);

            /* Can't continue without being able to call this routine */
            if (!Irp)
            {
              Status = STATUS_INSUFFICIENT_RESOURCES;
              /* So fail */
              RDDM_UnloadDxgkrnl(&DxgkrnlServiceName);
              return Status;
            }

            Status = IofCallDriver(DxgkrnlDeviceObject, Irp);
        }

        /* Execute the thing */
        if ( Status != STATUS_SUCCESS)
        {
            RDDM_UnloadDxgkrnl(&DxgkrnlServiceName);
        }
        else
        {
          DPRINT("Displib: Custom RDDM Driver has passed - IOCTL_VIDEO_DDI_FUNC_REGISTER sent\n");
          /* Intercept the StartDevice Miniport routine so we can grab a public version of the Dxgkrnl interface */
          PublicStartDevice = LocDriverInitializationData->DxgkDdiStartDevice;
          LocDriverInitializationData->DxgkDdiStartDevice = (PDXGKDDI_START_DEVICE)RDDM_StartDevice;
          Status = DpiInitialize(DriverObject, RegistryPath, LocDriverInitializationData);
          DPRINT1("Displib: return from DpiInitialize Success\n");
          return Status;
        }
        return Status;
    }
    return STATUS_INVALID_PARAMETER;
}
