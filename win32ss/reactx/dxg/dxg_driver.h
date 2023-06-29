#include <dxg_int.h>

#define TRACE() \
    DbgPrint("DXG: %s\n", __FUNCTION__)

DWORD
NTAPI
DxD3dContextCreate(
    PVOID p1,
    PVOID p2,
    PVOID p3,
    PVOID p4)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxD3dContextDestroy(
    PVOID p1)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxD3dContextDestroyAll(
    PVOID p1)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxD3dValidateTextureStageState(
    PVOID p1)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxD3dDrawPrimitives2(
    PVOID p1,
    PVOID p2,
    PVOID p3,
    PVOID p4,
    PVOID p5,
    PVOID p6,
    PVOID p7)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdGetDriverState(
    PVOID p1)
{
    TRACE();
    return 0;
}

BOOL
NTAPI
DxDdAddAttachedSurface(
    HANDLE hSurface1,
    HANDLE hSurface2,
    PEDD_DIRECTDRAW_GLOBAL* ppeDdGl)
{
    BOOL bSUCCESS = FALSE;

    PEDD_SURFACE LockedSurface1 = NULL;
    PEDD_SURFACE LockedSurface2 = NULL;

    // We have to flip through the PEDD_DIRECTDRAW_GLOBAL pointers here

    LockedSurface1 = (PEDD_SURFACE)DdHmgLock(hSurface1, ObjType_DDSURFACE_TYPE, FALSE);
    LockedSurface2 = (PEDD_SURFACE)DdHmgLock(hSurface2, ObjType_DDSURFACE_TYPE, FALSE);

    if(LockedSurface1 == NULL || LockedSurface2 == NULL ||
        (LockedSurface1->pobj.BaseFlags & 0x800) != 0 ||
        (LockedSurface2->pobj.BaseFlags & 0x800) != 0 ||
        LockedSurface1->ddsSurfaceLocal.dwFlags != LockedSurface2->ddsSurfaceLocal.dwFlags)
    {
        DbgPrint("DxDdAddAttachedSurface: Invalid surface or dwFlags\n");
    }
    else
    {

    }

    return bSUCCESS;
}

