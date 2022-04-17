

include_directories(${REACTOS_SOURCE_DIR}/subsystems/win32/csrss/include)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys)

spec2def(vmssrv.dll vmssrv.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    DllMain.c
    server.c
    vmssrv.h)

add_library(vmssrv MODULE
    ${SOURCE}
    vmssrv.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vmssrv.def)

set_module_type(vmssrv nativedll)
target_link_libraries(vmssrv ${PSEH_LIB} smlib)
add_importlibs(vmssrv smdll ntdll csrsrv)
add_pch(vmssrv vmssrv.h SOURCE)
add_dependencies(vmssrv psdk bugcodes)
add_cd_file(TARGET vmssrv DESTINATION reactos/system32 FOR all)
