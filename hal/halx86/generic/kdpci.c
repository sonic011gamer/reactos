/*
 * PROJECT:     ReactOS Hardware Abstraction Layer
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Kernel debugger PCI configurator
 * COPYRIGHT:   Copyright 2021 Dmitry Borisov <di.sean@protonmail.com>
 */

/* INCLUDES *******************************************************************/

#include <hal.h>

/* GLOBALS ********************************************************************/

#define DPRINT0

extern UCHAR PCIDeref[4][4];

PCI_TYPE1_CFG_CYCLE_BITS HalpPciDebuggingDevice[2] = {{{{0}}}};

/* FUNCTIONS ******************************************************************/

static
CODE_SEG("INIT")
ULONG
HalpPhase0GetPciDataByOffset(
    _In_ ULONG Bus,
    _In_ PCI_SLOT_NUMBER PciSlot,
    _Out_writes_all_(Length) PVOID Buffer,
    _In_ ULONG Offset,
    _In_ ULONG Length)
{
    ULONG BytesLeft = Length;
    PUCHAR BufferPtr = Buffer;
    PCI_TYPE1_CFG_BITS PciCfg;

    PciCfg.u.AsULONG = 0;
    PciCfg.u.bits.BusNumber = Bus;
    PciCfg.u.bits.DeviceNumber = PciSlot.u.bits.DeviceNumber;
    PciCfg.u.bits.FunctionNumber = PciSlot.u.bits.FunctionNumber;
    PciCfg.u.bits.Enable = TRUE;

    while (BytesLeft)
    {
        ULONG i = PCIDeref[Offset % sizeof(ULONG)][BytesLeft % sizeof(ULONG)];

        PciCfg.u.bits.RegisterNumber = Offset / sizeof(ULONG);
        WRITE_PORT_ULONG((PULONG)PCI_TYPE1_ADDRESS_PORT, PciCfg.u.AsULONG);

        switch (i)
        {
            case 0:
                *(PULONG)BufferPtr = READ_PORT_ULONG((PULONG)PCI_TYPE1_DATA_PORT);

                /* Number of bytes read */
                i = sizeof(ULONG);
                break;
            case 1:
                *BufferPtr = READ_PORT_UCHAR((PUCHAR)(PCI_TYPE1_DATA_PORT +
                                                Offset % sizeof(ULONG)));
                break;
            case 2:
                *(PUSHORT)BufferPtr = READ_PORT_USHORT((PUSHORT)(PCI_TYPE1_DATA_PORT +
                                                                    Offset % sizeof(ULONG)));
                break;
        }

        Offset += i;
        BufferPtr += i;
        BytesLeft -= i;
    }

    return Length;
}

static
CODE_SEG("INIT")
ULONG
HalpPhase0SetPciDataByOffset(
    _In_ ULONG Bus,
    _In_ PCI_SLOT_NUMBER PciSlot,
    _In_reads_bytes_(Length) PVOID Buffer,
    _In_ ULONG Offset,
    _In_ ULONG Length)
{
    ULONG BytesLeft = Length;
    PUCHAR BufferPtr = Buffer;
    PCI_TYPE1_CFG_BITS PciCfg;

    PciCfg.u.AsULONG = 0;
    PciCfg.u.bits.BusNumber = Bus;
    PciCfg.u.bits.DeviceNumber = PciSlot.u.bits.DeviceNumber;
    PciCfg.u.bits.FunctionNumber = PciSlot.u.bits.FunctionNumber;
    PciCfg.u.bits.Enable = TRUE;

    while (BytesLeft)
    {
        ULONG i = PCIDeref[Offset % sizeof(ULONG)][BytesLeft % sizeof(ULONG)];

        PciCfg.u.bits.RegisterNumber = Offset / sizeof(ULONG);
        WRITE_PORT_ULONG((PULONG)PCI_TYPE1_ADDRESS_PORT, PciCfg.u.AsULONG);

        switch (i)
        {
            case 0:
                WRITE_PORT_ULONG((PULONG)PCI_TYPE1_DATA_PORT, *(PULONG)BufferPtr);

                /* Number of bytes written */
                i = sizeof(ULONG);
                break;
            case 1:
                WRITE_PORT_UCHAR((PUCHAR)(PCI_TYPE1_DATA_PORT + Offset % sizeof(ULONG)),
                                 *BufferPtr);
                break;
            case 2:
                WRITE_PORT_USHORT((PUSHORT)(PCI_TYPE1_DATA_PORT + Offset % sizeof(ULONG)),
                                  *(PUSHORT)BufferPtr);
                break;
        }

        Offset += i;
        BufferPtr += i;
        BytesLeft -= i;
    }

    return Length;
}

