#pragma once

///
/// WDMAUD Interface Definition
///
/// History: 12/02/2008 Created

#include <sndtypes.h>

typedef struct
{
    KSSTREAM_HEADER Header;
    ULONG DeviceIndex;
    SOUND_DEVICE_TYPE DeviceType;

    HANDLE hDevice;
    ULONG Flags;

    PVOID Buffer;
    ULONG BufferSize;
    ULONG Position;
    LPWSTR DeviceInterfaceString;

    struct
    {
        HANDLE hMixer;
        ULONG NotificationType;
        ULONG Value;
    }MixerEvent;
    KSSTATE State;
    KSRESET ResetStream;
    ULONG FrameSize;

    char unk1[25];

}WDMAUD_DEVICE_INFO, *PWDMAUD_DEVICE_INFO;



/// IOCTL_INIT_WDMAUD
///
/// Description: This IOCTL does the initialization of wdmaud.sys
///
/// Arguments:   Buffer should be NULL,
///              BufferSize should be zero
/// Note:        DeviceType identifies the device type, DeviceIndex the index, WaveFormatEx the device details
/// Result:      is returned in hDevice
/// Return Code: STATUS_SUCCESS indicates success, otherwise appropriate error code
/// Prerequisites:  none

#define IOCTL_INIT_WDMAUD \
    CTL_CODE(FILE_DEVICE_SOUND, \
             0x0000, \
             METHOD_BUFFERED, \
             FILE_WRITE_ACCESS)

/// IOCTL_ADD_DEVNODE
///
/// Description: This IOCTL adds device node
///
/// Arguments:   Buffer should be NULL,
///              BufferSize should be zero
/// Note:        DeviceType identifies the device type, DeviceIndex the index, WaveFormatEx the device details
/// Result:      is returned in hDevice
/// Return Code: STATUS_SUCCESS indicates success, otherwise appropriate error code
/// Prerequisites:  none

#define IOCTL_ADD_DEVNODE \
    CTL_CODE(FILE_DEVICE_SOUND, \
             0x0001, \
             METHOD_BUFFERED, \
             FILE_WRITE_ACCESS)

/// IOCTL_REMOVE_DEVNODE
///
/// Description: This IOCTL removes device node
///
/// Arguments:   Buffer should be NULL,
///              BufferSize should be zero
/// Note:        DeviceType identifies the device type, DeviceIndex the index, WaveFormatEx the device details
/// Result:      is returned in hDevice
/// Return Code: STATUS_SUCCESS indicates success, otherwise appropriate error code
/// Prerequisites:  none

#define IOCTL_REMOVE_DEVNODE \
    CTL_CODE(FILE_DEVICE_SOUND, \
             0x0002, \
             METHOD_BUFFERED, \
             FILE_WRITE_ACCESS)

/// IOCTL_GETCAPABILITIES
///
/// Description: This IOCTL retrieves the capabilities of an specific wave out device
///
/// Arguments:  Buffer is a pointer to a capabilities data,
///             BufferSize is size of capabilities data
/// Note:       The DeviceType and DeviceIndex must be set
/// ReturnCode:  STATUS_SUCCESS indicates success
/// Prerequisites: none

#define IOCTL_GETCAPABILITIES \
    CTL_CODE(FILE_DEVICE_SOUND, \
             0x0003, \
             METHOD_BUFFERED, \
             FILE_WRITE_ACCESS)

/// IOCTL_GETNUMDEVS_TYPE
///
/// Description: This IOCTL queries the number of devices currently present of a specific type. The caller passes a WDMAUD_DEVICE_INFO structure.
///
/// Arguments:  Buffer is NULL,
///             BufferSize is zero
/// Note:       The DeviceType contains the requested device type.
/// Result:     The result is returned in DeviceCount
/// ReturnCode:  STATUS_SUCCESS indicates success
/// Prerequisites: none

#define IOCTL_GETNUMDEVS_TYPE \
    CTL_CODE(FILE_DEVICE_SOUND, \
             0x0004, \
             METHOD_BUFFERED, \
             FILE_WRITE_ACCESS)


/// IOCTL_OPEN_WDMAUD
///
/// Description: This IOCTL informs wdmaud that an application whats to use wdmsys for a waveOut / waveIn / aux operation
///
/// Arguments:   Buffer is a pointer to the WAVEFORMATEX structure,
///              BufferSize is size of the WAVEFORMATEX structure
/// Note:        DeviceType identifies the device type, DeviceIndex the index, WaveFormatEx the device details
/// Result:      is returned in hDevice
/// Return Code: STATUS_SUCCESS indicates success, otherwise appropriate error code
/// Prerequisites:  none

#define IOCTL_OPEN_WDMAUD \
    CTL_CODE(FILE_DEVICE_SOUND, \
             0x0005, \
             METHOD_BUFFERED, \
             FILE_WRITE_ACCESS)

