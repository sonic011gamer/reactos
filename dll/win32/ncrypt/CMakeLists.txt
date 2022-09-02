
add_definitions(-D_ncrypt_)

spec2def(ncrypt.dll ncrypt.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    DllMain.c
    KB.h)

add_library(ncrypt MODULE
    ${SOURCE}
    ncrypt.rc
    stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/ncrypt_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/ncrypt.def)

target_link_libraries(ncrypt)
set_module_type(ncrypt win32dll ENTRYPOINT DllMain)
add_importlibs(ncrypt ntdll ntdll_vista kernel32_vista advapi32 kernel32)
add_dependencies(ncrypt psdk)
add_cd_file(TARGET ncrypt DESTINATION reactos/system32 FOR all)