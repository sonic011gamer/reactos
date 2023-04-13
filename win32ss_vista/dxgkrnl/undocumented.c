/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DXGKRNL Undocumented Vista
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include <dxgkrnl.h>
//#define NDEBUG
#include <debug.h>

/*
 *  I can't really find much information on these exports...
 *  These are going to be made VIA guesses from me inserting our lovely little
 *  RDDM into vista.
 */

#if 0
//TODO: Turn into custom definitions
_DxgkSqmAddToStream(long long ptr)
_DxgkSqmCommonGeneric(long long ptr long)
_DxgkSqmCreateDwordStreamEntry(ptr long)
_DxgkSqmCreateStringStreamEntry(ptr ptr)
_DxgkSqmGenericDword64(ptr)
_DxgkSqmGenericDword(ptr)
DpiGetDriverVersion(long)
DpiGetDxgAdapter(long)
DpiGetSchedulerCallbackState(long)
DpiSetSchedulerCallbackState(long ptr)
DpSynchronizeExecution(long long ptr long ptr)
TdrCompleteRecoveryContext(ptr long)
TdrCreateRecoveryContext()
TdrIsRecoveryRequired(ptr)
TdrIsTimeoutForcedFlip()
TdrResetFromTimeout(ptr)
TraceDxgkBlockThread(long)
TraceDxgkContext(long ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr)
TraceDxgkDevice(long ptr ptr ptr ptr)
TraceDxgkFunctionProfiler(long long)
TraceDxgkPerformanceWarning(long)
TraceDxgkPresentHistory(long long ptr)
#endif