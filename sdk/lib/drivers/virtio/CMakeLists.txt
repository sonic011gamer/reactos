
include_directories(windows)

add_definitions(-DUNICODE -D_UNICODE)

list(APPEND SOURCE
    VirtIOPCICommon.c
    VirtIOPCILegacy.c
    VirtIOPCIModern.c
    VirtIORing.c
    VirtIORing-Packed.c)

add_library(virtio ${SOURCE})
add_dependencies(virtio bugcodes xdk)
