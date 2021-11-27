
add_definitions(
    -DNDIS_MINIPORT_DRIVER
    -DNDIS51_MINIPORT=1)

list(APPEND SOURCE
    dc21x4.c
    dc21x4.h
    eeprom.c
    interrupt.c
    nic.c
    nic.h
    phy.c
    requests.c
    send.c)

add_library(dc21x4 MODULE ${SOURCE} dc21x4.rc)
# add_pch(dc21x4 dc21x4.h SOURCE)
set_module_type(dc21x4 kernelmodedriver)
add_importlibs(dc21x4 ndis ntoskrnl hal)
add_cd_file(TARGET dc21x4 DESTINATION reactos/system32/drivers FOR all)
add_driver_inf(dc21x4 net21x4.inf)
