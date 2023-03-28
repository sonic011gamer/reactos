#include "KB.h"
#include <debug.h>


HRESULT WINAPI WerAddExcludedApplication(PCWSTR str, BOOL bool)
{
    return STATUS_SUCCESS;
}

HRESULT WINAPI WerRegisterFile(PCWSTR file, WER_REGISTER_FILE_TYPE regfiletype, DWORD flags)
{
    return STATUS_SUCCESS;
}

HRESULT WINAPI WerRemoveExcludedApplication(PCWSTR str, BOOL bool)
{
    return STATUS_SUCCESS;
}
HRESULT WINAPI WerReportCloseHandle(HREPORT report)
{
    return STATUS_SUCCESS;
}

HRESULT WINAPI WerReportCreate(PCWSTR str , WER_REPORT_TYPE repType, PWER_REPORT_INFORMATION pReportInformation, HREPORT *phReportHandle)
{
    return STATUS_SUCCESS;
}
HRESULT WINAPI WerReportSetParameter(HREPORT report, DWORD dword, PCWSTR str, PCWSTR strtwo)
{
    return STATUS_SUCCESS;
}
HRESULT WINAPI WerReportSubmit(HREPORT report, WER_CONSENT weconst, DWORD dword, PWER_SUBMIT_RESULT result)
{
    return STATUS_SUCCESS;
}

HRESULT WerReportAddDump(
    HREPORT                    hReportHandle,
     HANDLE                     hProcess,
  HANDLE                     hThread,
   WER_DUMP_TYPE              dumpType,
  PWER_EXCEPTION_INFORMATION pExceptionParam,
   PWER_DUMP_CUSTOM_OPTIONS   pDumpCustomOptions,
   DWORD                      dwFlags
)
{
    return STATUS_SUCCESS;
}


HRESULT WerReportAddFile(
  HREPORT       hReportHandle,
  PCWSTR        pwzPath,
  WER_FILE_TYPE repFileType,
  DWORD         dwFileFlags
)
{
    return STATUS_SUCCESS;
}