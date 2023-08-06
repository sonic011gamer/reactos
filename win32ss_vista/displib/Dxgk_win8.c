/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DISPLIB static library windows 8+
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include "pdisplib.h"
//#define NDEBUG
#include <debug.h>


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

    /* seems kinda stupid i know i know, but why waste code lol? it's literally just moving a pointer around */
    return DxgkInitialize(DriverObject,
                          RegistryPath,
                          (PDRIVER_INITIALIZATION_DATA)KmdDodInitializationData);
}
