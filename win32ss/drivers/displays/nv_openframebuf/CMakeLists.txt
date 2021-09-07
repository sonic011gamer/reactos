
list(APPEND SOURCE
    nv_openframebuf.h)

add_library(nv_openframebuf MODULE
    ${SOURCE}
    nv_openframebuf.rc)

set_module_type(nv_openframebuf kerneldll ENTRYPOINT DrvEnableDriver 12)
add_pch(nv_openframebuf nv_openframebuf.h SOURCE)
target_link_libraries(nv_openframebuf libcntpr)
add_importlibs(nv_openframebuf win32k)
add_dependencies(nv_openframebuf psdk)
add_cd_file(TARGET nv_openframebuf DESTINATION reactos/system32 FOR all)
