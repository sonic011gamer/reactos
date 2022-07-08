/*
 * PROJECT:     ReactOS Audio Subsystem
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     WDM Audio Driver Mapper (User-mode part)
 * COPYRIGHT:   Copyright 2007-2008 Andrew Greenwood (silverblade@reactos.org)
 *              Copyright 2009-2010 Johannes Anderwald
 *              Copyright 2022 Oleg Dubinskiy (oleg.dubinskij30@gmail.com)
 */

/*
 *
 * NOTES:       Looking for wodMessage & co? You won't find them here. Try
 *              the MME Buddy library, which is where these routines are
 *              actually implemented.
 *
 */

#include "wdmaud.h"

#define YDEBUG
#include <debug.h>
#include <mmebuddy_debug.h>

LPWSTR gpszDeviceInterfaceString = NULL;
HANDLE KernelHandle = INVALID_HANDLE_VALUE;
DWORD OpenCount = 0;

PWDMAUD_DEVICE_INFO
AllocateDeviceInfo(
    IN LPWSTR DeviceInterfaceString)
{
    PWDMAUD_DEVICE_INFO DeviceInfo;

#if 0 // FIXME: For some reason this doesn't work in ReactOS...
    /* Validate the string */
    if (IsBadReadPtr(DeviceInterfaceString, 1))
    {
        SND_ERR(L"Device interface %ls is invalid\n", DeviceInterfaceString);
        return NULL;
    }
#endif

    /* Allocate a new WDMAUD_DEVICE_INFO structure */
    DeviceInfo = HeapAlloc(GetProcessHeap(),
                           HEAP_ZERO_MEMORY, 
                           sizeof(WDMAUD_DEVICE_INFO));
    if (!DeviceInfo)
    {
        SND_ERR(L"Failed to allocate WDMAUD_DEVICE_INFO structure\n");
        return NULL;
    }

    /* Store the string */
    DeviceInfo->DeviceInterfaceString = DeviceInterfaceString;

    /* Done */
    return DeviceInfo;
}

VOID
FreeDeviceInfo(
    IN  PWDMAUD_DEVICE_INFO DeviceInfo)
{
    if (DeviceInfo)
    {
        /* Free WDMAUD_DEVICE_INFO structure */
        HeapFree(GetProcessHeap(), 0, DeviceInfo);
    }
}

/*
    This is a wrapper around DeviceIoControl which provides control over
    instantiated sound devices. It waits for I/O to complete (since an
    instantiated sound device is opened in overlapped mode, this is necessary).
*/
MMRESULT
WdmAudIoControl(
    IN  PWDMAUD_DEVICE_INFO DeviceInfo,
    IN OUT OPTIONAL ULONG BufferSize,
    IN OUT OPTIONAL PVOID Buffer,
    IN  DWORD IoControlCode)
{
    OVERLAPPED Overlapped;
    BOOLEAN IoResult;
    DWORD Transferred;

    /* Overlapped I/O is done here - this is used for waiting for completion */
    ZeroMemory(&Overlapped, sizeof(OVERLAPPED));
    Overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    if ( ! Overlapped.hEvent )
        return Win32ErrorToMmResult(GetLastError());

    /* Store input data */
    DeviceInfo->BufferSize = BufferSize;
    DeviceInfo->Buffer = Buffer;

    /* Talk to the device */
    IoResult = DeviceIoControl(KernelHandle,
                               IoControlCode,
                               DeviceInfo,
                               sizeof(WDMAUD_DEVICE_INFO),
                               DeviceInfo,
                               sizeof(WDMAUD_DEVICE_INFO),
                               NULL,
                               &Overlapped);

    /* If failure occurs, make sure it's not just due to the overlapped I/O */
    if ( ! IoResult )
    {
        if ( GetLastError() != ERROR_IO_PENDING )
        {
            CloseHandle(Overlapped.hEvent);
            return Win32ErrorToMmResult(GetLastError());
        }
        else
        {
            WaitForSingleObject(Overlapped.hEvent, INFINITE);
        }
    }

    /* Wait for the I/O to complete */
    IoResult = GetOverlappedResult(KernelHandle,
                                   &Overlapped,
                                   &Transferred,
                                   TRUE);

    /* Don't need this any more */
    CloseHandle(Overlapped.hEvent);

    if ( ! IoResult )
        return Win32ErrorToMmResult(GetLastError());

    /* Store output data */
    BufferSize = DeviceInfo->BufferSize;
    Buffer = DeviceInfo->Buffer;

    SND_TRACE(L"Transferred %d bytes in Sync overlapped I/O\n", Transferred);

    return MMSYSERR_NOERROR;
}

