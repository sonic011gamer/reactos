
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys/posix)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/ndk)

add_definitions(-D_PSXDLL_)
spec2def(psxdll.dll psxdll.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    #dirent/closedir.c
    #dirent/opendir.c
    #dirent/readdir.c
    #dlfcn/dlclose.c
    #dlfcn/dlerror.c
    #dlfcn/dlopen.c
    #dlfcn/dlsym.c
    #errno/errno.c
    #fcntl/fcntl.c
    #fcntl/open.c
    #libgen/basename.c
    #misc/fdtable.c
    ##misc/heap.c
    #misc/init.c
    #misc/interlock.c
    #misc/main.c
    #misc/path.c
    #misc/pdata.c
    #misc/safeobj.c
    #misc/spawn.c
    #misc/template.c
    #misc/tls.c
    #pthread/create.c
    #pthread/exit.c
    #pthread/join.c
    #pthread/kill.c
    #pthread/mutex.c
    #pthread/self.c
    #sched/yield.c
    #signal/raise.c
    #stdlib/abort.c
    #stdlib/exit.c
    #stdlib/malloc.c
    #string/strcoll.c
    #string/strdup.c
    #string/strerror.c
    #sys/stat/chmod.c
    #sys/stat/mkdir.c
    #sys/stat/mkfifo.c
    #sys/stat/mknod.c
    #sys/stat/stat.c
    #sys/stat/umask.c
    #sys/utsname/uname.c
    #unistd/access.c
    #unistd/close.c
    #unistd/dup.c
    #unistd/fork.c
    #unistd/getcwd.c
    #unistd/getpid.c
    #unistd/getppid.c
    #unistd/read.c
    unistd/sleep.c
    #unistd/write.c
    stubs.c
    )

add_library(psxdll MODULE
    ${SOURCE}
    psxdll.rc
    ${CMAKE_CURRENT_BINARY_DIR}/psxdll.def
	${CMAKE_CURRENT_BINARY_DIR}/psxdll_stubs.c)

set_module_type(psxdll win32dll ENTRYPOINT DllMain)
add_importlibs(psxdll ntdll msvcrt psxsrv)
add_dependencies(psxdll psdk)
add_cd_file(TARGET psxdll DESTINATION reactos/system32 FOR all)