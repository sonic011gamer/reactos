
include_directories(${REACTOS_SOURCE_DIR}/subsystems/win32/csrss/include)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys/os2)


list(APPEND SOURCE
    main.c)

add_executable(psxss
    ${SOURCE}
    psxss.rc)

set_module_type(psxss nativecui ENTRYPOINT main)
target_link_libraries(psxss nt)
add_importlibs(psxss ntdll smdll csrsrv)
add_dependencies(psxss psdk bugcodes)
add_cd_file(TARGET psxss DESTINATION reactos/system32 FOR all)