DWORD
WINAPI
MixerEventThreadRoutine(
    LPVOID Parameter)
{
    HANDLE WaitObjects[2];
    DWORD dwResult;
    MMRESULT Result;
    PWDMAUD_DEVICE_INFO DeviceInfo;
    PSOUND_DEVICE_INSTANCE Instance = (PSOUND_DEVICE_INSTANCE)Parameter;

    /* Setup wait objects */
    WaitObjects[0] = Instance->hNotifyEvent;
    WaitObjects[1] = Instance->hStopEvent;

    /* Allocate device info */
    DeviceInfo = AllocateDeviceInfo(gpszDeviceInterfaceString);
    if (!DeviceInfo)
    {
        /* No memory */
        return 1;
    }

    DeviceInfo->hDevice = Instance->Handle;
    DeviceInfo->DeviceType = MIXER_DEVICE_TYPE;

    do
    {
        dwResult = WaitForMultipleObjects(2, WaitObjects, FALSE, INFINITE);

        if (dwResult == WAIT_OBJECT_0 + 1)
        {
            /* Stop event was signalled */
            break;
        }

        do
        {
            Result = WdmAudIoControl(DeviceInfo,
                                     0,
                                     NULL,
                                     IOCTL_GET_MIXER_EVENT);

            if (Result == MMSYSERR_NOERROR)
            {
                DriverCallback(Instance->WinMM.ClientCallback,
                               HIWORD(Instance->WinMM.Flags),
                               Instance->WinMM.Handle,
                               DeviceInfo->MixerEvent.NotificationType,
                               Instance->WinMM.ClientCallbackInstanceData,
                               (DWORD_PTR)DeviceInfo->MixerEvent.Value,
                               0);
            }
        }while(Result == MMSYSERR_NOERROR);
    }while(TRUE);

    FreeDeviceInfo(DeviceInfo);

    /* Done */
    return 0;
}

MMRESULT
WdmAudCleanupByLegacy(VOID)
{
    if (KernelHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(KernelHandle);
        KernelHandle = INVALID_HANDLE_VALUE;
    }

    return MMSYSERR_NOERROR;
}

MMRESULT
WdmAudGetNumWdmDevsByLegacy(
    IN  MMDEVICE_TYPE DeviceType,
    OUT DWORD* DeviceCount)
{
    MMRESULT Result;
    PWDMAUD_DEVICE_INFO DeviceInfo;

    VALIDATE_MMSYS_PARAMETER( KernelHandle != INVALID_HANDLE_VALUE );
    VALIDATE_MMSYS_PARAMETER( IS_VALID_SOUND_DEVICE_TYPE(DeviceType) );
    VALIDATE_MMSYS_PARAMETER( DeviceCount );
    SND_TRACE(L"Size of char is %lu\n", sizeof(char));
    SND_TRACE(L"WDMAUD - GetNumWdmDevs DeviceType %u\n", DeviceType);
    SND_TRACE(L"Size of WDMAUD_DEVICE_INFO is %lu\n", sizeof(WDMAUD_DEVICE_INFO));
    DeviceInfo = AllocateDeviceInfo(gpszDeviceInterfaceString);
    if (!DeviceInfo)
    {
        /* No memory */
        *DeviceCount = 0;
        return MMSYSERR_NOMEM;
    }

    DeviceInfo->DeviceType = DeviceType;

    Result = WdmAudIoControl(DeviceInfo,
                             0,
                             NULL,
                             IOCTL_GETNUMDEVS_TYPE);

    if ( ! MMSUCCESS( Result ) )
    {
        SND_ERR(L"Call to IOCTL_GETNUMDEVS_TYPE failed with %d\n", GetLastError());
        *DeviceCount = 0;
        FreeDeviceInfo(DeviceInfo);
        return TranslateInternalMmResult(Result);
    }

    *DeviceCount = DeviceInfo->DeviceNumber;

    FreeDeviceInfo(DeviceInfo);

    return MMSYSERR_NOERROR;
}

