/*
 * PROJECT:     ReactOS Intel HD4000 graphics card driver
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Intel HD4000 primary header
 * COPYRIGHT:   Copyright 2022 Hervé Poussineau <hpoussin@reactos.org>
 */

#ifndef HD4000_H
#define HD4000_H

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
} HD4000_SIZE, *PHD4000_SIZE;

typedef struct
{
    PUCHAR Mapped;
    PHYSICAL_ADDRESS RangeStart;
    ULONG RangeLength;
    UCHAR RangeInIoSpace;
} HD4000_ADDRESS_RANGE;

typedef struct
{
    HD4000_SIZE AvailableModeInfo;
    ULONG AvailableModeCount;
    USHORT CurrentMode;

    HD4000_ADDRESS_RANGE FrameBuffer;
    HD4000_ADDRESS_RANGE IoPorts;

    ULONG MaxXResolution;
    ULONG MaxYResolution;
} HD4000_DEVICE_EXTENSION, *PBOCHS_DEVICE_EXTENSION;


#endif //HD4000_H