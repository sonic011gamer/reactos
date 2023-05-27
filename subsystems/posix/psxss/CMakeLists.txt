
include_directories(${REACTOS_SOURCE_DIR}/subsystems/win32/csrss/include)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys/posix)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys)

list(APPEND SOURCE
   	main.c
)

add_executable(psxss
    ${SOURCE}
    psxss.rc)

set_module_type(psxss nativecui)
target_link_libraries(psxss ${PSEH_LIB} smlib nt)
add_importlibs(psxss psxsrv ntdll)
add_dependencies(psxss psdk bugcodes)
add_cd_file(TARGET psxss DESTINATION reactos/system32 FOR all)
