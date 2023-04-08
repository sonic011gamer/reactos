/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS system libraries
 * PURPOSE:         Vista functions
 * PROGRAMMER:      Thomas Weidenmueller <w3seek@reactos.com>
 */

/* INCLUDES *******************************************************************/

#include <k32_vista.h>


// This is defined only in ntifs.h
#define REPARSE_DATA_BUFFER_HEADER_SIZE   FIELD_OFFSET(REPARSE_DATA_BUFFER, GenericReparseBuffer)

#define NDEBUG
#include <debug.h>


/* PUBLIC FUNCTIONS ***********************************************************/

/*
 * @implemented
 */
BOOL
WINAPI
QueryFullProcessImageNameW(HANDLE hProcess,
                           DWORD dwFlags,
                           LPWSTR lpExeName,
                           PDWORD pdwSize)
{
    BYTE Buffer[sizeof(UNICODE_STRING) + MAX_PATH * sizeof(WCHAR)];
    UNICODE_STRING *DynamicBuffer = NULL;
    UNICODE_STRING *Result = NULL;
    NTSTATUS Status;
    DWORD Needed;

    Status = NtQueryInformationProcess(hProcess,
                                       ProcessImageFileName,
                                       Buffer,
                                       sizeof(Buffer) - sizeof(WCHAR),
                                       &Needed);
    if (Status == STATUS_INFO_LENGTH_MISMATCH)
    {
        DynamicBuffer = RtlAllocateHeap(RtlGetProcessHeap(), 0, Needed + sizeof(WCHAR));
        if (!DynamicBuffer)
        {
            BaseSetLastNTError(STATUS_NO_MEMORY);
            return FALSE;
        }

        Status = NtQueryInformationProcess(hProcess,
                                           ProcessImageFileName,
                                           (LPBYTE)DynamicBuffer,
                                           Needed,
                                           &Needed);
        Result = DynamicBuffer;
    }
    else Result = (PUNICODE_STRING)Buffer;

    if (!NT_SUCCESS(Status)) goto Cleanup;

    if (Result->Length / sizeof(WCHAR) + 1 > *pdwSize)
    {
        Status = STATUS_BUFFER_TOO_SMALL;
        goto Cleanup;
    }

    *pdwSize = Result->Length / sizeof(WCHAR);
    memcpy(lpExeName, Result->Buffer, Result->Length);
    lpExeName[*pdwSize] = 0;

Cleanup:
    RtlFreeHeap(RtlGetProcessHeap(), 0, DynamicBuffer);

    if (!NT_SUCCESS(Status))
    {
        BaseSetLastNTError(Status);
    }

    return !Status;
}


/*
 * @implemented
 */
BOOL
WINAPI
QueryFullProcessImageNameA(HANDLE hProcess,
                           DWORD dwFlags,
                           LPSTR lpExeName,
                           PDWORD pdwSize)
{
    DWORD pdwSizeW = *pdwSize;
    BOOL Result;
    LPWSTR lpExeNameW;

    lpExeNameW = RtlAllocateHeap(RtlGetProcessHeap(),
                                 HEAP_ZERO_MEMORY,
                                 *pdwSize * sizeof(WCHAR));
    if (!lpExeNameW)
    {
        BaseSetLastNTError(STATUS_NO_MEMORY);
        return FALSE;
    }

    Result = QueryFullProcessImageNameW(hProcess, dwFlags, lpExeNameW, &pdwSizeW);

    if (Result)
        Result = (0 != WideCharToMultiByte(CP_ACP, 0,
                                           lpExeNameW,
                                           -1,
                                           lpExeName,
                                           *pdwSize,
                                           NULL, NULL));

    if (Result)
        *pdwSize = strlen(lpExeName);

    RtlFreeHeap(RtlGetProcessHeap(), 0, lpExeNameW);
    return Result;
}


/*
 * @unimplemented
 */
HRESULT
WINAPI
GetApplicationRecoveryCallback(IN HANDLE hProcess,
                               OUT APPLICATION_RECOVERY_CALLBACK* pRecoveryCallback,
                               OUT PVOID* ppvParameter,
                               PDWORD dwPingInterval,
                               PDWORD dwFlags)
{
    UNIMPLEMENTED;
    return E_FAIL;
}


