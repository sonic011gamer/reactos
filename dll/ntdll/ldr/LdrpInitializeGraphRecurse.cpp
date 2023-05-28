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

NTSTATUS
NTAPI
LdrpInitializeGraphRecurse(IN PLDR_DDAG_NODE DdagNode,
                           IN NTSTATUS* StatusResponse,
                           IN OUT BOOLEAN* HasInitializing)
{
    NTSTATUS Status;
    DPRINT1("assign lsit entry\n");
    PSINGLE_LIST_ENTRY ListHead = NULL;//;DdagNode->Dependencies.Tail;

    BOOLEAN HasInitializingDependency = FALSE;
    DPRINT1("reaching for loop\n");
    for (const auto* ListEntry = ListHead ? ListHead->Next : NULL;
         ListEntry;
         ListEntry = (ListEntry != ListHead) ? ListEntry->Next : NULL)
    {
        const auto* const DependencyEntry = CONTAINING_RECORD(ListEntry, LDRP_CSLIST_ENTRY, DependenciesLink);

       // LDRP_ASSERT_CSLIST_ENTRY(DependencyEntry);

        auto* const Dependency = DependencyEntry->DependencyNode;

        switch (Dependency->State)
        {
            case LdrModulesReadyToInit:
            {
                Status = LdrpInitializeGraphRecurse(Dependency, StatusResponse, &HasInitializingDependency);
                if (!NT_SUCCESS(Status))
                    goto Quickie;
                break;
            }
            case LdrModulesInitializing:
            {
                HasInitializingDependency = TRUE;
                break;
            }
            case LdrModulesInitError:
            {
                Status = STATUS_DLL_INIT_FAILED;
                goto Quickie;
            }
            default:
            {
                break;
            }
        }
    }
    DPRINT1("enter HasInit Dependency\n");
    if (HasInitializingDependency)
    {
        const auto* const Module = LDRP_FIRST_MODULE(DdagNode);

       // LDRP_ASSERT_MODULE_ENTRY(Module);
      //  LDRP_ASSERT_NODE_ENTRY(DdagNode, Module);

        const auto* const LoadContext = Module->LoadContext;

       // LDRP_ASSERT_LOAD_CONTEXT_OPTIONAL(LoadContext);

        *HasInitializing = TRUE;

        if (LoadContext)
        {
            if (StatusResponse != LoadContext->LoadStatus)
            {
                // This is not what we are here for. That's different load session.
                // Skip graph node initialization.
                Status = STATUS_SUCCESS;
                goto Quickie;
            }
        }
    }

SkipLoop:
    DPRINT1("LdrpinitializeNode entry\n");
  //  Status = LdrpInitializeNode(DdagNode);

Quickie:
    DPRINT1("Quickie: enter\n");

    return STATUS_SUCCESS;
}
