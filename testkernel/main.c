/* INCLUDES ******************************************************************/

/* C Headers */
#include <stdio.h>

/* WDK HAL Compilation hack */
#include <excpt.h>
#include <ntdef.h>
#ifndef _MINIHAL_
#undef NTSYSAPI
#define NTSYSAPI __declspec(dllimport)
#else
#undef NTSYSAPI
#define NTSYSAPI
#endif

#define CHAR_WIDTH  8
#define CHAR_HEIGHT 16
#define TOP_BOTTOM_LINES 0
#define LOWEST_SUPPORTED_RES 1

/* IFS/DDK/NDK Headers */
#include <ntifs.h>
#include <arc/arc.h>

#include <ndk/asm.h>
#include <ndk/halfuncs.h>
#include <ndk/inbvfuncs.h>
#include <ndk/iofuncs.h>
#include <ndk/kefuncs.h>
#include <ndk/rtlfuncs.h>


REACTOS_INTERNAL_BGCONTEXT framebufferData;
static VOID
UefiVideoClearScreenColor(ULONG Color, BOOLEAN FullScreen)
{
    ULONG Delta;
    ULONG Line, Col;
    PULONG p;

    Delta = (framebufferData.PixelsPerScanLine * 4 + 3) & ~ 0x3;
    for (Line = 0; Line < framebufferData.ScreenHeight - (FullScreen ? 0 : 2 * TOP_BOTTOM_LINES); Line++)
    {
        p = (PULONG) ((char *) framebufferData.BaseAddress + (Line + (FullScreen ? 0 : TOP_BOTTOM_LINES)) * Delta);
        for (Col = 0; Col < framebufferData.ScreenWidth; Col++)
        {
            *p++ = Color;
        }
    }
}

CODE_SEG("INIT")
DECLSPEC_NORETURN
VOID
NTAPI
KiSystemStartup(IN PLOADER_PARAMETER_BLOCK LoaderBlock)
{
    framebufferData = LoaderBlock->BgContext;
    UefiVideoClearScreenColor(0xF0, TRUE);
    for(;;)
    {

    }
}

VOID
NTAPI
testFunc()
{

}