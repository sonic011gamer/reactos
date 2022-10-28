
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define NTOS_MODE_USER
#include <ndk/iofuncs.h>
#include <ndk/obfuncs.h>
#include <ndk/rtlfuncs.h>
#include <ndk/cmfuncs.h>
#include <ndk/exfuncs.h>
#include <ndk/mmfuncs.h>
#include <ndk/psfuncs.h>
#include <ndk/lpcfuncs.h>
#include <ndk/setypes.h>
#include <ndk/umfuncs.h>
#include <ndk/kefuncs.h>

#include <sm/helper.h>
#include <psx/lpcproto.h>
#include "posixsleep.h"

#include <debug.h>

PRIVATE LPCSTR MyName = "POSIXTESTAPP";
/* 
 * \\POSIX+\\SessionPort 
*/
PRIVATE CSRTERM_SESSION  Session =
{
    0, //Identifier
    {  //ServerPort
        {0,0,NULL},
        L"\\"PSX_NS_SUBSYSTEM_DIRECTORY_NAME"\\"PSX_NS_SESSIONAPI_PORT_NAME,
        INVALID_HANDLE_VALUE
    }
};


int main ()
{
	/* 
	 * Subsystems are stupidly fucking complicated so here we go
	 * testing some features in here. and seeing if i can make a conversation with MR
	 * psx.
	 */

    PSX_CONNECT_PORT_DATA        ConnectData;   
    ULONG                        ConnectDataLength = sizeof ConnectData;
    SECURITY_QUALITY_OF_SERVICE  Sqos;
    NTSTATUS                     Status;
    LONG			 Count = 2;


    /*
     * Initialize the connection data object before
     * calling PSXSS.
     */
    ConnectData.ConnectionType = PSX_CONNECTION_TYPE_TERMINAL;
    ConnectData.Version = PSX_LPC_PROTOCOL_VERSION;
    RtlInitUnicodeString (& Session.ServerPort.Name, Session.ServerPort.NameBuffer);
    while (Count--)
    {
	    OutputDebugStringW(Session.ServerPort.Name.Buffer);
	    Status = NtConnectPort (
        	        & Session.ServerPort.Handle,
                	& Session.ServerPort.Name,
	                & Sqos,
                    NULL,
                    NULL,
	                NULL,
        	        &ConnectData,
                	&ConnectDataLength
	                );
	    if (STATUS_SUCCESS != Status)
	    {
	        if(Count)
		    {
		        DPRINT1("PSXSRV ISNT RUNNING?");
		    }
        	DPRINT1 ("%s: %s: NtConnectPort failed with %08x\n",
	            MyName, __FUNCTION__, Status);
        	return Status;
	    }
	    break;
    }
    Session.Identifier = ConnectData.PortIdentifier;
    DPRINT1("CreateTerminalToPsxChannel: Return success");
    return STATUS_SUCCESS;
}
/* EOF */
