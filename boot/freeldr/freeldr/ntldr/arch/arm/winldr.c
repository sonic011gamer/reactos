/*
 * PROJECT:         ReactOS Boot Loader
 * LICENSE:         BSD - See COPYING.ARM in the top level directory
 * FILE:            boot/freeldr/freeldr/arch/arm/winldr.c
 * PURPOSE:         ARM Kernel Loader
 * PROGRAMMERS:     ReactOS Portable Systems Group
 */

/* INCLUDES ***************************************************************/

#include <freeldr.h>
#include <debug.h>
DBG_DEFAULT_CHANNEL(WINDOWS);

#include <internal/arm/mm.h>
#include <internal/arm/intrin_i.h>
#include "../../winldr.h"

/* no idea if any of this is right */
PHARDWARE_PTE PDE;
PHARDWARE_PTE HalPageTable;
#define SELFMAP_ENTRY       0x300
PUCHAR PhysicalPageTablesBuffer;
PUCHAR KernelPageTablesBuffer;
ULONG PhysicalPageTables;
ULONG KernelPageTables;

/* just kernel shit */
ULONG PcrBasePage;


/* FUNCTIONS **************************************************************/

FORCEINLINE
ARM_STATUS_REGISTER
ArmStatusRegisterGet(VOID)
{
    ARM_STATUS_REGISTER Value;
#ifdef _MSC_VER
    Value.AsUlong = _ReadStatusReg(0);
#else
    __asm__ __volatile__ ("mrs %0, cpsr" : "=r"(Value.AsUlong) : : "cc");
#endif
    return Value;
}

pArmControlRegisterSet(ULONG Ttb);
FORCEINLINE
VOID
ArmControlRegisterSet(IN ARM_CONTROL_REGISTER ControlRegister)
{
#ifdef _MSC_VER
    pArmControlRegisterSet(ControlRegister.AsUlong);
#else
    __asm__ __volatile__ ("mcr p15, 0, %0, c1, c0, 0" : : "r"(ControlRegister.AsUlong) : "cc");
#endif
}

void pArmControlRegisterGet(ULONG* value);
FORCEINLINE
ARM_CONTROL_REGISTER
ArmControlRegisterGet(VOID)
{
    ARM_CONTROL_REGISTER Value;
#ifdef _MSC_VER
    Value.AsUlong = 0;
    pArmControlRegisterGet(&Value.AsUlong);
#else
    __asm__ __volatile__ ("mrc p15, 0, %0, c1, c0, 0" : "=r"(Value.AsUlong) : : "cc");
#endif
    return Value;
}

void pArmTranslationTableRegisterSet(ULONG Ttb);
FORCEINLINE
VOID
ArmTranslationTableRegisterSet(IN ARM_TTB_REGISTER Ttb)
{
#ifdef _MSC_VER
    pArmTranslationTableRegisterSet(Ttb.AsUlong);
#else
    __asm__ __volatile__ ("mcr p15, 0, %0, c2, c0, 0" : : "r"(Ttb.AsUlong) : "cc");
#endif
}

void pArmDomainRegisterSet(ULONG Ttb);
FORCEINLINE
VOID
ArmDomainRegisterSet(IN ARM_DOMAIN_REGISTER DomainRegister)
{
#ifdef _MSC_VER
    pArmDomainRegisterSet(DomainRegister.AsUlong);
#else
    __asm__ __volatile__ ("mcr p15, 0, %0, c3, c0, 0" : : "r"(DomainRegister.AsUlong) : "cc");
#endif
}

/*  IMportant shit *************************************************************/
// this is freeloader itself, if you want to make a hack. What is currently
// Executing at this moment.
extern PVOID OsLoaderBase;
extern SIZE_T OsLoaderSize;

/* pages being mapped, you will get all of ntoskrnl, hal, freeloader etc one segment at a time*/
BOOLEAN
MempSetupPaging(IN PFN_NUMBER StartPage,
                IN PFN_NUMBER NumberOfPages,
                IN BOOLEAN KernelMapping)
{
    TRACE("base Page %X\n", StartPage);
    // if it's a kernel mapping.. shift KSEG0_BASE

    return TRUE;
}

VOID
MempUnmapPage(IN PFN_NUMBER Page)
{
    return;
}

VOID
MempDump(VOID)
{
    return;
}

