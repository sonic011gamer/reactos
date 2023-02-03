#include "hd4000.h"
#include <debug.h>
VOID
GfxPrintPortState(PULONG BaseAddr)
{
    DPRINT1("HDMI_CTL_B: 0x%08X\n", VideoPortReadPortUlong(BaseAddr + HDMI_CTL_B));
    DPRINT1("HDMI_CTL_C: 0x%08X\n", VideoPortReadPortUlong(BaseAddr + HDMI_CTL_C));
    DPRINT1("HDMI_CTL_D: 0x%08X\n", VideoPortReadPortUlong(BaseAddr + HDMI_CTL_D));
}

VOID
GfxPrintPipeState(PULONG BaseAddr)
{
    //for (UINT32 i = 0; i < 3; ++i)
    UINT32 i = 0;
    {
        UINT32 pipe = 0x1000 * i;
        DPRINT1("PIPE %d\n", i);
        // Output Timing
        DPRINT1("  CONF: %08x\n", VideoPortReadPortUlong(BaseAddr + PIPE_CONF_A + pipe));
        UINT32 htotal = VideoPortReadPortUlong(BaseAddr + PIPE_HTOTAL_A + pipe);
        UINT32 hactive = (htotal & 0xffff) + 1;
        UINT32 htotal_size = ((htotal >> 16) & 0xffff) + 1;
        DPRINT1("  HTOTAL: %08x %d,%d\n", htotal, hactive, htotal_size);
        UINT32 hblank = VideoPortReadPortUlong(BaseAddr + PIPE_HBLANK_A + pipe);
        UINT32 hblank_start = (hblank & 0xffff) + 1;
        UINT32 hblank_end = ((hblank >> 16) & 0xffff) + 1;
        DPRINT1("  HBLANK: %08x %d,%d\n", hblank, hblank_start, hblank_end);
        UINT32  hsync = VideoPortReadPortUlong(BaseAddr + PIPE_HSYNC_A + pipe);
        UINT32  hsync_start = (hsync & 0xffff) + 1;
        UINT32  hsync_end = ((hsync >> 16) & 0xffff) + 1;
        DPRINT1("  HSYNC: %08x %d,%d\n", hsync, hsync_start, hsync_end);
        UINT32  vtotal = VideoPortReadPortUlong(BaseAddr + PIPE_VTOTAL_A + pipe);
        UINT32  vactive = (vtotal & 0xffff) + 1;
        UINT32  vtotal_size = ((vtotal >> 16) & 0xffff) + 1;
        DPRINT1("  VTOTAL: %08x %d,%d\n", vtotal, vactive, vtotal_size);
        UINT32  vblank = VideoPortReadPortUlong(BaseAddr + PIPE_VBLANK_A + pipe);
        UINT32  vblank_start = (vblank & 0xffff) + 1;
        UINT32  vblank_end = ((vblank >> 16) & 0xffff) + 1;
        DPRINT1("  VBLANK: %08x %d,%d\n", vblank, vblank_start, vblank_end);
        UINT32  vsync = VideoPortReadPortUlong(BaseAddr + PIPE_VSYNC_A + pipe);
        UINT32  vsync_start = (vsync & 0xffff) + 1;
        UINT32  vsync_end = ((vsync >> 16) & 0xffff) + 1;
        DPRINT1("  VSYNC: %08x %d,%d\n", vsync, vsync_start, vsync_end);
        UINT32  srcsz = VideoPortReadPortUlong(BaseAddr + PIPE_SRCSZ_A + pipe);
        UINT32  width = ((srcsz >> 16) & 0xfff) + 1;
        UINT32  height = (srcsz & 0xffff) + 1;
        DPRINT1("  SRCSZ: %08x %dx%d\n", srcsz, width, height);
        // Cursor Plane
        DPRINT1("  CUR_CTL: %08x\n", VideoPortReadPortUlong(BaseAddr + CUR_CTL_A + pipe));
        DPRINT1("  CUR_BASE: %08x\n", VideoPortReadPortUlong(BaseAddr + CUR_BASE_A + pipe));
        DPRINT1("  CUR_POS: %08x\n", VideoPortReadPortUlong(BaseAddr + CUR_POS_A + pipe));
        // Primary Plane
        DPRINT1("  PRI_CTL: %08x\n", VideoPortReadPortUlong(BaseAddr + PRI_CTL_A + pipe));
        DPRINT1("  PRI_LINOFF: %08x\n", VideoPortReadPortUlong(BaseAddr + PRI_LINOFF_A + pipe));
        DPRINT1("  PRI_STRIDE: %08x\n", VideoPortReadPortUlong(BaseAddr + PRI_STRIDE_A + pipe));
        DPRINT1("  PRI_SURF: %08x\n", VideoPortReadPortUlong(BaseAddr + PRI_SURF_A + pipe));
    }
}

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
    VideoPortReadPortUlong((PULONG)(BaseAddr + Reg));
}

VOID
IGFX_Write32(IN PULONG BaseAddr,
             IN UINT32 Reg,
             IN ULONG Data)
{
    WRITE_REGISTER_ULONG((BaseAddr + Reg), Data);
}
#endif