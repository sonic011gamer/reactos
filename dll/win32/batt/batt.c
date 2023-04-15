/*
 * PROJECT:     ReactOS system libraries
 * LICENSE:     GPL - See COPYING in the top level directory
 * FILE:        dll\win32\batt\batt.c
 * PURPOSE:     Battery Class installers
 * PROGRAMMERS: Copyright 2010 Eric Kohl
 */


#define WIN32_NO_STATUS
#include <stdarg.h>
#include <windef.h>
#include <winbase.h>
#include <winreg.h>
#include <winuser.h>
#include <setupapi.h>

#include <initguid.h>
#include <devguid.h>

#define NDEBUG
#include <debug.h>

static
DWORD
InstallCompositeBattery(
    _In_ HDEVINFO DeviceInfoSet,
    _In_opt_ PSP_DEVINFO_DATA DeviceInfoData,
    _In_ PSP_DEVINSTALL_PARAMS_W DeviceInstallParams)
{

    return 0;
}


DWORD
WINAPI
BatteryClassInstall(
    _In_ DI_FUNCTION InstallFunction,
    _In_ HDEVINFO DeviceInfoSet,
    _In_opt_ PSP_DEVINFO_DATA DeviceInfoData)
{
    SP_DEVINSTALL_PARAMS_W DeviceInstallParams;
    DWORD dwError;

    DPRINT("BatteryClassInstall(%u %p %p)\n",
           InstallFunction, DeviceInfoSet, DeviceInfoData);

    if (InstallFunction != DIF_INSTALLDEVICE)
        return ERROR_DI_DO_DEFAULT;

    DeviceInstallParams.cbSize = sizeof(DeviceInstallParams);
    if (!SetupDiGetDeviceInstallParamsW(DeviceInfoSet,
                                        DeviceInfoData,
                                        &DeviceInstallParams))
    {
        DPRINT1("SetupDiGetDeviceInstallParamsW() failed (Error %lu)\n", GetLastError());
        return GetLastError();
    }

    /* Install the composite battery device */
    dwError = InstallCompositeBattery(DeviceInfoSet,
                                      DeviceInfoData,
                                      &DeviceInstallParams);
    if (dwError == ERROR_SUCCESS)
    {
        /* Install the battery device */
        dwError = ERROR_DI_DO_DEFAULT;
    }

    return dwError;
}


DWORD
WINAPI
BatteryClassCoInstaller(IN DI_FUNCTION InstallFunction,
                        IN HDEVINFO DeviceInfoSet,
                        IN PSP_DEVINFO_DATA DeviceInfoData OPTIONAL,
                        IN OUT PCOINSTALLER_CONTEXT_DATA Context)
{
    switch (InstallFunction)
    {
        default:
            DPRINT("Install function %u ignored\n", InstallFunction);
            return ERROR_DI_DO_DEFAULT;
    }
}


BOOL
WINAPI
DllMain(
    _In_ HINSTANCE hinstDll,
    _In_ DWORD dwReason,
    _In_ LPVOID reserved)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDll);
            break;

        case DLL_PROCESS_DETACH:
            break;
    }

   return TRUE;
}

/* EOF */