/*
 * @unimplemented
 */
HRESULT
WINAPI
GetApplicationRestart(IN HANDLE hProcess,
                      OUT PWSTR pwzCommandline  OPTIONAL,
                      IN OUT PDWORD pcchSize,
                      OUT PDWORD pdwFlags  OPTIONAL)
{
    UNIMPLEMENTED;
    return E_FAIL;
}


/*
 * @unimplemented
 */
VOID
WINAPI
ApplicationRecoveryFinished(IN BOOL bSuccess)
{
    UNIMPLEMENTED;
}


/*
 * @unimplemented
 */
HRESULT
WINAPI
ApplicationRecoveryInProgress(OUT PBOOL pbCancelled)
{
    UNIMPLEMENTED;
    return E_FAIL;
}


/*
 * @unimplemented
 */
HRESULT
WINAPI
RegisterApplicationRecoveryCallback(IN APPLICATION_RECOVERY_CALLBACK pRecoveryCallback,
                                    IN PVOID pvParameter  OPTIONAL,
                                    DWORD dwPingInterval,
                                    DWORD dwFlags)
{
    UNIMPLEMENTED;
    return E_FAIL;
}


/*
 * @unimplemented
 */
HRESULT
WINAPI
RegisterApplicationRestart(IN PCWSTR pwzCommandline  OPTIONAL,
                           IN DWORD dwFlags)
{
    UNIMPLEMENTED;
    return E_FAIL;
}


/*
 * @implemented
 */
