#include "uxthemep.h"
#include <ole2.h>
#include <objidl.h>
#include <gdiplus.h>
#include <gdipluscolor.h>

//#define DEBUG_MSG

BOOL MSSTYLES_LoadPng(HINSTANCE hTheme, LPCWSTR szFile, OUT HBITMAP *pngBitmap)
{
#ifdef DEBUG_MSG
    HWND msgRoot = nullptr;
    wchar_t *msg;
#endif
    DWORD msgId = S_OK;
    HRSRC pngRes;
    DWORD pngSize;
    const void* pngData;
    HGLOBAL pngAlloc;
    void* pngLock;
    IStream* pngStream = NULL;
    Gdiplus::Bitmap* gdipBitmap;

    pngRes = FindResourceW(hTheme, szFile, TEXT(L"IMAGE"));
#ifdef DEBUG_MSG
    if (!pngRes)
    {
        msgId = GetLastError();
        goto msgBoxGetPng;
    }
#endif

    pngSize = SizeofResource(hTheme, pngRes);
#ifdef DEBUG_MSG
    if (!pngSize)
    {
        msgId = GetLastError();
        goto msgBoxGetPng;
    }
#endif

    pngData = LockResource(LoadResource(hTheme, pngRes));
#ifdef DEBUG_MSG
    if (!pngData)
    {
        msgId = GetLastError();
        goto msgBoxGetPng;
    }
#endif
    pngAlloc = GlobalAlloc(GMEM_MOVEABLE, pngSize);
    if (pngAlloc)
    {
        pngLock = GlobalLock(pngAlloc);
        CopyMemory(pngLock, pngData, pngSize);
        if (CreateStreamOnHGlobal(pngAlloc, FALSE, &pngStream) == S_OK)
        {
            gdipBitmap = Gdiplus::Bitmap::FromStream(pngStream, FALSE);
            pngStream->Release();
#ifdef DEBUG_MSG
            if (!gdipBitmap)
            {
                msgId = GetLastError();
                goto msgBoxGetPng;
            }
            msgId = gdipBitmap->GetLastStatus();
            if (msgId == Gdiplus::Ok)
            {
#endif
                msgId = gdipBitmap->GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), pngBitmap);

                if (msgId == Gdiplus::Ok)
                {
                    msgId = S_OK;
                    return TRUE;
                }
#ifdef DEBUG_MSG
                else
                    goto msgBoxGetPng;
            }
            else
                goto msgBoxGetPng;
#endif
        }
    }
#ifdef DEBUG_MSG
goto exitGetPng;

msgBoxGetPng:
    if (msgId == S_OK)
        goto exitGetPng;
    if (msgId == ERROR_SUCCESS)
        goto exitGetPng;

    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        msgId,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&msg,
        0,
        NULL
    );

    MessageBoxW(
        msgRoot
        , msg
        , szFile
        , MB_OK
    );

exitGetPng:
#endif
    return FALSE;
}
