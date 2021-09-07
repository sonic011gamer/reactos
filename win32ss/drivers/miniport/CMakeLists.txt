
add_subdirectory(vbe)
add_subdirectory(vga)
#add_subdirectory(vga_new)
add_subdirectory(vmx_svga)
add_subdirectory(open_nvminiport)

if(ARCH STREQUAL "i386")
    add_subdirectory(pc98vid)
    add_subdirectory(xboxvmp)
endif()
