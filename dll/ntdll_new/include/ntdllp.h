/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS System Libraries
 * FILE:            dll/ntdll/include/ntdllp.h
 * PURPOSE:         Native Library Internal Header
 * PROGRAMMER:      Alex Ionescu (alex@relsoft.net)
 */

#pragma once

#define LDR_HASH_TABLE_ENTRIES 32
#define LDR_GET_HASH_ENTRY(x) (RtlUpcaseUnicodeChar((x)) & (LDR_HASH_TABLE_ENTRIES - 1))

/* LdrpUpdateLoadCount2 flags */
#define LDRP_UPDATE_REFCOUNT   0x01
#define LDRP_UPDATE_DEREFCOUNT 0x02
#define LDRP_UPDATE_PIN        0x03

/* Loader flags */
#define IMAGE_LOADER_FLAGS_COMPLUS 0x00000001
#define IMAGE_LOADER_FLAGS_SYSTEM_GLOBAL 0x01000000

/* Page heap flags */
#define DPH_FLAG_DLL_NOTIFY 0x40

typedef struct _LDRP_TLS_DATA
{
    LIST_ENTRY TlsLinks;
    IMAGE_TLS_DIRECTORY TlsDirectory;
} LDRP_TLS_DATA, *PLDRP_TLS_DATA;

typedef
NTSTATUS
(NTAPI* PLDR_APP_COMPAT_DLL_REDIRECTION_CALLBACK_FUNCTION)(
    _In_ ULONG Flags,
    _In_ PCWSTR DllName,
    _In_ PCWSTR DllPath OPTIONAL,
    _Inout_opt_ PULONG DllCharacteristics,
    _In_ PVOID CallbackData,
    _Outptr_ PWSTR* EffectiveDllPath);

/* Global data */
extern RTL_CRITICAL_SECTION LdrpLoaderLock;
extern BOOLEAN LdrpInLdrInit;
extern PVOID LdrpHeap;
extern LIST_ENTRY LdrpHashTable[LDR_HASH_TABLE_ENTRIES];
extern BOOLEAN ShowSnaps;
extern UNICODE_STRING LdrpDefaultPath;
extern HANDLE LdrpKnownDllObjectDirectory;
extern ULONG LdrpNumberOfProcessors;
extern ULONG LdrpFatalHardErrorCount;
extern PUNICODE_STRING LdrpTopLevelDllBeingLoaded;
extern PLDR_DATA_TABLE_ENTRY LdrpCurrentDllInitializer;
extern UNICODE_STRING LdrApiDefaultExtension;
extern BOOLEAN LdrpLdrDatabaseIsSetup;
extern ULONG LdrpActiveUnloadCount;
extern BOOLEAN LdrpShutdownInProgress;
extern UNICODE_STRING LdrpKnownDllPath;
extern PLDR_DATA_TABLE_ENTRY LdrpGetModuleHandleCache, LdrpLoadedDllHandleCache;
extern BOOLEAN RtlpPageHeapEnabled;
extern ULONG RtlpDphGlobalFlags;
extern BOOLEAN g_ShimsEnabled;
extern PVOID g_pShimEngineModule;
extern PVOID g_pfnSE_DllLoaded;
extern PVOID g_pfnSE_DllUnloaded;
extern PVOID g_pfnSE_InstallBeforeInit;
extern PVOID g_pfnSE_InstallAfterInit;
extern PVOID g_pfnSE_ProcessDying;


/* path.c */
BOOLEAN
NTAPI
RtlDoesFileExists_UStr(
    IN PUNICODE_STRING FileName
);

/* EOF */
