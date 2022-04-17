
add_definitions(-D_VMSDLL_)
spec2def(vmsdll.dll vmsdll.spec)

list(APPEND SOURCE
    dllmain.c
    stubs.c)

add_library(vmsdll MODULE
    ${SOURCE}
    vmsdll.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vmsdll.def
	${CMAKE_CURRENT_BINARY_DIR}/vmsdll_stubs.c)

set_module_type(vmsdll win32dll ENTRYPOINT DllMain)
add_importlibs(vmsdll kernel32 ntdll msvcrt)
add_dependencies(vmsdll psdk)
add_cd_file(TARGET vmsdll DESTINATION reactos/system32 FOR all)