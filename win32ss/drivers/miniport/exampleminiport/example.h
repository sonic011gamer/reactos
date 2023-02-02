/*
 * PROJECT:     ReactOS Example graphics card driver
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     example primary header
 * COPYRIGHT:   Copyright 2022 Hervé Poussineau <hpoussin@reactos.org>
 */

#ifndef example_H
#define example_H

#include <ntdef.h>
#include <dderror.h>
#include <miniport.h>
#include <video.h>
#include <devioctl.h>
#include <section_attribs.h>

typedef struct
{
    USHORT XResolution;
    USHORT YResolution;
} example_SIZE, *Pexample_SIZE;

typedef struct
{
    PUCHAR Mapped;
    PHYSICAL_ADDRESS RangeStart;
    ULONG RangeLength;
    UCHAR RangeInIoSpace;
} example_ADDRESS_RANGE;

typedef struct
{
    example_SIZE AvailableModeInfo;
    ULONG AvailableModeCount;
    USHORT CurrentMode;

    example_ADDRESS_RANGE FrameBuffer;
    example_ADDRESS_RANGE IoPorts;

    ULONG MaxXResolution;
    ULONG MaxYResolution;
} example_DEVICE_EXTENSION, *PBOCHS_DEVICE_EXTENSION;


#endif //example_H