static
CODE_SEG("INIT")
ULONG
HalpPciBarLength(
    _In_ ULONG Bar)
{
    return ~((Bar & PCI_ADDRESS_IO_SPACE)
             ? (Bar & PCI_ADDRESS_IO_ADDRESS_MASK)
             : (Bar & PCI_ADDRESS_MEMORY_ADDRESS_MASK)) + 1;
}

static
CODE_SEG("INIT")
BOOLEAN
HalpConfigureDebuggingDevice(
    _In_ PDEBUG_DEVICE_DESCRIPTOR PciDevice,
    _In_ ULONG PciBus,
    _In_ PCI_SLOT_NUMBER PciSlot)
{
    ULONG i, Bytes, Register;
    PCI_COMMON_HEADER PciConfig;

    Bytes = HalpPhase0GetPciDataByOffset(PciBus,
                                         PciSlot,
                                         &PciConfig,
                                         0,
                                         PCI_COMMON_HDR_LENGTH);
    if (Bytes != sizeof(PciConfig))
        return FALSE;

    Register = PciConfig.Command & ~(PCI_ENABLE_MEMORY_SPACE | PCI_ENABLE_IO_SPACE);
    HalpPhase0SetPciDataByOffset(PciBus,
                                 PciSlot,
                                 &Register,
                                 FIELD_OFFSET(PCI_COMMON_HEADER, Command),
                                 sizeof(USHORT));

    for (i = 0; i < PCI_TYPE0_ADDRESSES; ++i)
    {
        ULONG Length;

        Register = 0xFFFFFFFF;
        HalpPhase0SetPciDataByOffset(PciBus,
                                     PciSlot,
                                     &Register,
                                     FIELD_OFFSET(PCI_COMMON_HEADER, u.type0.BaseAddresses[i]),
                                     sizeof(ULONG));
        HalpPhase0GetPciDataByOffset(PciBus,
                                     PciSlot,
                                     &Register,
                                     FIELD_OFFSET(PCI_COMMON_HEADER, u.type0.BaseAddresses[i]),
                                     sizeof(ULONG));
        HalpPhase0SetPciDataByOffset(PciBus,
                                     PciSlot,
                                     &PciConfig.u.type0.BaseAddresses[i],
                                     FIELD_OFFSET(PCI_COMMON_HEADER, u.type0.BaseAddresses[i]),
                                     sizeof(ULONG));

        Length = HalpPciBarLength(Register);
        if (Register == 0 || Length == 0)
            continue;

        DPRINT0("Bar 0x%lx\n", PciConfig.u.type0.BaseAddresses[i]);
        DPRINT0("Bar 0x%lx\n", Register);
        DPRINT0("Len 0x%lx\n", Length);

        if (Register & PCI_ADDRESS_IO_SPACE)
        {
            PciDevice->BaseAddress[i].Type = CmResourceTypePort;
            PciDevice->BaseAddress[i].TranslatedAddress =
                UlongToPtr(PciConfig.u.type0.BaseAddresses[i] & PCI_ADDRESS_IO_ADDRESS_MASK);
        }
        else
        {
            PHYSICAL_ADDRESS PhysicalAddress;

            PciDevice->BaseAddress[i].Type = CmResourceTypeMemory;
            PhysicalAddress.QuadPart =
                PciConfig.u.type0.BaseAddresses[i] & PCI_ADDRESS_MEMORY_ADDRESS_MASK;
            PciDevice->BaseAddress[i].TranslatedAddress =
                HalpMapPhysicalMemory64(PhysicalAddress, BYTES_TO_PAGES(Length));
        }
        PciDevice->BaseAddress[i].Length = Length;
        PciDevice->BaseAddress[i].Valid = TRUE;
    }

    PciConfig.Command |= (PCI_ENABLE_MEMORY_SPACE | PCI_ENABLE_IO_SPACE | PCI_ENABLE_BUS_MASTER);
    HalpPhase0SetPciDataByOffset(PciBus,
                                 PciSlot,
                                 &PciConfig.Command,
                                 FIELD_OFFSET(PCI_COMMON_HEADER, Command),
                                 sizeof(USHORT));

    PciDevice->Slot = PciSlot.u.AsULONG;
    PciDevice->Bus = PciBus;
    PciDevice->VendorID = PciConfig.VendorID;
    PciDevice->DeviceID = PciConfig.DeviceID;
    PciDevice->BaseClass = PciConfig.BaseClass;
    PciDevice->SubClass = PciConfig.SubClass;
    PciDevice->ProgIf = PciConfig.ProgIf;

    return TRUE;
}

