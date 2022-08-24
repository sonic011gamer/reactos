
spec2def(bthprops.cpl bthprops.spec)

list(APPEND SOURCE
    access.c
    display.c
    general.c
    keyboard.c
    mouse.c
    stubs.c
    sound.c
    access.h)

add_rc_deps(bthprops.rc ${CMAKE_CURRENT_SOURCE_DIR}/resources/applet.ico)

add_library(bthprops MODULE
    ${SOURCE}
    bthprops.rc
    ${CMAKE_CURRENT_BINARY_DIR}/bthprops_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/bthprops.def)

set_module_type(bthprops cpl UNICODE)
add_importlibs(bthprops user32 gdi32 advapi32 comctl32 msvcrt kernel32)
add_pch(bthprops access.h SOURCE)
add_cd_file(TARGET bthprops DESTINATION reactos/system32 FOR all)
