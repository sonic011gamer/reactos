
#include <freeldr.h>
#include <debug.h>
DBG_DEFAULT_CHANNEL(WINDOWS);
#include <internal/arm/mm.h>
//#include <internal/arm/intrin_i.h>
#include "../../winldr.h"


/* translation table descriptors */
#define TD_VALID                   (1 << 0)
#define TD_BLOCK                   (0 << 1)
#define TD_PAGE                    (1 << 1)
#define TD_TABLE                   (1 << 1)
/*
 * we can check ID_AA64MMFR1_EL1.HAFDBS to see if the hardware supports hw management
 * of the access flag and dirty state, but we'll just set the access flag to 1 by sw.
 */
#define TD_ACCESS                  (1 << 10)
/* D5-2739 */
/* EL1 rwx, EL0 - */
#define TD_KERNEL_PERMS            (1 << 54)
/* EL1 rw, EL0 rwx */
#define TD_USER_PERMS              (1 << 6)
/*
 * memory region shared by all cores, but this has no effect now because non-cachable and
 * device memory are always outer sharable
 * */
#define TD_INNER_SHARABLE          (3 << 8)

#define TD_KERNEL_TABLE_FLAGS      (TD_TABLE | TD_VALID)
#define TD_KERNEL_BLOCK_FLAGS      (TD_ACCESS | TD_INNER_SHARABLE | TD_KERNEL_PERMS | (MATTR_NORMAL_NC_INDEX << 2) | TD_BLOCK | TD_VALID)
#define TD_DEVICE_BLOCK_FLAGS      (TD_ACCESS | TD_INNER_SHARABLE | TD_KERNEL_PERMS | (MATTR_DEVICE_nGnRnE_INDEX << 2) | TD_BLOCK | TD_VALID)
#define TD_USER_TABLE_FLAGS        (TD_TABLE | TD_VALID)
#define TD_USER_PAGE_FLAGS         (TD_ACCESS | TD_INNER_SHARABLE | TD_USER_PERMS | (MATTR_NORMAL_NC_INDEX << 2) | TD_PAGE | TD_VALID)

/* memory attribute indirect register */
#define MATTR_DEVICE_nGnRnE        0x0
#define MATTR_NORMAL_NC            0x44
#define MATTR_DEVICE_nGnRnE_INDEX  0
#define MATTR_NORMAL_NC_INDEX      1
#define MAIR_EL1_VAL               ((MATTR_NORMAL_NC << (8 * MATTR_NORMAL_NC_INDEX)) | MATTR_DEVICE_nGnRnE << (8 * MATTR_DEVICE_nGnRnE_INDEX))

/* translation control register */
#define TCR_TG1_4K     (2 << 30)
#define TCR_T1SZ       ((64 - 48) << 16)
#define TCR_TG0_4K     (0 << 14)
#define TCR_T0SZ       (64 - 48)
#define TCR_EL1_VAL    (TCR_TG1_4K | TCR_T1SZ | TCR_TG0_4K | TCR_T0SZ)

#undef PAGE_MASK
#define PAGE_MASK  0xfffffffffffff000
#undef PAGE_SHIFT
#define PAGE_SHIFT 12
#define TABLE_SHIFT 9
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT)
#define SECTION_SIZE (1 << SECTION_SHIFT)

#define LOW_MEMORY (2* SECTION_SIZE)

#define ID_MAP_PAGES           3
#define HIGH_MAP_PAGES         6
#define ID_MAP_TABLE_SIZE      (ID_MAP_PAGES * PAGE_SIZE)
#define HIGH_MAP_TABLE_SIZE    (HIGH_MAP_PAGES * PAGE_SIZE)
#define ENTRIES_PER_TABLE      512
#define PGD_SHIFT              (PAGE_SHIFT + 3 * TABLE_SHIFT)
#define PUD_SHIFT              (PAGE_SHIFT + 2 * TABLE_SHIFT)
#define PMD_SHIFT              (PAGE_SHIFT + TABLE_SHIFT)
#define PUD_ENTRY_MAP_SIZE     (1 << PUD_SHIFT)
#define ID_MAP_SIZE            (8 * SECTION_SIZE)

