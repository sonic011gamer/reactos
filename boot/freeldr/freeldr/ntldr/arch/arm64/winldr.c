
#include <freeldr.h>
#include <debug.h>
DBG_DEFAULT_CHANNEL(WINDOWS);
#include <internal/arm/mm.h>
//#include <internal/arm/intrin_i.h>
#include "../../winldr.h"

#define PFN_SHIFT                   12
#define LARGE_PFN_SHIFT             20

#define PTE_BASE                    0xC0000000
#define PDE_BASE                    0xC0400000
#define PDR_BASE                    0xFFD00000
#define VECTOR_BASE                 0xFFFF0000

#ifdef _ZOOM2_
#define IDMAP_BASE                  0x81000000
#define MMIO_BASE                   0x10000000
#else
#define IDMAP_BASE                  0x00000000
#define MMIO_BASE                   0x10000000
#endif

#define LowMemPageTableIndex        (IDMAP_BASE >> PDE_SHIFT)
#define MmioPageTableIndex          (MMIO_BASE >> PDE_SHIFT)
#define KernelPageTableIndex        (KSEG0_BASE >> PDE_SHIFT)
#define StartupPtePageTableIndex    (PTE_BASE >> PDE_SHIFT)
#define StartupPdePageTableIndex    (PDE_BASE >> PDE_SHIFT)
#define PdrPageTableIndex           (PDR_BASE >> PDE_SHIFT)
#define VectorPageTableIndex        (VECTOR_BASE >> PDE_SHIFT)

#ifndef _ZOOM2_
PVOID MempPdrBaseAddress = (PVOID)0x70000;
PVOID MempKernelBaseAddress = (PVOID)0;
#else
PVOID MempPdrBaseAddress = (PVOID)0x81100000;
PVOID MempKernelBaseAddress = (PVOID)0x80000000;
#endif

/* Converts a Physical Address into a Page Frame Number */
#define PaToPfn(p)                  ((p) >> PFN_SHIFT)
#define PaToLargePfn(p)             ((p) >> LARGE_PFN_SHIFT)
#define PaPtrToPfn(p)               (((ULONG_PTR)(p)) >> PFN_SHIFT)

/* Converts a Physical Address into a Coarse Page Table PFN */
#define PaPtrToPdePfn(p)            (((ULONG_PTR)(p)) >> CPT_SHIFT)


/* FUNCTIONS **************************************************************/

BOOLEAN
MempSetupPaging(IN PFN_NUMBER StartPage,
                IN PFN_NUMBER NumberOfPages,
                IN BOOLEAN KernelMapping)
{
    TRACE("MempSetupPaging for page %X with %X pages in size Kernelmapping is %d\n", StartPage, NumberOfPages, KernelMapping);
    return TRUE;
}

VOID
MempUnmapPage(IN PFN_NUMBER Page)
{
    TRACE("Unmapping %X\n", Page);
    return;
}

VOID
MempDump(VOID)
{
    return;
}

static
BOOLEAN
WinLdrMapSpecialPages(ULONG PcrBasePage)
{
     TRACE("WinLdrMapSpecialPages: Entry\n");
    /* TODO: Map in the kernel CPTs */
    return TRUE;
}


static
BOOLEAN
MempAllocatePageTables(VOID)
{
    TRACE("MempAllocatePageTables: Entry\n");

    /* Done */
    return TRUE;
}

VOID
WinLdrSetProcessorContext(VOID)
{
    TRACE("WinLdrSetProcessorContext: Entry\n");
    //TODO: Enable paging
    //ARM_CONTROL_REGISTER ControlRegister
}

VOID
WinLdrSetupMachineDependent(
    PLOADER_PARAMETER_BLOCK LoaderBlock)
{
    TRACE("WinLdrSetupMachineDependent: Entry\n");

#if 0
    /* Allocate 2 pages for PCR: one for the boot processor PCR and one for KI_USER_SHARED_DATA */
    Pcr = (ULONG_PTR)MmAllocateMemoryWithType(sizeof(KPCR) + MM_PAGE_SIZE, LoaderStartupPcrPage);
    PcrBasePage = (ULONG_PTR)PCR >> MM_PAGE_SHIFT;
    if (Pcr == 0)
    {
        UiMessageBox("Could not allocate PCR.");
        return;
    }
#endif
    // Before we start mapping pages, create a block of memory, which will contain
    // PDE and PTEs
    if (MempAllocatePageTables() == FALSE)
    {
        BugCheck("MempAllocatePageTables failed!\n");
    }

    /* Map stuff like PCR, KI_USER_SHARED_DATA and Apic */
    WinLdrMapSpecialPages(0);
}

void _ChangeStack();
PLOADER_PARAMETER_BLOCK PubLoaderBlockVA;
KERNEL_ENTRY_POINT PubKiSystemStartup;
VOID
WinldrFinalizeBoot(PLOADER_PARAMETER_BLOCK LoaderBlockVA,
                   KERNEL_ENTRY_POINT KiSystemStartup)
{
    TRACE("Preparing to jump to kernel\n");
    PubLoaderBlockVA = LoaderBlockVA;
    PubKiSystemStartup = KiSystemStartup;
    _ChangeStack();
   // _LeaveUEFIStack();
}

VOID
Arm64MegaEnter()
{
    TRACE("ARM64 CPU Stack space has changed\n");
    for(;;)
    {

    }
}