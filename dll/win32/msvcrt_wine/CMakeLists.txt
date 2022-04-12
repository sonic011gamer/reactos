
remove_definitions(-D_WIN32_WINNT=0x502 -DWINVER=0x502)
add_definitions(-D_WIN32_WINNT=0x601 -DWINVER=0x601)
include_directories(${REACTOS_SOURCE_DIR}/sdk/lib/crt/include)

add_definitions(-D_MSVCRT_)

add_definitions(
    -DUSE_MSVCRT_PREFIX
    -D_MSVCRT_
    -D_MSVCRT_LIB_
    -D_MT
    -D_CTYPE_DISABLE_MACROS
    -D_NO_INLINING
    -DCRTDLL
    -D__MINGW_IMPORT="")

spec2def(msvcrt_wine.dll msvcrt_wine.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    DllMain.c
    stubs.c
    #math.c
    ${CMAKE_CURRENT_BINARY_DIR}/msvcrt_wine_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/msvcrt_wine.def)

add_library(msvcrt_wine MODULE ${SOURCE})
set_module_type(msvcrt_wine win32dll ENTRYPOINT DllMain 12)
target_link_libraries(msvcrt_wine crt wine ${PSEH_LIB})
add_importlibs(msvcrt_wine msvcrt kernel32 ntdll)
add_dependencies(msvcrt_wine psdk)
add_cd_file(TARGET msvcrt_wine DESTINATION reactos/system32 FOR all)
