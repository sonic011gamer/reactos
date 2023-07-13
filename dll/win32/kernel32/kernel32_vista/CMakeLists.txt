
remove_definitions(-D_WIN32_WINNT=0x502 -DWINVER=0x502)
add_definitions(-D_WIN32_WINNT=0x601 -DWINVER=0x601)

add_definitions(-D_KERNEL32_)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys)

list(APPEND SOURCE
    DllMain.c
    GetFileInformationByHandleEx.c
    GetTickCount64.c
    InitOnceExecuteOnce.c
    sync.c
    vista.c
    power.c
    proc.c
    k32.c)

add_library(kernel32_vista ${SOURCE})
target_link_libraries(kernel32_vista kernel32_shared)
add_dependencies(kernel32_vista psdk)
add_cd_file(TARGET kernel32_vista DESTINATION reactos/system32 FOR all)
