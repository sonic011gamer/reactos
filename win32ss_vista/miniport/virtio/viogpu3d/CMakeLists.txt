

remove_definitions(-D_WIN32_WINNT=0x502)
remove_definitions(-D_WIN32_WINNT=0x601)
add_definitions(-D_WIN32_WINNT=0x602)

include_directories("../common/")
include_directories("../shared/")
include_directories("../viogpu3d/")
include_directories("../../../../sdk/lib/drivers/virtio/")

list(APPEND SOURCE
    driver.cpp
    viogpu_adapter.cpp
    viogpu_allocation.cpp
    viogpu_command.cpp
    viogpu_device.cpp
    viogpu_vidpn.cpp)

add_library(viogpu3d MODULE
    ${SOURCE}
    viogpu3d.rc
    )

if(USE_CLANG_CL)
    target_compile_options(viogpu3d PRIVATE "-Wno-missing-braces")
endif()

set_module_type(viogpu3d kernelmodedriver)
target_link_libraries(viogpu3d displib virtio virtiogpucommon)
add_importlibs(viogpu3d ntoskrnl hal)
add_driver_inf(viogpu3d viogpu3d.inf)
add_cd_file(TARGET viogpu3d DESTINATION reactos/system32/drivers NO_CAB FOR all)