MMRESULT
WdmAudGetCapabilitiesByLegacy(
    IN  PSOUND_DEVICE SoundDevice,
    IN  DWORD DeviceId,
    OUT PVOID Capabilities,
    IN  DWORD CapabilitiesSize)
{
    MMRESULT Result;
    MMDEVICE_TYPE DeviceType;
    PWDMAUD_DEVICE_INFO DeviceInfo;

    SND_ASSERT( SoundDevice );
    SND_ASSERT( Capabilities );

    Result = GetSoundDeviceType(SoundDevice, &DeviceType);
    SND_ASSERT( Result == MMSYSERR_NOERROR );

    if ( ! MMSUCCESS(Result) )
        return Result;

    SND_TRACE(L"WDMAUD - GetWdmDeviceCapabilities DeviceType %u DeviceId %u\n", DeviceType, DeviceId);

    DeviceInfo = AllocateDeviceInfo(gpszDeviceInterfaceString);
    if (!DeviceInfo)
    {
        /* No memory */
        Capabilities = NULL;
        return MMSYSERR_NOMEM;
    }

    DeviceInfo->DeviceType = DeviceType;
    DeviceInfo->DeviceIndex = DeviceId;

    Result = WdmAudIoControl(DeviceInfo,
                             CapabilitiesSize,
                             Capabilities,
                             IOCTL_GETCAPABILITIES);

    FreeDeviceInfo(DeviceInfo);

    if ( ! MMSUCCESS(Result) )
    {
        SND_ERR(L"Call to IOCTL_GETCAPABILITIES failed with %d\n", GetLastError());
        Capabilities = NULL;
        return TranslateInternalMmResult(Result);
    }

    return MMSYSERR_NOERROR;
}

MMRESULT
WdmAudOpenKernelSoundDeviceByLegacy(VOID)
{
    DWORD dwSize;
    HDEVINFO hDevInfo;
    SP_DEVICE_INTERFACE_DATA DeviceInterfaceData;
    GUID SWBusGuid = {STATIC_KSCATEGORY_WDMAUD};
    PSP_DEVICE_INTERFACE_DETAIL_DATA_W DeviceInterfaceDetailData;

    if ( KernelHandle == INVALID_HANDLE_VALUE )
    {
        hDevInfo = SetupDiGetClassDevsW(&SWBusGuid, NULL, NULL,  DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
        if (!hDevInfo)
        {
            // failed
            return MMSYSERR_ERROR;
        }

        DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
        if (!SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &SWBusGuid, 0, &DeviceInterfaceData))
        {
            // failed
            SetupDiDestroyDeviceInfoList(hDevInfo);
            return MMSYSERR_ERROR;
        }

        SetupDiGetDeviceInterfaceDetailW(hDevInfo,  &DeviceInterfaceData, NULL, 0, &dwSize, NULL);

        DeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA_W)HeapAlloc(GetProcessHeap(), 0, dwSize);
        if (!DeviceInterfaceDetailData)
        {
            // failed
            SetupDiDestroyDeviceInfoList(hDevInfo);
            return MMSYSERR_ERROR;
        }

        DeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_W);
        if (!SetupDiGetDeviceInterfaceDetailW(hDevInfo,  &DeviceInterfaceData, DeviceInterfaceDetailData, dwSize, &dwSize, NULL))
        {
            // failed
            HeapFree(GetProcessHeap(), 0, DeviceInterfaceDetailData);
            SetupDiDestroyDeviceInfoList(hDevInfo);
            return MMSYSERR_ERROR;
        }

        gpszDeviceInterfaceString = DeviceInterfaceDetailData->DevicePath;

        SND_TRACE(L"Opening wdmaud device '%s'\n", gpszDeviceInterfaceString);
        KernelHandle = CreateFileW(gpszDeviceInterfaceString,
                                   GENERIC_READ | GENERIC_WRITE,
                                   0,
                                   NULL,
                                   OPEN_EXISTING,
                                   FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                                   NULL);

        HeapFree(GetProcessHeap(), 0, DeviceInterfaceDetailData);
        SetupDiDestroyDeviceInfoList(hDevInfo);
    }


    if ( KernelHandle == INVALID_HANDLE_VALUE )
        return MMSYSERR_ERROR;

    ++ OpenCount;
        return MMSYSERR_NOERROR;

}

