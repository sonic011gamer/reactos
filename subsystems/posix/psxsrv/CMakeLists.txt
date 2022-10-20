
include_directories(${REACTOS_SOURCE_DIR}/subsystems/win32/csrss/include)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys/os2)

spec2def(psxsrv.dll psxsrv.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    dllmain.c
    server.c
	psxsrv.h)

add_library(psxsrv MODULE
    ${SOURCE}
    psxsrv.rc
    ${CMAKE_CURRENT_BINARY_DIR}/psxsrv.def)

set_module_type(psxsrv nativedll)
target_link_libraries(psxsrv ${PSEH_LIB} smlib)
add_importlibs(psxsrv smdll ntdll csrsrv)
add_pch(psxsrv psxsrv.h SOURCE)
add_dependencies(psxsrv psdk bugcodes)
add_cd_file(TARGET psxsrv DESTINATION reactos/system32 FOR all)
