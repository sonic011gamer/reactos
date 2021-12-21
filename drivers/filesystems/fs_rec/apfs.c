/*
 * COPYRIGHT:        See COPYING in the top level directory
 * PROJECT:          ReactOS File System Recognizer
 * FILE:             drivers/filesystems/fs_rec/btrfs.c
 * PURPOSE:          Btrfs Recognizer
 * PROGRAMMER:       Peter Hater
 *                   Pierre Schweitzer (pierre@reactos.org)
 */

/* INCLUDES *****************************************************************/

#include "fs_rec.h"
#define NDEBUG
#include <debug.h>

/* FUNCTIONS ****************************************************************/

NTSTATUS
NTAPI
FsRecApfsFsControl(IN PDEVICE_OBJECT DeviceObject,
                    IN PIRP Irp)
{
    PIO_STACK_LOCATION Stack;
    NTSTATUS Status;
    PDEVICE_OBJECT MountDevice;
    ULONG SectorSize;
    //LARGE_INTEGER Offset;
    BOOLEAN DeviceError = FALSE;
    PAGED_CODE();

    /* Get the I/O Stack and check the function type */
    Stack = IoGetCurrentIrpStackLocation(Irp);
    switch (Stack->MinorFunction)
    {
        case IRP_MN_MOUNT_VOLUME:
            __debugbreak();
            /* Assume failure */
            Status = STATUS_UNRECOGNIZED_VOLUME;

            /* Get the device object and request the sector size */
            MountDevice = Stack->Parameters.MountVolume.DeviceObject;
            if (FsRecGetDeviceSectorSize(MountDevice, &SectorSize))
            {
                #if 0
                /* Try to read the superblock */
                Offset.QuadPart = BTRFS_SB_OFFSET;
                if (FsRecReadBlock(MountDevice,
                                   &Offset,
                                   BTRFS_SB_SIZE,
                                   SectorSize,
                                   (PVOID)&Spb,
                                   &DeviceError))
                {
                    /* Check if it's an actual BTRFS volume */
                    if (FsRecIsBtrfsVolume(Spb))
                    {
                        /* It is! */
                        Status = STATUS_FS_DRIVER_REQUIRED;
                    }
                }
                #endif

            }
            else
            {
                /* We have some sort of failure in the storage stack */
                DeviceError = TRUE;
            }

            break;

        case IRP_MN_LOAD_FILE_SYSTEM:

            /* Load the file system */
            Status = FsRecLoadFileSystem(DeviceObject,
                                         L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\apfs");
            break;

        default:

            /* Invalid request */
            Status = STATUS_INVALID_DEVICE_REQUEST;
    }

    /* Return Status */
    return Status;
}