MMRESULT
WdmAudOpenSoundDeviceByLegacy(
    IN PSOUND_DEVICE_INSTANCE SoundDeviceInstance,
    IN DWORD DeviceId,
    IN PWAVEFORMATEX WaveFormat,
    OUT PVOID *Handle)
{
    MMRESULT Result;
    HANDLE hThread;
    MMDEVICE_TYPE DeviceType;
    PSOUND_DEVICE SoundDevice;
    PWDMAUD_DEVICE_INFO DeviceInfo;

    Result = GetSoundDeviceFromInstance(SoundDeviceInstance, &SoundDevice);
    SND_ASSERT( SoundDevice );

    if ( ! MMSUCCESS(Result) )
    {
        return TranslateInternalMmResult(Result);
    }

    if ( OpenCount == 0 )
    {
        return MMSYSERR_NOERROR;
    }

    SND_ASSERT( KernelHandle != INVALID_HANDLE_VALUE );

    Result = GetSoundDeviceType(SoundDevice, &DeviceType);
    SND_ASSERT( Result == MMSYSERR_NOERROR );

    if ( ! MMSUCCESS(Result) )
        return Result;

    SND_TRACE(L"WDMAUD - OpenWdmSoundDevice DeviceType %u\n", DeviceType);

    if (DeviceType == MIXER_DEVICE_TYPE)
    {
        SoundDeviceInstance->hNotifyEvent = CreateEventW(NULL, FALSE, FALSE, NULL);
        if ( ! SoundDeviceInstance->hNotifyEvent )
            return MMSYSERR_NOMEM;

        if (SoundDeviceInstance->Handle != NULL)
        {
            /* Device is already opened */
            return MMSYSERR_NOERROR;
        }

        SoundDeviceInstance->hStopEvent = CreateEventW(NULL, FALSE, FALSE, NULL);
        if ( ! SoundDeviceInstance->hStopEvent )
            return MMSYSERR_NOMEM;
    }

    DeviceInfo = AllocateDeviceInfo(gpszDeviceInterfaceString);
    if (!DeviceInfo)
    {
        /* No memory */
        *Handle = NULL;
        return MMSYSERR_NOMEM;
    }

    DeviceInfo->DeviceType = DeviceType;
    DeviceInfo->DeviceIndex = DeviceId;

    Result = WdmAudIoControl(DeviceInfo,
                             sizeof(WAVEFORMATEX),
                             WaveFormat,
                             IOCTL_OPEN_WDMAUD);

    if ( ! MMSUCCESS( Result ) )
    {
        SND_ERR(L"Call to IOCTL_OPEN_WDMAUD failed with %d\n", GetLastError());
        *Handle = NULL;
        FreeDeviceInfo(DeviceInfo);
        return TranslateInternalMmResult(Result);
    }

    if (DeviceType == MIXER_DEVICE_TYPE)
    {
        hThread = CreateThread(NULL, 0, MixerEventThreadRoutine, (LPVOID)SoundDeviceInstance, 0, NULL);
        if (  hThread )
        {
            CloseHandle(hThread);
        }
    }

    *Handle = DeviceInfo->hDevice;

    if (DeviceType == WAVE_IN_DEVICE_TYPE || DeviceType == WAVE_OUT_DEVICE_TYPE)
    {
        /* Now determine framing requirements */
        Result = WdmAudIoControl(DeviceInfo,
                                 0,
                                 NULL,
                                 IOCTL_GETFRAMESIZE);

        if ( MMSUCCESS(Result) )
        {
            if (DeviceInfo->FrameSize)
            {
                SoundDeviceInstance->FrameSize = DeviceInfo->FrameSize * 2;
                SoundDeviceInstance->BufferCount = WaveFormat->nAvgBytesPerSec / SoundDeviceInstance->FrameSize;
                SND_TRACE(L"FrameSize %u BufferCount %u\n", SoundDeviceInstance->FrameSize, SoundDeviceInstance->BufferCount);
            }
        }
        else
        {
            /* Use a default of 100 buffers */
            SoundDeviceInstance->BufferCount = 100;
        }

        /* Now acquire resources */
        DeviceInfo->State = KSSTATE_ACQUIRE;
        WdmAudIoControl(DeviceInfo, 0, NULL, IOCTL_SETDEVICE_STATE);

        /* Pause the pin */
        DeviceInfo->State = KSSTATE_PAUSE;
        WdmAudIoControl(DeviceInfo, 0, NULL, IOCTL_SETDEVICE_STATE);

        /* Start the pin */
        DeviceInfo->State = KSSTATE_RUN;
        WdmAudIoControl(DeviceInfo, 0, NULL, IOCTL_SETDEVICE_STATE);
    }

    FreeDeviceInfo(DeviceInfo);

    return MMSYSERR_NOERROR;
}

