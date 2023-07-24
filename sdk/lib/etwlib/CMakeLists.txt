
add_definitions(
    -D_NTSYSTEM_)

add_library(etwlib etwlib.c)
add_dependencies(etwlib xdk)
