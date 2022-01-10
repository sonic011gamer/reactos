
list(APPEND SOURCE
    perm3.h
    perm3.c
    perm3io.c
    dmasup.c
    perm3dat.c
    interupt.c
    power.c
    video.c
    i2c.c)

add_library(perm3_mini MODULE
    ${SOURCE}
    perm3.rc)

set_module_type(perm3_mini kerneldll ENTRYPOINT DrvEnableDriver 12)
add_pch(perm3_mini perm3.h SOURCE)
target_link_libraries(perm3_mini libcntpr)
add_importlibs(perm3_mini win32k)
add_dependencies(perm3_mini psdk)
add_cd_file(TARGET perm3_mini DESTINATION reactos/system32 FOR all)
