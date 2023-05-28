
add_subdirectory(nt_0600)

spec2def(ntdll.dll def/ntdll.spec ADD_IMPORTLIB)

# Embed RTC libs
if (STACK_PROTECTOR)
    target_sources(libntdll PRIVATE $<TARGET_OBJECTS:gcc_ssp_nt>)
endif()

add_definitions(
    -D__NTDLL__
    -D_NTOSKRNL_
    -DCRTDLL)

include_directories(
    BEFORE include
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys)

list(APPEND SOURCE
    dbg/dbgui.c
    ldr/ldrapi.c
    #ldr/ldrapi.cpp
    #ldr/ldrcor.cpp
    #ldr/ldrfind.cpp
    #ldr/LdrGetDllHandleEx.cpp
    #ldr/LdrGetProcedureAddressForCaller.cpp
    #ldr/LdrImageDirectoryEntryToLoadConfig.cpp
    ldr/ldrinit.cpp
    #ldr/ldrlock.cpp
    #ldr/ldrmap.cpp
    #ldr/LdrpAllocatePlaceHolder.cpp
    #ldr/LdrpApplyFileNameRedirection.cpp
    #ldr/LdrpBuildForwarderLink.cpp
    #ldr/LdrpCompleteMapModule.cpp
    #ldr/LdrpCondenseGraph.cpp
    #ldr/LdrpCreateDllSection.cpp
    #ldr/LdrpDecrementNodeLoadCount.cpp
    #ldr/LdrpDependencyExist.cpp
    #ldr/LdrpDereferenceModule.cpp
    #ldr/LdrpDestroyNode.cpp
    #ldr/LdrpDoPostSnapWork.cpp
    #ldr/LdrpDrainWorkQueue.cpp
    #ldr/LdrpFastpthReloadedDll.cpp
    #ldr/LdrpFindDllActivationContext.cpp
    #ldr/LdrpFindExistingModule.cpp
    #ldr/LdrpFindKnownDll.cpp
    #ldr/LdrpFindLoadedDll.cpp
    #ldr/LdrpFindLoadedDllByAddress.cpp
    #ldr/LdrpFindLoadedDllByHandle.cpp
    #ldr/LdrpFindLoadedDllByName.cpp
    #ldr/LdrpFindLoadedDllByMapping.cpp
    #ldr/LdrpFindLoadedDllByMappingFile.cpp
    #ldr/LdrpFindOrPrepareLoadingModule.cpp
    #ldr/LdrpFreeLoadContext.cpp
    #ldr/LdrpFreeLoadContextOfNode.cpp
    #ldr/LdrpFreeReplacedModule.cpp
    #ldr/LdrpGetDelayloadExportDll.cpp
    #ldr/LdrpGetFullPath.cpp
    #ldr/LdrpGetImportDescriptorForSnap.cpp
    #ldr/LdrpGetNtPathFromDosPath.cpp
    #ldr/LdrpGetProcedureAddress.cpp
    #ldr/LdrpGetShimEngineInterface.cpp
    #ldr/LdrpHandlePendingModuleReplaced.cpp
    #ldr/LdrpHandleReplacedModule.cpp
    #ldr/LdrpHandleTlsData.cpp
    #ldr/LdrpIncrementModuleLoadCount.cpp
    #ldr/LdrpInitializeGraphRecurse.cpp
    #ldr/LdrpInitializeNode.cpp
    #ldr/LdrpInsertDataTableEntry.cpp
    #ldr/LdrpIsExtensionPresent.cpp
    #ldr/LdrpLoadContextReplaceModule.cpp
    #ldr/LdrpLoadDependentModule.cpp
    #ldr/LdrpLoadDll.cpp
    #ldr/LdrpLoadDllInternal.cpp
    #ldr/LdrpLoadForwardedDll.cpp
    #ldr/LdrpLoadKnownDll.cpp
    #ldr/LdrpMapAndSnapDependency.cpp
    #ldr/LdrpMapDllFullPath.cpp
    #ldr/LdrpMapDllNtFileName.cpp
    #ldr/LdrpMapDllSearchPath.cpp
    #ldr/LdrpMapDllWithSectionHandle.cpp
    #ldr/LdrpMinimalMapModule.cpp
    #ldr/LdrpNameToOrdinal.cpp
    #ldr/LdrpNotifyLoadOfGraph.cpp
    #ldr/LdrpParseForwarderDescription.cpp
    #ldr/LdrpPinModule.cpp
    #ldr/LdrpPrepareImportAddressTableForSnap.cpp
    #ldr/LdrpPrepareModuleForExecution.cpp
    #ldr/LdrpPreprocessDllName.cpp
    #ldr/LdrpProcessMappedModule.cpp
    #ldr/LdrpProcessWork.cpp
    #ldr/LdrpProtectAndRelocateImage.cpp
    #ldr/LdrpQueueWork.cpp
    #ldr/LdrpRecordModuleDependency.cpp
    #ldr/LdrpRelocateImage.cpp
    #ldr/LdrpReportSnapError.cpp
    #ldr/LdrpResolveDllName.cpp
    #ldr/LdrpResolveForwarder.cpp
    #ldr/LdrpResolveProcedureAddress.cpp
    #ldr/LdrpRunInitializeRoutine.cpp
    #ldr/LdrpSearchPath.cpp
    #ldr/LdrpSetProtection.cpp
    #ldr/LdrpSignalModuleMapped.cpp
    #ldr/LdrpSnapModule.cpp
    #ldr/LdrpTrimTrailingDots.cpp
    #ldr/LdrpUnloadNode.cpp
    #ldr/LdrpUnmapModule.cpp
    #ldr/ldrshim.cpp
    #ldr/ldrsnap.cpp
    #ldr/ldrstring.cpp
    #ldr/ldrtls.cpp
    #ldr/ldrutils.cpp
    #ldr/tracing.cpp
    ldr/tracing.hpp
    ldr/verifier.c
    rtl/libsupp.c
    rtl/uilist.c
    rtl/smlib
    rtl/version.c
    etw/trace.c)

