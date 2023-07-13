add_definitions(-D_WER_)

spec2def(wer.dll wer.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    DllMain.c
    stubs.c
    KB.h)

add_library(wer MODULE
    ${SOURCE}
    Wer.rc
    stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/wer_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/wer.def)

target_link_libraries(wer)
set_module_type(wer win32dll ENTRYPOINT DllMain)
add_importlibs(wer ntdll advapi32 kernel32)
add_dependencies(wer psdk)
add_cd_file(TARGET wer DESTINATION reactos/system32 FOR all)