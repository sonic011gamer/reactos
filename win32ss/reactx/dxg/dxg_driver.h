#include <dxg_int.h>

#define TRACE() \
    DbgPrint("DXG: %s\n", __FUNCTION__)

DWORD gWarningLevel = 0;

VOID 
NTAPI
DoDxgAssert(PCSTR Format)
{
    if(gWarningLevel >= 0)
    {
        DbgPrint("DXG Assertion: ");
        DbgPrint(Format);
        DbgPrint("\n");
        DbgBreakPoint();
    }
}

VOID 
NTAPI 
DoWarning(PCSTR Format, DWORD dwLevel)
{
  if (dwLevel <= gWarningLevel )
  {
    DbgPrint("DXG: ");
    DbgPrint(Format);
    DbgPrint("\n");
  }
}

VOID
NTAPI
vDdAssertDevlock(PEDD_DIRECTDRAW_GLOBAL peDdGl)
{
    if(!gpEngFuncs.DxEngIsHdevLockedByCurrentThread(peDdGl->hDev))
        DoDxgAssert("DD_ASSERTDEVLOCK failed because Devlock is not held");
}

VOID
NTAPI
vDdIncrementReferenceCount(PEDD_DIRECTDRAW_GLOBAL peDdGl)
{
    TRACE();

    vDdAssertDevlock(peDdGl);

    if (++peDdGl->unk_024 == 1)
        gpEngFuncs.DxEngReferenceHdev(peDdGl->hDev);
}

VOID
NTAPI
ProbeAndReadBuffer(void *Dst, void *Src, size_t MaxCount)
{
  if ( (char *)Src + MaxCount < (char *)Src || (unsigned int)Src + MaxCount > DxgUserProbeAddress )
    *(BYTE *)Dst = *(BYTE*)DxgUserProbeAddress;

  memcpy(Dst, Src, MaxCount);
}

VOID
NTAPI
ProbeAndWriteBuffer(void *Dst, void *Src, size_t MaxCount)
{
  if ( (char *)Dst + MaxCount < (char *)Dst || (unsigned int)Dst + MaxCount > DxgUserProbeAddress )
      DxgUserProbeAddress = 0;

  memcpy(Dst, Src, MaxCount);
}

// Implemented
BOOL
NTAPI
D3dSetup(EDD_DIRECTDRAW_GLOBAL *peDdGl, PKFLOATING_SAVE FloatSave)
{
    TRACE();

    BOOL result; // eax

    if (!peDdGl)
        DoDxgAssert("D3dSetup on NULL global\n");

    if (KeSaveFloatingPointState(FloatSave) >= 0)
    {
        gpEngFuncs.DxEngLockHdev(peDdGl->hDev);
        result = TRUE;
    }
    else
    {
        DoWarning("D3dSetup: Unable to save FP state\n", 0);
        result = FALSE;
    }

    return result;
}

// Implemented
VOID 
NTAPI
D3dCleanup(EDD_DIRECTDRAW_GLOBAL *peDdGl, PKFLOATING_SAVE FloatSave)
{
    TRACE();

    gpEngFuncs.DxEngUnlockHdev(peDdGl->hDev);
    KeRestoreFloatingPointState(FloatSave);
}

typedef struct _D3D_SURFACE
{
    PEDD_SURFACE peDdSurf;       // 0
    BOOL bMandatory;             // 1
    PEDD_SURFACE peDdLocked;     // 2
    PDD_SURFACE_LOCAL peDdLocal; // 3
} D3D_SURFACE, *PD3D_SURFACE;