MMRESULT
WdmAudCloseSoundDeviceByLegacy(
    IN  struct _SOUND_DEVICE_INSTANCE* SoundDeviceInstance,
    IN  PVOID Handle)
{
    MMRESULT Result;
    MMDEVICE_TYPE DeviceType;
    PSOUND_DEVICE SoundDevice;
    PWDMAUD_DEVICE_INFO DeviceInfo;

    Result = GetSoundDeviceFromInstance(SoundDeviceInstance, &SoundDevice);

    if ( ! MMSUCCESS(Result) )
    {
        return TranslateInternalMmResult(Result);
    }

    if ( OpenCount == 0 )
    {
        return MMSYSERR_NOERROR;
    }

    SND_ASSERT( KernelHandle != INVALID_HANDLE_VALUE );

    Result = GetSoundDeviceType(SoundDevice, &DeviceType);
    SND_ASSERT( Result == MMSYSERR_NOERROR );

    SND_TRACE(L"WDMAUD - CloseWdmSoundDevice DeviceType %u\n", DeviceType);

    if (SoundDeviceInstance->Handle != (PVOID)KernelHandle)
    {
        DeviceInfo = AllocateDeviceInfo(gpszDeviceInterfaceString);
        if (!DeviceInfo)
        {
            /* No memory */
            return MMSYSERR_NOMEM;
        }

        DeviceInfo->DeviceType = DeviceType;
        DeviceInfo->hDevice = SoundDeviceInstance->Handle;

        /* First stop the stream */
        if (DeviceType == WAVE_IN_DEVICE_TYPE || DeviceType == WAVE_OUT_DEVICE_TYPE)
        {
             DeviceInfo->State = KSSTATE_PAUSE;
             WdmAudIoControl(DeviceInfo, 0, NULL, IOCTL_SETDEVICE_STATE);

             DeviceInfo->State = KSSTATE_ACQUIRE;
             WdmAudIoControl(DeviceInfo, 0, NULL, IOCTL_SETDEVICE_STATE);

             DeviceInfo->State = KSSTATE_STOP;
             WdmAudIoControl(DeviceInfo, 0, NULL, IOCTL_SETDEVICE_STATE);
        }

        /* Then close device handle */
        WdmAudIoControl(DeviceInfo,
                        0,
                        NULL,
                        IOCTL_CLOSE_WDMAUD);

        FreeDeviceInfo(DeviceInfo);

        if ( ! MMSUCCESS(Result) )
        {
            SND_ERR(L"Call to IOCTL_CLOSE_WDMAUD failed with %d\n", GetLastError());
            return Result;
        }
    }

    if (DeviceType == MIXER_DEVICE_TYPE)
    {
        SetEvent(SoundDeviceInstance->hStopEvent);
        CloseHandle(SoundDeviceInstance->hStopEvent);
        CloseHandle(SoundDeviceInstance->hNotifyEvent);
    }

    --OpenCount;

    if ( OpenCount < 1 )
    {
        CloseHandle(KernelHandle);
        KernelHandle = INVALID_HANDLE_VALUE;
    }

    return MMSYSERR_NOERROR;
}

