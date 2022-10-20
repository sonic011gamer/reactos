
add_definitions(-D_doscalls_)
spec2def(doscalls.dll doscalls.spec)

list(APPEND SOURCE
    devices/devices.cpp
    devices/ioctl_async.cpp
    misc/doscalls.cpp
    misc/error.cpp
    run/process.cpp)

add_library(doscalls MODULE
    ${SOURCE}
    doscalls.rc
    ${CMAKE_CURRENT_BINARY_DIR}/doscalls.def
	${CMAKE_CURRENT_BINARY_DIR}/doscalls_stubs.c)

set_module_type(doscalls win32dll ENTRYPOINT DllMain)
add_importlibs(doscalls ntdll)
add_dependencies(doscalls psdk)
add_cd_file(TARGET doscalls DESTINATION reactos/system32 FOR all)
