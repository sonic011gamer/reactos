/*
 * Copyright (C) 2019-2020 Red Hat, Inc.
 *
 * Written By: Vadim Rozenfeld <vrozenfe@redhat.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met :
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and / or other materials provided with the distribution.
 * 3. Neither the names of the copyright holders nor the names of their contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "helper.h"
#include "baseobj.h"
#if !DBG
#include "viogpu_idr.tmh"
#endif


VioGpuIdr::VioGpuIdr()
{
    m_nextId = 0;
    ExInitializeFastMutex(&m_mutex);
    InitializeListHead(&m_blocks);
    m_current = new(NonPagedPoolNx) FreeIds;
    m_current->Available = 0;
}

VioGpuIdr::~VioGpuIdr()
{
    Close();
}

BOOLEAN VioGpuIdr::Init(
    _In_ ULONG start)
{
    Close();
    Lock();

    m_nextId = start;
    m_current = new(NonPagedPoolNx) FreeIds;
    m_current->Available = 0;

    Unlock();
    return true;
}

#pragma warning( disable: 28167 )
//_IRQL_raises_(APC_LEVEL)
//_IRQL_saves_global_(OldIrql, m_IdBitMapMutex)
VOID VioGpuIdr::Lock(VOID)
{
    ExAcquireFastMutex(&m_mutex);
}

#pragma warning( disable: 28167 )
//_IRQL_requires_(APC_LEVEL)
//_IRQL_restores_global_(OldIrql, m_IdBitMapMutex)
VOID VioGpuIdr::Unlock(VOID)
{
    ExReleaseFastMutex(&m_mutex);
}

ULONG VioGpuIdr::GetId(VOID)
{
    ULONG id = 0;
    Lock();

    while (m_current->Available == 0 && !IsListEmpty(&m_blocks)) {
        delete m_current;
        m_current = (FreeIds*)RemoveHeadList(&m_blocks);
    }
    if (m_current->Available != 0) {
        m_current->Available--;
        id = m_current->FreeIDs[m_current->Available];
    } else {
        id = m_nextId;
        m_nextId++;
    }
    Unlock();
    DbgPrint(TRACE_LEVEL_VERBOSE, ("[%s] id = %d\n", __FUNCTION__, id));
    
    return id;
}

VOID VioGpuIdr::PutId(_In_ ULONG id)
{
    DbgPrint(TRACE_LEVEL_VERBOSE, ("[%s] id = %d\n", __FUNCTION__, id));
    Lock();
    if (m_current->Available + 1 >= IdsBlockSize) {
        InsertHeadList(&m_blocks, (PLIST_ENTRY)m_current);
        m_current = new(NonPagedPoolNx) FreeIds;
        m_current->Available = 0;
    }

    m_current->FreeIDs[m_current->Available] = id;
    m_current->Available++;
    
    Unlock();
}

VOID VioGpuIdr::Close(VOID)
{
    Lock();

    while (!IsListEmpty(&m_blocks)) {
        FreeIds* block = (FreeIds*)RemoveHeadList(&m_blocks);
        delete block;
    }

    if (m_current) {
        delete m_current;
        m_current = NULL;
    }

    Unlock();
}
