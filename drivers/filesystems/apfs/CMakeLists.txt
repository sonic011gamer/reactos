
list(APPEND SOURCE
    apfs.c
    apfs.h)

add_library(apfs MODULE ${SOURCE} apfs.rc)
set_module_type(apfs kernelmodedriver)
target_link_libraries(apfs ${PSEH_LIB})
add_importlibs(apfs ntoskrnl hal)
add_pch(apfs apfs.h SOURCE)
add_cd_file(TARGET apfs DESTINATION reactos/system32/drivers FOR all)
