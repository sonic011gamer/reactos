
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys/posix)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys)

list(APPEND SOURCE
    posixsleep.c
    posixsleep.rc)

add_executable(posixsleep ${SOURCE})
set_module_type(posixsleep win32gui UNICODE ENTRYPOINT main)
add_importlibs(posixsleep psxdll)

add_cd_file(TARGET posixsleep DESTINATION reactos/system32 FOR all)
