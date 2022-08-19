
add_definitions(-DUNICODE -D_UNICODE)

list(APPEND SOURCE
    VirtIOPCICommon.c
    VirtIOPCILegacy.c
    VirtIOPCIModern.c
    VirtIORing.c
    VirtIORing-Packed.c)

add_library(virtio ${SOURCE})
add_dependencies(virtio bugcodes xdk)

if(NOT MSVC)
    target_compile_options(virtio PRIVATE
        -Wno-unknown-pragmas
        -Wno-incompatible-pointer-types)
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(virtio PRIVATE -Wno-old-style-declaration)
endif()
