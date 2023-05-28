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

typedef struct _LDRP_CSLIST_ENTRY
{
    SINGLE_LIST_ENTRY DependenciesLink;
    struct _LDR_DDAG_NODE* DependencyNode;
    SINGLE_LIST_ENTRY IncomingDependenciesLink;
    struct _LDR_DDAG_NODE* ParentNode;
#ifndef NDEBUG
    ULONG Debug;
#endif
} LDRP_CSLIST_ENTRY, *PLDRP_CSLIST_ENTRY;

typedef enum _LDR_DDAG_STATE
{
    LdrModulesMerged = -5,
    LdrModulesInitError = -4,
    LdrModulesSnapError = -3,
    LdrModulesUnloaded = -2,
    LdrModulesUnloading = -1,
    LdrModulesPlaceHolder = 0,
    LdrModulesMapping = 1,
    LdrModulesMapped = 2,
    LdrModulesWaitingForDependencies = 3,
    LdrModulesSnapping = 4,
    LdrModulesSnapped = 5,
    LdrModulesCondensed = 6,
    LdrModulesReadyToInit = 7,
    LdrModulesInitializing = 8,
    LdrModulesReadyToRun = 9
} LDR_DDAG_STATE, *PLDR_DDAG_STATE;
#define LdrpCreateUnicodeStringBundlePtr(Variable) RtlInitEmptyUnicodeString(&(Variable)->String, (Variable)->StaticBuffer, sizeof((Variable)->StaticBuffer)), (Variable)->String.Buffer[0] = UNICODE_NULL;
#define LdrpCreateUnicodeStringBundle(Variable) LdrpCreateUnicodeStringBundlePtr(&(Variable))
#define LdrpIsUsingUnicodeStringBundleBufferPtr(Variable) ((Variable)->String.Buffer == (Variable)->StaticBuffer)
#define LdrpFreeUnicodeStringBundlePtr(Variable) if (!LdrpIsUsingUnicodeStringBundleBufferPtr((Variable))) LdrpHeapFree(0, (Variable)->String.Buffer); LdrpCreateUnicodeStringBundlePtr(Variable);
#define LdrpFreeUnicodeStringBundle(Variable) LdrpFreeUnicodeStringBundlePtr(&(Variable))
#define LDRP_FIRST_MODULE(Node) (CONTAINING_RECORD((Node)->Modules.Flink, LDR_DATA_TABLE_ENTRY, NodeModuleLink))
#define LDRP_NODE_MODULE_PINNED(Node, Module) ((Node)->LoadCount == LDR_LOADCOUNT_MAX || (Module)->ProcessStaticImport)
#define LDRP_NODE_PINNED(Node) LDRP_NODE_MODULE_PINNED(Node, LDRP_FIRST_MODULE(Node))
#define LDRP_MODULE_PINNED(Module) LDRP_NODE_MODULE_PINNED((Module)->DdagNode, Module)

typedef struct _LDRP_UNICODE_STRING_BUNDLE
{
    UNICODE_STRING String;
    WCHAR StaticBuffer[128];
} LDRP_UNICODE_STRING_BUNDLE, * PLDRP_UNICODE_STRING_BUNDLE;

typedef struct _LDRP_CSLIST
{
    // see LDRP_CSLIST_ENTRY.DependenciesLink or LDRP_CSLIST_ENTRY.IncomingDependenciesLink
    PSINGLE_LIST_ENTRY Tail;
} LDRP_CSLIST, *PLDRP_CSLIST;

typedef struct _LDR_SERVICE_TAG_RECORD
{
    struct _LDR_SERVICE_TAG_RECORD* Next;
    ULONG32 ServiceTag;
} LDR_SERVICE_TAG_RECORD, *PLDR_SERVICE_TAG_RECORD;

// Loader Distributed Dependency Graph Node (as in Windows Internals)
// DDAG likely stands for Distributed Dependency Acyclic Graph
typedef struct _LDR_DDAG_NODE
{
    LIST_ENTRY Modules;
    LDR_SERVICE_TAG_RECORD* ServiceTagList;
    ULONG32 LoadCount;
    ULONG32 LoadWhileUnloadingCount;
    ULONG32 LowestLink;
    LDRP_CSLIST Dependencies;
    LDRP_CSLIST IncomingDependencies;
    LDR_DDAG_STATE State;
    SINGLE_LIST_ENTRY CondenseLink;
    ULONG32 PreorderNumber;
#ifndef NDEBUG
    ULONG Debug;
#endif
} LDR_DDAG_NODE, *PLDR_DDAG_NODE;