BOOLEAN
WINAPI
CreateSymbolicLinkW(IN LPCWSTR lpSymlinkFileName,
                    IN LPCWSTR lpTargetFileName,
                    IN DWORD dwFlags)
{
    IO_STATUS_BLOCK IoStatusBlock;
    OBJECT_ATTRIBUTES ObjectAttributes;
    HANDLE hSymlink = NULL;
    UNICODE_STRING SymlinkFileName = { 0, 0, NULL };
    UNICODE_STRING TargetFileName = { 0, 0, NULL };
    BOOLEAN bAllocatedTarget = FALSE, bRelativePath = FALSE;
    LPWSTR lpTargetFullFileName = NULL;
    SIZE_T cbPrintName;
    SIZE_T cbReparseData;
    PREPARSE_DATA_BUFFER pReparseData = NULL;
    PBYTE pBufTail;
    NTSTATUS Status;
    ULONG dwCreateOptions;
    DWORD dwErr;

    if(!lpSymlinkFileName || !lpTargetFileName || (dwFlags | SYMBOLIC_LINK_FLAG_DIRECTORY) != SYMBOLIC_LINK_FLAG_DIRECTORY)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    if(dwFlags & SYMBOLIC_LINK_FLAG_DIRECTORY)
        dwCreateOptions = FILE_DIRECTORY_FILE;
    else
        dwCreateOptions = FILE_NON_DIRECTORY_FILE;

    switch(RtlDetermineDosPathNameType_U(lpTargetFileName))
    {
    case RtlPathTypeUnknown:
    case RtlPathTypeRooted:
    case RtlPathTypeRelative:
        bRelativePath = TRUE;
        RtlInitUnicodeString(&TargetFileName, lpTargetFileName);
        break;

    case RtlPathTypeDriveRelative:
        {
            LPWSTR FilePart;
            SIZE_T cchTargetFullFileName;

            cchTargetFullFileName = GetFullPathNameW(lpTargetFileName, 0, NULL, &FilePart);

            if(cchTargetFullFileName == 0)
            {
                dwErr = GetLastError();
                goto Cleanup;
            }

            lpTargetFullFileName = RtlAllocateHeap(RtlGetProcessHeap(), 0, cchTargetFullFileName * sizeof(WCHAR));

            if(lpTargetFullFileName == NULL)
            {
                dwErr = ERROR_NOT_ENOUGH_MEMORY;
                goto Cleanup;
            }

            if(GetFullPathNameW(lpTargetFileName, cchTargetFullFileName, lpTargetFullFileName, &FilePart) == 0)
            {
                dwErr = GetLastError();
                goto Cleanup;
            }
        }

        lpTargetFileName = lpTargetFullFileName;

        // fallthrough

    case RtlPathTypeUncAbsolute:
    case RtlPathTypeDriveAbsolute:
    case RtlPathTypeLocalDevice:
    case RtlPathTypeRootLocalDevice:
    default:
        if(!RtlDosPathNameToNtPathName_U(lpTargetFileName, &TargetFileName, NULL, NULL))
        {
            bAllocatedTarget = TRUE;
            dwErr = ERROR_INVALID_PARAMETER;
            goto Cleanup;
        }
    }

    cbPrintName = wcslen(lpTargetFileName) * sizeof(WCHAR);
    cbReparseData = FIELD_OFFSET(REPARSE_DATA_BUFFER, SymbolicLinkReparseBuffer.PathBuffer) + TargetFileName.Length + cbPrintName;
    pReparseData = RtlAllocateHeap(RtlGetProcessHeap(), 0, cbReparseData);

    if(pReparseData == NULL)
    {
        dwErr = ERROR_NOT_ENOUGH_MEMORY;
        goto Cleanup;
    }

    pBufTail = (PBYTE)(pReparseData->SymbolicLinkReparseBuffer.PathBuffer);

    pReparseData->ReparseTag = (ULONG)IO_REPARSE_TAG_SYMLINK;
    pReparseData->ReparseDataLength = (USHORT)cbReparseData - REPARSE_DATA_BUFFER_HEADER_SIZE;
    pReparseData->Reserved = 0;

    pReparseData->SymbolicLinkReparseBuffer.SubstituteNameOffset = 0;
    pReparseData->SymbolicLinkReparseBuffer.SubstituteNameLength = TargetFileName.Length;
    pBufTail += pReparseData->SymbolicLinkReparseBuffer.SubstituteNameOffset;
    RtlCopyMemory(pBufTail, TargetFileName.Buffer, TargetFileName.Length);

    pReparseData->SymbolicLinkReparseBuffer.PrintNameOffset = pReparseData->SymbolicLinkReparseBuffer.SubstituteNameLength;
    pReparseData->SymbolicLinkReparseBuffer.PrintNameLength = (USHORT)cbPrintName;
    pBufTail += pReparseData->SymbolicLinkReparseBuffer.PrintNameOffset;
    RtlCopyMemory(pBufTail, lpTargetFileName, cbPrintName);

    pReparseData->SymbolicLinkReparseBuffer.Flags = 0;

    if(bRelativePath)
        pReparseData->SymbolicLinkReparseBuffer.Flags |= 1; // TODO! give this lone flag a name

    if(!RtlDosPathNameToNtPathName_U(lpSymlinkFileName, &SymlinkFileName, NULL, NULL))
    {
        dwErr = ERROR_PATH_NOT_FOUND;
        goto Cleanup;
    }

    InitializeObjectAttributes(&ObjectAttributes, &SymlinkFileName, OBJ_CASE_INSENSITIVE, NULL, NULL);

    Status = NtCreateFile
    (
        &hSymlink,
        FILE_WRITE_ATTRIBUTES | DELETE | SYNCHRONIZE,
        &ObjectAttributes,
        &IoStatusBlock,
        NULL,
        FILE_ATTRIBUTE_NORMAL,
        0,
        FILE_CREATE,
        FILE_SYNCHRONOUS_IO_NONALERT | FILE_OPEN_REPARSE_POINT | dwCreateOptions,
        NULL,
        0
    );

    if(!NT_SUCCESS(Status))
    {
        dwErr = RtlNtStatusToDosError(Status);
        goto Cleanup;
    }

    Status = NtFsControlFile
    (
        hSymlink,
        NULL,
        NULL,
        NULL,
        &IoStatusBlock,
        FSCTL_SET_REPARSE_POINT,
        pReparseData,
        cbReparseData,
        NULL,
        0
    );

    if(!NT_SUCCESS(Status))
    {
        FILE_DISPOSITION_INFORMATION DispInfo;
        DispInfo.DeleteFile = TRUE;
        NtSetInformationFile(hSymlink, &IoStatusBlock, &DispInfo, sizeof(DispInfo), FileDispositionInformation);

        dwErr = RtlNtStatusToDosError(Status);
        goto Cleanup;
    }

    dwErr = NO_ERROR;

Cleanup:
    if(hSymlink)
        NtClose(hSymlink);

    RtlFreeUnicodeString(&SymlinkFileName);
    if (bAllocatedTarget)
    {
        RtlFreeHeap(RtlGetProcessHeap(),
                    0,
                    TargetFileName.Buffer);
    }

    if(lpTargetFullFileName)
        RtlFreeHeap(RtlGetProcessHeap(), 0, lpTargetFullFileName);

    if(pReparseData)
        RtlFreeHeap(RtlGetProcessHeap(), 0, pReparseData);

    if(dwErr)
    {
        SetLastError(dwErr);
        return FALSE;
    }

    return TRUE;
}


