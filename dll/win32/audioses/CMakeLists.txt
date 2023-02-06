
spec2def(audioses.dll audioses.spec)

list(APPEND SOURCE
    ${CMAKE_CURRENT_BINARY_DIR}/audioses_stubs.c)

add_library(audioses MODULE
    ${SOURCE}
    ${CMAKE_CURRENT_BINARY_DIR}/audioses.def)

set_module_type(audioses win32dll)
add_importlibs(audioses msvcrt kernel32 ntdll)
add_cd_file(TARGET audioses DESTINATION reactos/system32 FOR all)
