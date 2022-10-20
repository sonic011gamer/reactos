/* $Id$
*/
/*
 *
 * COPYRIGHT:        See COPYING in the top level directory
 * PROJECT:          ReactOS OS/2 sub system
 * PART:			 doscalls
 * FILE:             process.cpp
 * CONTAINS:		 process and thread related CP-functions.
 * PURPOSE:          Kernelservices for OS/2 apps
 * PROGRAMMER:       Robert K. nonvolatil@yahoo.de
 * REVISION HISTORY:
 *  13-03-2002  Created
 *	25-07-2002	Work to make it compile	
 */


//#if defined (INCL_DOS)
#define INCL_DOSPROCESS
#define INCL_DOSERRORS
#include <ros2.h>


int __stdcall DosSleep(ULONG msec)
{
	NTSTATUS stat;
	TIME Interv;
	Interv.QuadPart= -(10000 * msec);
	stat = NT::NtDelayExecution( TRUE, &Interv );
	return 0;
}


/* $Id$ */
/* Terminates the current thread or the current Process.
	Decission is made by action 
	FIXME:	move this code to OS2.EXE */
VOID APIENTRY DosExit(ULONG action, ULONG result)
{
	// decide what to do
	if( action == EXIT_THREAD)
	{
		NtTerminateThread( NULL, result );
	}
	else	// EXIT_PROCESS
	{
		NtTerminateProcess( NULL, result );
	}
}


APIRET __stdcall DosCreateThread(PTID ptid, PFNTHREAD pfn,
                                   ULONG param, ULONG flag, ULONG cbStack)
{
	return ERROR_CALL_NOT_IMPLEMENTED;
}

/* EOF */
