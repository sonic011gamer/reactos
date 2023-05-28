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
typedef struct _TLS_VECTOR
{
    union
    {
        ULONG Length;
        HANDLE ThreadId;
    };

    struct _TLS_VECTOR* PreviousDeferredTlsVector;
    PVOID ModuleTlsData[ANYSIZE_ARRAY];
} TLS_VECTOR, *PTLS_VECTOR;

typedef struct _TLS_ENTRY
{
    LIST_ENTRY TlsEntryLinks;
    IMAGE_TLS_DIRECTORY TlsDirectory;
    PLDR_DATA_TABLE_ENTRY ModuleEntry;
} TLS_ENTRY, *PTLS_ENTRY;
typedef struct _LDRP_LAZY_PATH_EVALUATION_CONTEXT
{
    UNICODE_STRING RemainingSearchPath;
} LDRP_LAZY_PATH_EVALUATION_CONTEXT, *PLDRP_LAZY_PATH_EVALUATION_CONTEXT;
/* Page heap flags */
#define DPH_FLAG_DLL_NOTIFY 0x40
#define TLS_BITMAP_GROW_INCREMENT 8u
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
#if 1
        ULONG32 LogWarning : 1;
        ULONG32 LogInformation : 1;
        ULONG32 LogDebug : 1;
        ULONG32 LogTrace : 1;
#endif
        ULONG32 BreakInDebugger : 1;
    };
} LDRP_DEBUG_FLAGS;

extern LDRP_DEBUG_FLAGS LdrpDebugFlags;

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

NTSTATUS
NTAPI
LdrpLoadDll(IN PUNICODE_STRING RawDllName,
            IN PLDRP_PATH_SEARCH_CONTEXT PathSearchContext,
            IN LDRP_LOAD_CONTEXT_FLAGS LoaderFlags,
            OUT PLDR_DATA_TABLE_ENTRY* OutputLdrEntry);

#ifndef NTOS_MODE_USER
typedef struct _SYSTEM_MODULE_INFORMATION LDR_PROCESS_MODULES, *PLDR_PROCESS_MODULES;
#else
typedef struct _RTL_PROCESS_MODULES LDR_PROCESS_MODULES, *PLDR_PROCESS_MODULES;
#endif

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


PLDR_DATA_TABLE_ENTRY NTAPI
LdrpAllocateDataTableEntry(IN PVOID BaseAddress);

VOID NTAPI
LdrpInsertMemoryTableEntry(IN PLDR_DATA_TABLE_ENTRY LdrEntry);
NTSTATUS
NTAPI
LdrGetProcedureAddressForCaller(
    _In_ PVOID BaseAddress,
    _In_opt_ PANSI_STRING FunctionName,
    _In_opt_ ULONG Ordinal,
    _Out_ PVOID *ProcedureAddress,
    _In_ UINT8 Flags,
    _In_ PVOID *CallbackAddress
);


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
void
NTAPI
LdrpInsertModuleToIndex(IN PLDR_DATA_TABLE_ENTRY Module, IN PIMAGE_NT_HEADERS NtHeader);

NTSTATUS
NTAPI
LdrpInitParallelLoadingSupport(void);


NTSTATUS NTAPI
LdrpMapDll(IN PWSTR SearchPath OPTIONAL,
           IN PWSTR DllPath2,
           IN PWSTR DllName OPTIONAL,
           IN PULONG DllCharacteristics,
           IN BOOLEAN Static,
           IN BOOLEAN Redirect,
           OUT PLDR_DATA_TABLE_ENTRY *DataTableEntry);


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
typedef union
{
    UINT8 Flags;

    struct
    {
        UINT8 DoSecurityVerification : 1;
        UINT8 NoForwarderResolution : 1;
    };
} LDRP_RESOLVE_PROCEDURE_ADDRESS_FLAGS;
typedef enum _LDRP_PROCESS_WORK_MODE
{
    QueuedWork,
    StandaloneRequest
} LDRP_PROCESS_WORK_MODE;

typedef enum _LDRP_DRAIN_WORK_QUEUE_TARGET
{
    LoadComplete,
    WorkComplete
} LDRP_DRAIN_WORK_QUEUE_TARGET;
void
NTAPI
LdrpDereferenceModule(IN PLDR_DATA_TABLE_ENTRY Module);

NTSTATUS
NTAPI
LdrpFindLoadedDllByHandle(IN PVOID Base,
                          OUT PLDR_DATA_TABLE_ENTRY* Module,
                          LDR_DDAG_STATE* DdagState);
void
NTAPI
LdrpDropLastInProgressCount(void);
void
NTAPI
LdrpDrainWorkQueue(IN LDRP_DRAIN_WORK_QUEUE_TARGET Target);

VOID NTAPI
LdrpFreeUnicodeString(PUNICODE_STRING String);