#define LINEAR_MAP_BASE 0xffff000000000000
#define PA_TO_KVA(pa)   ((pa) + LINEAR_MAP_BASE)
#define KVA_TO_PA(kva)  ((kva) - LINEAR_MAP_BASE)

#define USER_SP_INIT_POS (2 * PAGE_SIZE)

#define PtrToPfn(p) \
    ((((ULONGLONG)p) >> PAGE_SHIFT) & 0xfffffffULL)
#define LowMemPageTableIndex        (IDMAP_BASE >> PDE_SHIFT)
#define MmioPageTableIndex          (MMIO_BASE >> PDE_SHIFT)
#define KernelPageTableIndex        (KSEG0_BASE >> PDE_SHIFT)
#define StartupPtePageTableIndex    (PTE_BASE >> PDE_SHIFT)
#define StartupPdePageTableIndex    (PDE_BASE >> PDE_SHIFT)
#define PdrPageTableIndex           (PDR_BASE >> PDE_SHIFT)
#define VectorPageTableIndex        (VECTOR_BASE >> PDE_SHIFT)

/* Converts a Physical Address into a Page Frame Number */
#define PaToPfn(p)                  ((p) >> PFN_SHIFT)
#define PaToLargePfn(p)             ((p) >> LARGE_PFN_SHIFT)
#define PaPtrToPfn(p)               (((ULONG_PTR)(p)) >> PFN_SHIFT)

/* Converts a Physical Address into a Coarse Page Table PFN */
#define PaPtrToPdePfn(p)            (((ULONG_PTR)(p)) >> CPT_SHIFT)

PHARDWARE_PTE PgdBase; /* PGD */
// PAGE SIZE IS 0x1000

/*
 PFD
 PUD
 PMD
 PTE
 */
#define PTI_SHIFT  12L
#define PDI_SHIFT  21L
#define PPI_SHIFT  30L
#define PXI_SHIFT  39L
#define VAtoPXI(va) ((((ULONG64)(va)) >> PGD_SHIFT) & 0x1FF)
#define VAtoPGI(va) ((((ULONG64)(va)) >> PGD_SHIFT) & 0x1FF)

#define VAtoPPI(va) ((((ULONG64)(va)) >> PUD_SHIFT) & 0x1FF)
#define VAtoPUI(va) ((((ULONG64)(va)) >> PUD_SHIFT) & 0x1FF)

#define VAtoPDI(va) ((((ULONG64)(va)) >> PMD_SHIFT) & 0x1FF)
#define VAtoPMI(va) ((((ULONG64)(va)) >> PMD_SHIFT) & 0x1FF)

#define VAtoPTI(va) ((((ULONG64)(va)) >> PAGE_SHIFT) & 0x1FF)
//#define VAtoPMI(va) ((((ULONG64)(va)) >> PAGE_SHIFT))


#define PGD_BASE    0xFFFFF6FB7DBED000ULL
#define PGD_SELFMAP 0xFFFFF6FB7DBEDF68ULL
#define PUD_BASE    0xFFFFF6FB7DA00000ULL
#define PMD_BASE    0xFFFFF6FB40000000ULL
#define PTE_BASE    0xFFFFF68000000000ULL
#define PGD_TOP     0xFFFFF6FB7DBEDFFFULL
#define PUD_TOP     0xFFFFF6FB7DBFFFFFULL
#define PMD_TOP     0xFFFFF6FB7FFFFFFFULL
#define PTE_TOP     0xFFFFF6FFFFFFFFFFULL

/* FUNCTIONS **************************************************************/
PLOADER_PARAMETER_BLOCK PubLoaderBlockVA;
KERNEL_ENTRY_POINT PubKiSystemStartup;
extern PVOID OsLoaderBase;
extern SIZE_T OsLoaderSize;
static
BOOLEAN
WinLdrMapSpecialPages(ULONG PcrBasePage)
{
     TRACE("WinLdrMapSpecialPages: Entry\n");
    /* TODO: Map in the kernel CPTs */
    return TRUE;
}


