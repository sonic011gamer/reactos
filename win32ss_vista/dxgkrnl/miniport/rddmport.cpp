/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     RDDM Miniport IOCTRL Calls
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */


#include <dxgkrnl.h>
//#define NDEBUG
#include <debug.h>

extern DXGKRNL_INTERFACE DxgkrnlInterface;

NTSTATUS
NTAPI
RDDM_MiniportDispatchCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
NTAPI
RDDM_MiniportDispatchPnp(PDEVICE_OBJECT DeviceObject, PVOID Tag)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

PSTR
NTAPI
RDDM_MiniportDispatchPower(PDEVICE_OBJECT DeviceObject, PSTR MutableMessage)
{
    UNIMPLEMENTED;
    __debugbreak();
    return MutableMessage;
}

NTSTATUS
NTAPI
RDDM_MiniportDispatchIoctl(PDEVICE_OBJECT DeviceObject, IRP *Irp)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}


NTSTATUS
NTAPI
RDDM_MiniportDispatchInternalIoctl(PDEVICE_OBJECT DeviceObject, IRP *Irp)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
NTAPI
RDDM_MiniportDispatchSystemControl(PDEVICE_OBJECT DeviceObject, PVOID Tag)
{
        UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

NTSTATUS
NTAPI
RDDM_MiniportDispatchClose(PDEVICE_OBJECT DeviceObject)
{
        UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

void
internalCallObject(PDXGKRNL_PRIVATE_EXTENSION Extension, DEVICE_OBJECT *PhysicalDeviceObject
                    , PVOID MiniportDeviceContext)
{
    NTSTATUS Status;

    /* i dont think that's how these work, at all */
    __try
    {
        __debugbreak();
        Status = Extension->DxgkDdiAddDevice(PhysicalDeviceObject, (PVOID*)&MiniportDeviceContext);
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        DPRINT1("Cannot call DxgkDdiAddDevice\n");
       // return STATUS_UNSUCCESSFUL;
    }


    DPRINT1("DxgkDdiAddDevice: Returned with Status: %d\n", Status);
    __debugbreak();
}
/**
 * @brief Intercepts and calls the AddDevice Miniport call back
 *
 * @param DriverObject - Pointer to DRIVER_OBJECT structure
 *
 * @param TargetDevice - Pointer to DEVICE_OBJECT structure
 *
 * @return NTSTATUS
 *
 *  HALF-IMPLEMENTED
 */

NTSTATUS
NTAPI
RDDM_MiniportAddDevice(_In_     DRIVER_OBJECT *DriverObject,
                       _Inout_  DEVICE_OBJECT *PhysicalDeviceObject)
{

	PDEVICE_OBJECT Fdo;
    IO_STATUS_BLOCK IoStatusBlock;
    NTSTATUS Status ;
    ULONG_PTR Context = 0;
    PDXGKRNL_PRIVATE_EXTENSION Extension = NULL;

    PAGED_CODE();

    /* MS does a whole bunch of bullcrap here so we will try to track it */
    if (!DriverObject || !PhysicalDeviceObject)
    {
        DPRINT1("RDDM_MiniportAddDevice: Something has seriously fucked up\n");
        return STATUS_INVALID_PARAMETER;
    }

    /*
     * This routine implemented the Miniport AddDevice along with trying enumerate what
     * the GPU is "attached to", On the off chance a GPU isn't enumerated over PCI we use ACPI here,
     * This routine also decides when to use MSI-(X) Interrupts; A Royal pain in the butt.
     *
     * 1) First lets get the device extension for DXGKRNL
     * 2) nextlet's call a private API to handle calling Add Device.
     * 3) After that we will call IoCreateDevice to create the device
     */

    /* Grab the DXGKRNL internal extension */
    Extension = (PDXGKRNL_PRIVATE_EXTENSION)IoGetDriverObjectExtension(DriverObject, DriverObject);
    if (!Extension)
    {
        DPRINT1("Could not gather DXGKRNL Extension\n");
    }

    internalCallObject(Extension,PhysicalDeviceObject,(VOID*)Context);

    IoStatusBlock.Information = 1024;
    Status = IoCreateDevice(DriverObject,
                            IoStatusBlock.Information,
                            FALSE,
                            FILE_DEVICE_VIDEO,
                            FILE_DEVICE_SECURE_OPEN,
                            FALSE,
                            &Fdo);

    /*
     * Attach our FDO to the device stack.
     * The return value of IoAttachDeviceToDeviceStack is the top of the
     * attachment chain.  This is where all the IRPs should be routed.
     */
    PDEVICE_OBJECT NextLowerobject = IoAttachDeviceToDeviceStack (
                                    Fdo,
                                    PhysicalDeviceObject);
    /* Doesnt really make any sense to me why though? */
    if (!NextLowerobject)
    {
        DPRINT1("RDDM_MiniportAddDevice: IoAttachDeviceToDeviceStack has failed\n");
    }
    ULONG NumberOfViews;
    ULONG NumberOfChildren;
    DXGK_START_INFO DxgkStartInfo = {0};
    Extension->MiniportFdo = Fdo;
       Extension->MiniportPdo = PhysicalDeviceObject;
    Extension->DxgkDdiStartDevice((PVOID)Context,
                                  &DxgkStartInfo,
                                   &DxgkrnlInterface,
                                  &NumberOfViews,
                                  &NumberOfChildren);
    __debugbreak();
    return STATUS_SUCCESS;
}
#if 0
NTSTATUS
NTAPI
RDDM_MiniportAddDevice(PDRIVER_OBJECT DriverObject, PDEVICE_OBJECT TargetDevice)
{
    PAGED_CODE();

    PDXGKDDI_ADD_DEVICE                       LocDxgkDdiAddDevice;

    PDXGKRNL_PRIVATE_EXTENSION Extension = NULL;
    PVOID MiniportDeviceContext = NULL;
    NTSTATUS Status = 0;
    DPRINT1("PVOID RDDM_MiniportAddDevice: Entry\n");

    /* Grab the DXGKRNL internal extension */
    Extension = (PDXGKRNL_PRIVATE_EXTENSION)IoGetDriverObjectExtension(DriverObject, DriverObject);
    if (!Extension)
    {
        DPRINT1("Could not gather DXGKRNL Extension\n");
    }

    LocDxgkDdiAddDevice = Extension->DxgkDdiAddDevice;
    __debugbreak();
    /* Call the actual miniport routine */
    Status = (NTSTATUS)(LocDxgkDdiAddDevice(TargetDevice, &MiniportDeviceContext));
    DPRINT1("DxgkDdiAddDevice Status 0x%X\n", Status);
    __debugbreak();
    return Status;
}
#endif
NTSTATUS
NTAPI
RDDM_MiniportDriverUnload(PDRIVER_OBJECT DriverObject)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}
