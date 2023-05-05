
list(APPEND MINI_HAL_SOURCE
    ../gic/gic.c
    ../include/hal.h)

add_library(mini_hal ${MINI_HAL_SOURCE})
target_compile_definitions(mini_hal PRIVATE _MINIHAL_ _BLDR_ _NTSYSTEM_)
add_dependencies(mini_hal psdk bugcodes)
add_pch(mini_hal ../include/hal.h MINI_HAL_SOURCE)