/// IOCTL_CLOSE_WDMAUD
///
/// Description: This IOCTL informs that an application has finished with wdmsys and closes the connection
///
/// Arguments:   Buffer should be NULL,
///              BufferSize should be zero
/// Note:        DeviceType, DeviceIndex and hDevice must be set
/// ReturnCode:  STATUS_SUCCESS indicates success
/// Prerequisites: openend device

#define IOCTL_CLOSE_WDMAUD \
    CTL_CODE(FILE_DEVICE_SOUND, \
             0x006, \
             METHOD_BUFFERED, \
             FILE_WRITE_ACCESS)

/// IOCTL_GETVOLUME
///
/// Description: This IOCTL returns the volume a device
///
/// Arguments:  InputBuffer is a pointer to a WDMAUD_DEVICE_INFO structure,
///             InputBufferSize is size of WDMAUD_DEVICE_INFO structure
/// Note:       The DeviceType and hDevice must be set
/// Result:     The result is returned in Volume
/// ReturnCode:  STATUS_SUCCESS indicates success
/// Prerequisites: opened device

#define IOCTL_GETVOLUME \
    CTL_CODE(FILE_DEVICE_SOUND, \
             0x0007, \
             METHOD_BUFFERED, \
             FILE_WRITE_ACCESS)


/// IOCTL_SETVOLUME
///
/// Description: This IOCTL sets the volume a device
///
/// Arguments:  InputBuffer is a pointer to a WDMAUD_DEVICE_INFO structure,
///             InputBufferSize is size of WDMAUD_DEVICE_INFO structure
/// Note:       The DeviceType, hDevice and Volume must be set
/// ReturnCode:  STATUS_SUCCESS indicates success
/// Prerequisites: opened device

#define IOCTL_SETVOLUME \
    CTL_CODE(FILE_DEVICE_SOUND, \
             0x0008, \
             METHOD_BUFFERED, \
             FILE_WRITE_ACCESS)

/// IOCTL_SETDEVICE_STATE
///
/// Description: This IOCTL sets an opened waveOut / waveIn / midiIn / midiOut / aux device to specific state
///
/// Arguments:  InputBuffer is a pointer to a WDMAUD_DEVICE_INFO structure,
///             InputBufferSize is size of WDMAUD_DEVICE_INFO structure
/// Note:       The DeviceType, DeviceIndex, hDevice and State member must be set. State determines the new state
/// ReturnCode:  STATUS_SUCCESS indicates success
/// Prerequisites: opened device

#define IOCTL_SETDEVICE_STATE \
    CTL_CODE(FILE_DEVICE_SOUND, \
             3, \
             METHOD_BUFFERED, \
             FILE_CREATE_TREE_CONNECTION | FILE_ANY_ACCESS)

/// IOCTL_GETDEVID
///
/// Description: This IOCTL returns the device index by its provided handle
///
/// Arguments:  InputBuffer is a pointer to a WDMAUD_DEVICE_INFO structure,
///             InputBufferSize is size of WDMAUD_DEVICE_INFO structure
/// Note:       The DeviceType and hDevice must be set
/// Result:     The result is returned in DeviceIndex
/// ReturnCode:  STATUS_SUCCESS indicates success
/// Prerequisites: opened device

#define IOCTL_GETDEVID \
    CTL_CODE(FILE_DEVICE_SOUND, \
             4, \
             METHOD_BUFFERED, \
             FILE_CREATE_TREE_CONNECTION | FILE_ANY_ACCESS)

/// IOCTL_WRITEDATA
///
/// Description: This IOCTL writes data to specified device
///
/// Arguments:  InputBuffer is a pointer to a WDMAUD_DEVICE_INFO structure,
///             InputBufferSize is size of WDMAUD_DEVICE_INFO structure
/// Note:       The DeviceType, DeviceIndex, hDevice, BufferSize and Buffer must be set
/// ReturnCode:  STATUS_SUCCESS indicates success
/// Prerequisites: opened device

#define IOCTL_WRITEDATA \
    CTL_CODE(FILE_DEVICE_SOUND, \
             8, \
             METHOD_BUFFERED, \
             FILE_CREATE_TREE_CONNECTION | FILE_ANY_ACCESS)

/// IOCTL_GETPOS
///
/// Description: This IOCTL retrieves the current playback / write position
///
/// Arguments:  InputBuffer is a pointer to a WDMAUD_DEVICE_INFO structure,
///             InputBufferSize is size of WDMAUD_DEVICE_INFO structure
/// Note:       The DeviceType and hDevice must be set
/// Result:     The result is returned in Position
/// ReturnCode:  STATUS_SUCCESS indicates success
/// Prerequisites: opened device

#define IOCTL_GETPOS \
    CTL_CODE(FILE_DEVICE_SOUND, \
             9, \
             METHOD_BUFFERED, \
             FILE_CREATE_TREE_CONNECTION | FILE_ANY_ACCESS)

