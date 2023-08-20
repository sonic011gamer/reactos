
add_definitions(-DUNICODE -D_UNICODE)

remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x601)


include_directories("../common/")
include_directories("../shared/")
include_directories("../viogpu3d/")
include_directories("../../../../sdk/lib/drivers/virtio/")
list(APPEND SOURCE
    log.cpp
    pipe.cpp)

add_library(virtiogpushared ${SOURCE})

if(NOT MSVC)
    target_compile_options(virtiogpushared PRIVATE
        -Wno-unknown-pragmas
        -Wno-incompatible-pointer-types)
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(virtiogpushared PRIVATE -Wno-old-style-declaration)
endif()
