
remove_definitions(-D_WIN32_WINNT=0x502 -DWINVER=0x502)
add_definitions(-D_WIN32_WINNT=0x600 -DWINVER=0x600)
#if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
#    target_compile_options(imageres PRIVATE -Wno-unused-but-set-variable)
#endif()
add_definitions(-D_IMAGERES_)
spec2def(imageres.dll imageres.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    imageres.h)



add_library(imageres MODULE
    ${SOURCE}
    imageres.rc
    ${CMAKE_CURRENT_BINARY_DIR}/imageres.def
	${CMAKE_CURRENT_BINARY_DIR}/imageres_stubs.c)


set_module_type(imageres win32dll)
#add_importlibs(imageres user32 gdi32 comctl32 msvcrt kernel32 ntdll)

add_cd_file(TARGET imageres DESTINATION reactos/system32 FOR all)