VOID
CALLBACK
LegacyCompletionRoutine(
    IN  DWORD dwErrorCode,
    IN  DWORD dwNumberOfBytesTransferred,
    IN  LPOVERLAPPED lpOverlapped)
{
    PSOUND_OVERLAPPED Overlap;
    PWDMAUD_DEVICE_INFO DeviceInfo;

    Overlap = (PSOUND_OVERLAPPED)lpOverlapped;
    DeviceInfo = (PWDMAUD_DEVICE_INFO)Overlap->CompletionContext;

    /* Call mmebuddy overlap routine */
    Overlap->OriginalCompletionRoutine(dwErrorCode, DeviceInfo->Header.DataUsed, lpOverlapped);

    HeapFree(GetProcessHeap(), 0, DeviceInfo);
}

MMRESULT
WdmAudCommitWaveBufferByLegacy(
    IN  PSOUND_DEVICE_INSTANCE SoundDeviceInstance,
    IN  PVOID OffsetPtr,
    IN  DWORD Length,
    IN  PSOUND_OVERLAPPED Overlap,
    IN  LPOVERLAPPED_COMPLETION_ROUTINE CompletionRoutine)
{
    HANDLE Handle;
    MMRESULT Result;
    PWDMAUD_DEVICE_INFO DeviceInfo;
    PSOUND_DEVICE SoundDevice;
    MMDEVICE_TYPE DeviceType;
    BOOL Ret;

    VALIDATE_MMSYS_PARAMETER( SoundDeviceInstance );
    VALIDATE_MMSYS_PARAMETER( OffsetPtr );
    VALIDATE_MMSYS_PARAMETER( Overlap );
    VALIDATE_MMSYS_PARAMETER( CompletionRoutine );

    GetSoundDeviceInstanceHandle(SoundDeviceInstance, &Handle);
    SND_ASSERT(Handle);

    Result = GetSoundDeviceFromInstance(SoundDeviceInstance, &SoundDevice);

    if ( ! MMSUCCESS(Result) )
    {
        return TranslateInternalMmResult(Result);
    }

    Result = GetSoundDeviceType(SoundDevice, &DeviceType);
    SND_ASSERT( Result == MMSYSERR_NOERROR );

    SND_TRACE(L"WDMAUD - CommitWaveBuffer DeviceType %u\n", DeviceType);

    DeviceInfo = AllocateDeviceInfo(gpszDeviceInterfaceString);
    if (!DeviceInfo)
    {
        /* No memory */
        return MMSYSERR_NOMEM;
    }

    DeviceInfo->Header.FrameExtent = Length;
    if (DeviceType == WAVE_OUT_DEVICE_TYPE)
    {
        DeviceInfo->Header.DataUsed = Length;
    }
    DeviceInfo->Header.Data = OffsetPtr;
    DeviceInfo->Header.Size = sizeof(WDMAUD_DEVICE_INFO);
    DeviceInfo->Header.PresentationTime.Numerator = 1;
    DeviceInfo->Header.PresentationTime.Denominator = 1;
    DeviceInfo->hDevice = Handle;
    DeviceInfo->DeviceType = DeviceType;

    /* Create completion event */
    Overlap->Standard.hEvent = Handle = CreateEventW(NULL, FALSE, FALSE, NULL);
    if (Overlap->Standard.hEvent == NULL)
    {
        /* No memory */
        SND_ERR(L"Failed to create event handle");
        FreeDeviceInfo(DeviceInfo);
        return MMSYSERR_NOMEM;
    }

    Overlap->OriginalCompletionRoutine = CompletionRoutine;
    Overlap->CompletionContext = (PVOID)DeviceInfo;

    if (DeviceType == WAVE_OUT_DEVICE_TYPE)
    {
        Ret = WriteFileEx(KernelHandle, DeviceInfo, sizeof(WDMAUD_DEVICE_INFO), (LPOVERLAPPED)Overlap, LegacyCompletionRoutine);
        if (Ret)
            WaitForSingleObjectEx (KernelHandle, INFINITE, TRUE);
    }
    else if (DeviceType == WAVE_IN_DEVICE_TYPE)
    {
        Ret = ReadFileEx(KernelHandle, DeviceInfo, sizeof(WDMAUD_DEVICE_INFO), (LPOVERLAPPED)Overlap, LegacyCompletionRoutine);
        if (Ret)
            WaitForSingleObjectEx (KernelHandle, INFINITE, TRUE);
    }

    /* Close event handle */
    CloseHandle(Handle);

    return MMSYSERR_NOERROR;
}

