
include_directories(${REACTOS_SOURCE_DIR}/subsystems/win32/csrss/include)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys/os2)


add_definitions(-D_doscalls_)
spec2def(doscalls.dll doscalls.spec ADD_IMPORTLIB)

list(APPEND SOURCE
stubs.c)
   # devices/devices.cpp
    #devices/ioctl_async.cpp
    #misc/doscalls.cpp
    #misc/error.cpp
    #run/process.cpp)

add_library(doscalls MODULE
    ${SOURCE}
    doscalls.rc
    ${CMAKE_CURRENT_BINARY_DIR}/doscalls.def
	${CMAKE_CURRENT_BINARY_DIR}/doscalls_stubs.c)

set_module_type(doscalls nativedll ENTRYPOINT DllMain)
add_importlibs(doscalls ntdll)
add_dependencies(doscalls psdk)
add_cd_file(TARGET doscalls DESTINATION reactos/system32 FOR all)