static
CODE_SEG("INIT")
BOOLEAN
HalpMatchDebuggingDevice(
    _In_ PDEBUG_DEVICE_DESCRIPTOR PciDevice,
    _In_ ULONG PciBus,
    _In_ PCI_SLOT_NUMBER PciSlot,
    _In_ PPCI_COMMON_HEADER PciConfig)
{
    /* If we weren't given a specific device location */
    if (PciDevice->Bus == 0xFFFFFFFF &&
        PciDevice->Slot == 0xFFFFFFFF)
    {
        if (PciDevice->DeviceID == 0xFFFF &&
            PciDevice->VendorID == 0xFFFF)
        {
            if (PciDevice->BaseClass == PciConfig->BaseClass &&
                PciDevice->SubClass == PciConfig->SubClass)
            {
                if (PciDevice->ProgIf == 0xFF ||
                    PciDevice->ProgIf == PciConfig->ProgIf)
                {
                    return TRUE;
                }
            }
        }
        else if (PciDevice->DeviceID == PciConfig->DeviceID &&
                 PciDevice->VendorID == PciConfig->VendorID)
        {
            return TRUE;
        }
    }
    else if (PciDevice->Bus == PciBus &&
             PciDevice->Slot == PciSlot.u.AsULONG)
    {
         return TRUE;
    }

    return FALSE;
}

static
CODE_SEG("INIT")
BOOLEAN
HalpFindMatchingDebuggingDevice(
    _In_ PDEBUG_DEVICE_DESCRIPTOR PciDevice)
{
    ULONG BusNumber, DeviceNumber, FunctionNumber;

    for (BusNumber = 0; BusNumber < 0xFF; ++BusNumber)
    {
        for (DeviceNumber = 0; DeviceNumber < PCI_MAX_DEVICES; ++DeviceNumber)
        {
            for (FunctionNumber = 0; FunctionNumber < PCI_MAX_FUNCTION; ++FunctionNumber)
            {
                ULONG Bytes;
                PCI_SLOT_NUMBER PciSlot;
                PCI_COMMON_HEADER PciConfig;

                PciSlot.u.bits.DeviceNumber = DeviceNumber;
                PciSlot.u.bits.FunctionNumber = FunctionNumber;
                PciSlot.u.bits.Reserved = 0;
                Bytes = HalpPhase0GetPciDataByOffset(BusNumber,
                                                     PciSlot,
                                                     &PciConfig,
                                                     0,
                                                     PCI_COMMON_HDR_LENGTH);
                if (Bytes != sizeof(PciConfig))
                    continue;
                if (PciConfig.VendorID == PCI_INVALID_VENDORID || PciConfig.VendorID == 0)
                {
                    if (FunctionNumber == 0)
                        break;
                    continue;
                }

                DPRINT0("B:%02x D:%02x F:%x [%04X:%04X]\n",
                        BusNumber, DeviceNumber, FunctionNumber,
                        PciConfig.VendorID,PciConfig.DeviceID);

                // DumpBuffer(&PciConfig, PCI_COMMON_HDR_LENGTH);

                switch (PCI_CONFIGURATION_TYPE(&PciConfig))
                {
                    case PCI_DEVICE_TYPE:
                    {
                        if (HalpMatchDebuggingDevice(PciDevice, BusNumber, PciSlot, &PciConfig))
                        {
                            DPRINT0("Found device\n");

                            if (HalpConfigureDebuggingDevice(PciDevice, BusNumber, PciSlot))
                            {
                                DPRINT0("Device is ready\n");
                                return TRUE;
                            }
                        }
                        break;
                    }
                    case PCI_BRIDGE_TYPE:
                    {
                        break;
                    }
                    case PCI_CARDBUS_BRIDGE_TYPE:
                    {
                        break;
                    }

                    default:
                        break;
                }

                if (!PCI_MULTIFUNCTION_DEVICE(&PciConfig))
                    break;
            }
        }
    }

    return FALSE;
}

