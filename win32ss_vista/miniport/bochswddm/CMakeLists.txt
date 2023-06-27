

remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x601)

list(APPEND SOURCE
     entry.c
     device_manage.c
    )

add_library(bochswddm MODULE
    ${SOURCE}
    bochswddm.rc
    )

if(USE_CLANG_CL)
    target_compile_options(bochswddm PRIVATE "-Wno-missing-braces")
endif()

set_module_type(bochswddm kernelmodedriver ENTRYPOINT DriverEntry)
target_link_libraries(bochswddm displib)
add_importlibs(bochswddm ntoskrnl dxgkrnl)
add_driver_inf(bochswddm bochswddm.inf)
add_cd_file(TARGET bochswddm DESTINATION reactos/system32/drivers NO_CAB FOR all)
