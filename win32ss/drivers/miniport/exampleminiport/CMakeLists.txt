list(APPEND SOURCE
    example.c
    example.h)

add_library(example MODULE ${SOURCE} example.rc)
set_module_type(example kernelmodedriver)
add_importlibs(example videoprt)
target_link_libraries(example libcntpr)
add_pch(example example.h SOURCE)
add_cd_file(TARGET example DESTINATION reactos/system32/drivers FOR all)
add_registry_inf(example_reg.inf)
add_driver_inf(example example.inf)
