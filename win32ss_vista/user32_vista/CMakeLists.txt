
spec2def(user32_vista.dll user32_vista.spec ADD_IMPORTLIB)

include_directories(
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys
    include
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine
    ${REACTOS_SOURCE_DIR}/win32ss/include)

list(APPEND SOURCE
    stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/user32_vista_stubs.c
    include/user32_vista.h)

add_library(user32_vista MODULE
    ${SOURCE}
    user32_vista.rc
    ${CMAKE_CURRENT_BINARY_DIR}/user32_vista.def)

set_module_type(user32_vista win32dll UNICODE ENTRYPOINT 0)
target_link_libraries(user32_vista wine win32ksys ${PSEH_LIB})
add_dependencies(user32_vista asm)

if(MSVC AND (ARCH STREQUAL "i386"))
    target_sources(user32_vista PRIVATE $<TARGET_OBJECTS:ftol2_sse>)
endif()

add_delay_importlibs(user32_vista usp10)
add_importlibs(user32_vista gdi32 advapi32 kernel32 ntdll)
add_pch(user32_vista include/user32_vista.h SOURCE)
add_cd_file(TARGET user32_vista DESTINATION reactos/system32 FOR all)