/// IOCTL_GETFRAMESIZE
///
/// Description: This IOCTL retrieves the frame size requirements for an audio pin
///
/// Arguments:  InputBuffer is a pointer to a WDMAUD_DEVICE_INFO structure,
///             InputBufferSize is size of WDMAUD_DEVICE_INFO structure
/// Note:       The DeviceType and hDevice must be set
/// Result:     The result is returned in FrameSize
/// ReturnCode:  STATUS_SUCCESS indicates success
/// Prerequisites: opened device

#define IOCTL_GETFRAMESIZE \
    CTL_CODE(FILE_DEVICE_SOUND, \
             10, \
             METHOD_BUFFERED, \
             FILE_CREATE_TREE_CONNECTION | FILE_ANY_ACCESS)

/// IOCTL_GETLINEINFO
///
/// Description: This IOCTL retrieves information on a mixerline
///
/// Arguments:  InputBuffer is a pointer to a WDMAUD_DEVICE_INFO structure,
///             InputBufferSize is size of WDMAUD_DEVICE_INFO structure
/// Note:       The hDevice member must be set
/// Result:     The result is returned in MixLine
/// ReturnCode:  STATUS_SUCCESS indicates success
/// Prerequisites: opened device

#define IOCTL_GETLINEINFO \
    CTL_CODE(FILE_DEVICE_SOUND, \
             11, \
             METHOD_BUFFERED, \
             FILE_CREATE_TREE_CONNECTION | FILE_ANY_ACCESS)


/// IOCTL_GETLINECONTROLS
///
/// Description: This IOCTL retrieves controls of a mixerline
///
/// Arguments:  InputBuffer is a pointer to a WDMAUD_DEVICE_INFO structure,
///             InputBufferSize is size of WDMAUD_DEVICE_INFO structure
/// Note:       The hDevice member must be set
/// Result:     The result is returned in MixControls
/// ReturnCode:  STATUS_SUCCESS indicates success
/// Prerequisites: opened device

#define IOCTL_GETLINECONTROLS \
    CTL_CODE(FILE_DEVICE_SOUND, \
             12, \
             METHOD_BUFFERED, \
             FILE_CREATE_TREE_CONNECTION | FILE_ANY_ACCESS)


/// IOCTL_SETCONTROLDETAILS
///
/// Description: This IOCTL sets details of a control of a mixerline
///
/// Arguments:  InputBuffer is a pointer to a WDMAUD_DEVICE_INFO structure,
///             InputBufferSize is size of WDMAUD_DEVICE_INFO structure
/// Note:       The hDevice member must be set
/// ReturnCode:  STATUS_SUCCESS indicates success
/// Prerequisites: opened device

#define IOCTL_SETCONTROLDETAILS \
    CTL_CODE(FILE_DEVICE_SOUND, \
             13, \
             METHOD_BUFFERED, \
             FILE_CREATE_TREE_CONNECTION | FILE_ANY_ACCESS)


/// IOCTL_GETCONTROLDETAILS
///
/// Description: This IOCTL gets details of a control of a mixerline
///
/// Arguments:  InputBuffer is a pointer to a WDMAUD_DEVICE_INFO structure,
///             InputBufferSize is size of WDMAUD_DEVICE_INFO structure
/// Note:       The hDevice member must be set
/// Result:     The result is returned in MixDetails
/// ReturnCode:  STATUS_SUCCESS indicates success
/// Prerequisites: opened device

#define IOCTL_GETCONTROLDETAILS \
    CTL_CODE(FILE_DEVICE_SOUND, \
             14, \
             METHOD_BUFFERED, \
             FILE_CREATE_TREE_CONNECTION | FILE_ANY_ACCESS)

/// IOCTL_GET_MIXER_EVENT
///
/// Description: This IOCTL queries for wdmaud driver if there any new kernel streaming events available
///
/// Arguments:  InputBuffer is a pointer to a WDMAUD_DEVICE_INFO structure,
///             InputBufferSize is size of WDMAUD_DEVICE_INFO structure
/// Note:       The hDevice member must be set
/// Result:     The result is returned in the struct MixerInfo
/// ReturnCode:  STATUS_SUCCESS indicates success

#define IOCTL_GET_MIXER_EVENT \
    CTL_CODE(FILE_DEVICE_SOUND, \
             16, \
             METHOD_BUFFERED, \
             FILE_CREATE_TREE_CONNECTION | FILE_ANY_ACCESS)

/// IOCTL_RESET_STREAM
///
/// Description: This IOCTL instructs wdmaud to reset a stream
///
/// Arguments:  InputBuffer is a pointer to a WDMAUD_DEVICE_INFO structure,
///             InputBufferSize is size of WDMAUD_DEVICE_INFO structure
/// Note:       The hDevice member must be set and DeviceType
/// ReturnCode:  STATUS_SUCCESS indicates success

#define IOCTL_RESET_STREAM \
    CTL_CODE(FILE_DEVICE_SOUND, \
             17, \
             METHOD_BUFFERED, \
             FILE_CREATE_TREE_CONNECTION | FILE_ANY_ACCESS)
