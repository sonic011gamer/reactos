
spec2def(uapfs.dll uapfs.spec)

list(APPEND SOURCE
    uapfs.c
    uapfs.rc
    ${CMAKE_CURRENT_BINARY_DIR}/uapfs.def)

add_library(uapfs MODULE ${SOURCE})
set_module_type(uapfs nativedll)
target_link_libraries(uapfs apfslib)
add_importlibs(uapfs ntdll)
add_cd_file(TARGET uapfs DESTINATION reactos/system32 FOR all)