BOOL
NTAPI
D3dLockSurfaces(DWORD dwCount, D3D_SURFACE **pD3dSurf)
{
    TRACE();

    if(dwCount <= 0)
        return TRUE;

    for(DWORD i = 0; i < dwCount; i++)
    {
        if(pD3dSurf[i]->peDdSurf == NULL && pD3dSurf[i]->bMandatory)
        {
            DoWarning("D3dLockSurfaces: NULL for mandatory surface", 0);
            return FALSE;
        }

        PEDD_SURFACE peDdSurf = DdHmgLock(pD3dSurf[i]->peDdSurf, ObjType_DDSURFACE_TYPE, TRUE);

        if(!peDdSurf) 
        {
            DoWarning("D3dLockSurfaces unable to lock buffer\n", 0);
            return FALSE;
        }

        if(peDdSurf->bLost)
        {
            DoWarning("D3DLockSurfaces unable to lock buffer Surface is Lost", 0);
            return FALSE;
        }

        pD3dSurf[i]->peDdLocked = peDdSurf;
        pD3dSurf[i]->peDdLocal = &peDdSurf->ddsSurfaceLocal;
    }

    return FALSE;
}

DWORD
NTAPI
DxD3dContextCreate(
    HANDLE hDdLocal,
    HANDLE Obj1,
    HANDLE Obj2,
    ULONG_PTR *RegionSize)
{
    TRACE();

    DWORD result = 0;

    BYTE* puByte = (BYTE*)RegionSize;

    for(int i = 0; i < 0x70u; i++)
    {
        DbgPrint("%X ", puByte[i]);
        DbgBreakPoint();
    }

    /*KFLOATING_SAVE FloatSave;

    VOID* ContextHandle = NULL;
    HANDLE SecureHandle = NULL;
    PVOID BaseAddress = NULL;

    D3D_SURFACE d3dSurfacesStorage[2] = {0};
    D3D_SURFACE* d3dSurfaces = &d3dSurfacesStorage[0];

    D3DNTHAL_CONTEXTCREATEDATA ContextCreateData = {0};

    // Ensure the RegionSize is 4 Byte Aligned
    if(RegionSize[0] & 3 != 0)
        ExRaiseDatatypeMisalignment();

    if(DxgUserProbeAddress <= (ULONG64)RegionSize) {
        DxgUserProbeAddress = NULL;
    }

    if(RegionSize) 
    {
        if(RegionSize - 0x4000 > 0xFC000)
        {
            return 0;
        }
    }
    else 
    {
        RegionSize = 0x10000;
    }

    PEDD_DIRECTDRAW_LOCAL peDdL = DdHmgLock(hDdLocal, ObjType_DDLOCAL_TYPE, FALSE);

    if(!peDdL)
    {
        return 0;
    }

    PEDD_DIRECTDRAW_GLOBAL peDdGl = peDdL->peDirectDrawGlobal2;

    d3dSurfaces[0].peDdSurf = Obj1;
    d3dSurfaces[0].bMandatory = FALSE;
    d3dSurfaces[1].peDdSurf = Obj2;
    d3dSurfaces[1].bMandatory = TRUE;

    DdHmgAcquireHmgrSemaphore();

    if(D3dLockSurfaces(2, &d3dSurfaces))
    {
        DdHmgReleaseHmgrSemaphore();

        ContextHandle = DdHmgAlloc(0x140u, ObjType_DDCONTEXT_TYPE, TRUE);

        if(ContextHandle)
        {
            if(ZwAllocateVirtualMemory(NtCurrentProcess(), &BaseAddress, 0, &RegionSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE))
            {
                SecureHandle = MmSecureVirtualMemory(BaseAddress, RegionSize, PAGE_READWRITE);
                
                if(SecureHandle)
                {
                    if(D3dSetup(peDdGl, &FloatSave))
                    {
                        if(peDdGl->ddMiscellanous2Callbacks.CreateSurfaceEx)
                        {
                            if(peDdGl->d3dNtHalCallbacks.ContextCreate)
                            {
                                ContextCreateData.lpDDGbl = peDdL != NULL ? (PDD_DIRECTDRAW_GLOBAL)peDdL->peDirectDrawGlobal : NULL;
                                ContextCreateData.lpDDS = d3dSurfaces[0].peDdLocal;
                                ContextCreateData.lpDDSZ = d3dSurfaces[1].peDdLocal;

                                result = peDdGl->d3dNtHalCallbacks.ContextCreate(&ContextCreateData);
                            }
                            else
                                DoWarning("DxD3dContextCreate: ContextCreate callback not found", 0);

                            if(result && !(RegionSize + 20))
                            {
                            }

                            //InterlockedDecrement(v10 + 2);
                            D3dCleanUp(peDdGl, &FloatSave);

                            // v5 + 16 = v18
                            // v5 + 20 = v19
                            // v5 + 24 = v4
                            // v5 + 28 = RegionSize
                        }
                        else
                        {
                            D3dCleanUp(peDdGl, &FloatSave);
                        }
                    }
                }
            }
        }
        else
        {
            DoWarning("DxD3dContextCreate unable to alloc handle", 0);
        }
    }
    else
    {
        DdHmgReleaseHmgrSemaphore();
    }

    for(DWORD i = 0; i < 2; i++)
        if(d3dSurfaces[i].peDdSurf) InterlockedDecrement(&d3dSurfaces[i].peDdSurf->pobj.cExclusiveLock);

    if(SecureHandle)
        MmUnsecureVirtualMemory(SecureHandle);

    if(BaseAddress)
    {
        RegionSize = 0;
        ZwFreeVirtualMemory(NtCurrentProcess(), &BaseAddress, &RegionSize, MEM_RELEASE);
    }

    if(ContextHandle)
    {
        //DdHmgFree(ContextHandle);
    }

    if(peDdL)
    {
        InterlockedDecrement((VOID*)peDdL->pobj.cExclusiveLock);
    }*/

    return result;
}

