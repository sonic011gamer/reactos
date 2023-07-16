
if (NOT MSDX)
    add_subdirectory(dxapi)
endif()
add_subdirectory(dxg) # On x64, use dxg.sys from XP x64 3790.1069
add_subdirectory(dxgthk) # Also needed for dxg.sys from XP x64 3790.1069