MMRESULT
WdmAudSetWaveStateByLegacy(
    IN  struct _SOUND_DEVICE_INSTANCE* SoundDeviceInstance,
    IN BOOL bStart)
{
    MMRESULT Result;
    PSOUND_DEVICE SoundDevice;
    PWDMAUD_DEVICE_INFO DeviceInfo;
    MMDEVICE_TYPE DeviceType;
    HANDLE Handle;

    Result = GetSoundDeviceFromInstance(SoundDeviceInstance, &SoundDevice);

    if ( ! MMSUCCESS(Result) )
    {
        return TranslateInternalMmResult(Result);
    }

    Result = GetSoundDeviceType(SoundDevice, &DeviceType);
    SND_ASSERT( Result == MMSYSERR_NOERROR );

    Result = GetSoundDeviceInstanceHandle(SoundDeviceInstance, &Handle);
    SND_ASSERT( Result == MMSYSERR_NOERROR );

    SND_TRACE(L"WDMAUD - SetWaveState DeviceType %u\n", DeviceType);

    DeviceInfo = AllocateDeviceInfo(gpszDeviceInterfaceString);
    if (!DeviceInfo)
    {
        /* No memory */
        return MMSYSERR_NOMEM;
    }

    DeviceInfo->hDevice = Handle;
    DeviceInfo->DeviceType = DeviceType;

    if (bStart)
        DeviceInfo->State = KSSTATE_RUN;
    else
        DeviceInfo->State = KSSTATE_PAUSE;
    Result = WdmAudIoControl(DeviceInfo,
                             0,
                             NULL,
                             IOCTL_SETDEVICE_STATE);

    FreeDeviceInfo(DeviceInfo);

    if ( ! MMSUCCESS(Result) )
    {
        SND_ERR(L"Call to IOCTL_SETDEVICE_STATE failed with %d\n", GetLastError());
        return Result;
    }

    return Result;
}

MMRESULT
WdmAudGetDeviceInterfaceStringByLegacy(
    IN  MMDEVICE_TYPE DeviceType,
    IN  DWORD DeviceId,
    IN  LPWSTR Interface,
    IN  DWORD  InterfaceLength,
    OUT  DWORD * InterfaceSize)
{
    /* Unused */
    return MMSYSERR_NOTSUPPORTED;
}

MMRESULT
WdmAudGetWavePositionByLegacy(
    IN  struct _SOUND_DEVICE_INSTANCE* SoundDeviceInstance,
    IN  MMTIME* Time)
{
    MMRESULT Result;
    PSOUND_DEVICE SoundDevice;
    PWDMAUD_DEVICE_INFO DeviceInfo;
    MMDEVICE_TYPE DeviceType;
    HANDLE Handle;

    Result = GetSoundDeviceFromInstance(SoundDeviceInstance, &SoundDevice);

    if ( ! MMSUCCESS(Result) )
    {
        return TranslateInternalMmResult(Result);
    }

    Result = GetSoundDeviceType(SoundDevice, &DeviceType);
    SND_ASSERT( Result == MMSYSERR_NOERROR );

    Result = GetSoundDeviceInstanceHandle(SoundDeviceInstance, &Handle);
    SND_ASSERT( Result == MMSYSERR_NOERROR );

    SND_TRACE(L"WDMAUD - GetWavePosition DeviceType %u\n", DeviceType);

    DeviceInfo = AllocateDeviceInfo(gpszDeviceInterfaceString);
    if (!DeviceInfo)
    {
        /* No memory */
        return MMSYSERR_NOMEM;
    }

    DeviceInfo->hDevice = Handle;
    DeviceInfo->DeviceType = DeviceType;

    Result = WdmAudIoControl(DeviceInfo,
                             0,
                             NULL,
                             IOCTL_GETPOS);

    if ( ! MMSUCCESS(Result) )
    {
        SND_ERR(L"Call to IOCTL_GETPOS failed with %d\n", GetLastError());
        FreeDeviceInfo(DeviceInfo);
        return TranslateInternalMmResult(Result);
    }

    Time->wType = TIME_BYTES;
    Time->u.cb = (DWORD)DeviceInfo->Position;

    FreeDeviceInfo(DeviceInfo);

    return MMSYSERR_NOERROR;
}

