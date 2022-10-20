
include_directories(${REACTOS_SOURCE_DIR}/subsystems/win32/csrss/include)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys/os2)


list(APPEND SOURCE
    main.c)

add_executable(os2ss
    ${SOURCE}
    os2ss.rc)

set_module_type(os2ss win32gui)
target_link_libraries(os2ss nt)
add_importlibs(os2ss smdll ntdll os2srv)
add_dependencies(os2ss psdk bugcodes)
add_cd_file(TARGET os2ss DESTINATION reactos/system32 FOR all)
