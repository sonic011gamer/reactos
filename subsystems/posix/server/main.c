/* $Id: main.c,v 1.3 2002/10/29 04:45:54 rex Exp $
 *
 * PROJECT    : ReactOS / POSIX+ Environment Subsystem Server
 * FILE       : reactos/subsys/psx/server/misc/main.c 
 * DESCRIPTION: POSIX+ server main.
 * DATE       : 2001-05-05
 * AUTHOR     : Emanuele Aliberti <eal@users.sf.net>
 * 
 * --------------------------------------------------------------------
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING. If not, write
 * to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge,
 * MA 02139, USA.  
 *
 * --------------------------------------------------------------------
 * 
 * 	19990605 (Emanuele Aliberti)
 * 		Compiled successfully with egcs 1.1.2
 *      20020323 (Emanuele Aliberti)
 *              Converted to Win32 for testing it using NT LPC.
 */
#include "include/psxss.h"

/*** EXTERNAL ********************************************************/

NTSTATUS NTAPI
PsxServerInitialization (
    IN ULONG ArgumentCount,
    IN PWSTR *ArgumentArray
    );

/*** ENTRY POINT *****************************************************/

int _main (int argc, char * argv[])
{
    INT c;

    debug_print("POSIX+ Subsystem for ReactOS\n");

    if (STATUS_SUCCESS == PsxServerInitialization(0,NULL))
    {
        debug_print("PSXSS: server active\n");
        while (TRUE)
        {
           // c = getch();
            c = 0;
            if (c == 1) break;
        }
    }
    else
    {
        debug_print("PSXSS: Subsystem initialization failed.\n");
    }
    return 0;
}
/* EOF */
