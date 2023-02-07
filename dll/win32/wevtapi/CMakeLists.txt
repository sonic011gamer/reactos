
spec2def(wevtapi.dll wevtapi.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    wevtapi.c
    wevtapi.rc
    ${CMAKE_CURRENT_BINARY_DIR}/wevtapi.def
    ${CMAKE_CURRENT_BINARY_DIR}/wevtapi_stubs.c)

add_library(wevtapi MODULE ${SOURCE})
set_module_type(wevtapi win32dll)
target_link_libraries(wevtapi ${PSEH_LIB})
add_importlibs(wevtapi msvcrt kernel32 ntdll)
add_cd_file(TARGET wevtapi DESTINATION reactos/system32 FOR all)
