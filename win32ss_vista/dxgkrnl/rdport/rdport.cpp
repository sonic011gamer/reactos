/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     RDDM Miniport Standard Setup Routines
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#include <dxgkrnl.h>
//#define NDEBUG
#include <debug.h>

/*
 * @ UNIMPLEMENTED
 */
NTSTATUS
NTAPI
RdPortSetupGlobalState()
{
    /**
     * FIXME: Windows seems to dedicate an entire routine to setting up the gloal
     *  variables needed for DxgKrnl miniport handling. This needs to be implemented.
     *
     *  Vista :
     *  1) Checks registry for: DisableUSWC, DisableStop, DisableDMD
     *  and saves state globally
     *
     *  2) Fills in the GraphicsDrivers registry entry
     *
     *  3) Saves DxgKrnl versioning
     *
     *  4) Saves ram amount for later use in DxgkCbGetDeviceInformation
     *
     *  5) Some other things i need to reverse what's going on -
     *
     *  6) Invokes SMgr to save DpiSession callback.
     *  This specifically is very important because down the line after win32k loads
     *  SMgr in watchdog gets triggered again to begin reading from the adapter list and running
     *  ssStartAdapter on every single initialized miniport drier
     */

    /* doesnt matter for our baby dxgkrnl yet */
    return STATUS_SUCCESS;
}
