#pragma once

#include <section_attribs.h>



#ifdef _NTOSKRNL_

#ifndef _ARM_
#define KeGetCurrentThread  _KeGetCurrentThread
#define KeGetPreviousMode   _KeGetPreviousMode
#endif
#undef  PsGetCurrentProcess
#define PsGetCurrentProcess _PsGetCurrentProcess

#define RVA(m, b) ((PVOID)((ULONG_PTR)(b) + (ULONG_PTR)(m)))

//
// We are very lazy on ARM -- we just import intrinsics
// Question: Why wasn't this done for x86 too? (see fastintrlck.asm)
//
#define InterlockedDecrement         _InterlockedDecrement
#define InterlockedDecrement16       _InterlockedDecrement16
#define InterlockedIncrement         _InterlockedIncrement
#define InterlockedIncrement16       _InterlockedIncrement16
#define InterlockedCompareExchange   _InterlockedCompareExchange
#define InterlockedCompareExchange16 _InterlockedCompareExchange16
#define InterlockedCompareExchange64 _InterlockedCompareExchange64
#define InterlockedExchange          _InterlockedExchange
#define InterlockedExchangeAdd       _InterlockedExchangeAdd
#define InterlockedOr                _InterlockedOr
#define InterlockedAnd               _InterlockedAnd

//
// Use inlined versions of fast/guarded mutex routines
//
#define ExEnterCriticalRegionAndAcquireFastMutexUnsafe _ExEnterCriticalRegionAndAcquireFastMutexUnsafe
#define ExReleaseFastMutexUnsafeAndLeaveCriticalRegion _ExReleaseFastMutexUnsafeAndLeaveCriticalRegion
#define ExAcquireFastMutex _ExAcquireFastMutex
#define ExReleaseFastMutex _ExReleaseFastMutex
#define ExAcquireFastMutexUnsafe _ExAcquireFastMutexUnsafe
#define ExReleaseFastMutexUnsafe _ExReleaseFastMutexUnsafe
#define ExTryToAcquireFastMutex _ExTryToAcquireFastMutex

#define KeInitializeGuardedMutex _KeInitializeGuardedMutex
#define KeAcquireGuardedMutex _KeAcquireGuardedMutex
#define KeReleaseGuardedMutex _KeReleaseGuardedMutex
#define KeAcquireGuardedMutexUnsafe _KeAcquireGuardedMutexUnsafe
#define KeReleaseGuardedMutexUnsafe _KeReleaseGuardedMutexUnsafe
#define KeTryToAcquireGuardedMutex _KeTryToAcquireGuardedMutex

#include "tag.h"
#include "ke.h"
#include "ob.h"
#include "mm.h"
#include "ex.h"
#include "cm.h"
#include "ps.h"
#include "cc.h"
#include "io.h"
#include "po.h"
#include "se.h"
#include "ldr.h"
#ifndef _WINKD_
#include "kd.h"
#endif
#include "kd64.h"
#include "fsrtl.h"
#include "lpc.h"
#include "rtl.h"
#ifdef KDBG
#include <kdbg/kdb.h>
#endif
#include "dbgk.h"
#include "spinlock.h"
#include "test.h"
#include "inbv.h"
#include "vdm.h"
#include "hal.h"
#include "hdl.h"
#include "icif.h"
#include "arch/intrin_i.h"
#include <arbiter.h>

/*
 * Use IsPointerOffset to test whether a pointer should be interpreted as an offset
 * or as a pointer
 */
#if defined(_X86_) || defined(_M_AMD64) || defined(_MIPS_) || defined(_PPC_) || defined(_ARM_)

/* for x86 and x86-64 the MSB is 1 so we can simply test on that */
#define IsPointerOffset(Ptr) ((LONG_PTR)(Ptr) >= 0)

#elif defined(_IA64_)

/* on Itanium if the 24 most significant bits are set, we're not dealing with
   offsets anymore. */
#define IsPointerOffset(Ptr)  (((ULONG_PTR)(Ptr) & 0xFFFFFF0000000000ULL) == 0)

#else
#error IsPointerOffset() needs to be defined for this architecture
#endif

#endif