PVOID NTAPI
LdrpFetchAddressOfEntryPoint(PVOID ImageBase);












NTSTATUS
NTAPI
LdrpResolveProcedureAddress(IN PLDR_DATA_TABLE_ENTRY Module,
                            IN PSTR Name,
                            IN ULONG Ordinal,
                            IN LDRP_RESOLVE_PROCEDURE_ADDRESS_FLAGS Flags,
                            OUT PVOID* ProcedureAddress);

NTSTATUS
NTAPI
LdrpLoadForwardedDll(IN PANSI_STRING RawDllName,
                     IN PLDRP_PATH_SEARCH_CONTEXT PathSearchContext,
                     IN PLDR_DATA_TABLE_ENTRY ForwarderSource,
                     IN PLDR_DATA_TABLE_ENTRY ParentEntry,
                     IN LDR_DLL_LOAD_REASON LoadReason,
                     OUT PLDR_DATA_TABLE_ENTRY* OutputLdrEntry);

void
NTAPI
LdrpReportSnapError(IN PVOID BaseAddress OPTIONAL,
                    IN PUNICODE_STRING ModuleName OPTIONAL,
                    IN PANSI_STRING FunctionName OPTIONAL,
                    IN ULONG Ordinal OPTIONAL,
                    NTSTATUS Status,
                    BOOLEAN Static);
LONG
NTAPI
LdrpDebugExceptionFilter(unsigned int code, PEXCEPTION_POINTERS ep);

NTSTATUS
NTAPI
LdrpResolveForwarder(IN PVOID ForwarderPointer,
                     IN PLDR_DATA_TABLE_ENTRY ExportLdrEntry,
                     IN PLDR_DATA_TABLE_ENTRY ImportLdrEntry,
                     OUT PVOID* ProcedureAddress);



_Must_inspect_result_
_Ret_maybenull_
_Post_writable_byte_size_(Size)
PVOID
NTAPI
LdrpHeapReAlloc(_In_opt_ ULONG Flags,
                _In_ _Post_invalid_ PVOID Ptr,
                _In_ SIZE_T Size);


void
NTAPI
LdrpDoDebuggerBreak(void);

/* Global data */
extern ULONG LdrInitState;
extern PVOID LdrpSystemDllBase;
extern RTL_CRITICAL_SECTION LdrpLoaderLock;
extern PVOID LdrpHeap;
extern LIST_ENTRY LdrpHashTable[LDR_HASH_TABLE_ENTRIES];
extern UNICODE_STRING LdrpDefaultPath;
extern HANDLE LdrpKnownDllDirectoryHandle;
extern ULONG LdrpFatalHardErrorCount;
extern PUNICODE_STRING LdrpTopLevelDllBeingLoaded;
extern PLDR_DATA_TABLE_ENTRY LdrpCurrentDllInitializer;
extern PLDR_DATA_TABLE_ENTRY LdrpNtDllDataTableEntry;
extern UNICODE_STRING LdrpDefaultExtension;
extern UNICODE_STRING LdrpKnownDllPath;
extern PLDR_DATA_TABLE_ENTRY LdrpLoadedDllHandleCache;
EXTERN_C BOOLEAN RtlpPageHeapEnabled;
EXTERN_C ULONG RtlpDphGlobalFlags;
extern BOOLEAN g_ShimsEnabled;
extern PVOID g_pShimEngineModule;
extern PVOID g_pfnSE_DllLoaded;
extern PVOID g_pfnSE_DllUnloaded;
extern PVOID g_pfnSE_InstallBeforeInit;
extern PVOID g_pfnSE_InstallAfterInit;
extern PVOID g_pfnSE_ProcessDying;
extern PVOID g_pfnSE_LdrResolveDllName;
extern PVOID LdrpMscoreeDllHandle;
extern BOOLEAN UseCOR, UseWOW64;
extern RTL_SRWLOCK LdrpModuleDatatableLock;
extern LIST_ENTRY LdrpWorkQueue;
extern LIST_ENTRY LdrpRetryQueue;
extern RTL_CRITICAL_SECTION LdrpWorkQueueLock;
EXTERN_C PLDR_DATA_TABLE_ENTRY LdrpImageEntry;
extern PCONTEXT LdrpProcessInitContextRecord;
extern PLDR_MANIFEST_PROBER_ROUTINE LdrpManifestProberRoutine;
extern PIMAGE_NT_HEADERS LdrpAppHeaders;
extern LIST_ENTRY LdrpTlsList;
#define TLS_BITMAP_GROW_INCREMENT 8u
extern RTL_BITMAP LdrpTlsBitmap;
extern ULONG LdrpActiveThreadCount;
extern ULONG LdrpActualBitmapSize;
extern RTL_SRWLOCK LdrpTlsLock;
extern UNICODE_STRING SlashSystem32SlashString;
extern ULONG LdrpWorkInProgress;
extern HANDLE LdrpLoadCompleteEvent, LdrpWorkCompleteEvent;
extern const UNICODE_STRING LdrpKernel32DllName;




