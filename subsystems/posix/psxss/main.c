#include "psxss.h"

//#include <test.h>
#include <debug.h>



HANDLE Os2ApiPort = NULL;

/**********************************************************************
 * NAME							PRIVATE
 * 	PsxStaticServerThread/1
 */
VOID
PsxStaticServerThread (PVOID x)
{
	NTSTATUS Status = STATUS_SUCCESS;
	PPORT_MESSAGE Request = (PPORT_MESSAGE) x;
	PPORT_MESSAGE Reply = NULL;
	ULONG MessageType = 0;

	DPRINT1("psxsrv: %s called\n", __FUNCTION__);

	MessageType = Request->u2.s2.Type;
	DPRINT1("psxsrv: %s received a message (Type=%d)\n",
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
PsxServerDllInitialization (ULONG ArgumentCount,
					  LPWSTR *Argument)
{
	NTSTATUS Status = STATUS_SUCCESS;
	
	DPRINT("psxsrv: %s called\n", __FUNCTION__);

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
                                         (PVOID)PsxStaticServerThread,
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
				DPRINT1("ERROR POSIX+ SUBSYSTEM HAS FAILED TO START");
			}
			return 0;
}

/* EOF */
#endif

int main (int argc, char * argv[])
{
    int c;
    DPRINT1("POSIX+ Subsystem for ReactOS");

    if (STATUS_SUCCESS == PsxServerDllInitialization(0,NULL))
    {
        DPRINT1("PSXSS: server active");
        while (TRUE)
        {
            c = 0;
            if (c == 1) break;
        }
    }
    else
    {
        DPRINT1("PSXSS: Subsystem initialization failed.\n");
        return 1;
    }
    return 0;
}