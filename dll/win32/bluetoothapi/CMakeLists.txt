
remove_definitions(-D_WIN32_WINNT=0x502 -DWINVER=0x502)
add_definitions(-D_WIN32_WINNT=0x601 -DWINVER=0x601)

include_directories(${REACTOS_SOURCE_DIR}/win32ss/include)

add_definitions(-D_bluetoothapis_)
spec2def(bluetoothapis.dll bluetoothapis.spec)

list(APPEND SOURCE
    DllMain.c
    stubs.c)

add_library(bluetoothapis MODULE
    ${SOURCE}
    bluetoothapis.rc
    ${CMAKE_CURRENT_BINARY_DIR}/bluetoothapis.def
	${CMAKE_CURRENT_BINARY_DIR}/bluetoothapis_stubs.c)

set_module_type(bluetoothapis win32dll ENTRYPOINT DllMain)
add_importlibs(bluetoothapis kernel32 ntdll msvcrt user32)
add_dependencies(bluetoothapis psdk)
add_cd_file(TARGET bluetoothapis DESTINATION reactos/system32 FOR all)