typedef union
{
    ULONG32 Flags;

    struct
    {
#if 0
        ULONG32 LogWarning : 1;
        ULONG32 LogInformation : 1;
        ULONG32 LogDebug : 1;
        ULONG32 LogTrace : 1;
#endif
        ULONG32 BreakInDebugger : 1;
    };
} LDRP_DEBUG_FLAGS;

#define LDRP_BITMAP_BITALIGN (sizeof(ULONG)*8)
#define LDRP_BITMAP_CALC_ALIGN(x, base) (((x) + (base) - 1) / (base))

#define PTR_ADD_OFFSET(Pointer, Offset) ((PVOID)((ULONG_PTR)(Pointer) + (ULONG_PTR)(Offset)))
#define PTR_SUB_OFFSET(Pointer, Offset) ((PVOID)((ULONG_PTR)(Pointer) - (ULONG_PTR)(Offset)))

typedef struct _LDRP_TLS_DATA
{
    LIST_ENTRY TlsLinks;
    IMAGE_TLS_DIRECTORY TlsDirectory;
} LDRP_TLS_DATA, *PLDRP_TLS_DATA;
typedef struct _LDRP_PATH_SEARCH_CONTEXT
{
    UNICODE_STRING DllSearchPath;
    BOOLEAN AllocatedOnLdrpHeap;
    LDRP_PATH_SEARCH_OPTIONS SearchOptions;
    UNICODE_STRING OriginalFullDllName; // e.g. for forwarders
} LDRP_PATH_SEARCH_CONTEXT, *PLDRP_PATH_SEARCH_CONTEXT;
typedef union _LDRP_LOAD_CONTEXT_FLAGS
{
    ULONG32 Flags;

    struct
    {
        ULONG32 ProcessImage : 1;
        ULONG32 Redirected : 1;
        ULONG32 BaseNameOnly : 1;
        ULONG32 HasFullPath : 1;
        ULONG32 KnownDll : 1;
        ULONG32 SystemImage : 1;
        ULONG32 ExecutableImage : 1;
        ULONG32 AppContainerImage : 1;
        ULONG32 CallInit : 1;
        ULONG32 UserAllocated : 1;
        ULONG32 SearchOnlyFirstPathSegment : 1;
        ULONG32 RedirectedByAPISet : 1;
        ULONG32 CorImage : 1;
        ULONG32 InRetryQueue : 1;
        ULONG32 RetryDependentLoadWhenMissing : 1;
    };
} LDRP_LOAD_CONTEXT_FLAGS, *PLDRP_LOAD_CONTEXT_FLAGS;

typedef struct _LDRP_LOAD_CONTEXT
{
    UNICODE_STRING DllName;
    PLDRP_PATH_SEARCH_CONTEXT PathSearchContext;
    HANDLE MappedSectionHandle;
    LDRP_LOAD_CONTEXT_FLAGS Flags;
    // Unknown DWORD
    NTSTATUS* LoadStatus;
    PLDR_DATA_TABLE_ENTRY ParentModule;
    PLDR_DATA_TABLE_ENTRY Module;
    LIST_ENTRY QueueListEntry; // LdrpWorkQueue or LdrpRetryQueue
    PLDR_DATA_TABLE_ENTRY PendingModule;
    struct
    {
        PLDR_DATA_TABLE_ENTRY* ImportArray;
        ULONG ImportCount;
        ULONG32 UnmappedChildCount;
        PVOID IATBase;
        ULONG IATSize;
        ULONG ImportIndex; // 0 <= ImportIndex <= ImportCount
        ULONG ThunkIndex;
        PIMAGE_IMPORT_DESCRIPTOR ImportDescriptors;
        ULONG IATOriginalProtection;
    };
    // Unknown DWORD
    PVOID GuardCFCheckFunctionPointer;
    PVOID* GuardCFCheckFunctionPointerThunk;
    SIZE_T MappedSectionSize;
    // Enclave Context
    HANDLE MappedFileHandle;
    // ProcessImageSectionViewBase
#ifndef NDEBUG
    ULONG Debug;
#endif
    WCHAR DllNameStorage[ANYSIZE_ARRAY];
} LDRP_LOAD_CONTEXT, *PLDRP_LOAD_CONTEXT;

typedef
NTSTATUS
(NTAPI* PLDR_APP_COMPAT_DLL_REDIRECTION_CALLBACK_FUNCTION)(
    _In_ ULONG Flags,
    _In_ PCWSTR DllName,
    _In_ PCWSTR DllPath OPTIONAL,
    _Inout_opt_ PULONG DllCharacteristics,
    _In_ PVOID CallbackData,
    _Outptr_ PWSTR* EffectiveDllPath);