NTSTATUS
NTAPI
LdrpDecrementNodeLoadCountLockHeld(IN PLDR_DDAG_NODE Node, IN BOOLEAN DisallowOrphaning, OUT BOOLEAN* BecameOrphan);


void
NTAPI
LdrpRemoveDependencyEntry(PLDRP_CSLIST List, PSINGLE_LIST_ENTRY TargetEntry);

PLDRP_CSLIST_ENTRY
NTAPI
LdrpRecordModuleDependency(PLDR_DATA_TABLE_ENTRY LdrEntry1,
                           PLDR_DATA_TABLE_ENTRY LdrEntry2,
                           PLDRP_CSLIST_ENTRY Dependency,
                           NTSTATUS* StatusResponse);






void
NTAPI
LdrpQueueWork(IN PLDRP_LOAD_CONTEXT LoadContext);


NTSTATUS
NTAPI
LdrpResolveDllName(IN PUNICODE_STRING DosName,
                   IN OUT PLDRP_UNICODE_STRING_BUNDLE NtName,
                   OUT PUNICODE_STRING BaseDllName,
                   OUT PUNICODE_STRING FullDosName,
                   IN LDRP_LOAD_CONTEXT_FLAGS Flags);








void
NTAPI
LdrpProcessWork(PLDRP_LOAD_CONTEXT LoadContext, LDRP_PROCESS_WORK_MODE Mode);


NTSTATUS
NTAPI
LdrpDecrementModuleLoadCountEx(IN PLDR_DATA_TABLE_ENTRY Module, IN BOOLEAN DisallowOrphaning);


NTSTATUS
NTAPI
LdrpBuildForwarderLink(IN PLDR_DATA_TABLE_ENTRY Source OPTIONAL,
                       IN PLDR_DATA_TABLE_ENTRY Target);

NTSTATUS
NTAPI
LdrpIncrementModuleLoadCount(IN PLDR_DATA_TABLE_ENTRY Module);

NTSTATUS
NTAPI
LdrpFindLoadedDllByName(IN PUNICODE_STRING BaseDllName OPTIONAL,
                        IN PUNICODE_STRING FullDllName OPTIONAL,
                        IN LDRP_LOAD_CONTEXT_FLAGS LoadContextFlags OPTIONAL,
                        OUT PLDR_DATA_TABLE_ENTRY* Module,
                        OUT LDR_DDAG_STATE* DdagState OPTIONAL);


NTSTATUS
NTAPI
LdrpParseForwarderDescription(IN PVOID ForwarderPointer,
                              IN OUT PANSI_STRING ForwardedDllName,
                              IN OUT PSTR* ForwardedFunctionName,
                              IN OUT PULONG ForwardedFunctionOrdinal);

NTSTATUS
NTAPI
LdrpLoadDependentModule(IN PANSI_STRING RawDllName,
                        IN PLDRP_LOAD_CONTEXT LoadContext,
                        IN PLDR_DATA_TABLE_ENTRY ParentModule,
                        IN LDR_DLL_LOAD_REASON LoadReason,
                        OUT PLDR_DATA_TABLE_ENTRY* DataTableEntry,
                        IN OUT PLDRP_CSLIST_ENTRY* DependencyEntryPlaceholder);


NTSTATUS
NTAPI
LdrpGetProcedureAddress(IN PVOID BaseAddress,
                        IN PSTR Name,
                        IN ULONG Ordinal,
                        OUT PVOID* ProcedureAddress);

NTSTATUS
NTAPI
LdrpDoPostSnapWork(IN PLDRP_LOAD_CONTEXT LoadContext);



void
NTAPI
LdrpFreeReplacedModule(PLDR_DATA_TABLE_ENTRY Module);



PLDR_DATA_TABLE_ENTRY
NTAPI
LdrpHandleReplacedModule(PLDR_DATA_TABLE_ENTRY Entry);

NTSTATUS NTAPI
AVrfInitializeVerifier(VOID);

VOID NTAPI
AVrfDllLoadNotification(IN PLDR_DATA_TABLE_ENTRY LdrEntry);

VOID NTAPI
AVrfDllUnloadNotification(IN PLDR_DATA_TABLE_ENTRY LdrEntry);

VOID NTAPI
AVrfPageHeapDllNotification(IN PLDR_DATA_TABLE_ENTRY LdrEntry);


void
NTAPI
LdrpHandlePendingModuleReplaced(PLDRP_LOAD_CONTEXT LoadContext);


