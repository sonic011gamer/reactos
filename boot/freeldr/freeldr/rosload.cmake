##
## PROJECT:     FreeLoader
## LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
## PURPOSE:     Build definitions for UEFI
## COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
##

include_directories(BEFORE
    ${REACTOS_SOURCE_DIR}/boot/environ/include/efi
    ${REACTOS_SOURCE_DIR}/boot/freeldr/freeldr
    ${REACTOS_SOURCE_DIR}/boot/freeldr/freeldr/include
    ${REACTOS_SOURCE_DIR}/boot/freeldr/freeldr/include/arch/uefi)

list(APPEND UEFILDR_ARC_SOURCE
    ${FREELDR_ARC_SOURCE}
    arch/uefi/stubs.c
    arch/uefi/ueficon.c
    arch/uefi/uefidisk.c
    arch/uefi/uefimem.c
    arch/uefi/uefisetup.c
    arch/uefi/uefiutil.c
    arch/uefi/uefivid.c
    arch/uefi/rosload.c
    arch/vgafont.c)

if(ARCH STREQUAL "i386")
    list(APPEND UEFILDR_COMMON_ASM_SOURCE
        arch/i386/i386trap.S)

elseif(ARCH STREQUAL "amd64")
    #TBD
elseif(ARCH STREQUAL "arm")
    list(APPEND UEFILDR_ARC_SOURCE
        arch/arm/macharm.c
        arch/arm/debug.c)
    #TBD
elseif(ARCH STREQUAL "arm64")
    list(APPEND UEFILDR_ARC_SOURCE
        arch/arm64/mach.c)
    #TBD
else()
    #TBD
endif()

add_asm_files(rosload_common_asm ${FREELDR_COMMON_ASM_SOURCE} ${UEFILDR_COMMON_ASM_SOURCE})

add_library(rosload_common
    ${rosload_common_asm}
    ${UEFILDR_ARC_SOURCE}
    ${FREELDR_BOOTLIB_SOURCE}
    ${FREELDR_BOOTMGR_SOURCE}
    ${FREELDR_NTLDR_SOURCE})

target_compile_definitions(rosload_common PRIVATE UEFIBOOT ROSLOAD)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    # Prevent using SSE (no support in freeldr)
    target_compile_options(rosload_common PUBLIC -mno-sse)
endif()

set(PCH_SOURCE
    ${UEFILDR_ARC_SOURCE}
    ${FREELDR_BOOTLIB_SOURCE}
    ${FREELDR_BOOTMGR_SOURCE}
    ${FREELDR_NTLDR_SOURCE})

add_pch(rosload_common include/arch/uefi/uefildr.h PCH_SOURCE)
add_dependencies(rosload_common bugcodes asm xdk)

## GCC builds need this extra thing for some reason...
if(ARCH STREQUAL "i386" AND NOT MSVC)
    target_link_libraries(rosload_common mini_hal)
endif()


spec2def(rosload.exe freeldr.spec)

list(APPEND UEFILDR_BASE_SOURCE
    include/arch/uefi/uefildr.h
    arch/uefi/uefildr.c
    ${FREELDR_BASE_SOURCE})

if(ARCH STREQUAL "i386")
    # Must be included together with disk/scsiport.c
    list(APPEND UEFILDR_BASE_SOURCE
        ${CMAKE_CURRENT_BINARY_DIR}/rosload.def)
endif()

add_executable(rosload ${UEFILDR_BASE_SOURCE})
set_target_properties(rosload PROPERTIES SUFFIX ".efi")

target_compile_definitions(rosload PRIVATE UEFIBOOT)

if(MSVC)
if(NOT ARCH STREQUAL "arm")
endif()
    target_link_options(rosload PRIVATE /NXCOMPAT:NO /ignore:4078 /ignore:4254 /DRIVER)
    # We don't need hotpatching
    remove_target_compile_option(rosload "/hotpatch")
else()
    target_link_options(rosload PRIVATE -Wl,--exclude-all-symbols,--file-alignment,0x200,--section-alignment,0x200)
endif()
if(ARCH STREQUAL "arm64")
set_image_base(rosload 0x800000000)
else()
set_image_base(rosload 0x10000)
endif()
if(MSVC)
    set_subsystem(rosload BOOT_APPLICATION)
else()
    set_subsystem(rosload 16)
endif()

if(ARCH STREQUAL "i386")
    set_entrypoint(rosload OslMain 4)
else()
    set_entrypoint(rosload OslMain)
endif()

if(ARCH STREQUAL "i386")
    target_link_libraries(rosload mini_hal)
endif()

target_link_libraries(rosload rosload_common cportlib blcmlib blrtl libcntpr)

# dynamic analysis switches
if(STACK_PROTECTOR)
    target_sources(rosload PRIVATE $<TARGET_OBJECTS:gcc_ssp_nt>)
endif()

if(RUNTIME_CHECKS)
    target_link_libraries(rosload runtmchk)
endif()

add_dependencies(rosload xdk)