/*
 * @implemented
 */
BOOLEAN
NTAPI
CreateSymbolicLinkA(IN LPCSTR lpSymlinkFileName,
                    IN LPCSTR lpTargetFileName,
                    IN DWORD dwFlags)
{
    PWCHAR SymlinkW, TargetW;
    BOOLEAN Ret;

    if(!lpSymlinkFileName || !lpTargetFileName)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    if (!(SymlinkW = FilenameA2W(lpSymlinkFileName, FALSE)))
        return FALSE;

    if (!(TargetW = FilenameA2W(lpTargetFileName, TRUE)))
        return FALSE;

    Ret = CreateSymbolicLinkW(SymlinkW,
                              TargetW,
                              dwFlags);

    RtlFreeHeap(RtlGetProcessHeap(), 0, SymlinkW);
    RtlFreeHeap(RtlGetProcessHeap(), 0, TargetW);

    return Ret;
}


/*
 * @unimplemented
 */
DWORD
WINAPI
GetFinalPathNameByHandleW(IN HANDLE hFile,
                          OUT LPWSTR lpszFilePath,
                          IN DWORD cchFilePath,
                          IN DWORD dwFlags)
{
    if (dwFlags & ~(VOLUME_NAME_DOS | VOLUME_NAME_GUID | VOLUME_NAME_NT |
                    VOLUME_NAME_NONE | FILE_NAME_NORMALIZED | FILE_NAME_OPENED))
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return 0;
    }

    UNIMPLEMENTED;
    return 0;
}


/*
 * @implemented
 */
DWORD
WINAPI
GetFinalPathNameByHandleA(IN HANDLE hFile,
                          OUT LPSTR lpszFilePath,
                          IN DWORD cchFilePath,
                          IN DWORD dwFlags)
{
    WCHAR FilePathW[MAX_PATH];
    UNICODE_STRING FilePathU;
    DWORD PrevLastError;
    DWORD Ret = 0;

    if (cchFilePath != 0 &&
        cchFilePath > sizeof(FilePathW) / sizeof(FilePathW[0]))
    {
        FilePathU.Length = 0;
        FilePathU.MaximumLength = (USHORT)cchFilePath * sizeof(WCHAR);
        FilePathU.Buffer = RtlAllocateHeap(RtlGetProcessHeap(),
                                           0,
                                           FilePathU.MaximumLength);
        if (FilePathU.Buffer == NULL)
        {
            SetLastError(ERROR_NOT_ENOUGH_MEMORY);
            return 0;
        }
    }
    else
    {
        FilePathU.Length = 0;
        FilePathU.MaximumLength = sizeof(FilePathW);
        FilePathU.Buffer = FilePathW;
    }

    /* save the last error code */
    PrevLastError = GetLastError();
    SetLastError(ERROR_SUCCESS);

    /* call the unicode version that does all the work */
    Ret = GetFinalPathNameByHandleW(hFile,
                                    FilePathU.Buffer,
                                    cchFilePath,
                                    dwFlags);

    if (GetLastError() == ERROR_SUCCESS)
    {
        /* no error, restore the last error code and convert the string */
        SetLastError(PrevLastError);

        Ret = FilenameU2A_FitOrFail(lpszFilePath,
                                    cchFilePath,
                                    &FilePathU);
    }

    /* free allocated memory if necessary */
    if (FilePathU.Buffer != FilePathW)
    {
        RtlFreeHeap(RtlGetProcessHeap(),
                    0,
                    FilePathU.Buffer);
    }

    return Ret;
}


/*
 * @unimplemented
 */