NTSTATUS
NTAPI
LdrpGetProcedureAddressImpl(IN PIMAGE_EXPORT_DIRECTORY ExportDir,
                            IN ULONG ExportDirSize,
                            IN PVOID ExportDirPointer,
                            IN PVOID BaseAddress,
                            IN PSTR Name,
                            IN ULONG Ordinal,
                            OUT PVOID* ProcedureAddress);




NTSTATUS
NTAPI
LdrpReleaseTlsEntry(
    IN PLDR_DATA_TABLE_ENTRY ModuleEntry,
    OUT PTLS_ENTRY* FoundTlsEntry OPTIONAL
);
void
LdrpQueueDeferredTlsData(
    IN OUT PVOID TlsVector,
    IN OUT PVOID ThreadId
);


void
NTAPI
LdrpFreeLoadContextOfNode(PLDR_DDAG_NODE Node, NTSTATUS* StatusResponse);


NTSTATUS
NTAPI
LdrpPrepareModuleForExecution(IN PLDR_DATA_TABLE_ENTRY Module,
                              IN NTSTATUS* StatusResponse);

NTSTATUS
NTAPI
// ReSharper disable once IdentifierTypo
LdrpFastpthReloadedDll(IN PUNICODE_STRING BaseDllName,
                       IN LDRP_LOAD_CONTEXT_FLAGS LoadContextFlags,
                       IN PLDR_DATA_TABLE_ENTRY ForwarderSource OPTIONAL,
                       OUT PLDR_DATA_TABLE_ENTRY* OutputLdrEntry);


BOOLEAN
NTAPI
LdrpCheckForRetryLoading(IN PLDRP_LOAD_CONTEXT LoadContext, IN BOOLEAN InsertIntoRetryQueue);


PVOID* LdrpGetNewTlsVector(IN ULONG TlsBitmapLength);


NTSTATUS
LdrpAllocateTlsEntry(
    IN PIMAGE_TLS_DIRECTORY TlsDirectory,
    IN PLDR_DATA_TABLE_ENTRY ModuleEntry,
    OUT PULONG TlsIndex,
    OUT PBOOLEAN AllocatedBitmap OPTIONAL,
    OUT PTLS_ENTRY* TlsEntry
);

void
NTAPI
LdrpDecrementUnmappedChildCount(IN PLDRP_LOAD_CONTEXT ParentContext,
                           IN PLDR_DATA_TABLE_ENTRY ParentModule,
                           IN PLDRP_LOAD_CONTEXT ChildContext,
                           IN PLDR_DATA_TABLE_ENTRY ChildModule);


UINT32
NTAPI
LdrpNameToOrdinal(IN LPSTR ImportName,
                  IN ULONG NumberOfNames,
                  IN PVOID ExportBase,
                  IN PULONG NameTable,
                  IN PUSHORT OrdinalTable);



NTSTATUS
NTAPI
LdrGetProcedureAddressEx(
    _In_ PVOID BaseAddress,
    _In_opt_ PANSI_STRING FunctionName,
    _In_opt_ ULONG Ordinal,
    _Out_ PVOID *ProcedureAddress,
    _In_ UINT8 Flags
);


NTSYSAPI
NTSTATUS
NTAPI
RtlpDosPathNameToRelativeNtPathName(
    _In_ RTLP_DosPathNameToRelativeNtPathName_FLAGS Flags,
    _In_ PCUNICODE_STRING DosName,
    _Inout_opt_ PUNICODE_STRING StaticBuffer,
    _Inout_opt_ PUNICODE_STRING DynamicBuffer,
    _Out_opt_ PUNICODE_STRING* NtName,
    _Out_opt_ PCWSTR* NtFileNamePart,
    _Out_opt_ PRTL_RELATIVE_NAME_U RelativeName
);


NTSTATUS
NTAPI
LdrpFindLoadedDllByMappingFile(PUNICODE_STRING NtPathName, PLDR_DATA_TABLE_ENTRY* Module, LDR_DDAG_STATE* DdagState);


NTSTATUS
NTAPI
LdrpAllocateFileNameBufferIfNeeded(PLDRP_UNICODE_STRING_BUNDLE Destination, ULONG Length);

NTSYSAPI
ULONG
NTAPI
RtlGetFullPathName_Ustr(
    _In_ PUNICODE_STRING FileName,
    _In_ ULONG Size,
    _Out_z_bytecap_(Size) PWSTR Buffer,
    _Out_opt_ PCWSTR *ShortName,
    _Out_opt_ PBOOLEAN InvalidName,
    _Out_ PRTLP_PATH_INFO PathInfo
);


NTSTATUS
NTAPI
LdrpUnmapModule(IN PLDR_DATA_TABLE_ENTRY Module);


NTSTATUS
NTAPI
LdrpReleaseTlsEntry(
    IN PLDR_DATA_TABLE_ENTRY ModuleEntry,
    OUT PTLS_ENTRY* FoundTlsEntry OPTIONAL
);