CODE_SEG("INIT")
VOID
NTAPI
HalpRegisterPciDebuggingDeviceInfo(VOID)
{
    ULONG i;
    NTSTATUS Status;
    WCHAR StringBuffer[16];
    BOOLEAN HasActivePciDebugger = FALSE;
    UNICODE_STRING KeyName = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\System\\"
                                                 L"CurrentControlSet\\Services\\PCI\\Debug");
    HANDLE Handle, KeyHandle;

    PAGED_CODE();

    for (i = 0; i < RTL_NUMBER_OF(HalpPciDebuggingDevice); ++i)
    {
        if (HalpPciDebuggingDevice[i].u.bits.Reserved1)
        {
            HasActivePciDebugger = TRUE;
            break;
        }
    }
    if (!HasActivePciDebugger)
    {
        /* Nothing to register */
        return;
    }

    Status = HalpOpenRegistryKey(&Handle, 0, &KeyName, KEY_ALL_ACCESS, TRUE);
    if (NT_SUCCESS(Status))
    {
        for (i = 0; i < RTL_NUMBER_OF(HalpPciDebuggingDevice); ++i)
        {
            ULONG Value;
            PCI_SLOT_NUMBER PciSlot;

            if (!HalpPciDebuggingDevice[i].u.bits.Reserved1)
                continue;

            RtlInitEmptyUnicodeString(&KeyName, StringBuffer, sizeof(StringBuffer));
            RtlIntegerToUnicodeString(i, 10, &KeyName);
            Status = HalpOpenRegistryKey(&KeyHandle,
                                         Handle,
                                         &KeyName,
                                         KEY_ALL_ACCESS,
                                         TRUE);
            if (!NT_SUCCESS(Status))
                continue;

            Value = HalpPciDebuggingDevice[i].u.bits.BusNumber;
            RtlInitUnicodeString(&KeyName, L"Bus");
            ZwSetValueKey(KeyHandle,
                          &KeyName,
                          0,
                          REG_DWORD,
                          &Value,
                          sizeof(Value));

            PciSlot.u.AsULONG = 0;
            PciSlot.u.bits.DeviceNumber = HalpPciDebuggingDevice[i].u.bits.DeviceNumber;
            PciSlot.u.bits.FunctionNumber = HalpPciDebuggingDevice[i].u.bits.FunctionNumber;
            Value = PciSlot.u.AsULONG;
            RtlInitUnicodeString(&KeyName, L"Slot");
            ZwSetValueKey(KeyHandle,
                          &KeyName,
                          0,
                          REG_DWORD,
                          &Value,
                          sizeof(Value));

            ZwClose(KeyHandle);
        }

        ZwClose(Handle);
    }
}

CODE_SEG("INIT")
NTSTATUS
NTAPI
HalpReleasePciDeviceForDebugging(
    _Inout_ PDEBUG_DEVICE_DESCRIPTOR PciDevice)
{
    DPRINT0("%s Unimplemented!\n", __FUNCTION__);

    return STATUS_NOT_IMPLEMENTED;
}

CODE_SEG("INIT")
NTSTATUS
NTAPI
HalpSetupPciDeviceForDebugging(
    _In_opt_ PVOID LoaderBlock,
    _Inout_ PDEBUG_DEVICE_DESCRIPTOR PciDevice)
{
    ULONG64 MaxAddress;
    PFN_NUMBER PageCount;
    PCI_SLOT_NUMBER PciSlot;
    PHYSICAL_ADDRESS PhysicalAddress;

    DPRINT0("%s called\n", __FUNCTION__);

#if DBG
    // DumpDescriptor(PciDevice);
#endif

    if (!LoaderBlock)
        return STATUS_INVALID_PARAMETER_1;

    if (!HalpFindMatchingDebuggingDevice(PciDevice))
        return STATUS_DEVICE_DOES_NOT_EXIST;

    MaxAddress = ALIGN_DOWN_BY(0xFFFFFFFF - PciDevice->Memory.Length, PAGE_SIZE);
    PageCount = BYTES_TO_PAGES(PciDevice->Memory.Length);

    PhysicalAddress.QuadPart = HalpAllocPhysicalMemory(LoaderBlock,
                                                       MaxAddress,
                                                       PageCount,
                                                       FALSE);
    PciDevice->Memory.Start = PhysicalAddress;
    if (!PhysicalAddress.QuadPart)
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }
    PciDevice->Memory.VirtualAddress = HalpMapPhysicalMemory64(PhysicalAddress, PageCount);

    PciDevice->Memory.MaxEnd.QuadPart = -1;
    PciDevice->Initialized = TRUE;

    PciSlot.u.AsULONG = PciDevice->Slot;
    HalpPciDebuggingDevice[0].u.bits.DeviceNumber = PciSlot.u.bits.DeviceNumber;
    HalpPciDebuggingDevice[0].u.bits.FunctionNumber = PciSlot.u.bits.FunctionNumber;
    HalpPciDebuggingDevice[0].u.bits.BusNumber = PciDevice->Bus;
    HalpPciDebuggingDevice[0].u.bits.Reserved1 = TRUE;

#if DBG
    // DumpDescriptor(PciDevice);
#endif

    return STATUS_SUCCESS;
}
