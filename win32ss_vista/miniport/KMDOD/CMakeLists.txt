

remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x601)

list(APPEND SOURCE
      bdd_ddi.cxx
      bdd_dmm.cxx
      bdd_util.cxx
      bdd.cxx
      bltfuncs.cxx
      blthw.cxx
      memory.cxx
    )

add_library(kmdod MODULE
    ${SOURCE}
    sampledisplay.rc
    )

if(USE_CLANG_CL)
    target_compile_options(kmdod PRIVATE "-Wno-missing-braces")
endif()

set_module_type(kmdod kernelmodedriver ENTRYPOINT DriverEntry)
add_importlibs(kmdod ntoskrnl hal dxgkrnl)
add_driver_inf(xboxvmp sampledisplay.inf)
add_cd_file(TARGET kmdod DESTINATION reactos/system32/drivers NO_CAB FOR all)