static
PHARDWARE_PTE
MempGetOrCreatePageDir(PHARDWARE_PTE PdeBase, ULONG Index)
{
    PHARDWARE_PTE SubDir;

    if (!PdeBase)
        return NULL;

    if (!PdeBase[Index].Valid)
    {
        SubDir = MmAllocateMemoryWithType(PAGE_SIZE, LoaderMemoryData);
        if (!SubDir)
            return NULL;
        RtlZeroMemory(SubDir, PAGE_SIZE);
        PdeBase[Index].PageFrameNumber = PtrToPfn(SubDir);
        PdeBase[Index].Valid = 1;
        PdeBase[Index].Writable = 1;
    }
    else
    {
        SubDir = (PVOID)((ULONG64)(PdeBase[Index].PageFrameNumber) * PAGE_SIZE);
    }
    return SubDir;
}

#if 0
PpeBase = MempGetOrCreatePageDir(PxeBase, VAtoPXI(VirtualAddress));
PdeBase = MempGetOrCreatePageDir(PpeBase, VAtoPPI(VirtualAddress));
PteBase = MempGetOrCreatePageDir(PdeBase, VAtoPDI(VirtualAddress));
#endif
/*
 PFD
 PUD
 PMD
 PTE
 */
static
BOOLEAN
MempMapSinglePage(ULONG64 VirtualAddress, ULONG64 PhysicalAddress)
{
    PHARDWARE_PTE PpeBase, PdeBase, PteBase;
    ULONG Index;

    PpeBase = MempGetOrCreatePageDir(PgdBase, VAtoPGI(VirtualAddress));
    PdeBase = MempGetOrCreatePageDir(PpeBase, VAtoPUI(VirtualAddress));
    PteBase = MempGetOrCreatePageDir(PdeBase, VAtoPMI(VirtualAddress));

    if (!PteBase)
    {
        ERR("!!!No Dir %p, %p, %p, %p\n", PgdBase, PpeBase, PdeBase, PteBase);
        return FALSE;
    }

    Index = VAtoPTI(VirtualAddress);
    if (PteBase[Index].Valid)
    {
        ERR("!!!Already mapped %ld\n", Index);
        return FALSE;
    }

    PteBase[Index].Valid = 1;
    PteBase[Index].Writable = 1;
    PteBase[Index].PageFrameNumber = PhysicalAddress / PAGE_SIZE;

    return TRUE;
}

BOOLEAN
MempIsPageMapped(PVOID VirtualAddress)
{
    PHARDWARE_PTE PpeBase, PdeBase, PteBase;
    ULONG Index;

    Index = VAtoPGI(VirtualAddress);
    if (!PgdBase[Index].Valid)
        return FALSE;

    PpeBase = (PVOID)((ULONG64)(PgdBase[Index].PageFrameNumber) * PAGE_SIZE);
    Index = VAtoPUI(VirtualAddress);
    if (!PpeBase[Index].Valid)
        return FALSE;

    PdeBase = (PVOID)((ULONG64)(PpeBase[Index].PageFrameNumber) * PAGE_SIZE);
    Index = VAtoPMI(VirtualAddress);
    if (!PdeBase[Index].Valid)
        return FALSE;

    PteBase = (PVOID)((ULONG64)(PdeBase[Index].PageFrameNumber) * PAGE_SIZE);
    Index = VAtoPTI(VirtualAddress);
    if (!PteBase[Index].Valid)
        return FALSE;

    return TRUE;
}