/* This is all just a guess based on i386 paging----- */
static
BOOLEAN
MempAllocatePageTables(VOID)
{
    TRACE("Setting up page tables\n");
    ULONG NumPageTables, TotalSize;
    PUCHAR Buffer;

    NumPageTables = TotalPagesInLookupTable >> 10;

    TRACE("NumPageTables = %d\n", NumPageTables);

    // Allocate memory block for all these things:
    // PDE, HAL mapping page table, physical mapping, kernel mapping
    TotalSize = (1 + 1 + NumPageTables * 2) * MM_PAGE_SIZE;

    // PDE+HAL+KernelPTEs == MemoryData
    Buffer = MmAllocateMemoryWithType(TotalSize, LoaderMemoryData);

    // Physical PTEs = FirmwareTemporary
    PhysicalPageTablesBuffer = (PUCHAR)Buffer + TotalSize - NumPageTables*MM_PAGE_SIZE;
    MmSetMemoryType(PhysicalPageTablesBuffer,
                    NumPageTables*MM_PAGE_SIZE,
                    LoaderFirmwareTemporary);

    // This check is now redundant
    if (Buffer + (TotalSize - NumPageTables*MM_PAGE_SIZE) !=
        PhysicalPageTablesBuffer)
    {
        TRACE("There was a problem allocating two adjacent blocks of memory!\n");
    }

    if (Buffer == NULL || PhysicalPageTablesBuffer == NULL)
    {
        UiMessageBox("Impossible to allocate memory block for page tables!");
        return FALSE;
    }

    // Zero all this memory block
    RtlZeroMemory(Buffer, TotalSize);

    // Set up pointers correctly now
    PDE = (PHARDWARE_PTE)Buffer;

    // Map the page directory at 0xC0000000 (maps itself)
    PDE[SELFMAP_ENTRY].PageFrameNumber = (ULONG)PDE >> MM_PAGE_SHIFT;
    PDE[SELFMAP_ENTRY].Valid = 1;
    PDE[SELFMAP_ENTRY].ReadOnly = 0;

    // The last PDE slot is allocated for HAL's memory mapping (Virtual Addresses 0xFFC00000 - 0xFFFFFFFF)
    HalPageTable = (PHARDWARE_PTE)&Buffer[MM_PAGE_SIZE*1];

    // Map it
    PDE[1023].PageFrameNumber = (ULONG)HalPageTable >> MM_PAGE_SHIFT;
    PDE[1023].Valid = 1;
    PDE[1023].ReadOnly = 0;

    // Store pointer to the table for easier access
    KernelPageTablesBuffer = &Buffer[MM_PAGE_SIZE*2];

    // Zero counters of page tables used
    PhysicalPageTables = 0;
    KernelPageTables = 0;

    /* Done */
    return TRUE;
}

/* THIS IS BEFORE PAGES START BEING MAPPED ------*/
VOID
WinLdrSetupMachineDependent(
    PLOADER_PARAMETER_BLOCK LoaderBlock)
{
    TRACE("Preperaing for paging\n");
    ULONG_PTR Pcr = 0;
    Pcr = (ULONG_PTR)MmAllocateMemoryWithType(OsLoaderSize + MM_PAGE_SIZE, LoaderStartupPcrPage);
    PcrBasePage = (ULONG_PTR)OsLoaderBase >> MM_PAGE_SHIFT;
    if (Pcr == 0)
    {
        UiMessageBox("Could not allocate PCR.");
        return;
    }

    // Before we start mapping pages, create a block of memory, which will contain
    // PDE and PTEs
    if (MempAllocatePageTables() == FALSE)
    {
        BugCheck("MempAllocatePageTables failed!\n");
    }
}

/* THIS IS AFTER ALL PAGES ARE MAPPED ------*/
extern PLOADER_PARAMETER_BLOCK PubLoaderBlockVA;
extern KERNEL_ENTRY_POINT PubKiSystemStartup;

VOID
WinLdrSetProcessorContext(VOID)
{
    TRACE("WinLdrSetProcessorContext: Entry");
    ARM_CONTROL_REGISTER ControlRegister;
    ARM_TTB_REGISTER TtbRegister;
    ARM_DOMAIN_REGISTER DomainRegister;

    /* Set the TTBR */
    TtbRegister.AsUlong = (ULONG_PTR)PDE; //I have no idea if this is right
    if (PDE == NULL)
    {
        TRACE("The page tables are null\n");
    }

    ArmTranslationTableRegisterSet(TtbRegister);

    /* Disable domains and simply use access bits on PTEs */
    DomainRegister.AsUlong = 0;
    DomainRegister.Domain0 = ClientDomain;
    ArmDomainRegisterSet(DomainRegister);

    TRACE("Enabling paging\n");
    /* Enable ARMv6+ paging (MMU), caches and the access bit */
    ControlRegister = ArmControlRegisterGet();
    ControlRegister.MmuEnabled = TRUE;
    ControlRegister.ICacheEnabled = TRUE;
    ControlRegister.DCacheEnabled = TRUE;
    ControlRegister.ForceAp = TRUE;
    ControlRegister.ExtendedPageTables = TRUE;
    ArmControlRegisterSet(ControlRegister);

    TRACE("Jumping to kernel\n");
    /* this will launch ntoskrnl */
    (*PubKiSystemStartup)(PubLoaderBlockVA);
}
