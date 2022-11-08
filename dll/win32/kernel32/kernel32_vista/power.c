#include "k32_vista.h"

#define NDEBUG
#include <debug.h>


typedef struct _REASON_CONTEXT {
    ULONG Version;
    DWORD Flags;
    union {
        struct {
            HMODULE LocalizedReasonModule;
            ULONG LocalizedReasonId;
            ULONG ReasonStringCount;
            LPWSTR *ReasonStrings;

        } Detailed;

        LPWSTR SimpleReasonString;
    } Reason;
} REASON_CONTEXT, *PREASON_CONTEXT;

/***********************************************************************
 *           PowerCreateRequest      (KERNEL32.@)
 */
HANDLE WINAPI PowerCreateRequest(REASON_CONTEXT *context)
{
    //FIXME("(%p): stub\n", context);

    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return INVALID_HANDLE_VALUE;
}

typedef enum _POWER_REQUEST_TYPE
{
    PowerRequestDisplayRequired,
    PowerRequestSystemRequired,
    PowerRequestAwayModeRequired
} POWER_REQUEST_TYPE, *PPOWER_REQUEST_TYPE;

/***********************************************************************
 *           PowerSetRequest      (KERNEL32.@)
 */
BOOL WINAPI PowerSetRequest(HANDLE request, POWER_REQUEST_TYPE type)
{
    //FIXME("(%p, %u): stub\n", request, type);

    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
}

/***********************************************************************
 *           PowerClearRequest      (KERNEL32.@)
 */
BOOL WINAPI PowerClearRequest(HANDLE request, POWER_REQUEST_TYPE type)
{
    //FIXME("(%p, %u): stub\n", request, type);

    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
}