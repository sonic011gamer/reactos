#ifndef PRIVATE_FUNCTIONS_H
#define PRIVATE_FUNCTIONS_H

/* random utils */
VOID
GfxPrintPortState(PULONG BaseAddr);
/* RW */
ULONGLONG
IGFX_Read64(IN PULONG BaseAddr, 
            IN UINT32 Reg);
VOID
IGFX_Write64(IN PULONG BaseAddr,
             IN UINT32 Reg,
             IN ULONGLONG Data);

ULONG
IGFX_Read32(IN PULONG BaseAddr,
             IN UINT32 Reg);

VOID
IGFX_Write32(IN PULONG BaseAddr,
             IN UINT32 Reg,
             IN ULONG Data);

/* GMBus */
VOID
IGFX_GmBus_Reset();

VOID
IGFX_GmBus_StopTransaction();

VOID
IGFX_GmBus_Read();

VOID
IGFX_GmBus_Wait();

#endif // PRIVATE_FUNCTIONS_H
