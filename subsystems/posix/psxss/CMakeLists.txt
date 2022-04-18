

include_directories(${REACTOS_SOURCE_DIR}/subsystems/win32/csrss/include)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys/posix)
add_definitions(-D__PSXSS_ON_W32__ )
add_executable(psxss main.c)


set_module_type(psxss nativecui ENTRYPOINT main)
target_link_libraries(psxss nt)
add_importlibs(psxss ntdll csrsrv msvcrt kernel32 psxsrv )
add_dependencies(psxss psdk bugcodes)
add_cd_file(TARGET psxss DESTINATION reactos/system32 FOR all)
