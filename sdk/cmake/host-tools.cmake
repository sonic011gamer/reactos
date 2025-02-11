
include(ExternalProject)

function(setup_host_tools)
    list(APPEND HOST_TOOLS asmpp bin2c widl gendib cabman fatten hpp isohybrid mkhive mkisofs obj2bin spec2def geninc mkshelllink txt2nls utf16le xml2sdb)
    if(NOT MSVC)
        list(APPEND HOST_TOOLS rsym pefixup)
    endif()
    if ((ARCH STREQUAL "amd64") AND (CMAKE_C_COMPILER_ID STREQUAL "GNU"))
        execute_process(
            COMMAND ${CMAKE_C_COMPILER} --print-file-name=plugin
            OUTPUT_VARIABLE GCC_PLUGIN_DIR)
        string(STRIP ${GCC_PLUGIN_DIR} GCC_PLUGIN_DIR)
        list(APPEND CMAKE_HOST_TOOLS_EXTRA_ARGS -DGCC_PLUGIN_DIR=${GCC_PLUGIN_DIR})
        list(APPEND HOST_MODULES gcc_plugin_seh)
        if (CMAKE_HOST_WIN32)
            list(APPEND HOST_MODULES g++_plugin_seh)
        endif()
    endif()
    list(TRANSFORM HOST_TOOLS PREPEND "${REACTOS_BINARY_DIR}/host-tools/bin/" OUTPUT_VARIABLE HOST_TOOLS_OUTPUT)
    if (CMAKE_HOST_WIN32)
        list(TRANSFORM HOST_TOOLS_OUTPUT APPEND ".exe")
        if(MSVC_IDE)
            set(HOST_EXTRA_DIR "$(ConfigurationName)/")
        endif()
        set(HOST_EXE_SUFFIX ".exe")
        set(HOST_MODULE_SUFFIX ".dll")
    else()
        set(HOST_MODULE_SUFFIX ".so")
    endif()

    # Normalize to the same format as our own ARCH, and add one for the VC shell
    string(TOLOWER "${CMAKE_HOST_SYSTEM_PROCESSOR}" lowercase_CMAKE_HOST_SYSTEM_PROCESSOR)
    if(lowercase_CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL x86 OR lowercase_CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "^i[3456]86$")
        set(HOST_ARCH i386)
        set(VCVARSALL_ARCH x86)
    elseif(lowercase_CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL x86_64 OR lowercase_CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL amd64)
        set(HOST_ARCH amd64)
        set(HOST_ARCH_64BIT TRUE)
        set(VCVARSALL_ARCH amd64)
    elseif(lowercase_CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL arm)
        set(HOST_ARCH arm)
        set(VCVARSALL_ARCH arm)
    # 'aarch64' is used in GNU tools instead of 'arm64'
    elseif(lowercase_CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL arm64 OR lowercase_CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL aarch64)
        set(HOST_ARCH arm64)
        set(HOST_ARCH_64BIT TRUE)
        set(VCVARSALL_ARCH arm64)
    else()
        message(FATAL_ERROR "Unknown host architecture: ${lowercase_CMAKE_HOST_SYSTEM_PROCESSOR}")
    endif()
    
    if(ARCH STREQUAL amd64 OR ARCH STREQUAL arm64)
        set(TARGET_ARCH_64BIT TRUE)
    endif()

    if(NOT HOST_ARCH_64BIT STREQUAL TARGET_ARCH_64BIT)
         add_definitions(-DMISMATCHED_BITNESS)
    endif()

    if(ARCH STREQUAL HOST_ARCH)
        set(HOST_TOOLS_CMAKE_COMMAND "${CMAKE_COMMAND}")
        message("Not cross-compiling, no special host-tools cmake command")
    elseif(MSVC)
        message("Compiling on ${HOST_ARCH} for ${ARCH} (MSVC)")
        set(HOST_TOOLS_CMAKE_COMMAND "${REACTOS_BINARY_DIR}/host-tools/cmake_shim.cmd")
        if(MSVC_VERSION EQUAL 1900)
            file(WRITE ${HOST_TOOLS_CMAKE_COMMAND}
                "set VSCMD_SKIP_SENDTELEMETRY=1\n"
                "@call \"$ENV{VCINSTALLDIR}\\vcvarsall.bat\" ${VCVARSALL_ARCH}\n"
                "\"${CMAKE_COMMAND}\" %*"
            )
        elseif(MSVC_VERSION GREATER_EQUAL 1910)
            # 2017 and 2019 use the same folder structure
            file(WRITE ${HOST_TOOLS_CMAKE_COMMAND}
                "@set VSCMD_ARG_no_logo=1\n"
                "@call \"$ENV{VCINSTALLDIR}\\Auxiliary\\Build\\vcvarsall.bat\" /clean_env\n"
                "@call \"$ENV{VCINSTALLDIR}\\Auxiliary\\Build\\vcvarsall.bat\" ${VCVARSALL_ARCH}\n"
                "\"${CMAKE_COMMAND}\" %*"
            )
        else()
            message(FATAL "Unable to figure out vcvarsall path")
        endif()
    else()
        # CMAKE_HOST_SYSTEM_PROCESSOR is unreliable on non-Windows, so we need to do this to not break builds on Unix
        remove_definitions(-DMISMATCHED_BITNESS)
        set(HOST_ARCH ARCH)
        set(HOST_TOOLS_CMAKE_COMMAND "${CMAKE_COMMAND}")
        message("Cross-compiling on non-msvc, no special host-tools cmake command")
    endif()

    # CMake might choose clang if it finds it in the PATH. Always prefer cl for host tools
    if (MSVC)
        list(APPEND CMAKE_HOST_TOOLS_EXTRA_ARGS
            -DCMAKE_C_COMPILER=cl
            -DCMAKE_CXX_COMPILER=cl)
    endif()

    if (MSVC_IDE)
        # Required for Bison/Flex wrappers created by /CMakeLists.txt.
        list(APPEND CMAKE_HOST_TOOLS_EXTRA_ARGS
            -DROS_SAVED_BISON_PKGDATADIR=${ROS_SAVED_BISON_PKGDATADIR}
            -DROS_SAVED_M4=${ROS_SAVED_M4}
            )
    endif()

    ExternalProject_Add(host-tools
        SOURCE_DIR ${REACTOS_SOURCE_DIR}
        PREFIX ${REACTOS_BINARY_DIR}/host-tools
        BINARY_DIR ${REACTOS_BINARY_DIR}/host-tools/bin
        CMAKE_COMMAND ${HOST_TOOLS_CMAKE_COMMAND}
        CMAKE_ARGS
            -UCMAKE_TOOLCHAIN_FILE
            -DARCH:STRING=${HOST_ARCH}
            -DCMAKE_INSTALL_PREFIX=${REACTOS_BINARY_DIR}/host-tools
            -DTOOLS_FOLDER=${REACTOS_BINARY_DIR}/host-tools/bin
            -DTARGET_COMPILER_ID=${CMAKE_C_COMPILER_ID}
            ${CMAKE_HOST_TOOLS_EXTRA_ARGS}
        BUILD_ALWAYS TRUE
        INSTALL_COMMAND ${CMAKE_COMMAND} -E true
        BUILD_BYPRODUCTS ${HOST_TOOLS_OUTPUT}
    )

    ExternalProject_Get_Property(host-tools INSTALL_DIR)

    foreach(_tool ${HOST_TOOLS})
        add_executable(native-${_tool} IMPORTED)
        set_target_properties(native-${_tool} PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/bin/${HOST_EXTRA_DIR}${_tool}${HOST_EXE_SUFFIX})
        add_dependencies(native-${_tool} host-tools ${INSTALL_DIR}/bin/${HOST_EXTRA_DIR}${_tool}${HOST_EXE_SUFFIX})
    endforeach()

    foreach(_module ${HOST_MODULES})
        add_library(native-${_module} MODULE IMPORTED)
        set_target_properties(native-${_module} PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/bin/${HOST_EXTRA_DIR}${_module}${HOST_MODULE_SUFFIX})
        add_dependencies(native-${_module} host-tools ${INSTALL_DIR}/bin/${HOST_EXTRA_DIR}${_module}${HOST_MODULE_SUFFIX})
    endforeach()
endfunction()
