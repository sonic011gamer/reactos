/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     Primary Multi-GPU Handling
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <include/dxgkrnl.h>
//#define NDEBUG
#include <debug.h>

/*
 * This is where things get spicy: taking a PoC into a actual driver.
 * Allow me to explain:
 * DXGGLOBAL -> is the primary class that contains information about everything in the system.
 * DXGADAPTER -> is the individual adapters
 * When for a command is called, it creates a handle ffor the adapter, which is used to index into the DXGADAPTER
 * list stored right inside DXGGLOBAL itself. This will be a primitive version of that functionality
 * We've seperated all the generic driver handling away into classes. 
 * 
 * Entry.cpp/kmdod.cpp/rport and friends are the NT side. This is the *TRUE* driver entry point so to speak.
 * 
 */

DXGGLOBAL* GlobalDxgGlobal;

DXGGLOBAL::DXGGLOBAL()
{

}

DXGGLOBAL*
DXGGLOBAL::GetGlobal(VOID)
{
    /* Nothing else to do here... */
    return GlobalDxgGlobal;
}

NTSTATUS
DXGGLOBAL::CreateGlobal(VOID)
{
    return STATUS_SUCCESS;
}

NTSTATUS
DXGGLOBAL::DestroyGlobal(VOID)
{
   // GlobalDxgGlobal->~DXGGLOBAL();
    return TRUE;
}

DXGADAPTER
DXGGLOBAL::ReferenceAdapterByLuid(LUID TargetLuid)
{
    //HACK: Work on multiple adapters in one system. this is a hack for now
    return *GlobalAdapterList;
}

BOOLEAN
DXGGLOBAL::CheckExclusiveOwnership(VOID)
{
   UNIMPLEMENTED;
    __debugbreak();
    return FALSE;
}

NTSTATUS
DXGGLOBAL::CreateAdapter(PDEVICE_OBJECT AdapterDeviceObject, DXGADAPTER **DxgAdapter)
{
   UNIMPLEMENTED;
    __debugbreak();
    return STATUS_UNSUCCESSFUL;
}

/*
 * we don't acutally have a watchdog or anything useful
 * so this will stay a stub for the foreseeable future.
 */
VOID
DXGGLOBAL::ReportState()
{
    UNIMPLEMENTED;
}