/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     DXGKRNL Undocumented Winblue
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include <dxgkrnl.h>
//#define NDEBUG
#include <debug.h>

/*
 *  I can't really find much information on these exports...
 *  These are going to be made VIA guesses from me inserting our lovely little
 *  RDDM into Windows 8!.
 */
#if 0
DxgkSqmCommonGeneric
DxgkSqmCreateDwordStreamEntry
DxgkSqmCreateStringStreamEntry
DxgkSqmGenericDword
DxgkSqmGenericDword64
DxgkSqmGenericString
DxgkSqmOptedIn
DxgkSqmSetDword
TraceDxgkBlockThread
TraceDxgkContext
TraceDxgkDevice
TraceDxgkFunctionProfiler
TraceDxgkPerformanceWarning
DxgkSqmSetDword
#endif