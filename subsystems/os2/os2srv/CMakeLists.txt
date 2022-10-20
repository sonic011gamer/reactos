
include_directories(${REACTOS_SOURCE_DIR}/subsystems/win32/csrss/include)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys/os2)

spec2def(os2srv.dll os2srv.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    dllmain.c
    server.c)

add_library(os2srv MODULE
    ${SOURCE}
    os2srv.rc
    ${CMAKE_CURRENT_BINARY_DIR}/os2srv.def)

set_module_type(os2srv nativedll)
target_link_libraries(os2srv ${PSEH_LIB} smlib)
add_importlibs(os2srv smdll ntdll csrsrv)
add_dependencies(os2srv psdk bugcodes)
add_cd_file(TARGET os2srv DESTINATION reactos/system32 FOR all)