NTSTATUS
NTAPI
LdrpPinModule(IN PLDR_DATA_TABLE_ENTRY Module);

NTSTATUS
NTAPI
LdrpFindOrPrepareLoadingModule(IN PUNICODE_STRING BaseDllName,
                               IN PLDRP_PATH_SEARCH_CONTEXT PathSearchContext,
                               IN LDRP_LOAD_CONTEXT_FLAGS ContextFlags,
                               IN LDR_DLL_LOAD_REASON LoadReason,
                               IN PLDR_DATA_TABLE_ENTRY ParentEntry,
                               OUT PLDR_DATA_TABLE_ENTRY* OutLdrEntry,
                               OUT NTSTATUS* OutStatus);

void
LdrpLoadDllInternal(IN PUNICODE_STRING DllName,
                    IN PLDRP_PATH_SEARCH_CONTEXT PathSearchContext,
                    IN LDRP_LOAD_CONTEXT_FLAGS LoadContextFlags,
                    IN LDR_DLL_LOAD_REASON LoadReason,
                    IN PLDR_DATA_TABLE_ENTRY ParentEntry,
                    IN PLDR_DATA_TABLE_ENTRY ForwarderSource OPTIONAL,
                    OUT PLDR_DATA_TABLE_ENTRY* OutputLdrEntry,
                    OUT NTSTATUS* OutStatus);

NTSTATUS
NTAPI
LdrpAppendAnsiStringToFilenameBuffer(PLDRP_UNICODE_STRING_BUNDLE Destination, ANSI_STRING* Source);


NTSTATUS
NTAPI
LdrpPrepareImportAddressTableForSnap(PLDRP_LOAD_CONTEXT LoadContext);

PIMAGE_IMPORT_DESCRIPTOR
NTAPI
LdrpGetImportDescriptorForSnap(PLDRP_LOAD_CONTEXT LoadContext);


NTSTATUS
NTAPI
LdrpFindDllActivationContext(PLDR_DATA_TABLE_ENTRY Module);

NTSTATUS
NTAPI
LdrpCreateDllSection(IN PUNICODE_STRING FullName,
                     IN LDRP_LOAD_CONTEXT_FLAGS LoaderFlags,
                     OUT PHANDLE FileHandle,
                     OUT PHANDLE SectionHandle);


NTSTATUS
NTAPI
LdrpFindExistingModule(IN PUNICODE_STRING BaseDllName OPTIONAL,
                       IN PUNICODE_STRING FullDllName OPTIONAL,
                       IN LDRP_LOAD_CONTEXT_FLAGS LoadContextFlags OPTIONAL,
                       IN ULONG32 BaseNameHashValue,
                       OUT PLDR_DATA_TABLE_ENTRY* Module);

NTSTATUS
NTAPI
LdrpAppCompatRedirect(PLDRP_LOAD_CONTEXT LoadContext, PUNICODE_STRING FullDosPath, PUNICODE_STRING BaseDllName, PLDRP_UNICODE_STRING_BUNDLE NtPath, NTSTATUS Status);

NTSTATUS
NTAPI
LdrpSearchPath(IN PUNICODE_STRING DllName,
               IN PLDRP_PATH_SEARCH_CONTEXT PathSearchContext,
               IN BOOLEAN SearchInFirstSegmentOnly,
               IN OUT PLDRP_LAZY_PATH_EVALUATION_CONTEXT* LazyEvaluationContext OPTIONAL,
               IN OUT PLDRP_UNICODE_STRING_BUNDLE NtName,
               OUT PUNICODE_STRING BaseDllName,
               OUT PUNICODE_STRING FullDosName);













#define IMAGE_GUARD_CF_INSTRUMENTED                    0x00000100 // Module performs control flow integrity checks using system-supplied support
#define IMAGE_DLLCHARACTERISTICS_GUARD_CF               0x4000
#define IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE  0x8000

NTSYSAPI
RTL_PATH_TYPE
NTAPI
RtlDetermineDosPathNameType_Ustr(
    _In_ PCUNICODE_STRING PathString
);


PIMAGE_LOAD_CONFIG_DIRECTORY
NTAPI
LdrImageDirectoryEntryToLoadConfig(_In_ PVOID DllBase);

NTSTATUS
NTAPI
LdrpAppendUnicodeStringToFilenameBuffer(PLDRP_UNICODE_STRING_BUNDLE Destination, UNICODE_STRING* Source);

void
NTAPI
LdrpTrimTrailingDots(IN OUT PUNICODE_STRING DllName);


BOOLEAN
NTAPI
LdrpIsExtensionPresent(IN PUNICODE_STRING DllName);