#ifdef __cplusplus
extern "C" {
#endif
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
#ifdef __cplusplus
}
#endif
/* ldrinit.c */
NTSTATUS NTAPI LdrpRunInitializeRoutines(IN PCONTEXT Context OPTIONAL);
VOID NTAPI LdrpInitializeThread(IN PCONTEXT Context);
NTSTATUS NTAPI LdrpInitializeTls(VOID);
NTSTATUS NTAPI LdrpAllocateTls(VOID);
VOID NTAPI LdrpFreeTls(VOID);

NTSTATUS NTAPI LdrpInitializeProcess(IN PCONTEXT Context, IN PVOID SystemArgument1);
VOID NTAPI LdrpInitFailure(NTSTATUS Status);
VOID NTAPI LdrpValidateImageForMp(IN PLDR_DATA_TABLE_ENTRY LdrDataTableEntry);
VOID NTAPI LdrpEnsureLoaderLockIsHeld(VOID);

/* ldrpe.c */
NTSTATUS
NTAPI
LdrpSnapThunk(IN PVOID ExportBase,
              IN PVOID ImportBase,
              IN PIMAGE_THUNK_DATA OriginalThunk,
              IN OUT PIMAGE_THUNK_DATA Thunk,
              IN PIMAGE_EXPORT_DIRECTORY ExportEntry,
              IN ULONG ExportSize,
              IN BOOLEAN Static,
              IN LPSTR DllName);

NTSTATUS NTAPI
LdrpWalkImportDescriptor(IN LPWSTR DllPath OPTIONAL,
                         IN PLDR_DATA_TABLE_ENTRY LdrEntry);


/* ldrutils.c */
NTSTATUS NTAPI
LdrpGetProcedureAddress(IN PVOID BaseAddress,
                        IN PANSI_STRING Name,
                        IN ULONG Ordinal,
                        OUT PVOID *ProcedureAddress,
                        IN BOOLEAN ExecuteInit);

PLDR_DATA_TABLE_ENTRY NTAPI
LdrpAllocateDataTableEntry(IN PVOID BaseAddress);

VOID NTAPI
LdrpInsertMemoryTableEntry(IN PLDR_DATA_TABLE_ENTRY LdrEntry);

NTSTATUS NTAPI
LdrpLoadDll(IN BOOLEAN Redirected,
            IN PWSTR DllPath OPTIONAL,
            IN PULONG DllCharacteristics OPTIONAL,
            IN PUNICODE_STRING DllName,
            OUT PVOID *BaseAddress,
            IN BOOLEAN CallInit);

VOID NTAPI
LdrpUpdateLoadCount2(IN PLDR_DATA_TABLE_ENTRY LdrEntry,
                     IN ULONG Flags);

ULONG NTAPI
LdrpClearLoadInProgress(VOID);

NTSTATUS
NTAPI
LdrpSetProtection(PVOID ViewBase,
                  BOOLEAN Restore);

BOOLEAN NTAPI
LdrpCheckForLoadedDll(IN PWSTR DllPath,
                      IN PUNICODE_STRING DllName,
                      IN BOOLEAN Flag,
                      IN BOOLEAN RedirectedDll,
                      OUT PLDR_DATA_TABLE_ENTRY *LdrEntry);

NTSTATUS NTAPI
LdrpMapDll(IN PWSTR SearchPath OPTIONAL,
           IN PWSTR DllPath2,
           IN PWSTR DllName OPTIONAL,
           IN PULONG DllCharacteristics,
           IN BOOLEAN Static,
           IN BOOLEAN Redirect,
           OUT PLDR_DATA_TABLE_ENTRY *DataTableEntry);

PVOID NTAPI
LdrpFetchAddressOfEntryPoint(PVOID ImageBase);

VOID NTAPI
LdrpFreeUnicodeString(PUNICODE_STRING String);

VOID NTAPI
LdrpRecordUnloadEvent(_In_ PLDR_DATA_TABLE_ENTRY LdrEntry);

VOID NTAPI
LdrpGetShimEngineInterface(VOID);

VOID
NTAPI
LdrpLoadShimEngine(IN PWSTR ImageName,
                   IN PUNICODE_STRING ProcessImage,
                   IN PVOID pShimData);

VOID NTAPI
LdrpUnloadShimEngine(VOID);

/* verifier.c */

NTSTATUS NTAPI
LdrpInitializeApplicationVerifierPackage(IN HANDLE KeyHandle,
                                         IN PPEB Peb,
                                         IN BOOLEAN SystemWide,
                                         IN BOOLEAN ReadAdvancedOptions);

NTSTATUS NTAPI
AVrfInitializeVerifier(VOID);

