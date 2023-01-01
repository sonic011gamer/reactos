/*
 * PROJECT:     ReactOS Kernel
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Architecture specific source file to hold multiprocessor functions
 * COPYRIGHT:   Copyright 2021 Justin Miller <justinmiller100@gmail.com>
 *              Copyright 2021 Victor Perevertkin <victor.perevertkin@reactos.org>
 */

/* INCLUDES *****************************************************************/

#include <ntoskrnl.h>
//#define NDEBUG
#include <debug.h>

typedef struct _APINFO
{
    KIPCR Pcr;
    KTSS Tss;
    ETHREAD Thread;
    KTSS TssDoubleFault;
    KTSS TssNMI;
    DECLSPEC_ALIGN(16) UINT8 DoubleFaultStack[DOUBLE_FAULT_STACK_SIZE];
    DECLSPEC_ALIGN(PAGE_SIZE) KGDTENTRY Gdt[128];/* TODO: Soften these hardcodes up */
    DECLSPEC_ALIGN(PAGE_SIZE) KIDTENTRY Idt[256];
} APINFO, *PAPINFO;

/* GLOBALS *******************************************************************/

/* FUNCTIONS *****************************************************************/

FORCEINLINE
PKGDTENTRY
KiGetGdtEntry(
    IN PVOID pGdt,
    IN USHORT Selector)
{
    return (PKGDTENTRY)((ULONG_PTR)pGdt + (Selector & ~RPL_MASK));
}

FORCEINLINE
VOID
KiSetGdtDescriptorBase(
    IN OUT PKGDTENTRY Entry,
    IN UINT32 Base)
{
    Entry->BaseLow = (UINT16)(Base & 0xffff);
    Entry->HighWord.Bytes.BaseMid = (UINT8)((Base >> 16) & 0xff);
    Entry->HighWord.Bytes.BaseHi  = (UINT8)((Base >> 24) & 0xff);
}

FORCEINLINE
VOID
KiSetGdtDescriptorLimit(
    IN OUT PKGDTENTRY Entry,
    IN UINT32 Limit)
{
    if (Limit < 0x100000)
    {
        Entry->HighWord.Bits.Granularity = 0;
    }
    else
    {
        Limit >>= 12;
        Entry->HighWord.Bits.Granularity = 1;
    }
    Entry->LimitLow = (UINT16)(Limit & 0xffff);
    Entry->HighWord.Bits.LimitHi = ((Limit >> 16) & 0x0f);
}


#ifdef _M_AMD64
VOID
NTAPI
KxInitAPProcessorState(_Out_ PKPROCESSOR_STATE ProcessorState,
                       _In_ PVOID KernelStack)
{
    UNIMPLEMENTED;
}

#else //_M_IX86
VOID
NTAPI
KxInitAPProcessorState(_Out_ PKPROCESSOR_STATE ProcessorState,
                       _In_ PVOID KernelStack)
{
        RtlZeroMemory(ProcessorState, sizeof(PKPROCESSOR_STATE));

        /* Prep Cr Regsters */
        ProcessorState->SpecialRegisters.Cr0 = __readcr0();
        ProcessorState->SpecialRegisters.Cr2 = __readcr2();
        ProcessorState->SpecialRegisters.Cr3 = __readcr3();
        ProcessorState->SpecialRegisters.Cr4 = __readcr4();

        /* Prepare Segment Registers */
        ProcessorState->ContextFrame.SegCs = KGDT_R0_CODE;
        ProcessorState->ContextFrame.SegSs = KGDT_R0_DATA;
        ProcessorState->ContextFrame.SegFs = KGDT_R0_PCR;
        ProcessorState->ContextFrame.SegDs = KGDT_R3_DATA;
        ProcessorState->ContextFrame.SegEs = KGDT_R3_DATA;

        /* Clear GS */
        ProcessorState->ContextFrame.SegGs = 0;

        /* Set  KernelStack */
        ProcessorState->ContextFrame.Esp = (ULONG_PTR)(&((PULONG)KernelStack)[-2]);

        /* Write other objects */
        ProcessorState->SpecialRegisters.Tr = KGDT_TSS;
        ProcessorState->ContextFrame.Eip = (ULONG_PTR)KiSystemStartup;
        ProcessorState->ContextFrame.EFlags = __readeflags();
}
#endif

