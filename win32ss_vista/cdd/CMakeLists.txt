
list(APPEND SOURCE
    anotherblt.c
    cdd.c
    entry.c
    cdd.h)

add_library(cdd MODULE
    ${SOURCE}
    cdd.rc)

set_module_type(cdd kerneldll ENTRYPOINT DrvEnableDriver 12)
add_pch(cdd cdd.h SOURCE)
target_link_libraries(cdd libcntpr)
add_importlibs(cdd win32k ntoskrnl)
add_dependencies(cdd psdk)
add_cd_file(TARGET cdd DESTINATION reactos/system32 FOR all)
