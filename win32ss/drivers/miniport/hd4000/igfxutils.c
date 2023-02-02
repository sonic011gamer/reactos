#include "hd4000.h"

#if 0
TODO: Conver this to videoprt functions later
VOID
NTAPI
General_Write64(IN PULONG BaseAddr,
                IN ULONGLONG Data)
{
    WRITE_REGISTER_ULONG(BaseAddr, Data);
    WRITE_REGISTER_ULONG(BaseAddr + 1, Data >> 32);
}

ULONGLONG
IGFX_Read64(IN PULONG BaseAddr, 
            IN UINT32 Reg)
{
    VideoPortReadPortBufferUchar
    return READ_REGISTER_ULONGLONG(BaseAddr + Reg);
}

VOID
IGFX_Write64(IN PULONG BaseAddr,
             IN UINT32 Reg,
             IN ULONGLONG Data)
{
    General_Write64((BaseAddr + Reg),
                    Data);
}

ULONG
IGFX_Read32(IN PULONG BaseAddr,
             IN UINT32 Reg)
{
    return READ_REGISTER_ULONG(BaseAddr + Reg);
}

VOID
IGFX_Write32(IN PULONG BaseAddr,
             IN UINT32 Reg,
             IN ULONG Data)
{
    WRITE_REGISTER_ULONG((BaseAddr + Reg), Data);
}
#endif