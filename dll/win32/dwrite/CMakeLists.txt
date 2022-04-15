
remove_definitions(-D_WIN32_WINNT=0x502 -DWINVER=0x502)
add_definitions(-D_WIN32_WINNT=0x601 -DWINVER=0x601)

add_definitions(-D_DWRITE_)
spec2def(DWrite.dll DWrite.spec)

list(APPEND SOURCE
    DWrite.c)

add_library(DWrite MODULE
    ${SOURCE}
    DWrite.rc
    ${CMAKE_CURRENT_BINARY_DIR}/DWrite.def
    ${CMAKE_CURRENT_BINARY_DIR}/DWrite_stubs.c)

set_module_type(DWrite win32dll ENTRYPOINT DllMain)
add_importlibs(DWrite kernel32 ntdll)
add_dependencies(DWrite psdk)
add_cd_file(TARGET DWrite DESTINATION reactos/system32 FOR all)
