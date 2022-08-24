
remove_definitions(-D_WIN32_WINNT=0x502 -DWINVER=0x502)
add_definitions(-D_WIN32_WINNT=0x601 -DWINVER=0x601)

include_directories(${REACTOS_SOURCE_DIR}/win32ss/include)

add_definitions(-D_user32-delay_)
spec2def(user32-delay.dll user32-delay.spec)

list(APPEND SOURCE
    DllMain.c
    stubs.c)

add_library(user32-delay MODULE
    ${SOURCE}
    user32-delay.rc
    ${CMAKE_CURRENT_BINARY_DIR}/user32-delay.def
	${CMAKE_CURRENT_BINARY_DIR}/user32-delay_stubs.c)

set_module_type(user32-delay win32dll ENTRYPOINT DllMain)
add_importlibs(user32-delay kernel32 ntdll msvcrt user32)
add_dependencies(user32-delay psdk)
add_cd_file(TARGET user32-delay DESTINATION reactos/system32 FOR all)