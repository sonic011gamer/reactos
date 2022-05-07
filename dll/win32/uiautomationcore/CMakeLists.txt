
add_definitions(-D_UIAUTOMATIONCORE_)

spec2def(UIAutomationCore.dll UIAutomationCore.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    DllMain.c
    KB.h)

add_library(UIAutomationCore MODULE
    ${SOURCE}
    UIAutomationCore.rc
    stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/UIAutomationCore_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/UIAutomationCore.def)

target_link_libraries(UIAutomationCore)
set_module_type(UIAutomationCore win32dll ENTRYPOINT DllMain)
add_importlibs(UIAutomationCore ntdll ntdll_vista kernel32_vista advapi32 kernel32)
add_dependencies(UIAutomationCore psdk)
add_cd_file(TARGET UIAutomationCore DESTINATION reactos/system32 FOR all)