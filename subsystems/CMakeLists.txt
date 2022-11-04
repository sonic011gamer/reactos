
add_subdirectory(csr)
if(ARCH STREQUAL "i386" OR ARCH STREQUAL "amd64")
    add_subdirectory(mvdm) #TODO: Im too lazy to fix this right now
endif()
add_subdirectory(win)