BOOLEAN
NTAPI
LdrpIsBaseNameOnly(IN PUNICODE_STRING DllName);


NTSTATUS
NTAPI
LdrpApplyFileNameRedirection(IN PUNICODE_STRING DllName,
                             IN OUT PLDRP_UNICODE_STRING_BUNDLE RedirectedDllName,
                             OUT PBOOLEAN RedirectedSxS);


NTSTATUS
NTAPI
LdrpNotifyLoadOfGraph(PLDR_DDAG_NODE Node);

void
NTAPI
LdrpCondenseGraph(PLDR_DDAG_NODE Node);


NTSTATUS
NTAPI
LdrpMapDllSearchPath(IN PLDRP_LOAD_CONTEXT LoadContext);


NTSTATUS
NTAPI
LdrpMapDllFullPath(IN PLDRP_LOAD_CONTEXT LoadContext);


NTSTATUS
NTAPI
LdrpMapDllRetry(IN PLDRP_LOAD_CONTEXT LoadContext);


NTSTATUS
NTAPI
LdrpSnapModule(IN PLDRP_LOAD_CONTEXT LoadContext);

void
NTAPI
LdrpSignalModuleMapped(PLDR_DATA_TABLE_ENTRY Module);


BOOLEAN
NTAPI
LdrpInitSecurityCookie(PVOID BaseAddress,
                       ULONG_PTR SizeOfImage,
                       PULONG_PTR CookieOverride,
                       ULONG_PTR Salt,
                       OUT PIMAGE_LOAD_CONFIG_DIRECTORY* OutLoadConfig);


typedef unsigned char RTLP_FLS_DATA_CLEANUP_MODE;
#define RTLP_FLS_DATA_CLEANUP_RUN_CALLBACKS (1 << 0)
#define RTLP_FLS_DATA_CLEANUP_DEALLOCATE (1 << 1)
NTSYSAPI
void
NTAPI
RtlpFlsInitialize(void);

NTSYSAPI
void
NTAPI
RtlProcessFlsData(IN PVOID FlsData, IN RTLP_FLS_DATA_CLEANUP_MODE Mode);

typedef VOID(NTAPI* PFLS_CALLBACK_FUNCTION) (_In_ PVOID pFlsData);

NTSYSAPI
NTSTATUS
NTAPI
RtlFlsAlloc(PFLS_CALLBACK_FUNCTION pCallback, ULONG32* pFlsIndex);

NTSYSAPI
NTSTATUS
NTAPI
RtlFlsFree(ULONG32 FlsIndex);

NTSYSAPI
NTSTATUS
NTAPI
RtlFlsGetValue(ULONG32 FlsIndex, PVOID* pFlsValue);

NTSYSAPI
NTSTATUS
NTAPI
RtlFlsSetValue(ULONG32 FlsIndex, PVOID FlsValue);

BOOLEAN
NTAPI
LdrpDependencyExist(PLDR_DDAG_NODE Node1, PLDR_DDAG_NODE Node2);
NTSTATUS
NTAPI
LdrpValidateImageForMp(IN PLDR_DATA_TABLE_ENTRY LdrEntry);


ULONG
NTAPI
LdrpProtectAndRelocateImage(IN PVOID NewBase,
                            IN PCCH LoaderName,
                            IN ULONG Success,
                            IN ULONG Conflict,
                            IN ULONG Invalid);


BOOL
NTAPI
LdrpIsILOnlyImage(PVOID BaseAddress);

NTSTATUS
NTAPI
LdrpGetNtPathFromDosPath(IN PUNICODE_STRING DosPath,
                         OUT PLDRP_UNICODE_STRING_BUNDLE NtPath);

NTSTATUS
NTAPI
LdrpMapDllNtFileName(IN PLDRP_LOAD_CONTEXT LoadContext,
                     IN PUNICODE_STRING NtPathDllName);



LDRP_LOAD_CONTEXT_FLAGS
NTAPI
LdrpDllCharacteristicsToLoadFlags(ULONG DllCharacteristics);

NTSTATUS
NTAPI
LdrpMapDllWithSectionHandle(PLDRP_LOAD_CONTEXT LoadContext, HANDLE SectionHandle);



void
NTAPI
LdrpLoadContextReplaceModule(IN PLDRP_LOAD_CONTEXT LoadContext, PLDR_DATA_TABLE_ENTRY LoadedEntry);

ULONG32
NTAPI
LdrpHashUnicodeString(IN PUNICODE_STRING NameString);


NTSTATUS
NTAPI
LdrpFindKnownDll(PUNICODE_STRING DllName,
                 PUNICODE_STRING BaseDllName,
                 PUNICODE_STRING FullDllName,
                 HANDLE* SectionHandle);


NTSTATUS
NTAPI
LdrpMapAndSnapDependency(PLDRP_LOAD_CONTEXT LoadContext);