CODE_SEG("INIT")
VOID
NTAPI
KeStartAllProcessors()
{
    PVOID KernelStack, DPCStack;
    SIZE_T ProcessorCount = 0;
    PAPINFO APInfo;

    while (TRUE)
    {
        ProcessorCount++;
        KernelStack = NULL;
        DPCStack = NULL;

        // Allocate structures for a new CPU.
        APInfo = ExAllocatePoolZero(NonPagedPool, sizeof(APINFO), TAG_KERNEL);
        if (!APInfo)
        {
            break;
        }
        KernelStack = MmCreateKernelStack(FALSE, 0);
        if (!KernelStack)
        {
            break;
        }
        DPCStack = MmCreateKernelStack(FALSE, 0);
        if (!DPCStack)
        {
            break;
        }

#if _M_IX86
        /* Initalize a new PCR for the specific AP */
        KiInitializePcr(ProcessorCount,
                        &APInfo->Pcr,
                        &APInfo->Idt[0],
                        &APInfo->Gdt[0],
                        &APInfo->Tss,
                        (PKTHREAD)&APInfo->Thread,
                        DPCStack);
#else
        KiInitializePcr(&APInfo->Pcr,
                        ProcessorCount,
                        (PKTHREAD)&APInfo->Thread,
                        DPCStack);
#endif

        // Prepare descriptor tables
        KDESCRIPTOR bspGdt, bspIdt;
        __sgdt(&bspGdt.Limit);
        __sidt(&bspIdt.Limit);
        RtlCopyMemory(&APInfo->Gdt, (PVOID)bspGdt.Base, bspGdt.Limit + 1);
        RtlCopyMemory(&APInfo->Idt, (PVOID)bspIdt.Base, bspIdt.Limit + 1);

        KiSetGdtDescriptorBase(KiGetGdtEntry(&APInfo->Gdt, KGDT_R0_PCR), (ULONG_PTR)&APInfo->Pcr);
        KiSetGdtDescriptorBase(KiGetGdtEntry(&APInfo->Gdt, KGDT_DF_TSS), (ULONG_PTR)&APInfo->TssDoubleFault);
        KiSetGdtDescriptorBase(KiGetGdtEntry(&APInfo->Gdt, KGDT_NMI_TSS), (ULONG_PTR)&APInfo->TssNMI);

        KiSetGdtDescriptorBase(KiGetGdtEntry(&APInfo->Gdt, KGDT_TSS), (ULONG_PTR)&APInfo->Tss);

#if _M_IX86
        // Clear TSS Busy flag (aka set the type to "TSS (Available)")
        KiGetGdtEntry(&APInfo->Gdt, KGDT_TSS)->HighWord.Bits.Type = I386_TSS;

        APInfo->TssDoubleFault.Esp0 = (ULONG_PTR)&APInfo->DoubleFaultStack;
        APInfo->TssDoubleFault.Esp = (ULONG_PTR)&APInfo->DoubleFaultStack;

        APInfo->TssNMI.Esp0 = (ULONG_PTR)&APInfo->DoubleFaultStack;
        APInfo->TssNMI.Esp = (ULONG_PTR)&APInfo->DoubleFaultStack;
#endif

        // Push LOADER_BLOCK on stack as a parameter
        ((PULONG)KernelStack)[-1] = (ULONG)KeLoaderBlock;

        // Push NULL address on stack as a "return" addr
        ((PULONG)KernelStack)[-2] = (ULONG)NULL;

        // Fill the processor state
        PKPROCESSOR_STATE ProcessorState = &APInfo->Pcr.Prcb->ProcessorState;
        KxInitAPProcessorState(ProcessorState, KernelStack);

        ProcessorState->SpecialRegisters.Gdtr.Base = (ULONG_PTR)APInfo->Gdt;
        ProcessorState->SpecialRegisters.Gdtr.Limit = sizeof(APInfo->Gdt) - 1;
        ProcessorState->SpecialRegisters.Idtr.Base = (ULONG_PTR)APInfo->Idt;
        ProcessorState->SpecialRegisters.Idtr.Limit = sizeof(APInfo->Idt) - 1;

        // Prepare the LOADER_PARAMETER_BLOCK structure
        KeLoaderBlock->KernelStack = (ULONG_PTR)KernelStack;
        KeLoaderBlock->Prcb = (ULONG_PTR)&APInfo->Pcr.Prcb;
        KeLoaderBlock->Thread = (ULONG_PTR)&APInfo->Pcr.Prcb->IdleThread;

        // Start the CPU
        if (!HalStartNextProcessor(KeLoaderBlock, ProcessorState))
        {
            break;
        }

        DPRINT("Waiting for init confirmation from AP CPU: #%u\n", ProcessorCount);
        while (KeNumberProcessors < (ProcessorCount + 1))
        {
            KeMemoryBarrier();
            YieldProcessor();
        }
        DPRINT("CPU Startup sucessfull!\n");
    }

    // The last CPU didn't start - clean the data
    ProcessorCount--;

    if (APInfo)
        ExFreePoolWithTag(APInfo, TAG_KERNEL);
    if (KernelStack)
        MmDeleteKernelStack(KernelStack, FALSE);
    if (DPCStack)
        MmDeleteKernelStack(DPCStack, FALSE);

    DPRINT1("KeStartAllProcessors: Sucessful AP startup count is %u\n", ProcessorCount);
}