if(ARCH STREQUAL "i386")
    list(APPEND ASM_SOURCE dispatch/i386/dispatch.S)
elseif(ARCH STREQUAL "amd64")
    list(APPEND ASM_SOURCE dispatch/amd64/dispatch.S)
elseif(ARCH STREQUAL "arm")
    list(APPEND ASM_SOURCE dispatch/arm/stubs_asm.s)
else()
    list(APPEND SOURCE dispatch/dispatch.c)
endif()

add_asm_files(ntdll_asm ${ASM_SOURCE})

list(APPEND PCH_SKIP_SOURCE
    ${CMAKE_CURRENT_BINARY_DIR}/ntdll_stubs.c)

add_library(ntdll MODULE
    ${SOURCE}
    ${ntdll_asm}
    ${PCH_SKIP_SOURCE}
    def/ntdll.rc
    ${CMAKE_CURRENT_BINARY_DIR}/ntdll.def)

set_module_type(ntdll win32dll ENTRYPOINT 0)
#############################################
## HACK FOR MSVC COMPILATION WITH win32dll ##
set_subsystem(ntdll console)
################# END  HACK #################

target_link_libraries(ntdll csrlib rtl rtl_vista ntdllsys libcntpr uuid ${PSEH_LIB})

if (STACK_PROTECTOR)
    target_sources(ntdll PRIVATE $<TARGET_OBJECTS:gcc_ssp_nt>)
endif()

add_pch(ntdll include/ntdll.h "${PCH_SKIP_SOURCE}")
add_dependencies(ntdll ntstatus asm)
add_cd_file(TARGET ntdll DESTINATION reactos/system32 NO_CAB FOR all)
