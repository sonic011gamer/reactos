/* We are a core NT DLL, we don't import syscalls */
#define _INC_SWPRINTF_INL_
#undef __MSVCRT__

/* C Headers */
#include <stdio.h>
#include <stdlib.h>

/* PSDK/NDK Headers */
#define WIN32_NO_STATUS
#define _INC_WINDOWS
#define COM_NO_WINDOWS_H
#define COBJMACROS
#define CONST_VTABLE
#include <windef.h>
#include <winbase.h>
#include <winreg.h>
#include <objbase.h>
#include <ntintsafe.h>
#include <ndk/exfuncs.h>
#include <ndk/iofuncs.h>
#include <ndk/kefuncs.h>
#include <ndk/ldrfuncs.h>
#include <ndk/mmfuncs.h>
#include <ndk/obfuncs.h>
#include <ndk/psfuncs.h>
#include <ndk/rtlfuncs.h>
#include <ndk/setypes.h>
#include <ndk/sefuncs.h>
#include <ndk/umfuncs.h>


#define NDEBUG
#include <debug.h>

/**
 * @brief
 * Removes all privileges in the specified access token.
 *
 * @param[in] TokenHandle
 * A handle to the access token that contains the privileges to be removed.
 *
 * @param[in] PrivilegesToKeep
 * A pointer to an array of privilege values (defined as SE_XXX_PRIVILEGE) that specify
 * the privileges to keep in the token.
 *
 * @param[in] PrivilegeCount
 * Specifies the number of entries in the PrivilegesToKeep array.
 *
 * @return
 * Returns STATUS_SUCCESS if privileges removed successfully.
 * STATUS_INVALID_PARAMETER is returned if input privilege value greater than
 * SE_MAX_WELL_KNOWN_PRIVILEGE. STATUS_NOT_ALL_ASSIGNED is returned if The token does
 * not have one or more of the privileges specified in the PrivilegesToKeep parameter,
 * and no privileges were removed. A failure NTSTATUS code is returned otherwise.
 */
NTSTATUS
NTAPI
RtlRemovePrivileges(
    _In_ HANDLE TokenHandle,
    _In_reads_opt_(PrivilegeCount) _When_(PrivilegeCount != 0, _Notnull_)
         PULONG PrivilegesToKeep,
    _In_ ULONG PrivilegeCount)
{
    NTSTATUS Status;
    UINT64 PrivilegesToKeepBitmap;
    ULONG i, ReturnLength;
    UCHAR Buffer[sizeof(TOKEN_PRIVILEGES) +
                 sizeof(LUID_AND_ATTRIBUTES) * (SE_MAX_WELL_KNOWN_PRIVILEGE - SE_MIN_WELL_KNOWN_PRIVILEGE)];
    PTOKEN_PRIVILEGES Privileges;

    C_ASSERT(SE_MAX_WELL_KNOWN_PRIVILEGE < 64);

    DPRINT("RtlRemovePrivileges(%p, %p, %u)\n", TokenHandle, PrivilegesToKeep, PrivilegeCount);

    /* Save privileges that should be keep */
    PrivilegesToKeepBitmap = 0;
    if (PrivilegeCount)
    {
        for (i = 0; i < PrivilegeCount; i++)
        {
            if (PrivilegesToKeep[i] > SE_MAX_WELL_KNOWN_PRIVILEGE)
            {
                return STATUS_INVALID_PARAMETER;
            }
            PrivilegesToKeepBitmap |= (1ULL << PrivilegesToKeep[i]);
        }
    }

    /* Get token privileges information */
    Status = ZwQueryInformationToken(TokenHandle,
                                     TokenPrivileges,
                                     Buffer,
                                     sizeof(Buffer),
                                     &ReturnLength);
    if (!NT_SUCCESS(Status))
    {
        return Status;
    }

    /* Remove all privileges that we don't need to keep */
    Privileges = (PTOKEN_PRIVILEGES)Buffer;
    for (i = 0; i < Privileges->PrivilegeCount; i++)
    {
        LARGE_INTEGER Privilege = *(LARGE_INTEGER*)&Privileges->Privileges[i].Luid;
        ASSERT(Privilege.QuadPart <= SE_MAX_WELL_KNOWN_PRIVILEGE);
        if (PrivilegesToKeepBitmap & (1ULL << Privilege.QuadPart))
        {
            PrivilegesToKeepBitmap &= ~(1ULL << Privilege.QuadPart);
        }
        else
        {
            Privileges->Privileges[i].Attributes = SE_PRIVILEGE_REMOVED;
        }
    }

    if (PrivilegesToKeepBitmap)
    {
        Status = STATUS_NOT_ALL_ASSIGNED;
    }
    else
    {
        Status = ZwAdjustPrivilegesToken(TokenHandle,
                                         FALSE,
                                         (PTOKEN_PRIVILEGES)Buffer,
                                         sizeof(Buffer),
                                         NULL,
                                         NULL);
    }

    return Status;
}


/***********************************************************************
 *           RtlGetProductInfo    (NTDLL.@)
 *
 * Gives info about the current Windows product type, in a format compatible
 * with the given Windows version
 *
 * Returns TRUE if the input is valid, FALSE otherwise
 */
BOOLEAN WINAPI RtlGetProductInfo(DWORD dwOSMajorVersion, DWORD dwOSMinorVersion, DWORD dwSpMajorVersion,
                                 DWORD dwSpMinorVersion, PDWORD pdwReturnedProductType)
{
    //TRACE("(%d, %d, %d, %d, %p)\n", dwOSMajorVersion, dwOSMinorVersion,
    //      dwSpMajorVersion, dwSpMinorVersion, pdwReturnedProductType);

    if (!pdwReturnedProductType)
        return FALSE;

    if (dwOSMajorVersion < 6)
    {
        *pdwReturnedProductType = PRODUCT_UNDEFINED;
        return FALSE;
    }

    //if (current_version->wProductType == VER_NT_WORKSTATION)
        *pdwReturnedProductType = PRODUCT_ULTIMATE_N;
    //else
    //    *pdwReturnedProductType = PRODUCT_STANDARD_SERVER;

    return TRUE;
}
