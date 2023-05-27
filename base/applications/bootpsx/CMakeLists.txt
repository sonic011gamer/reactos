
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys/posix)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys)

list(APPEND SOURCE
    bootpsx.c
    bootpsx.rc)

add_executable(bootpsx ${SOURCE})
set_module_type(bootpsx win32gui UNICODE ENTRYPOINT main)
add_importlibs(bootpsx ntdll kernel32)
target_link_libraries(bootpsx smlib)
if(MSVC)
    add_importlibs(bootpsx ntdll)
endif()

add_cd_file(TARGET bootpsx DESTINATION reactos/system32 FOR all)
