list(APPEND SOURCE
    hd4000.c
    igfxutils.c
    gmbus.c
    hd4000.h)

add_library(hd4000 MODULE ${SOURCE} hd4000.rc)
set_module_type(hd4000 kernelmodedriver)
add_importlibs(hd4000 ntoskrnl videoprt)
target_link_libraries(hd4000 libcntpr)
add_pch(hd4000 hd4000.h SOURCE)
add_cd_file(TARGET hd4000 DESTINATION reactos/system32/drivers FOR all)
add_registry_inf(hd4000_reg.inf)
add_driver_inf(hd4000 hd4000.inf)
