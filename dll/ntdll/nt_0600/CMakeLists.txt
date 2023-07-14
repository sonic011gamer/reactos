remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x601)

add_definitions(
    -D__NTDLL__
    -D_NTOSKRNL_
    -DCRTDLL)

include_directories(
    BEFORE include
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys)

list(APPEND SOURCE
    DllMain.c)

add_library(ntdll_vista ${SOURCE})
target_link_libraries(ntdll_vista smlib rtl_vista)

if(ARCH STREQUAL "arm")
    target_link_libraries(ntdll_vista chkstk)
endif()

add_dependencies(ntdll_vista psdk)
add_cd_file(TARGET ntdll_vista DESTINATION reactos/system32 FOR all)