VOID NTAPI
AVrfDllLoadNotification(IN PLDR_DATA_TABLE_ENTRY LdrEntry);

VOID NTAPI
AVrfDllUnloadNotification(IN PLDR_DATA_TABLE_ENTRY LdrEntry);

VOID NTAPI
AVrfPageHeapDllNotification(IN PLDR_DATA_TABLE_ENTRY LdrEntry);


/* FIXME: Cleanup this mess */
typedef NTSTATUS (NTAPI *PEPFUNC)(PPEB);
NTSTATUS LdrMapSections(HANDLE ProcessHandle,
                        PVOID ImageBase,
                        HANDLE SectionHandle,
                        PIMAGE_NT_HEADERS NTHeaders);
NTSTATUS LdrMapNTDllForProcess(HANDLE ProcessHandle,
                               PHANDLE NTDllSectionHandle);
ULONG
LdrpGetResidentSize(PIMAGE_NT_HEADERS NTHeaders);

NTSTATUS
NTAPI
LdrpLoadImportModule(IN PWSTR DllPath OPTIONAL,
                     IN LPSTR ImportName,
                     OUT PLDR_DATA_TABLE_ENTRY *DataTableEntry,
                     OUT PBOOLEAN Existing);

VOID
NTAPI
LdrpFinalizeAndDeallocateDataTableEntry(IN PLDR_DATA_TABLE_ENTRY Entry);


/* path.c */
BOOLEAN
NTAPI
RtlDoesFileExists_UStr(
    IN PUNICODE_STRING FileName
);



/* c kekw kekw*/
#ifdef __cplusplus
extern "C" {
#endif

NTSTATUS
NTAPI
LdrpFindLoadedDllByAddress(IN PVOID Base,
                           OUT PLDR_DATA_TABLE_ENTRY *Module,
                           OUT LDR_DDAG_STATE *DdagState OPTIONAL);

NTSTATUS
NTAPI
LdrpBuildSystem32FileName(IN PLDRP_UNICODE_STRING_BUNDLE DestinationString,
                          IN PUNICODE_STRING FileName OPTIONAL);
BOOLEAN
NTAPI
LdrpHeapFree(IN ULONG Flags OPTIONAL,
             IN PVOID BaseAddress);
NTSTATUS
NTAPI
LdrpCorInitialize(OUT PLDR_DATA_TABLE_ENTRY* TargetEntry);

NTSTATUS
NTAPI
LdrpCorValidateImage(IN PVOID ImageBase, IN LPCWSTR FileName);

NTSTATUS
NTAPI
LdrpInitializeGraphRecurse(IN PLDR_DDAG_NODE DdagNode,
                           IN NTSTATUS* StatusResponse,
                           IN OUT BOOLEAN* HasInitializing);

NTSTATUS
NTAPI
LdrpInitializeNode(IN PLDR_DDAG_NODE DdagNode);

NTSTATUS
NTAPI
LdrpRunInitializeRoutine(IN PLDR_DATA_TABLE_ENTRY Module);


typedef __int32 (STDMETHODCALLTYPE LDRP_COREXEMAIN_FUNC)();
typedef HRESULT (STDMETHODCALLTYPE LDRP_CORIMAGEUNLOADING_FUNC)(IN PVOID* ImageBase);
typedef HRESULT (STDMETHODCALLTYPE LDRP_CORVALIDATEIMAGE_FUNC)(IN PVOID* ImageBase, IN LPCWSTR FileName);

static LDRP_COREXEMAIN_FUNC* LdrpCorExeMainRoutine;
static LDRP_CORIMAGEUNLOADING_FUNC* LdrpCorImageUnloadingRoutine;
static LDRP_CORVALIDATEIMAGE_FUNC* LdrpCorValidateImageRoutine;

LDRP_COREXEMAIN_FUNC*
NTAPI
LdrpCorReplaceStartContext(IN PCONTEXT Context);

VOID
NTAPI
LdrpCallTlsInitializers(IN PLDR_DATA_TABLE_ENTRY LdrEntry,
                        IN ULONG Reason);
BOOLEAN
NTAPI
LdrpCheckForLoadedDllHandle(IN PVOID Base,
                            OUT PLDR_DATA_TABLE_ENTRY *LdrEntry);

                            NTSTATUS
NTAPI
LdrpCorProcessImports(IN PLDR_DATA_TABLE_ENTRY Module);
BOOLEAN NTAPI LdrpCallInitRoutine(IN PDLL_INIT_ROUTINE EntryPoint, IN PVOID BaseAddress, IN ULONG Reason, IN PVOID Context);
#ifdef __cplusplus
}
#endif
/* EOF */
