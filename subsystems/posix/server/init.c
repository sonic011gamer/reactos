#include "include/psxsrv.h"
#include "../psx.h"
//#define NDEBUG
#include <debug.h>

SERVER Server =
{
     /* .Heap */
    INVALID_HANDLE_VALUE,
    /* .Directory */
    {
        {   INVALID_HANDLE_VALUE,
            L"\\"PSX_NS_SUBSYSTEM_DIRECTORY_NAME,
            {0,0,NULL}
        },
        {   INVALID_HANDLE_VALUE,
            L"\\"PSX_NS_SUBSYSTEM_DIRECTORY_NAME"\\"PSX_NS_SESSION_DIRECTORY_NAME,
            {0,0,NULL}
        },
        {   INVALID_HANDLE_VALUE,
            L"\\"PSX_NS_SUBSYSTEM_DIRECTORY_NAME"\\"PSX_NS_SYSTEM_DIRECTORY_NAME,
            {0,0,NULL}
        },
    },
    /* .Port */
    {
        {   INVALID_HANDLE_VALUE,
            L"\\"PSX_NS_SUBSYSTEM_DIRECTORY_NAME"\\"PSX_NS_API_PORT_NAME,
            {0,0,NULL},
            ApiPortListener
        },
        {   INVALID_HANDLE_VALUE,
            L"\\"PSX_NS_SUBSYSTEM_DIRECTORY_NAME"\\"PSX_NS_SBAPI_PORT_NAME,
            {0,0,NULL},
            SbApiPortListener
        },
        {   INVALID_HANDLE_VALUE,
            L"\\"PSX_NS_SUBSYSTEM_DIRECTORY_NAME"\\"PSX_NS_SESSIONAPI_PORT_NAME,
            {0,0,NULL},
            SessionPortListener
        }
    }
};

NTSTATUS NTAPI
PsxServerInitialization (
    IN ULONG ArgumentCount,
    IN PWSTR *ArgumentArray
    )
{
    DPRINT1("PsxServerInitialization: Attempting to initialize 
            the POSIX+ Subsystem");
    NTSTATUS Status;
	Status = 0;

    /* Done */
    return Status;
}