MMRESULT
WdmAudResetStreamByLegacy(
    IN  struct _SOUND_DEVICE_INSTANCE* SoundDeviceInstance,
    IN  MMDEVICE_TYPE DeviceType,
    IN  BOOLEAN bStartReset)
{
    MMRESULT Result;
    HANDLE Handle;
    PWDMAUD_DEVICE_INFO DeviceInfo;

    Result = GetSoundDeviceInstanceHandle(SoundDeviceInstance, &Handle);
    SND_ASSERT( Result == MMSYSERR_NOERROR );

    SND_TRACE(L"WDMAUD - ResetWaveStream DeviceType %u\n", DeviceType);

    DeviceInfo = AllocateDeviceInfo(gpszDeviceInterfaceString);
    if (!DeviceInfo)
    {
        /* No memory */
        return MMSYSERR_NOMEM;
    }

    DeviceInfo->hDevice = Handle;
    DeviceInfo->DeviceType = DeviceType;
    DeviceInfo->ResetStream = (bStartReset ? KSRESET_BEGIN : KSRESET_END);

    Result = WdmAudIoControl(DeviceInfo,
                             0,
                             NULL,
                             IOCTL_RESET_STREAM);

    FreeDeviceInfo(DeviceInfo);

    if ( ! MMSUCCESS(Result) )
    {
        SND_ERR(L"Call to IOCTL_RESET_STREAM failed with %d\n", GetLastError());
        return TranslateInternalMmResult(Result);
    }

    return MMSYSERR_NOERROR;
}

MMRESULT
WdmAudQueryMixerInfoByLegacy(
    IN  struct _SOUND_DEVICE_INSTANCE* SoundDeviceInstance,
    IN DWORD DeviceId,
    IN UINT uMsg,
    IN LPVOID Parameter,
    IN DWORD Flags)
{
    MMRESULT Result;
    PWDMAUD_DEVICE_INFO DeviceInfo;
    HANDLE Handle;
    DWORD IoControlCode;

    Result = GetSoundDeviceInstanceHandle(SoundDeviceInstance, &Handle);
    SND_ASSERT( Result == MMSYSERR_NOERROR );

    SND_TRACE(L"WDMAUD - QueryMixerInfo: uMsg %x Flags %x\n", uMsg, Flags);

    DeviceInfo = AllocateDeviceInfo(gpszDeviceInterfaceString);
    if (!DeviceInfo)
    {
        /* No memory */
        return MMSYSERR_NOMEM;
    }

    DeviceInfo->hDevice = Handle;
    DeviceInfo->DeviceIndex = DeviceId;
    DeviceInfo->DeviceType = MIXER_DEVICE_TYPE;
    DeviceInfo->Flags = Flags;

    switch(uMsg)
    {
        case MXDM_GETLINEINFO:
            IoControlCode = IOCTL_GETLINEINFO;
            break;
        case MXDM_GETLINECONTROLS:
            IoControlCode = IOCTL_GETLINECONTROLS;
            break;
        case MXDM_SETCONTROLDETAILS:
            IoControlCode = IOCTL_SETCONTROLDETAILS;
            break;
        case MXDM_GETCONTROLDETAILS:
            IoControlCode = IOCTL_GETCONTROLDETAILS;
            break;
        default:
            SND_ASSERT(0);
            return MMSYSERR_NOTSUPPORTED;
    }

    Result = WdmAudIoControl(DeviceInfo,
                             sizeof(Parameter),
                             Parameter,
                             IoControlCode);

    FreeDeviceInfo(DeviceInfo);

    if ( ! MMSUCCESS(Result) )
    {
        SND_ERR(L"Call to %d failed with %d\n", IoControlCode, GetLastError());
        return TranslateInternalMmResult(Result);
    }

    return MMSYSERR_NOERROR;
}