void
NTAPI
LdrpInsertModuleToIndexLockHeld(IN PLDR_DATA_TABLE_ENTRY Module, IN PIMAGE_NT_HEADERS NtHeader);

NTSTATUS
NTAPI
LdrpProcessMappedModule(PLDR_DATA_TABLE_ENTRY Module, LDRP_LOAD_CONTEXT_FLAGS LoadContextFlags, BOOL AdvanceLoad);


NTSTATUS
NTAPI
LdrpCompleteMapModule(PLDRP_LOAD_CONTEXT LoadContext, PIMAGE_NT_HEADERS NtHeaders, NTSTATUS ImageStatus,
                      SIZE_T ViewSize);



void NTAPI
LdrpInsertDataTableEntry(IN PLDR_DATA_TABLE_ENTRY Module);


void
NTAPI
LdrpLoadContextReplaceModule(IN PLDRP_LOAD_CONTEXT LoadContext, PLDR_DATA_TABLE_ENTRY LoadedEntry);


NTSTATUS
NTAPI
LdrpMinimalMapModule(PLDRP_LOAD_CONTEXT LoadContext, HANDLE SectionHandle, PSIZE_T ViewSize);



NTSTATUS
NTAPI
LdrpCodeAuthzCheckDllAllowed(IN PUNICODE_STRING FullName,
                             IN HANDLE DllHandle);
NTSTATUS
NTAPI
LdrpUnloadNode(IN PLDR_DDAG_NODE Node);

NTSTATUS
NTAPI
LdrpPreprocessDllName(IN PUNICODE_STRING DllName,
                      IN OUT PLDRP_UNICODE_STRING_BUNDLE OutputDllName,
                      IN PLDR_DATA_TABLE_ENTRY ParentEntry OPTIONAL,
                      OUT PLDRP_LOAD_CONTEXT_FLAGS LoadContextFlags);


PLDR_DATA_TABLE_ENTRY
NTAPI
LdrpAllocateModuleEntry(IN PLDRP_LOAD_CONTEXT LoadContext OPTIONAL);


NTSTATUS
NTAPI
LdrpFindLoadedDllByNameLockHeld(IN PUNICODE_STRING BaseDllName,
                                IN PUNICODE_STRING FullDllName OPTIONAL,
                                IN LDRP_LOAD_CONTEXT_FLAGS LoadContextFlags OPTIONAL,
                                IN ULONG32 BaseNameHashValue,
                                OUT PLDR_DATA_TABLE_ENTRY* Module);

NTSTATUS NTAPI LdrpHandleTlsData(IN OUT PLDR_DATA_TABLE_ENTRY ModuleEntry);

NTSTATUS
NTAPI
LdrpFindLoadedDllByMapping(PVOID ViewBase, PIMAGE_NT_HEADERS NtHeader, PLDR_DATA_TABLE_ENTRY* Module,
                           LDR_DDAG_STATE* DdagState);
void
NTAPI
LdrpFreeLoadContext(PLDRP_LOAD_CONTEXT LoadContext);

NTSTATUS
NTAPI
LdrpLoadKnownDll(IN PLDRP_LOAD_CONTEXT LoadContext);


NTSTATUS
NTAPI
LdrpAllocatePlaceHolder(IN PUNICODE_STRING DllName,
                        IN PLDRP_PATH_SEARCH_CONTEXT PathSearchContext,
                        IN LDRP_LOAD_CONTEXT_FLAGS Flags,
                        IN LDR_DLL_LOAD_REASON LoadReason,
                        IN PLDR_DATA_TABLE_ENTRY ParentEntry,
                        OUT PLDR_DATA_TABLE_ENTRY* OutLdrEntry,
                        IN NTSTATUS* OutStatus);

NTSTATUS
NTAPI
LdrpFindLoadedDll(PUNICODE_STRING RawDllName,
                  PLDRP_PATH_SEARCH_CONTEXT PathSearchContext,
                  PLDR_DATA_TABLE_ENTRY* Module);
#define LDR_INFO DPRINT1

ULONG
NTAPI
LdrpRelocateImage(IN PVOID NewBase,
                  IN PCCH LoaderName,
                  IN PIMAGE_NT_HEADERS NtHeaders,
                  IN ULONG Success,
                  IN ULONG Conflict,
                  IN ULONG Invalid);


NTSTATUS
NTAPI
LdrpGetFullPath(IN PUNICODE_STRING OriginalName,
                IN OUT PLDRP_UNICODE_STRING_BUNDLE ExpandedName);


void
NTAPI
LdrpDestroyNode(IN PLDR_DDAG_NODE Node);


FORCEINLINE
void
RtlpCheckListEntry(_In_ PLIST_ENTRY Entry)
{

}

