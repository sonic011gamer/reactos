#ifdef __cplusplus
extern "C" {
#endif

#include <ntdll.h>
#include <ndk/ldrtypes.h>
//extern PLDR_DATA_TABLE_ENTRY LdrpLoadedDllHandleCache;
#ifdef __cplusplus
}
#endif
#define NDEBUG
#include <debug.h>
extern RTL_SRWLOCK LdrpModuleDatatableLock;

NTSTATUS
NTAPI
LdrpFindLoadedDllByAddress(IN PVOID Base,
                           OUT PLDR_DATA_TABLE_ENTRY *Module,
                           OUT LDR_DDAG_STATE *DdagState OPTIONAL)
{

    DPRINT1("LoadedDllByAddress Entry\n");
    PLDR_DATA_TABLE_ENTRY FoundEntry = NULL;
    if (Base)
    {
            DPRINT1("FindDLlByAddr: valid base\n");
      //  RtlAcquireSRWLockExclusive(&LdrpModuleDatatableLock);

        /* Check the cache first */
        if (LdrpCheckForLoadedDllHandle(Base,Module))
        {
            DPRINT1("checking the cache\n");
            /* We got lucky, return the cached entry */
            FoundEntry = LdrpLoadedDllHandleCache;
            goto Quickie;
        }
        DPRINT1("looking uo\n");
        /* Time for a lookup */
        {
            const auto* const ListHead = &NtCurrentPeb()->Ldr->InLoadOrderModuleList;
            for (auto* Next = ListHead->Flink; Next != ListHead; Next = Next->Flink)
            {
                /* Get the current entry */
                auto* const Current = CONTAINING_RECORD(Next, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

                /* Make sure it's not unloading and check for a match */
                if (Current->InMemoryOrderLinks.Flink && Base == Current->DllBase)
                {
                    /* Save in cache */
                    LdrpLoadedDllHandleCache = Current;

                    /* Return it */
                    FoundEntry = Current;
                    goto Quickie;
                }
            }
        }

Quickie:
        DPRINT1("quickie enter\n");
        FoundEntry->ReferenceCount += 1;
        #if 0
        if (FoundEntry && !LDRP_MODULE_PINNED(FoundEntry))
        {
            InterlockedIncrement(reinterpret_cast<long volatile*>(&FoundEntry->ReferenceCount));
        }
        #endif

      //  RtlReleaseSRWLockExclusive(&LdrpModuleDatatableLock);
    }
    DPRINT1("Left LdrpFindLoadedDllByAddr\n");
    *Module = FoundEntry;
        DPRINT1("returning\n");
    /* Nothing found */
    return STATUS_SUCCESS;
}
