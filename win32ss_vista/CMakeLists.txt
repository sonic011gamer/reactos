include_directories("include/")
add_subdirectory(displib)
add_subdirectory(dxgkrnl)

#win32ss
add_subdirectory(gdi32_vista)
add_subdirectory(user32_vista)