static
PFN_NUMBER
MempMapRangeOfPages(ULONG64 VirtualAddress, ULONG64 PhysicalAddress, PFN_NUMBER cPages)
{
    PFN_NUMBER i;

    for (i = 0; i < cPages; i++)
    {
        if (!MempMapSinglePage(VirtualAddress, PhysicalAddress))
        {
            ERR("Failed to map page %ld from %p to %p\n",
                    i, (PVOID)VirtualAddress, (PVOID)PhysicalAddress);
            return i;
        }
        VirtualAddress += PAGE_SIZE;
        PhysicalAddress += PAGE_SIZE;
    }
    return i;
}

BOOLEAN
MempSetupPaging(IN PFN_NUMBER StartPage,
                IN PFN_NUMBER NumberOfPages,
                IN BOOLEAN KernelMapping)
{
    TRACE(">>> MempSetupPaging(0x%lx, %ld, %p)\n",
            StartPage, NumberOfPages, StartPage * PAGE_SIZE + KSEG0_BASE);
    /* Kernel mapping */
    if (KernelMapping)
    {
        if (MempMapRangeOfPages(StartPage * PAGE_SIZE + KSEG0_BASE,
                                StartPage * PAGE_SIZE,
                                NumberOfPages) != NumberOfPages)
        {
            ERR("Failed to map pages %ld, %ld\n",
                    StartPage, NumberOfPages);
            return FALSE;
        }
    }
    /* Identity mapping */
    if (MempMapRangeOfPages(StartPage * PAGE_SIZE,
                            StartPage * PAGE_SIZE,
                            NumberOfPages) != NumberOfPages)
    {
        ERR("Failed to map pages %ld, %ld\n",
                StartPage, NumberOfPages);
        return FALSE;
    }

    return TRUE;
}


#define VAtoPGD(va) ((((ULONG64)(va)) >> PGD_SHIFT) & 0x1FF)
static
BOOLEAN
MempAllocatePageTables(VOID)
{
    TRACE(">>> MempAllocatePageTables\n");

    /* Allocate a page for the PGD */
    (PVOID)PgdBase = MmAllocateMemoryWithType(PAGE_SIZE, LoaderMemoryData);
    if (!PgdBase)
    {
        ERR("failed to allocate PGD\n");
        return FALSE;
    }

    /* Zero the PML4 */
    RtlZeroMemory((PVOID)PgdBase, PAGE_SIZE);
    PgdBase[VAtoPGD(PGD_BASE)].Valid = 1;
    PgdBase[VAtoPGD(PGD_BASE)].Writable = 1;
    PgdBase[VAtoPGD(PGD_BASE)].PageFrameNumber = PtrToPfn(PgdBase);

    TRACE(">>> leave MempAllocatePageTables\n");
    MempMapRangeOfPages((ULONG_PTR)OsLoaderBase,(ULONG_PTR)OsLoaderBase, OsLoaderSize / PAGE_SIZE);

    return TRUE;
}






#if 0
https://github.com/s-matyukevich/raspberry-pi-os/blob/master/docs/lesson06/rpi-os.md

here are 4 levels in the table hierarchy: PGD (Page Global Directory), P
UD (Page Upper Directory), PMD (Page Middle Directory),
PTE (Page Table Entry). PTE is the last table in the hierarchy
and it points to the actual page in the physical memory.

Memory translation process starts by locating the address of PGD (Page Global Directory) table.
The address of this table is stored in the ttbr0_el1 register. Each process has its own copy of all page tables,
including PGD, and therefore each process must keep its PGD address. During a context switch, PGD address of the next process is
loaded into the ttbr0_el1 register.

Next, MMU uses PGD pointer and virtual address to calculate the corresponding physical address. All virtual addresses use only 48 out of 64 available bits. When doing a translation, MMU splits an address into 4 parts:
Bits [39 - 47] contain an index in the PGD table. MMU uses this index to find the location of the PUD.
Bits [30 - 38] contain an index in the PUD table. MMU uses this index to find the location of the PMD.
Bits [21 - 29] contain an index in the PMD table. MMU uses this index to find the location of the PTE.
Bits [12 - 20] contain an index in the PTE table. MMU uses this index to find a page in the physical memory.
Bits [0 - 11] contain an offset in the physical page. MMU uses this offset to determine the exact position in the previously found page that corresponds to the original virtual address.

                           Virtual address                                                                 Physical Memory
