#pragma once
#define PATHCCH_NONE                            0x00
#define PATHCCH_ALLOW_LONG_PATHS                0x01
#define PATHCCH_FORCE_ENABLE_LONG_NAME_PROCESS  0x02
#define PATHCCH_FORCE_DISABLE_LONG_NAME_PROCESS 0x04
#define PATHCCH_DO_NOT_NORMALIZE_SEGMENTS       0x08
#define PATHCCH_ENSURE_IS_EXTENDED_LENGTH_PATH  0x10
#define PATHCCH_ENSURE_TRAILING_SLASH           0x20

#define PATHCCH_MAX_CCH 0x8000

HRESULT WINAPI PathCchStripPrefix(WCHAR *path, SIZE_T size);
HRESULT WINAPI PathAllocCanonicalize(const WCHAR *path_in, DWORD flags, WCHAR **path_out);
HRESULT WINAPI PathCchAddBackslashEx(WCHAR *path, SIZE_T size, WCHAR **endptr, SIZE_T *remaining);
//HRESULT WINAPI PathAllocCanonicalize(const WCHAR *path_in, DWORD flags, WCHAR **path_out);
//HRESULT WINAPI PathAllocCombine(const WCHAR *path1, const WCHAR *path2, DWORD flags, WCHAR **out);
//HRESULT WINAPI PathCchAddBackslash(WCHAR *path, SIZE_T size);
//HRESULT WINAPI PathCchAddBackslashEx(WCHAR *path, SIZE_T size, WCHAR **end, SIZE_T *remaining);
//HRESULT WINAPI PathCchAddExtension(WCHAR *path, SIZE_T size, const WCHAR *extension);
//HRESULT WINAPI PathCchAppend(WCHAR *path1, SIZE_T size, const WCHAR *path2);
//HRESULT WINAPI PathCchAppendEx(WCHAR *path1, SIZE_T size, const WCHAR *path2, DWORD flags);
//HRESULT WINAPI PathCchCanonicalize(WCHAR *out, SIZE_T size, const WCHAR *in);
//HRESULT WINAPI PathCchCanonicalizeEx(WCHAR *out, SIZE_T size, const WCHAR *in, DWORD flags);
//HRESULT WINAPI PathCchCombine(WCHAR *out, SIZE_T size, const WCHAR *path1, const WCHAR *path2);
//HRESULT WINAPI PathCchCombineEx(WCHAR *out, SIZE_T size, const WCHAR *path1, const WCHAR *path2, DWORD flags);
//HRESULT WINAPI PathCchFindExtension(const WCHAR *path, SIZE_T size, const WCHAR **extension);
//BOOL    WINAPI PathCchIsRoot(const WCHAR *path);
//HRESULT WINAPI PathCchRemoveBackslash(WCHAR *path, SIZE_T path_size);
//HRESULT WINAPI PathCchRemoveBackslashEx(WCHAR *path, SIZE_T path_size, WCHAR **path_end, SIZE_T *free_size);
//HRESULT WINAPI PathCchRemoveExtension(WCHAR *path, SIZE_T size);
//HRESULT WINAPI PathCchRemoveFileSpec(WCHAR *path, SIZE_T size);
//HRESULT WINAPI PathCchRenameExtension(WCHAR *path, SIZE_T size, const WCHAR *extension);
//HRESULT WINAPI PathCchSkipRoot(const WCHAR *path, const WCHAR **root_end);
//HRESULT WINAPI PathCchStripPrefix(WCHAR *path, SIZE_T size);
//HRESULT WINAPI PathCchStripToRoot(WCHAR *path, SIZE_T size);
//BOOL    WINAPI PathIsUNCEx(const WCHAR *path, const WCHAR **server);
