

include_directories(${REACTOS_SOURCE_DIR}/subsystems/win32/csrss/include)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys/posix)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys)

spec2def(psxsrv.dll psxsrv.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    include/psxsrv.h
    init.c
    print.c
    main.c
	ob/process.c
	ob/session.c
	ob/terminal.c
	port/api.c
	port/sbapi.c
	port/session.c
	port/utils.c
	call/null.c
	call/_exit.c
	call/stubs.c
	call/syscall.c
)

add_library(psxsrv MODULE
    ${SOURCE}
    psxsrv.rc
    ${CMAKE_CURRENT_BINARY_DIR}/psxsrv.def)

set_module_type(psxsrv nativedll)
target_link_libraries(psxsrv ${PSEH_LIB} smlib nt)
add_importlibs(psxsrv smdll ntdll)
add_pch(psxsrv include/psxsrv.h SOURCE)
add_dependencies(psxsrv psdk bugcodes)
add_cd_file(TARGET psxsrv DESTINATION reactos/system32 FOR all)
