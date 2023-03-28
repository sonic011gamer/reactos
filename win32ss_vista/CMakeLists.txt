include_directories("include/")
add_subdirectory(dxgkrnl)
add_subdirectory(miniport)

#win32ss
add_subdirectory(gdi32_vista)