/*
 * uxtheme Double-buffered Drawing API
 *
 * Copyright (C) 2008 Reece H. Dunn
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

extern "C"
{
#include "uxthemep.h"
}
#include <debug.h>
typedef struct _CThemeServer
{
    ULONG_PTR value[0xFF];
} CThemeServer;

CThemeServer *
WINAPI
SessionAllocate(HANDLE hSourceHandle, unsigned int ProcessInformation, ULONG ReturnLength, void *a4, void *a5, unsigned int a6, unsigned int a7)
{

  CThemeServer *result; // eax

   result = (CThemeServer *)RtlAllocateHeap(RtlGetProcessHeap(),
                                 HEAP_ZERO_MEMORY,
                                 0xFF);
    return result;
}