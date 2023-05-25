
add_definitions(-D_SHUNIMPL_)

spec2def(SHUNIMPL.dll SHUNIMPL.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    DllMain.c
    KB.h)

add_library(SHUNIMPL MODULE
    ${SOURCE}
    SHUNIMPL.rc
    stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/SHUNIMPL_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/SHUNIMPL.def)

target_link_libraries(SHUNIMPL)
set_module_type(SHUNIMPL win32dll ENTRYPOINT DllMain)
add_importlibs(SHUNIMPL ntdll ntdll_vista kernel32_vista advapi32 kernel32)
add_dependencies(SHUNIMPL psdk)
add_cd_file(TARGET SHUNIMPL DESTINATION reactos/system32 FOR all)