+-----------------------------------------------------------------------+                                +------------------+
|         | PGD Index | PUD Index | PMD Index | PTE Index | Page offset |                                |                  |
+-----------------------------------------------------------------------+                                |                  |
63        47     |    38      |   29     |    20    |     11      |     0                                |     Page N       |
                 |            |          |          |             +--------------------+           +---->+------------------+
                 |            |          |          +---------------------+            |           |     |                  |
          +------+            |          |                                |            |           |     |                  |
          |                   |          +----------+                     |            |           |     |------------------|
+------+  |        PGD        |                     |                     |            +---------------->| Physical address |
| ttbr |---->+-------------+  |           PUD       |                     |                        |     |------------------|
+------+  |  |             |  | +->+-------------+  |          PMD        |                        |     |                  |
          |  +-------------+  | |  |             |  | +->+-------------+  |          PTE           |     +------------------+
          +->| PUD address |----+  +-------------+  | |  |             |  | +->+--------------+    |     |                  |
             +-------------+  +--->| PMD address |----+  +-------------+  | |  |              |    |     |                  |
             |             |       +-------------+  +--->| PTE address |----+  +-------------_+    |     |                  |
             +-------------+       |             |       +-------------+  +--->| Page address |----+     |                  |
                                   +-------------+       |             |       +--------------+          |                  |
                                                         +-------------+       |              |          |                  |
                                                                               +--------------+          +------------------+
#endif








/* ************************************************************************/
void KiEnableMmu(void);
void KiDisableMmu(void);
VOID
WinLdrSetProcessorContext(VOID)
{
    TRACE("WinLdrSetProcessorContext: Entry\n");
    KiEnableMmu();

    TRACE("Enabled MMU\n");
    //TODO: Enable paging
    //ARM_CONTROL_REGISTER ControlRegister
}

VOID
ARM64IsAwesome()
{
    TRACE("Jumping to kernel\n");
    TRACE("Kernel paged Status is %d\n", MempIsPageMapped(PubLoaderBlockVA));
    TRACE("Freeloader Paged status is %d\n", MempIsPageMapped(OsLoaderBase));
    TRACE("Hello from paged mode, KiSystemStartup %p, LoaderBlockVA %p!\n",
          PubKiSystemStartup, PubLoaderBlockVA);

    (*PubKiSystemStartup)(PubLoaderBlockVA);
    for(;;)
    {

    }
}
VOID
WinLdrSetupMachineDependent(
    PLOADER_PARAMETER_BLOCK LoaderBlock)
{
    TRACE("WinLdrSetupMachineDependent: Entry\n");
    ULONG PcrBasePage;
    ULONG_PTR Pcr;
    /* Allocate 2 pages for PCR: one for the boot processor PCR and one for KI_USER_SHARED_DATA */
    Pcr = (ULONG_PTR)MmAllocateMemoryWithType(sizeof(KIPCR) + MM_PAGE_SIZE, LoaderStartupPcrPage);
    PcrBasePage = (ULONG_PTR)Pcr >> MM_PAGE_SHIFT;
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

  //  WinLdrMapSpecialPages(PcrBasePage);
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

/* THIS IS FINISHED SHOULDN'T BE CHANGED -----------------------------------*/
void _ChangeStack();
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

int KiGetCurrentExceptionLevel(VOID);
VOID
Arm64MegaEnter()
{
    /* Actually start kernel */
    TRACE("ARM64 CPU Stack space has changed\n");
    WinLdrSetProcessorContext();
    TRACE("Paging should be enabled, passing to kernel\n");

    TRACE("Stack test\n");
    TRACE("Current exception level is: %d\n", KiGetCurrentExceptionLevel());
    /* Pass control */
    (*PubKiSystemStartup)(PubLoaderBlockVA);
    for(;;)
    {

    }
}