BOOL
WINAPI
SetFileBandwidthReservation(IN HANDLE hFile,
                            IN DWORD nPeriodMilliseconds,
                            IN DWORD nBytesPerPeriod,
                            IN BOOL bDiscardable,
                            OUT LPDWORD lpTransferSize,
                            OUT LPDWORD lpNumOutstandingRequests)
{
    UNIMPLEMENTED;
    return FALSE;
}


/*
 * @unimplemented
 */
BOOL
WINAPI
GetFileBandwidthReservation(IN HANDLE hFile,
                            OUT LPDWORD lpPeriodMilliseconds,
                            OUT LPDWORD lpBytesPerPeriod,
                            OUT LPBOOL pDiscardable,
                            OUT LPDWORD lpTransferSize,
                            OUT LPDWORD lpNumOutstandingRequests)
{
    UNIMPLEMENTED;
    return FALSE;
}


/*
 * @unimplemented
 */
HANDLE
WINAPI
OpenFileById(IN HANDLE hFile,
             IN LPFILE_ID_DESCRIPTOR lpFileID,
             IN DWORD dwDesiredAccess,
             IN DWORD dwShareMode,
             IN LPSECURITY_ATTRIBUTES lpSecurityAttributes  OPTIONAL,
             IN DWORD dwFlags)
{
    UNIMPLEMENTED;
    return INVALID_HANDLE_VALUE;
}



/*
  Vista+ MUI support functions

  References:
   Evolution of MUI Support across Windows Versions: http://msdn.microsoft.com/en-US/library/ee264317.aspx
   Comparing Windows XP Professional Multilingual Options: http://technet.microsoft.com/en-us/library/bb457045.aspx

  More info:
   http://msdn.microsoft.com/en-us/goglobal/bb978454.aspx
   http://msdn.microsoft.com/en-us/library/dd319074.aspx
*/

/* FUNCTIONS *****************************************************************/

BOOL
WINAPI
GetFileMUIInfo(
    DWORD dwFlags,
    PCWSTR pcwszFilePath,
    PFILEMUIINFO pFileMUIInfo,
    DWORD *pcbFileMUIInfo)
{
    DPRINT1("%x %p %p %p\n", dwFlags, pcwszFilePath, pFileMUIInfo, pcbFileMUIInfo);
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
}

/*
 * @unimplemented
 */
BOOL
WINAPI
GetFileMUIPath(
    DWORD dwFlags,
    PCWSTR pcwszFilePath,
    PWSTR pwszLanguage,
    PULONG pcchLanguage,
    PWSTR pwszFileMUIPath,
    PULONG pcchFileMUIPath,
    PULONGLONG pululEnumerator)
{
    DPRINT1("%x %p %p %p %p %p\n", dwFlags, pcwszFilePath, pwszLanguage, pwszFileMUIPath, pcchFileMUIPath, pululEnumerator);
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
}

/*
 * @unimplemented
 */
#if 1 // This is Windows 7+
BOOL
WINAPI
GetProcessPreferredUILanguages(
    DWORD dwFlags,
    PULONG pulNumLanguages,
    PZZWSTR pwszLanguagesBuffer,
    PULONG pcchLanguagesBuffer)
{
    DPRINT1("%x %p %p %p\n", dwFlags, pulNumLanguages, pwszLanguagesBuffer, pcchLanguagesBuffer);
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
}
#endif

/*
* @unimplemented
*/
BOOL
WINAPI
GetSystemPreferredUILanguages(
    DWORD dwFlags,
    PULONG pulNumLanguages,
    PZZWSTR pwszLanguagesBuffer,
    PULONG pcchLanguagesBuffer)
{
    DPRINT1("%x %p %p %p\n", dwFlags, pulNumLanguages, pwszLanguagesBuffer, pcchLanguagesBuffer);
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
}

/*
 * @unimplemented
 */
BOOL
WINAPI
GetThreadPreferredUILanguages(
    DWORD dwFlags,
    PULONG pulNumLanguages,
    PZZWSTR pwszLanguagesBuffer,
    PULONG pcchLanguagesBuffer)
{
    DPRINT1("%x %p %p %p\n", dwFlags, pulNumLanguages, pwszLanguagesBuffer, pcchLanguagesBuffer);
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
}

