
spec2def(winusb.dll winusb.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    winusb.c
    winusb.rc
    ${CMAKE_CURRENT_BINARY_DIR}/winusb.def
    ${CMAKE_CURRENT_BINARY_DIR}/winusb_stubs.c)

add_library(winusb MODULE ${SOURCE})
set_module_type(winusb win32dll)
target_link_libraries(winusb ${PSEH_LIB})
add_importlibs(winusb msvcrt kernel32 ntdll)
add_cd_file(TARGET winusb DESTINATION reactos/system32 FOR all)