void
AppendTailList(
    _Inout_ PLIST_ENTRY ListHead,
    _Inout_ PLIST_ENTRY ListToAppend
)
{
    PLIST_ENTRY ListEnd = ListHead->Blink;

#ifndef NO_KERNEL_LIST_ENTRY_CHECKS
    RtlpCheckListEntry(ListHead);
    RtlpCheckListEntry(ListToAppend);
#endif
    ListHead->Blink->Flink = ListToAppend;
    ListHead->Blink = ListToAppend->Blink;
    ListToAppend->Blink->Flink = ListHead;
    ListToAppend->Blink = ListEnd;
}


 NTSTATUS
NTAPI
LdrpCorProcessImports(IN PLDR_DATA_TABLE_ENTRY Module);
BOOLEAN NTAPI LdrpCallInitRoutine(IN PDLL_INIT_ROUTINE EntryPoint, IN PVOID BaseAddress, IN ULONG Reason, IN PVOID Context);
#ifdef __cplusplus
}

NTSTATUS
LdrpWrapResultReturn(UINT32 result)
{
    return result;
}

#define LDR_FUNC_END_IMPL_EXCEPT(statement) __except(LdrpDebugExceptionFilter(GetExceptionCode(), GetExceptionInformation())) { LoaderLogContext ldr_log; LDR_LOG_IMPL(ldr_log, Error, "Exception " LDR_HEX32_FMT, GetExceptionCode()); statement; }
#define LDR_FUNC_END_IMPL(statement) }()); } LDR_FUNC_END_IMPL_EXCEPT(statement)
#define LDR_FUNC_END_RETURN LDR_FUNC_END_IMPL(return GetExceptionCode())
#define LDR_FUNC_END_RETURN_NULL LDR_FUNC_END_IMPL(return nullptr)
#define LDR_FUNC_MANUAL(returntype, ...) __try { return LdrpWrapResultReturn([&]() -> returntype { LDR_FUNC_IMPL(__VA_ARGS__)
#define LDR_FUNC(returntype, ...) LDR_FUNC_MANUAL(returntype, FOR_EACH_C_(FOR_EACH_NARG(__VA_ARGS__), LDR_ARG, __VA_ARGS__))
#undef ASSERTMSG
#define ASSERTMSG(exp, message, ...) DPRINT1(message); __debugbreak();


#define LDRP_ASSERT_CSLIST_ENTRY(Entry) ASSERTMSG(Entry, LDR_HEXPTR_FMT, (ULONG_PTR)Entry)
#define LDRP_ASSERT_CSLIST_ENTRY_OPTIONAL(Entry)
#define LDRP_MARK_CSLIST_ENTRY(Entry) ASSERTMSG(Entry, LDR_HEXPTR_FMT, (ULONG_PTR)Entry)
#define LDRP_ASSERT_DDAG_NODE(Node) ASSERTMSG(Node, LDR_HEXPTR_FMT, (ULONG_PTR)Node)
#define LDRP_ASSERT_DDAG_NODE_OPTIONAL(Node)
#define LDRP_MARK_DDAG_NODE(Node) ASSERTMSG(Node, LDR_HEXPTR_FMT, (ULONG_PTR)Node)
#define LDRP_ASSERT_MODULE_ENTRY(Entry) ASSERTMSG(Entry, LDR_HEXPTR_FMT, (ULONG_PTR)Entry)
#define LDRP_ASSERT_MODULE_ENTRY_OPTIONAL(Entry)
#define LDRP_MARK_MODULE_ENTRY(Entry) ASSERTMSG(Entry, LDR_HEXPTR_FMT, (ULONG_PTR)Entry)
#define LDRP_ASSERT_LOAD_CONTEXT(LoadContext) ASSERTMSG(LoadContext, LDR_HEXPTR_FMT, (ULONG_PTR)LoadContext)
#define LDRP_ASSERT_LOAD_CONTEXT_OPTIONAL(LoadContext)
#define LDRP_MARK_LOAD_CONTEXT(LoadContext) ASSERTMSG(LoadContext, LDR_HEXPTR_FMT, (ULONG_PTR)LoadContext)
#define LDRP_ASSERT_NODE_ENTRY(Node, Entry) ASSERTMSG((Node) && (Entry), LDR_HEXPTR_FMT "," LDR_HEXPTR_FMT, (ULONG_PTR)Node, (ULONG_PTR)Entry)



_Must_inspect_result_
_Ret_maybenull_
_Post_writable_byte_size_(Size)
PVOID
NTAPI
LdrpHeapAlloc(_In_opt_ ULONG Flags,
              _In_ SIZE_T Size);



#endif

#define LDR_HEX32_FMT "0x{:08X}"
#define LDR_HEX64_FMT "0x{:016X}"
#define LDR_HEXPTR_FMT "0x{:08X}"
/* EOF */
