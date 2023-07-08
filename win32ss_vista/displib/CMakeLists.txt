remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x601)

add_definitions(-DUNICODE -D_UNICODE -D_RDDM_ -D_NTSYSTEM_)

list(APPEND SOURCE
     Dxgk.c
     Dxgk_manage.c
     Dxgk_win8.c)

add_library(displib ${SOURCE})
add_dependencies(displib bugcodes xdk)
