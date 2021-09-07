
list(APPEND SOURCE
    open_nvminiport.c
    open_nvminiport.h)

add_library(open_nvminiport MODULE
    ${SOURCE}
    open_nvminiport.rc)

set_module_type(open_nvminiport kernelmodedriver)
add_importlibs(open_nvminiport ntoskrnl videoprt)
add_cd_file(TARGET open_nvminiport DESTINATION reactos/system32/drivers FOR all)
add_registry_inf(open_nvminiport_reg.inf)
add_driver_inf(open_nvminiport open_nvminiport.inf)