DWORD
NTAPI
DxDdAlphaBlt(
    PVOID p1,
    PVOID p2,
    PVOID p3)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdAttachSurface(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdBeginMoCompFrame(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdBlt(
    PVOID p1,
    PVOID p2,
    PVOID p3)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdColorControl(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdCreateMoComp(
    PVOID p1,
    PVOID p2)
{
    return 0;
}

DWORD
NTAPI
DxDdDeleteDirectDrawObject(
    PVOID p1)
{
    return 0;
}

DWORD
NTAPI
DxDdDeleteSurfaceObject(
    PVOID p1)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdDestroyMoComp(
    PVOID p1,
    PVOID p2)
{
    return 0;
}

DWORD
NTAPI
DxDdDestroySurface(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdDestroyD3DBuffer(
    PVOID p1)
{
    return 0;
}

DWORD
NTAPI
DxDdEndMoCompFrame(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdFlip(
    PVOID p1,
    PVOID p2,
    PVOID p3,
    PVOID p4,
    PVOID p5)
{
    return 0;
}

DWORD
NTAPI
DxDdFlipToGDISurface(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdGetAvailDriverMemory(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdGetBltStatus(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdGetDC(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdGetDxHandle(
    PVOID p1,
    PVOID p2,
    PVOID p3)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdGetFlipStatus(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdGetInternalMoCompInfo(
    PVOID p1,
    PVOID p2)
{
    return 0;
}

DWORD
NTAPI
DxDdGetMoCompBuffInfo(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdGetMoCompGuids(
    PVOID p1,
    PVOID p2)
{
    return 0;
}

DWORD
NTAPI
DxDdGetMoCompFormats(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdGetScanLine(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdLockD3D(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdQueryMoCompStatus(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdReleaseDC(
    PVOID p1)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdRenderMoComp(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdResetVisrgn(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdSetColorKey(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdSetExclusiveMode(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdSetGammaRamp(
    PVOID p1,
    PVOID p2,
    PVOID p3)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdCreateSurfaceEx(
    PVOID p1,
    PVOID p2,
    PVOID p3)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdSetOverlayPosition(
    PVOID p1,
    PVOID p2,
    PVOID p3)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdUnattachSurface(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdUnlockD3D(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdUpdateOverlay(
    PVOID p1,
    PVOID p2,
    PVOID p3)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdWaitForVerticalBlank(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDvpCanCreateVideoPort(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDvpColorControl(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDvpCreateVideoPort(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDvpDestroyVideoPort(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDvpFlipVideoPort(
    PVOID p1,
    PVOID p2,
    PVOID p3,
    PVOID p4)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDvpGetVideoPortBandwidth(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDvpGetVideoPortField(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDvpGetVideoPortFlipStatus(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDvpGetVideoPortInputFormats(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDvpGetVideoPortLine(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDvpGetVideoPortOutputFormats(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDvpGetVideoPortConnectInfo(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDvpGetVideoSignalStatus(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDvpUpdateVideoPort(
    PVOID p1,
    PVOID p2,
    PVOID p3,
    PVOID p4)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDvpWaitForVideoPortSync(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDvpAcquireNotification(
    PVOID p1,
    PVOID p2,
    PVOID p3)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDvpReleaseNotification(
    PVOID p1,
    PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdHeapVidMemAllocAligned(
    PVOID p1,
    PVOID p2,
    PVOID p3,
    PVOID p4,
    PVOID p5)
{
    TRACE();
    return 0;
}

VOID
NTAPI
insertIntoList(LPVMEML Block, LPVMEML *List)
{
/*  LPVMEML *ListStart;
    LPVMEML *NextItem;
    LPVMEML *PrevItem;
    LPVMEML *pp_Var4;

    ListStart = (LPVMEML *)*List;

    if (ListStart != NULL)
    {
        PrevItem = ListStart;
        pp_Var4 = NULL;

        do
        {
            NextItem = PrevItem;
            PrevItem = NextItem;

            if (Block-> < NextItem[2])
                break;

            PrevItem = (LPVMEML *)*NextItem;
            pp_Var4 = NextItem;
        } while (PrevItem != NULL);

        if (pp_Var4 != NULL)
        {
            *(LPVMEML **)Block = PrevItem;
            *pp_Var4 = Block;
            return;
        }
    }

    *(LPVMEML **)Block = ListStart;
    *List = Block;

    return;*/
}

LPVMEML
NTAPI
coalesceFreeBlocks(LPVMEMHEAP Heap, LPVMEML Block)
{
    int EndBlock = Block->ptr + Block->size;
    LPVMEML* FreeList = (LPVMEML*)Heap->freeList;
    LPVMEML v5 = *FreeList;
    Block->next = NULL;
    int v6;
    int v7;
    LPVMEML result;
    LPVMEML* v9 = FreeList;
    LPVMEML pContext0 = NULL;

    if(v5)
    {
        while(TRUE)
        {
            v6 = v5->size;
            v7 = Block->ptr;

            if(v7 == v6 + v5->ptr)
                break;

            if(EndBlock == v5->ptr)
            {
                v5->ptr = v7;
                v5->ptr = v6 + Block->ptr;
                goto Done;
            }

            pContext0 = v5;
            v5 = *(LPVMEML*)v5;

            if(!v5)
            {
                FreeList = v9;
                goto InsertIntoList;
            }
        }

        v5->size += Block->size;
Done:
        if(pContext0)
            *pContext0 = *v5;
        else
            *v9 = *(LPVMEML*)v5;
        
        EngFreeMem(Block);
        result = v5;
    }
    else
    {
InsertIntoList:
        insertIntoList(Block, FreeList);
        result = NULL;
    }

    return result;
}

VOID 
NTAPI
linFreeVidMem(LPVMEMHEAP Heap, int Ptr)
{
    LPVMEML Current;
    LPVMEML Prev;

    if (Heap && Ptr)
    {
        Current = *(LPVMEML *)(Heap->allocList);
        Prev = NULL;

        while (Current)
        {
            // If this is the pointer we are looking for
            if (Current->ptr == Ptr)
            {
                if (Prev)
                    // Copy Current to Prev
                    *Prev = *Current;
                else
                    // Turn Current into VMEML** then store it as a void pointer
                    Heap->allocList = (LPVOID)&Current;
                do
                    // Free all blocks in this line
                    Current = coalesceFreeBlocks(Heap, Current);
                while (Current);
                return;
            }

            // Otherwise we set current to previous and move along
            Prev = Current;
            Current = *(LPVMEML *)Current;
        }
    }
}

VOID
NTAPI
rectFreeVidMem(int Heap, FLATPTR Ptr)
{

}

VOID
NTAPI
DxDdHeapVidMemFree(LPVMEMHEAP Heap, FLATPTR Ptr)
{
    if ((Heap->dwFlags & VMEMHEAP_LINEAR) == 0)
    {
        // Free Rectangular Memory
        rectFreeVidMem((int)Heap, Ptr);
    }
    else
    {
        // Free Linear Memory
        linFreeVidMem(Heap, (int)Ptr);
    }
}

VOID
NTAPI
vDdUnloadDxApiImage(PEDD_DIRECTDRAW_GLOBAL peDdGl)
{

}

VOID
NTAPI
HeapVidMemFini(VIDEOMEMORY* pvMemory, PEDD_DIRECTDRAW_GLOBAL peDdGl)
{
/*int v2; // esi
  void *v3; // edi
  int v4; // ebx
  _DWORD *v5; // eax
  size_t v6; // ecx
  int v7; // eax
  int v8; // esi
  size_t Size; // [esp+Ch] [ebp-8h] BYREF
  int v10; // [esp+10h] [ebp-4h]

  v2 = a1;
  v3 = 0;
  v4 = 0;
  if ( !*(_DWORD *)(a1 + 20) )
    DoRip("DDASSERT");
  if ( (*(_BYTE *)a1 & 8) != 0 )
  {
    v5 = *(_DWORD **)(a1 + 20);
    v4 = v5[7];
    v3 = (void *)v5[50];
    a1 = v5[49];
    v6 = v5[51];
    v7 = v5[4];
    Size = v6;
    v10 = v7;
  }
  VidMemFini(*(PVOID *)(v2 + 20));
  *(_DWORD *)(v2 + 20) = 0;
  if ( (*(_BYTE *)v2 & 8) != 0 )
  {
    v8 = 1;
    if ( v4 )
    {
      if ( !v3 )
      {
LABEL_12:
        if ( a1 )
          v8 = AGPFree(a2, a1);
        if ( !v8 )
          DoRip("DDASSERT");
        return;
      }
      v8 = AGPDecommitAll(a2, a1, v3, Size, (int)&Size, v10);
      if ( !v8 )
        DoRip("DDASSERT");
    }
    if ( v3 )
      EngFreeMem(v3);
    goto LABEL_12;
  }*/
}

VOID
NTAPI
vDdDisableDriver(PEDD_DIRECTDRAW_GLOBAL peDdGl)
{
    // If we tried to disable a locked device?
    // vDdAssertDevlock(a1);

    VIDEOMEMORY *pvmList = peDdGl->pvmList;

    if (pvmList)
    {
        DWORD dwHeapNum = 0;
        VIDEOMEMORY *pvMemory = pvmList;

        // If we have any video memory heaps to free
        if (peDdGl->dwNumHeaps)
        {
            do
            {
                if (!(pvMemory->dwFlags & VIDMEM_HEAPDISABLED) && pvMemory->lpHeap)
                    HeapVidMemFini(pvMemory, peDdGl);

                dwHeapNum++;
                pvMemory++;
            } while (dwHeapNum < peDdGl->dwNumHeaps);
        }

        EngFreeMem(peDdGl->pvmList);
        peDdGl->pvmList = NULL;
    }

    if (peDdGl->pdwFourCC)
    {
        EngFreeMem(peDdGl->pdwFourCC);
        peDdGl->pdwFourCC = NULL;
    }

    if (peDdGl->lpDDVideoPortCaps)
    {
        EngFreeMem(peDdGl->lpDDVideoPortCaps);
        peDdGl->lpDDVideoPortCaps = NULL;
    }

    // Unload ApiImage
    if (peDdGl->hModule)
        vDdUnloadDxApiImage(peDdGl);

    if(peDdGl->unk_610[0] != 0)
    {
        peDdGl->unk_610[0] = 0;
    }

    // Checking if DirectDraw acceleration was enabled
    if (peDdGl->fl & 1)
    {
        // Reset DirectDraw acceleration flag
        peDdGl->fl = peDdGl->fl & 0xFFFFFFFE;

        //PPDEVOBJ Obj = (PPDEVOBJ)gpEngFuncs.DxEngGetHdevData(peDdGl->hDev, DxEGShDevData_dhpdev);
        //Obj->pfn.DisableDirectDraw((DHPDEV)peDdGl->dhpdev);

        PDRIVER_FUNCTIONS DriverFunctions = (PDRIVER_FUNCTIONS)gpEngFuncs.DxEngGetHdevData(peDdGl->hDev, DxEGShDevData_DrvFuncs);
        DriverFunctions->DisableDirectDraw(peDdGl->dhpdev);
    }

    // We start to zero the structure at driver references?
    memset(&peDdGl->cDriverReferences, 0, 0x590);
}

// Implemented
VOID
NTAPI
DxDdDisableDirectDraw(HDEV hDev, BOOLEAN bDisableVDd)
{
    PEDD_DIRECTDRAW_GLOBAL peDdGl = NULL;

    peDdGl = (PEDD_DIRECTDRAW_GLOBAL)gpEngFuncs.DxEngGetHdevData(hDev, DxEGShDevData_eddg);

    if ((peDdGl != NULL && peDdGl->hDev != NULL))
    {
        gpEngFuncs.DxEngLockHdev(hDev);

        if (bDisableVDd != 0)
        {
            vDdDisableDriver(peDdGl);
        }

        // Destory an event?
        if (peDdGl->pAssertModeEvent != NULL)
        {
            EngFreeMem(peDdGl->pAssertModeEvent);
        }

        // 0x618 size?
        memset(peDdGl, 0, sizeof(EDD_DIRECTDRAW_GLOBAL));

        gpEngFuncs.DxEngUnlockHdev(hDev);
    }
}

DWORD
NTAPI
DxDdSuspendDirectDraw(PVOID p1, PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdResumeDirectDraw(HDEV hDev, PVOID p2)
{
    TRACE();
    return 0;
}

DWORD
NTAPI
DxDdDynamicModeChange(PVOID p1, PVOID p2, PVOID p3)
{
    TRACE();
    return 0;
}

// Implemented
BOOL
NTAPI
DdHmgOwnedBy(PDD_ENTRY pDdObj, HANDLE hProcess)
{
  BOOL bSUCCESS = FALSE;
  
  if (pDdObj != NULL &&
   (((ULONG_PTR)hProcess ^ (ULONG_PTR)pDdObj->Pid) & 0xfffffffd) & 0xfffffffe == 0)
  {
    bSUCCESS = TRUE;
  }

  return bSUCCESS;
}

// Implemented
PDD_ENTRY
NTAPI
DdHmgNextOwned(PDD_ENTRY pDdObj, HANDLE hProcess)
{
  DWORD dwIndex;

  dwIndex = ((DWORD)pDdObj & 0x1FFFFF) + 1;

  if (dwIndex < gcMaxDdHmgr) 
  {
    DD_ENTRY* pEntry = (PDD_ENTRY)(gpentDdHmgr + (sizeof(DD_ENTRY) * dwIndex));

    do 
    {
      if (DdHmgOwnedBy(pDdObj, hProcess)) 
      {
        return (PDD_ENTRY)(((DWORD)pEntry->FullUnique << 0x15) | dwIndex);
      }

      dwIndex++;
      pEntry++;
    } 
    while (dwIndex < gcMaxDdHmgr);
  }

  return NULL;
}

BOOL NTAPI
DdHmgCloseProcess(HANDLE hProcess)
{
  PDD_ENTRY pDdObj;
  DWORD dwObjectType;
  //DWORD dwCount;

  BOOL bSUCCESS = TRUE;

  // We are iterating a list here so we need to lock it
  EngAcquireSemaphore(ghsemHmgr);
  pDdObj = DdHmgNextOwned(NULL, hProcess);
  EngReleaseSemaphore(ghsemHmgr);

  while (pDdObj)
  {
    switch (pDdObj->Objt)
    {
      case ObjType_DDLOCAL_TYPE:
        //bSUCCESS = bDdDeleteDirectDrawObject(pDdObj, TRUE);
        break;
      case ObjType_DDSURFACE_TYPE:
      //bSUCCESS = bDdDeleteSurfaceObject(pDdObj, 0);
        break;
      case ObjType_DDCONTEXT_TYPE:
        // if(D3dDeleteHandle(pDdObj, 0, 0, &dwCount) == 1)
        break;
      case ObjType_DDVIDEOPORT_TYPE:
      //bSUCCESS = bDdDeleteVideoPortObject(pDdObj, 0);
        break;
      case ObjType_DDMOTIONCOMP_TYPE:
      //bSUCCESS = bDdDeleteMotionCompObject(pDdObj, 0);
        break;
      default:
        bSUCCESS = FALSE;
        break;
    }

    if (!bSUCCESS)
    {
      //DbgPrint("DDRAW ERROR: DdHmgCloseProcess couldn\'t delete obj = %p, type j=%lx\n", pDdObj, dwObjectType);
    }

    EngAcquireSemaphore(ghsemHmgr);
    pDdObj = DdHmgNextOwned(pDdObj, hProcess);
    EngReleaseSemaphore(ghsemHmgr);
  }

  return bSUCCESS;
}

VOID
NTAPI
DxDdCloseProcess(HANDLE hProcess)
{
    DdHmgCloseProcess(hProcess);
}

BOOL
NTAPI
DxDdGetDirectDrawBound(HDEV hDev, LPRECT lpRect)
{
    BOOL Result = FALSE;
    PEDD_DIRECTDRAW_GLOBAL peDdGl;

    peDdGl = (PEDD_DIRECTDRAW_GLOBAL)gpEngFuncs.DxEngGetHdevData(hDev, DxEGShDevData_eddg);

    // vDdAssertDevlock(pEVar2);

    // Do we have some kind of global bounds locked
    Result = (peDdGl->fl & 4) != 0;

    if (Result)
    {
        lpRect->left = peDdGl->rcbounds.left;
        lpRect->top = peDdGl->rcbounds.top;
        lpRect->right = peDdGl->rcbounds.right;
        lpRect->bottom = peDdGl->rcbounds.bottom;

        peDdGl->fl = peDdGl->fl & 0xfffffffb;
    }

    return Result;
}

DWORD
NTAPI
DxDdEnableDirectDrawRedirection(PVOID p1, PVOID p2)
{
    // Not Implemented In Vista
    return 0;
}

PVOID
NTAPI
DxDdAllocPrivateUserMem(PEDD_SURFACE pEDDSurface, SIZE_T cjMemSize, ULONG ulTag)
{
    PVOID pvUserMem = NULL;

    HANDLE hProcess = PsGetCurrentProcess();

    // If the creator is this process allow the engine to allocate
    if(pEDDSurface->ddsSurfaceGlobal.hCreatorProcess == hProcess)
    {
        pvUserMem = EngAllocUserMem(cjMemSize, ulTag);
    }

    return pvUserMem;
}

VOID
NTAPI
SafeFreeUserMem(PVOID pvMem, HANDLE hProcess)
{  
  if (hProcess == PsGetCurrentProcess()) {
    EngFreeUserMem(pvMem);
  } 
  else {
    KeAttachProcess((PKPROCESS)hProcess);
    EngFreeUserMem(pvMem);
    KeDetachProcess();
  }
}

VOID
NTAPI
DeferMemoryFree(PVOID pvMem, EDD_SURFACE *pEDDSurface)
{
  PVOID* ppvMem;

  ppvMem = (PVOID*)EngAllocMem(1, 0xc, TAG_GDDP);

  if (ppvMem != NULL) {
    ppvMem[0] = pvMem;

    ppvMem[1] = pEDDSurface;

    ppvMem[2] = pEDDSurface->ddsSurfaceGlobal.hCreatorProcess;

    // *(void ***)(*(int *)(*(int *)(param_2 + 0xcc) + 0x24) + 0x608) = ppvVar1;
    pEDDSurface->peDirectDrawGlobal->unk_608 = (ULONG_PTR)ppvMem;

    pEDDSurface->ddsSurfaceLocal.dwFlags |= 0x1000;
  }
}


VOID
NTAPI
DxDdFreePrivateUserMem(int p1, PVOID pvMem)
{
    // Some strange pointer math going on here...
    PEDD_SURFACE surface;

    // This requires further investigation
    if(p1 != 0)
    {
        surface = (PEDD_SURFACE)(p1 - 0x10U);
    }

    // 0x800 is some system memory flag
    if(surface->ddsSurfaceLocal.dwFlags & 0x800)
    {
        DeferMemoryFree(pvMem, surface);
    }
    else
    {
        SafeFreeUserMem(pvMem, surface->ddsSurfaceGlobal.hCreatorProcess);
    }
}

VOID
NTAPI
DxDdSetAccelLevel(HDEV hDev, DWORD dwLevel, BYTE bFlags)
{
    PEDD_DIRECTDRAW_GLOBAL pGlobal = NULL;
    (PEDD_DIRECTDRAW_GLOBAL)gpEngFuncs.DxEngGetHdevData(hDev, DxEGShDevData_eddg);

    if(dwLevel >= 3 || bFlags & 2)
    {
        pGlobal->llAssertModeTimeout.HighPart = 0;
        pGlobal->llAssertModeTimeout.LowPart = 0;
    }
}

PEDD_SURFACE
NTAPI
DxDdGetSurfaceLock(HDEV hDev)
{
    PEDD_DIRECTDRAW_GLOBAL peDdGl = NULL;

    peDdGl = (PEDD_DIRECTDRAW_GLOBAL)gpEngFuncs.DxEngGetHdevData(hDev, DxEGShDevData_eddg);

    return peDdGl->peSurface_LockList;
}

PEDD_SURFACE
NTAPI
DxDdEnumLockedSurfaceRect(HDEV hDev, PEDD_SURFACE pEDDSurface, LPRECT lpRect)
{
    PEDD_SURFACE result = NULL;

    if(pEDDSurface == NULL)
    {
        PEDD_DIRECTDRAW_GLOBAL peDdGl = NULL;
        peDdGl = (PEDD_DIRECTDRAW_GLOBAL)gpEngFuncs.DxEngGetHdevData(hDev, DxEGShDevData_eddg);

        // Do we have any locked surfaces in the global list?
        result = peDdGl->peSurface_LockList;
    }
    else
    {
        // Is there another locked surface below this one?
        result = pEDDSurface->peSurface_LockNext;
    }

    if(result)
    {
        // If we found a locked surface copy it's rectangle
        lpRect->left = result->rclLock.left;
        lpRect->top = result->rclLock.top;
        lpRect->right = result->rclLock.right;
        lpRect->bottom = result->rclLock.bottom;
    }

    // Return the surface we found
    return result;
}

// This is a very common call gpEngFuncs + 0xa4(param_1 = HDEV?, DxEGShDevData_eddg = 7)
// (PEDD_DIRECTDRAW_GLOBAL)gpEngFuncs.DxEngGetHdevData(hDev, DxEGShDevData_eddg);

DRVFN gaDxgFuncs[] = 
{
    {DXG_INDEX_DxDxgGenericThunk, (PFN)DxDxgGenericThunk},
    {DXG_INDEX_DxD3dContextCreate, (PFN)DxD3dContextCreate},
    {DXG_INDEX_DxD3dContextDestroy, (PFN)DxD3dContextDestroy},
    {DXG_INDEX_DxD3dContextDestroyAll, (PFN)DxD3dContextDestroyAll},
    {DXG_INDEX_DxD3dValidateTextureStageState, (PFN)DxD3dValidateTextureStageState},
    {DXG_INDEX_DxD3dDrawPrimitives2, (PFN)DxD3dDrawPrimitives2},
    {DXG_INDEX_DxDdGetDriverState, (PFN)DxDdGetDriverState},
    {DXG_INDEX_DxDdAddAttachedSurface, (PFN)DxDdAddAttachedSurface},
    {DXG_INDEX_DxDdAlphaBlt, (PFN)DxDdAlphaBlt},
    {DXG_INDEX_DxDdAttachSurface, (PFN)DxDdAttachSurface},
    {DXG_INDEX_DxDdBeginMoCompFrame, (PFN)DxDdBeginMoCompFrame},
    {DXG_INDEX_DxDdBlt, (PFN)DxDdBlt},
    {DXG_INDEX_DxDdCanCreateSurface, (PFN)DxDdCanCreateSurface},
    {DXG_INDEX_DxDdCanCreateD3DBuffer, (PFN)DxDdCanCreateD3DBuffer},
    {DXG_INDEX_DxDdColorControl, (PFN)DxDdColorControl},
    {DXG_INDEX_DxDdCreateDirectDrawObject, (PFN)DxDdCreateDirectDrawObject},
    {DXG_INDEX_DxDdCreateSurface, (PFN)DxDdCreateD3DBuffer},
    {DXG_INDEX_DxDdCreateD3DBuffer, (PFN)DxDdCreateD3DBuffer},
    {DXG_INDEX_DxDdCreateMoComp, (PFN)DxDdCreateMoComp},
    {DXG_INDEX_DxDdCreateSurfaceObject, (PFN)DxDdCreateSurfaceObject},
    {DXG_INDEX_DxDdDeleteDirectDrawObject, (PFN)DxDdDeleteDirectDrawObject},
    {DXG_INDEX_DxDdDeleteSurfaceObject, (PFN)DxDdDeleteSurfaceObject},
    {DXG_INDEX_DxDdDestroyMoComp, (PFN)DxDdDestroyMoComp},
    {DXG_INDEX_DxDdDestroySurface, (PFN)DxDdDestroySurface},
    {DXG_INDEX_DxDdDestroyD3DBuffer, (PFN)DxDdDestroyD3DBuffer},
    {DXG_INDEX_DxDdEndMoCompFrame, (PFN)DxDdEndMoCompFrame},
    {DXG_INDEX_DxDdFlip, (PFN)DxDdFlip},
    {DXG_INDEX_DxDdFlipToGDISurface, (PFN)DxDdFlipToGDISurface},
    {DXG_INDEX_DxDdGetAvailDriverMemory, (PFN)DxDdGetAvailDriverMemory},
    {DXG_INDEX_DxDdGetBltStatus, (PFN)DxDdGetBltStatus},
    {DXG_INDEX_DxDdGetDC, (PFN)DxDdGetDC},
    {DXG_INDEX_DxDdGetDriverInfo, (PFN)DxDdGetDriverInfo},
    {DXG_INDEX_DxDdGetDxHandle, (PFN)DxDdGetDxHandle},
    {DXG_INDEX_DxDdGetFlipStatus, (PFN)DxDdGetFlipStatus},
    {DXG_INDEX_DxDdGetInternalMoCompInfo, (PFN)DxDdGetInternalMoCompInfo},
    {DXG_INDEX_DxDdGetMoCompBuffInfo, (PFN)DxDdGetMoCompBuffInfo},
    {DXG_INDEX_DxDdGetMoCompGuids, (PFN)DxDdGetMoCompGuids},
    {DXG_INDEX_DxDdGetMoCompFormats, (PFN)DxDdGetMoCompFormats},
    {DXG_INDEX_DxDdGetScanLine, (PFN)DxDdGetScanLine},
    {DXG_INDEX_DxDdLock, (PFN)DxDdLock},
    {DXG_INDEX_DxDdLockD3D, (PFN)DxDdLockD3D},
    {DXG_INDEX_DxDdQueryDirectDrawObject, (PFN)DxDdQueryDirectDrawObject},
    {DXG_INDEX_DxDdQueryMoCompStatus, (PFN)DxDdQueryMoCompStatus},
    {DXG_INDEX_DxDdReenableDirectDrawObject, (PFN)DxDdReenableDirectDrawObject},
    {DXG_INDEX_DxDdReleaseDC, (PFN)DxDdReleaseDC},
    {DXG_INDEX_DxDdRenderMoComp, (PFN)DxDdRenderMoComp},
    {DXG_INDEX_DxDdResetVisrgn, (PFN)DxDdResetVisrgn},
    {DXG_INDEX_DxDdSetColorKey, (PFN)DxDdSetColorKey},
    {DXG_INDEX_DxDdSetExclusiveMode, (PFN)DxDdSetExclusiveMode},
    {DXG_INDEX_DxDdSetGammaRamp, (PFN)DxDdSetGammaRamp},
    {DXG_INDEX_DxDdCreateSurfaceEx, (PFN)DxDdCreateSurfaceEx},
    {DXG_INDEX_DxDdSetOverlayPosition, (PFN)DxDdSetOverlayPosition},
    {DXG_INDEX_DxDdUnattachSurface, (PFN)DxDdUnattachSurface},
    {DXG_INDEX_DxDdUnlock, (PFN)DxDdUnlock},
    {DXG_INDEX_DxDdUnlockD3D, (PFN)DxDdUnlockD3D},
    {DXG_INDEX_DxDdUpdateOverlay, (PFN)DxDdUpdateOverlay},
    {DXG_INDEX_DxDdWaitForVerticalBlank, (PFN)DxDdWaitForVerticalBlank},
    {DXG_INDEX_DxDvpCanCreateVideoPort, (PFN)DxDvpCanCreateVideoPort},
    {DXG_INDEX_DxDvpColorControl, (PFN)DxDvpColorControl},
    {DXG_INDEX_DxDvpCreateVideoPort, (PFN)DxDvpCreateVideoPort},
    {DXG_INDEX_DxDvpDestroyVideoPort, (PFN)DxDvpDestroyVideoPort},
    {DXG_INDEX_DxDvpFlipVideoPort, (PFN)DxDvpFlipVideoPort},
    {DXG_INDEX_DxDvpGetVideoPortBandwidth, (PFN)DxDvpGetVideoPortBandwidth},
    {DXG_INDEX_DxDvpGetVideoPortField, (PFN)DxDvpGetVideoPortField},
    {DXG_INDEX_DxDvpGetVideoPortFlipStatus, (PFN)DxDvpGetVideoPortFlipStatus},
    {DXG_INDEX_DxDvpGetVideoPortInputFormats, (PFN)DxDvpGetVideoPortInputFormats},
    {DXG_INDEX_DxDvpGetVideoPortLine, (PFN)DxDvpGetVideoPortLine},
    {DXG_INDEX_DxDvpGetVideoPortOutputFormats, (PFN)DxDvpGetVideoPortOutputFormats},
    {DXG_INDEX_DxDvpGetVideoPortConnectInfo, (PFN)DxDvpGetVideoPortConnectInfo},
    {DXG_INDEX_DxDvpGetVideoSignalStatus, (PFN)DxDvpGetVideoSignalStatus},
    {DXG_INDEX_DxDvpUpdateVideoPort, (PFN)DxDvpUpdateVideoPort},
    {DXG_INDEX_DxDvpWaitForVideoPortSync, (PFN)DxDvpWaitForVideoPortSync},
    {DXG_INDEX_DxDvpAcquireNotification, (PFN)DxDvpAcquireNotification},
    {DXG_INDEX_DxDvpReleaseNotification, (PFN)DxDvpReleaseNotification},
    {DXG_INDEX_DxDdHeapVidMemAllocAligned, (PFN)DxDdHeapVidMemAllocAligned},
    {DXG_INDEX_DxDdHeapVidMemFree, (PFN)DxDdHeapVidMemFree},
    {DXG_INDEX_DxDdEnableDirectDraw, (PFN)DxDdEnableDirectDraw},
    {DXG_INDEX_DxDdDisableDirectDraw, (PFN)DxDdDisableDirectDraw},
    {DXG_INDEX_DxDdSuspendDirectDraw, (PFN)DxDdSuspendDirectDraw},
    {DXG_INDEX_DxDdResumeDirectDraw, (PFN)DxDdResumeDirectDraw},
    {DXG_INDEX_DxDdDynamicModeChange, (PFN)DxDdDynamicModeChange},
    {DXG_INDEX_DxDdCloseProcess, (PFN)DxDdCloseProcess},
    {DXG_INDEX_DxDdGetDirectDrawBound, (PFN)DxDdGetDirectDrawBound},
    {DXG_INDEX_DxDdEnableDirectDrawRedirection, (PFN)DxDdEnableDirectDrawRedirection},
    {DXG_INDEX_DxDdAllocPrivateUserMem, (PFN)DxDdAllocPrivateUserMem},
    {DXG_INDEX_DxDdFreePrivateUserMem, (PFN)DxDdFreePrivateUserMem},
    {DXG_INDEX_DxDdLockDirectDrawSurface, (PFN)DxDdLockDirectDrawSurface},
    {DXG_INDEX_DxDdUnlockDirectDrawSurface, (PFN)DxDdUnlockDirectDrawSurface},
    {DXG_INDEX_DxDdSetAccelLevel, (PFN)DxDdSetAccelLevel},
    {DXG_INDEX_DxDdGetSurfaceLock, (PFN)DxDdGetSurfaceLock},
    {DXG_INDEX_DxDdEnumLockedSurfaceRect, (PFN)DxDdEnumLockedSurfaceRect},
    {DXG_INDEX_DxDdIoctl, (PFN)DxDdIoctl}
};