/*
 * @unimplemented
 */
LANGID
WINAPI
GetThreadUILanguage(VOID)
{
    UNIMPLEMENTED;
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return 0;
}

/*
 * @unimplemented
 */
BOOL
WINAPI
GetUILanguageInfo(
    DWORD dwFlags,
    PCZZWSTR pwmszLanguage,
    PZZWSTR pwszFallbackLanguages,
    PDWORD pcchFallbackLanguages,
    PDWORD pdwAttributes)
{
    DPRINT1("%x %p %p %p %p\n", dwFlags, pwmszLanguage, pwszFallbackLanguages, pcchFallbackLanguages, pdwAttributes);
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
}


/*
 * @unimplemented
 */
BOOL
WINAPI
GetUserPreferredUILanguages(
    DWORD dwFlags,
    PULONG pulNumLanguages,
    PZZWSTR pwszLanguagesBuffer,
    PULONG pcchLanguagesBuffer)
{
    DPRINT1("%x %p %p %p\n", dwFlags, pulNumLanguages, pwszLanguagesBuffer, pcchLanguagesBuffer);
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
}

/*
 * @unimplemented
 */
#if 1 // Tis is Windows 7+
BOOL
WINAPI
SetProcessPreferredUILanguages(
    DWORD dwFlags,
    PCZZWSTR pwszLanguagesBuffer,
    PULONG pulNumLanguages)
{
    DPRINT1("%x %p %p\n", dwFlags, pwszLanguagesBuffer, pulNumLanguages);
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
}
#endif

/*
 * @unimplemented
 */
BOOL
WINAPI
SetThreadPreferredUILanguages(
    DWORD dwFlags,
    PCZZWSTR pwszLanguagesBuffer,
    PULONG pulNumLanguages
    )
{
    DPRINT1("%x %p %p\n", dwFlags, pwszLanguagesBuffer, pulNumLanguages);
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
}

/******************************************************************************
 *           ResolveLocaleName (KERNEL32.@)
 */

INT WINAPI ResolveLocaleName(LPCWSTR name, LPWSTR localename, INT len)
{
    //FIXME("stub: %s, %p, %d\n", wine_dbgstr_w(name), localename, len);

    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return 0;
}


#include <iotypes.h>

/* Quick and dirty table for conversion */
FILE_INFORMATION_CLASS ConvertToFileInfo[MaximumFileInfoByHandlesClass] =
{
    FileBasicInformation, FileStandardInformation, FileNameInformation, FileRenameInformation,
    FileDispositionInformation, FileAllocationInformation, FileEndOfFileInformation, FileStreamInformation,
    FileCompressionInformation, FileAttributeTagInformation, FileIdBothDirectoryInformation, (FILE_INFORMATION_CLASS)-1
};


/* Quick implementation, still going farther than Wine implementation */
BOOL
WINAPI
SetFileInformationByHandle(HANDLE hFile,
                           FILE_INFO_BY_HANDLE_CLASS FileInformationClass,
                           LPVOID lpFileInformation,
                           DWORD dwBufferSize)
{
    NTSTATUS Status;
    IO_STATUS_BLOCK IoStatusBlock;
    FILE_INFORMATION_CLASS FileInfoClass;

    FileInfoClass = (FILE_INFORMATION_CLASS)-1;

    /* Attempt to convert the class */
    if (FileInformationClass < MaximumFileInfoByHandlesClass)
    {
        FileInfoClass = ConvertToFileInfo[FileInformationClass];
    }

    /* If wrong, bail out */
    if (FileInfoClass == -1)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    /* And set the information */
    Status = NtSetInformationFile(hFile, &IoStatusBlock, lpFileInformation,
                                  dwBufferSize, FileInfoClass);

    if (!NT_SUCCESS(Status))
    {
        BaseSetLastNTError(Status);
        return FALSE;
    }

    return TRUE;
}


typedef struct _PERFORMANCE_INFORMATION {
	DWORD cb;
	SIZE_T CommitTotal;
	SIZE_T CommitLimit;
	SIZE_T CommitPeak;
	SIZE_T PhysicalTotal;
	SIZE_T PhysicalAvailable;
	SIZE_T SystemCache;
	SIZE_T KernelTotal;
	SIZE_T KernelPaged;
	SIZE_T KernelNonpaged;
	SIZE_T PageSize;
	DWORD HandleCount;
	DWORD ProcessCount;
	DWORD ThreadCount;
} PERFORMANCE_INFORMATION, *PPERFORMANCE_INFORMATION;


