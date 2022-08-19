

include_directories(${REACTOS_SOURCE_DIR}/subsystems/win32/csrss/include)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys/posix)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys)

spec2def(psxss.exe psxss.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    include/psxss.h
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

add_executable(psxss
    ${SOURCE}
    psxss.rc
    ${CMAKE_CURRENT_BINARY_DIR}/psxss.def)

set_module_type(psxss nativecui)
target_link_libraries(psxss ${PSEH_LIB} smlib nt)
add_importlibs(psxss smdll ntdll)
add_pch(psxss include/psxss.h SOURCE)
add_dependencies(psxss psdk bugcodes)
add_cd_file(TARGET psxss DESTINATION reactos/system32 FOR all)
