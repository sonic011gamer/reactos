/* $Id$
 *
 * server.c - OS/2 Enviroment Subsystem Server - Initialization
 * 
 * ReactOS Operating System
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
 * along with this software; see the file COPYING.LIB. If not, write
 * to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge,
 * MA 02139, USA.  
 *
 * --------------------------------------------------------------------
 */
#include "os2srv.h"

//#define NDEBUG
#include <test.h>
#include <debug.h>

HANDLE Os2ApiPort = NULL;

/**********************************************************************
 * NAME							PRIVATE
 * 	Os2StaticServerThread/1
 */
VOID
Os2StaticServerThread (PVOID x)
{
	NTSTATUS Status = STATUS_SUCCESS;
	PPORT_MESSAGE Request = (PPORT_MESSAGE) x;
	PPORT_MESSAGE Reply = NULL;
	ULONG MessageType = 0;

	DPRINT("OS2SRV: %s called\n", __FUNCTION__);

	MessageType = Request->u2.s2.Type;
	DPRINT("OS2SRV: %s received a message (Type=%d)\n",
		__FUNCTION__, MessageType);
	switch (MessageType)
	{
		default:
			Reply = Request;
			Status = NtReplyPort (Os2ApiPort, Reply);
			break;
	}
}

/*=====================================================================
 * 	PUBLIC API
 *===================================================================*/
#if 1
NTSTATUS
ServerDllInitialization (ULONG ArgumentCount,
					  LPWSTR *Argument)
{
	NTSTATUS Status = STATUS_SUCCESS;
	
	DPRINT("OS2SRV: %s called\n", __FUNCTION__);

	// Get the listening port from csrsrv.dll
	Os2ApiPort = CsrQueryApiPort ();

	CLIENT_ID ClientId;
	HANDLE hThread;
	            /* Create a new dynamic thread */
    Status = RtlCreateUserThread(NtCurrentProcess(),
                                         NULL,
                                         TRUE,
                                         0,
                                         0,
                                         0,
                                         (PVOID)Os2StaticServerThread ,
                                         NULL,
                                         &hThread,
                                         &ClientId);
	    /* Check success */
            if (NT_SUCCESS(Status))
            {

                /* Add a new server thread */
                if (CsrAddStaticServerThread(hThread,
                                             &ClientId,
                                             CsrThreadIsServerThread))
                {
                    /* Activate it */
                    NtResumeThread(hThread, NULL);
                }
			}
			else
			{
				DPRINT1("ERROR OS2 SUBSYSTEM HAS FAILED TO START");
			}
			return 0;
}

/* EOF */
#endif