/***********************************************************************
 *         K32GetPerformanceInfo   (kernelbase.@)
 */
BOOL WINAPI K32GetPerformanceInfo( PPERFORMANCE_INFORMATION info, DWORD size )
{
    SYSTEM_PERFORMANCE_INFORMATION perf;
    SYSTEM_BASIC_INFORMATION basic;
    SYSTEM_PROCESS_INFORMATION *process, *spi;
    DWORD info_size;
    NTSTATUS status;

    DPRINT1( "(%p, %d)\n", info, size );

    if (size < sizeof(*info))
    {
        SetLastError( ERROR_BAD_LENGTH );
        return FALSE;
    }

    status = NtQuerySystemInformation( SystemPerformanceInformation, &perf, sizeof(perf), NULL );
    if (!NT_SUCCESS( status )) return FALSE;
    status = NtQuerySystemInformation( SystemBasicInformation, &basic, sizeof(basic), NULL );
    if (!NT_SUCCESS( status )) return FALSE;

    info->cb                 = sizeof(*info);
    info->CommitTotal        = perf.CommittedPages;
    info->CommitLimit        = perf.CommitLimit;
    info->CommitPeak         = perf.PeakCommitment;
    info->PhysicalTotal      = basic.NumberOfPhysicalPages;
    info->PhysicalAvailable  = perf.AvailablePages;
    info->SystemCache        = 0;
    info->KernelTotal        = perf.PagedPoolPages + perf.NonPagedPoolPages;
    info->KernelPaged        = perf.PagedPoolPages;
    info->KernelNonpaged     = perf.NonPagedPoolPages;
    info->PageSize           = basic.PageSize;

    /* fields from SYSTEM_PROCESS_INFORMATION */
    NtQuerySystemInformation( SystemProcessInformation, NULL, 0, &info_size );
    for (;;)
    {
        process = HeapAlloc( GetProcessHeap(), 0, info_size );
        if (!process)
        {
            SetLastError( ERROR_OUTOFMEMORY );
            return FALSE;
        }
        status = NtQuerySystemInformation( SystemProcessInformation, process, info_size, &info_size );
        if (!status) break;
        HeapFree( GetProcessHeap(), 0, process );
        if (status != STATUS_INFO_LENGTH_MISMATCH)
        {
            SetLastError( RtlNtStatusToDosError( status ) );
            return FALSE;
        }
    }
    info->HandleCount = info->ProcessCount = info->ThreadCount = 0;
    spi = process;
    for (;;)
    {
        info->ProcessCount++;
        info->HandleCount += spi->HandleCount;
        info->ThreadCount += spi->NumberOfThreads;
        if (spi->NextEntryOffset == 0) break;
        spi = (SYSTEM_PROCESS_INFORMATION *)((char *)spi + spi->NextEntryOffset);
    }
    HeapFree( GetProcessHeap(), 0, process );
    return TRUE;
}


DWORD
WINAPI
K32GetMappedFileNameW( HANDLE process, void *addr, WCHAR *name, DWORD size )
{
    ULONG_PTR buffer[(sizeof(MEMORY_SECTION_NAME) + MAX_PATH * sizeof(WCHAR)) / sizeof(ULONG_PTR)];
    MEMORY_SECTION_NAME *mem = (MEMORY_SECTION_NAME *)buffer;
    DWORD len;

    if (size && !name)
    {
        SetLastError( ERROR_INVALID_PARAMETER );
        return 0;
    }
   NtQueryVirtualMemory( process, addr, 0,
                                             mem, sizeof(buffer), NULL );

    len = mem->SectionFileName.Length / sizeof(WCHAR);
    memcpy( name, mem->SectionFileName.Buffer, min( mem->SectionFileName.Length, size * sizeof(WCHAR) ));
    if (len >= size) SetLastError( ERROR_INSUFFICIENT_BUFFER );
    name[min(len, size - 1)] = 0;
    return len;
}