DWORD
NTAPI
DxD3dContextDestroy(
    PVOID p1)
{
    TRACE();

    struct HDD_OBJ__ **v1; // esi
    unsigned int v3;       // [esp+14h] [ebp-1Ch]

    v1 = (struct HDD_OBJ__ **)Address;
    ProbeForWrite(Address, 8u, 4u);
    v3 = D3dDeleteHandle(*v1, 0, 0, (int *)&Address);
    v1[1] = (struct HDD_OBJ__ *)Address;

    return v3;
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

// Implemented (Not even the disassembler knew what was going on here)
BOOL
NTAPI
DxDdAddAttachedSurface(
    HANDLE hSurfaceTarget,
    HANDLE hSurfaceSrc,
    PDD_ADDATTACHEDSURFACEDATA pDdAttachedSurfaceData)
{
    TRACE();

    BOOL bFAILED = FALSE;

    PEDD_SURFACE peDdSTarget = NULL;
    PEDD_SURFACE peDdSSrc = NULL;

    peDdSTarget = (PEDD_SURFACE)DdHmgLock(hSurfaceTarget, ObjType_DDSURFACE_TYPE, FALSE);
    peDdSSrc = (PEDD_SURFACE)DdHmgLock(hSurfaceSrc, ObjType_DDSURFACE_TYPE, FALSE);

    DD_ADDATTACHEDSURFACEDATA addSurfaceData = {0};

    memcpy(&addSurfaceData, pDdAttachedSurfaceData, sizeof(addSurfaceData));

    addSurfaceData.ddRVal = DDERR_GENERIC;

    if(peDdSTarget && peDdSSrc && 
        (peDdSTarget->ddsSurfaceLocal.dwFlags & 0x800) == 0 &&
        (peDdSSrc->ddsSurfaceLocal.dwFlags & 0x800) == 0 &&
        peDdSTarget->peDirectDrawLocal->pobj.hHmgr == peDdSSrc->peDirectDrawLocal->pobj.hHmgr)
    {
        // Can we attach surfaces?
        if(peDdSTarget->peDirectDrawGlobal->ddSurfaceCallbacks.dwFlags & DDHAL_SURFCB32_ADDATTACHEDSURFACE) 
        {
            // Fill out attach surface data
            addSurfaceData.lpDD = (PDD_DIRECTDRAW_GLOBAL)peDdSTarget->peDirectDrawGlobal;
            addSurfaceData.lpDDSurface = &peDdSTarget->ddsSurfaceLocal;
            addSurfaceData.lpSurfAttached = &peDdSSrc->ddsSurfaceLocal;

            // Lock the surface hDev?
            gpEngFuncs.DxEngLockHdev(peDdSTarget->peDirectDrawGlobal->hDev);

            // Fail if either surface is marked lost
            if(peDdSTarget->bLost || peDdSSrc->bLost)
            {
                bFAILED = TRUE;
                addSurfaceData.ddRVal = DDERR_SURFACELOST;
            }
            else
            {
                // attach surface here return the result
                bFAILED = peDdSTarget->peDirectDrawGlobal->ddSurfaceCallbacks.AddAttachedSurface(&addSurfaceData);
            }

            gpEngFuncs.DxEngUnlockHdev(peDdSTarget->peDirectDrawGlobal->hDev);
        }
    }
    else
    {
        DoWarning("DxDdAddAttachedSurface: Invalid surface or dwFlags\n", 0);
    }

    // We do this because AddAttachedSurface modifies this value
    pDdAttachedSurfaceData->ddRVal = addSurfaceData.ddRVal;

    if(peDdSTarget)
        InterlockedDecrement((VOID*)&peDdSTarget->pobj.cExclusiveLock);
    
    if(peDdSSrc)
        InterlockedDecrement((VOID*)&peDdSSrc->pobj.cExclusiveLock);

    return bFAILED;
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
                LPVIDMEM lpVidMem,
                DWORD dwWidth,
                DWORD dwHeight,
                LPSURFACEALIGNMENT lpAlignment ,
                LPLONG lpNewPitch)
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
  vDdAssertDevlock(peDdGl);

  EDD_DIRECTDRAW_LOCAL* v2 = peDdGl->peDirectDrawLocalList;

  EDD_DIRECTDRAW_LOCAL* v10 = v2;

  if ( v2 )
  {
    while ( 1 )
    {
        PEDD_SURFACE v3 = v2->fpProcess;
        PEDD_SURFACE v4 = NULL;
        PEDD_SURFACE v6 = NULL;
      if ( v3 )
      {
        do
        {
          v4 = v3->peSurface_DdNext;
          bDdDeleteVideoPortObject(*(struct HDD_OBJ__ **)v3, 0);
          v3 = v4;
        }
        while ( v4 );
      }
      PEDD_SURFACE v5 = v2->peSurface_DdList;
      if ( v5 )
      {
        do
        {
          v6 = (struct DXOBJ *)*((_DWORD *)v5 + 60);
          if ( v6 )
          {
            vDdDxApiFreeSurface(v6, 0);
            *((_DWORD *)v5 + 60) = 0;
          }
          v7 = (struct DXOBJ **)*((_DWORD *)v5 + 49);
          if ( v7 )
            vDdLoseDxObjects(a1, v7[25], v7, 2u);
          v5 = EDD_DIRECTDRAW_LOCAL::peSurface_Enum(v10, v5);
        }
        while ( v5 );
        v2 = v10;
      }

      v10 = v2->peDirectDrawLocal_prev;

      // We reached the end of the list
      if ( !v10 )
        break;

      v2 = v2->peDirectDrawLocal_prev;
    }
  }

  HANDLE v8 = peDdGl->hDxHeldObj;
  if ( v8 )
    vDdDxApiFreeDirectDraw(v8, 0);

  HANDLE v9 = peDdGl->hDxLoseObj;
  if ( v9 )
    vDdLoseDxObjects(peDdGl, v9[2], v9, 0);

  EngUnloadImage(peDdGl->hDxApi);

  peDdGl->hDxApi = NULL;
  peDdGl->unk_5f0 = NULL;
}

