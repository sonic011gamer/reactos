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
#define KERNEL_VERSION_BUILD        20230510
#define KERNEL_VERSION_BUILD_STR    "20230510-0.4.15-dev-6031-g390f96b"
#define KERNEL_VERSION_BUILD_RC     "20230510-0.4.15-dev-6031-g390f96b\0"

#define KERNEL_VERSION_RC           "0.4.15-arm64-dev\0"
#define KERNEL_VERSION_STR          "0.4.15-arm64-dev"
#define KERNEL_VERSION_COMMIT_HASH  "707e1b4aaa60b0a6c49a9ddccb220bca47329e43"

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
#define QEMUUART 0x09000000
volatile unsigned int * UART0DR = (unsigned int *) QEMUUART;

BOOLEAN
Rs232PortInitialize(IN ULONG ComPort,
                    IN ULONG BaudRate)
{
    return TRUE;
}

VOID
Rs232PortPutByte(UCHAR ByteToSend)
{
    *UART0DR = ByteToSend;
}

ULONG
DbgPrintEarly(const char *fmt, ...)
{
    va_list args;
    unsigned int i;
    char Buffer[1024];
    PCHAR String = Buffer;

    va_start(args, fmt);
    i = vsprintf(Buffer, fmt, args);
    va_end(args);

    /* Output the message */
    while (*String != 0)
    {
        if (*String == '\n')
        {
            Rs232PortPutByte('\r');
        }
        Rs232PortPutByte(*String);
        String++;
    }

    return STATUS_SUCCESS;
}

#define DPRINT1 DbgPrintEarly
static CODE_SEG("INIT")
VOID
KdpPrintBanner(IN SIZE_T MemSizeMBs)
{
    DbgPrintEarly("-----------------------------------------------------\n");
    DbgPrintEarly("ReactOS " KERNEL_VERSION_STR " (Build " KERNEL_VERSION_BUILD_STR ") (Commit " KERNEL_VERSION_COMMIT_HASH ")\n");
    DbgPrintEarly("%u System Processor [%u MB Memory]\n", KeNumberProcessors, MemSizeMBs);

    if (KeLoaderBlock)
    {
        DbgPrintEarly("Command Line: %s\n", KeLoaderBlock->LoadOptions);
        DbgPrintEarly("ARC Paths: %s %s %s %s\n", KeLoaderBlock->ArcBootDeviceName, KeLoaderBlock->NtHalPathName, KeLoaderBlock->ArcHalDeviceName, KeLoaderBlock->NtBootPathName);
    }
}


CODE_SEG("INIT")
DECLSPEC_NORETURN
VOID
NTAPI
KiSystemStartup(IN PLOADER_PARAMETER_BLOCK LoaderBlock)
{
    Rs232PortPutByte('C');
    for(;;)
    {

    }
}

VOID
NTAPI
testFunc()
{

}