/*
 * PROJECT:     ReactOS Intel HD4000 graphics card driver
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     GMBus management source
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */

#include "hd4000.h"
#if 0
PVOID BaseAddr = NULL;
/*
 * GMBus is just a fancy I2C bus
 */

VOID
IGFX_GmBus_Reset()
{

}

VOID
IGFX_GmBus_StopTransaction()
{

}

VOID
IGFX_GmBus_Read()
{

}

VOID
IGFX_GmBus_Wait()
{
   while (!(IGFX_Read32(BaseAddr, IGFX_GMBUS2) & (1 << 11)))
       ;
}
#endif