VOID 
NTAPI
VidMemFini(VMEMHEAP* lpHeap)
{
  /*if ((lpHeap->dwFlags & VMEMHEAP_LINEAR) != 0 )
    linVidMemFini(lpHeap);
  else
    rectVidMemFini(lpHeap);*/
}

VOID
NTAPI
HeapVidMemFini(VIDEOMEMORY* pvMemory, PEDD_DIRECTDRAW_GLOBAL peDdGl)
{
    TRACE();

    DWORD CommitedSize;
    //LPVMEMHEAP CommitedHeap;

    if(!pvMemory->lpHeap)
        DoDxgAssert("DDASSERT");

    if((pvMemory->dwFlags & VIDMEM_ISNONLOCAL) != 0)
    {
        LPVMEMHEAP lpHeap = pvMemory->lpHeap;
        CommitedSize = lpHeap->dwCommitedSize;
    }
/*int v2; // esi
  void *pAgpCommitMask; // edi
  int CommitedSize; // ebx
  _DWORD *heap; // eax
  size_t v6; // ecx
  int v7; // eax
  BOOL bSUCCESS; // esi
  size_t Size; // [esp+Ch] [ebp-8h] BYREF
  int v10; // [esp+10h] [ebp-4h]

  v2 = pvMemory;
  pAgpCommitMask = 0;
  CommitedSize = 0;
  if ( !heap )
    DoRip("DDASSERT");

  if ( (pvMemory->dwFlags & VIDMEM_ISNONLOCAL) != 0 )
  {
    heap = *(_DWORD **)(pvMemory + 20); // VMem Heap
    CommitedSize = heap[7]; // Commited Size
    pAgpCommitMask = (void *)heap[50]; // pAgpCommitMask
    pvMemory = heap[49]; // pvPhysRsrv
    v6 = heap[51]; // pAgpCommitMask
    v7 = heap[4]; // DwTotal
    Size = v6;
    v10 = v7;
  }
  VidMemFini(heap);
  heap = 0;
  if ( (pvMemory->dwFlags & VIDMEM_ISNONLOCAL) != 0 )
  {
    bSUCCESS = 1;
    if ( CommitedSize )
    {
      if ( !pAgpCommitMask )
      {
LABEL_12:
        if ( pvMemory )
          bSUCCESS = AGPFree(peDdGl globalstate, pvMemory); // No Commit Mask
        if ( !bSUCCESS )
          DoRip("DDASSERT");
        return;
      }
      bSUCCESS = AGPDecommitAll(peDdGl, pvMemory, pAgpCommitMask, Size, (int)&Size, v10); // Has Commit Mask
      if ( !bSUCCESS )
        DoRip("DDASSERT");
    }
    if ( pAgpCommitMask )
      EngFreeMem(pAgpCommitMask);
    goto LABEL_12;
  }*/
}

