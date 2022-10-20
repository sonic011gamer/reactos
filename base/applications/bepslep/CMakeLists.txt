
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys/os2)

list(APPEND SOURCE
    bepslep.c
    bepslep.rc)

add_executable(bepslep ${SOURCE})
set_module_type(bepslep win32gui)
add_importlibs(bepslep doscalls)
add_cd_file(TARGET bepslep DESTINATION reactos/system32 FOR all)
