

remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x601)

list(APPEND SOURCE
     drivertest.c
    )

add_library(openwddm MODULE
    ${SOURCE}
    openwddm.rc
    )

if(USE_CLANG_CL)
    target_compile_options(openwddm PRIVATE "-Wno-missing-braces")
endif()

set_module_type(openwddm kernelmodedriver ENTRYPOINT DriverEntry)
add_importlibs(openwddm ntoskrnl hal dxgkrnl)
add_driver_inf(openwddm openwddm.inf)
add_cd_file(TARGET openwddm DESTINATION reactos/system32/drivers NO_CAB FOR all)