VOID
NTAPI
vDdDisableDriver(PEDD_DIRECTDRAW_GLOBAL peDdGl)
{
    TRACE();

    // If we tried to disable a locked device?
    vDdAssertDevlock(peDdGl);

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
    if (peDdGl->hDxApi)
        vDdUnloadDxApiImage(peDdGl);

    if(peDdGl->unk_hdc != NULL)
    {
        gpEngFuncs.DxEngSetDCOwner((HGDIOBJ)&peDdGl->unk_hdc, GDI_OBJ_HMGR_POWNED /*0x80000002*/);
        // _gpEngFuncs+0x310 Used in DdReleaseDC which means that unk_610 is an HDC
        gpEngFuncs.DxEngDeleteDC(peDdGl->unk_hdc, TRUE);
        peDdGl->unk_hdc = NULL;
    }

    // Checking if DirectDraw acceleration was enabled
    if (peDdGl->fl & 1)
    {
        // Reset DirectDraw acceleration flag
        peDdGl->fl = peDdGl->fl & 0xFFFFFFFE;

        //PPDEVOBJ Obj = (PPDEVOBJ)gpEngFuncs.DxEngGetHdevData(peDdGl->hDev, DxEGShDevData_dhpdev);
        //Obj->pfn.DisableDirectDraw((DHPDEV)peDdGl->dhpdev);

        PDRIVER_FUNCTIONS DriverFunctions = (PDRIVER_FUNCTIONS)gpEngFuncs.DxEngGetHdevData(peDdGl->hDev, DxEGShDevData_DrvFuncs);
        DriverFunctions->DisableDirectDraw((DHPDEV)peDdGl->dhpdev);
    }

    // We zero the structure starting from cDriverReferences and ending right at peDirectDrawLocalList
    memset(&peDdGl->cDriverReferences, 0, 0x590);
}

