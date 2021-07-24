/*
 * PROJECT:     ReactOS nVidia nForce Ethernet Controller Driver
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Debugging support
 * COPYRIGHT:   Copyright 2021 Dmitry Borisov <di.sean@protonmail.com>
 */

#pragma once

#define MIN_TRACE    0x00000001

#if DBG

#ifndef NDEBUG

#include <string.h>
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define NDIS_DbgPrint(_t_, _x_) \
    do { \
    if (DbgPrint("  NVNET: (%s:%d) %s ", __FILENAME__, __LINE__, __FUNCTION__)) \
        DbgPrint("(%s:%d) DbgPrint() failed!\n", __FILENAME__, __LINE__); \
    if (DbgPrint _x_) \
        DbgPrint("(%s:%d) DbgPrint() failed!\n", __FILENAME__, __LINE__); \
    } while(0)

#else
#define NDIS_DbgPrint(_t_, _x_)
#endif

#else

#define NDIS_DbgPrint(_t_, _x_)

#define ASSERT_IRQL(x)
#define ASSERT_IRQL_EQUAL(x)

#endif