// Implemented
VOID
NTAPI
DxDdDisableDirectDraw(HDEV hDev, BOOL bDisableVDd)
{
    TRACE();

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
        memset(peDdGl, 0, 0x618);

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
DdHmgOwnedBy(PDD_ENTRY pDdObj, DWORD dwPid)
{
    TRACE();

    BOOL bSUCCESS = FALSE;

    if (pDdObj != NULL && ((((ULONG_PTR)dwPid ^ (ULONG_PTR)pDdObj->Pid) & 0xfffffffd) & 0xfffffffe) == 0)
    {
        bSUCCESS = TRUE;
    }

    return bSUCCESS;
}

// Implemented
PDD_ENTRY
NTAPI
DdHmgNextOwned(PDD_ENTRY pDdObj, DWORD dwPid)
{
  TRACE();

  DWORD dwIndex;

  dwIndex = ((DWORD)pDdObj & 0x1FFFFF) + 1;

  if (dwIndex < gcMaxDdHmgr)
  {
    DD_ENTRY *pEntry = (PDD_ENTRY)(gpentDdHmgr + (sizeof(DD_ENTRY) * dwIndex));

    do
    {
            if (DdHmgOwnedBy(pDdObj, dwPid))
            {
                return (PDD_ENTRY)(((DWORD)pEntry->FullUnique << 0x15) | dwIndex);
            }

            dwIndex++;
            pEntry++;
    } while (dwIndex < gcMaxDdHmgr);
  }

  return NULL;
}

BOOL NTAPI
DdHmgCloseProcess(DWORD dwPid)
{
    TRACE();

    PDD_ENTRY pDdObj;
    // DWORD dwObjectType;
    // DWORD dwCount;

    BOOL bSUCCESS = TRUE;

    // We are iterating a list here so we need to lock it
    DdHmgAcquireHmgrSemaphore();
    pDdObj = DdHmgNextOwned(NULL, dwPid);
    DdHmgReleaseHmgrSemaphore();

    while (pDdObj)
    {
        switch (pDdObj->Objt)
        {
            case ObjType_DDLOCAL_TYPE:
            // bSUCCESS = bDdDeleteDirectDrawObject(pDdObj, TRUE);
            break;
            case ObjType_DDSURFACE_TYPE:
            // bSUCCESS = bDdDeleteSurfaceObject(pDdObj, 0);
            break;
            case ObjType_DDCONTEXT_TYPE:
            // if(D3dDeleteHandle(pDdObj, 0, 0, &dwCount) == 1)
            break;
            case ObjType_DDVIDEOPORT_TYPE:
            // bSUCCESS = bDdDeleteVideoPortObject(pDdObj, 0);
            break;
            case ObjType_DDMOTIONCOMP_TYPE:
            // bSUCCESS = bDdDeleteMotionCompObject(pDdObj, 0);
            break;
            default:
            bSUCCESS = FALSE;
            break;
        }

        if (!bSUCCESS)
        {
            // DbgPrint("DDRAW ERROR: DdHmgCloseProcess couldn\'t delete obj = %p, type j=%lx\n", pDdObj, dwObjectType);
        }

        DdHmgAcquireHmgrSemaphore();
        pDdObj = DdHmgNextOwned(pDdObj, dwPid);
        DdHmgReleaseHmgrSemaphore();
    }

    return bSUCCESS;
}

// Implemented
VOID
NTAPI
DxDdCloseProcess(DWORD dwPid)
{
    TRACE();

    DdHmgCloseProcess(dwPid);
}

// Implemented
BOOL
NTAPI
DxDdGetDirectDrawBound(HDEV hDev, LPRECT lpRect)
{
    TRACE();

    BOOL Result = FALSE;
    PEDD_DIRECTDRAW_GLOBAL peDdGl;

    peDdGl = (PEDD_DIRECTDRAW_GLOBAL)gpEngFuncs.DxEngGetHdevData(hDev, DxEGShDevData_eddg);

    vDdAssertDevlock(peDdGl);

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

// Implemented
DWORD
NTAPI
DxDdEnableDirectDrawRedirection(PVOID p1, PVOID p2)
{
    TRACE();

    // Not Implemented In Vista
    return 0;
}

// Implemented
PVOID
NTAPI
DxDdAllocPrivateUserMem(PEDD_SURFACE pEDDSurface, SIZE_T cjMemSize, ULONG ulTag)
{
    TRACE();

    PVOID pvUserMem = NULL;

    HANDLE hProcess = PsGetCurrentProcess();

    // Check owning process?
    if(pEDDSurface != NULL && pEDDSurface->peDirectDrawLocal->Process == hProcess)
    {
        pvUserMem = EngAllocUserMem(cjMemSize, ulTag);
    }

    return pvUserMem;
}

// Implemented
VOID
NTAPI
SafeFreeUserMem(PVOID pvMem, PKPROCESS pKProcess)
{
    TRACE();

    if (pKProcess == (PKPROCESS)PsGetCurrentProcess())
    {
        EngFreeUserMem(pvMem);
    }
    else
    {
        KeAttachProcess(pKProcess);
        EngFreeUserMem(pvMem);
        KeDetachProcess();
    }
}

// Implemented
VOID
NTAPI
DeferMemoryFree(PVOID pvMem, EDD_SURFACE *pEDDSurface)
{
  PVOID* ppvMem;

  ppvMem = (PVOID*)EngAllocMem(FL_ZERO_MEMORY, 0xc, TAG_GDDP);

  if (ppvMem != NULL) {
    ppvMem[0] = pvMem;

    ppvMem[1] = pEDDSurface;

    ppvMem[2] = (PVOID)pEDDSurface->peDirectDrawLocal->peDirectDrawGlobal2->unk_608;

    // *(void ***)(*(int *)(*(int *)(param_2 + 0xcc) + 0x24) + 0x608) = ppvVar1;
    pEDDSurface->peDirectDrawLocal->peDirectDrawGlobal2->unk_608 = (ULONG_PTR)ppvMem;

    pEDDSurface->cLocks |= 0x1000;
  }
}

VOID
NTAPI
DxDdFreePrivateUserMem(int p1, PVOID pvMem)
{
    TRACE();

    PEDD_SURFACE pEDDSurface = p1 != 0 ? (PEDD_SURFACE)(p1 - 0x10U) : NULL;

    if(pEDDSurface != NULL) 
    {
        // 0x800 is some system memory flag
        // Unsure what exactly we are looking at for a flag here
        if((pEDDSurface->cLocks & 0x800) != 0)
        {
            DeferMemoryFree(pvMem, pEDDSurface);
        }
        else
        {
            SafeFreeUserMem(pvMem, (PKPROCESS)pEDDSurface->peDirectDrawLocal->Process);
        }
    }
}

// Implemented
VOID
NTAPI
DxDdSetAccelLevel(HDEV hDev, DWORD dwLevel, BYTE bFlags)
{
    TRACE();

    PEDD_DIRECTDRAW_GLOBAL peDdGl = NULL;
    peDdGl = (PEDD_DIRECTDRAW_GLOBAL)gpEngFuncs.DxEngGetHdevData(hDev, DxEGShDevData_eddg);

    if(dwLevel >= 3 || bFlags & 2)
    {
        peDdGl->llAssertModeTimeout.HighPart = 0;
        peDdGl->llAssertModeTimeout.LowPart = 0;
    }
}

// Implemented
PEDD_SURFACE
NTAPI
DxDdGetSurfaceLock(HDEV hDev)
{
    TRACE();

    PEDD_DIRECTDRAW_GLOBAL peDdGl = NULL;

    peDdGl = (PEDD_DIRECTDRAW_GLOBAL)gpEngFuncs.DxEngGetHdevData(hDev, DxEGShDevData_eddg);

    return peDdGl->peSurface_LockList;
}

// Implemented
PEDD_SURFACE
NTAPI
DxDdEnumLockedSurfaceRect(HDEV hDev, PEDD_SURFACE pEDDSurface, LPRECT lpRect)
{
    TRACE();

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
        result = pEDDSurface->field_C0;
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

// This is a very common call gpEngFuncs + 0xa4(164)(param_1 = HDEV?, DxEGShDevData_eddg = 7)
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
