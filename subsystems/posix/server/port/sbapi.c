/* $Id: sbapi.c,v 1.4 2003/12/21 20:11:46 ea Exp $
 *
 * PROJECT    : ReactOS / POSIX+ Environment Subsystem Server
 * FILE       : reactos/subsys/psx/server/port/sbapi.c
 * DESCRIPTION: \POSIX+\SbApiPort LPC logic.
 * DATE       : 2001-03-23
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
 */
#include "../include/psxsrv.h"
#include "utils.h"


/**********************************************************************
 *	ProcessConnectionRequest/				PRIVATE
 */
PRIVATE NTSTATUS NTAPI
ProcessConnectionRequest (PPSX_MESSAGE pRequest)
{
    return STATUS_NOT_IMPLEMENTED;
}
#if 0
/**********************************************************************
 *	ProcessRequest/				PRIVATE
 */
PRIVATE NTSTATUS NTAPI
ProcessRequest (PPSX_MESSAGE pRequest)
{
    return STATUS_NOT_IMPLEMENTED;
}
#endif
/**********************************************************************
 *	SbApiPortListener/1
 *
 * DESCRIPTION
 *	The \POSIX+\SbApiPort LPC port message dispatcher.
 *
 * NOTE
 *	what is this port for? Is "Sb" for "shared block"?
 */
ULONG NTAPI
SbApiPortListener (PVOID pArg)
{
    ULONG ulIndex = (ULONG)(ULONG_PTR)pArg;
    NTSTATUS Status;
    SB_API_MSG ReceiveMsg;
    PSB_API_MSG ReplyMsg = NULL;
    ULONG MessageType;
    PVOID PortContext;

    debug_print ("psxsrv: ->%s pArg=%d\n", __FUNCTION__, ulIndex);

    while (TRUE)
    {
        Status = NtReplyWaitReceivePort(Server.Port[ulIndex].hObject,
                                        &PortContext,
                                        &ReplyMsg->h,
                                        &ReceiveMsg.h);
        if (!NT_SUCCESS(Status))
        {
            debug_print("psxsrv: %s: NtReplyWaitReceivePort() failed with Status %08lx\n",
                        __FUNCTION__,
                        Status);
            continue;
        }

        MessageType = ReceiveMsg.h.u2.s2.Type;
        debug_print("psxsrv: %s: got MessageType 0x%x\n", __FUNCTION__, MessageType);
        switch (MessageType)
        {
        case LPC_CONNECTION_REQUEST:
        {
            HANDLE hConnectedPort;
            ULONG ulPortIdentifier;
            /* FIXME: this is all wrong! */
            ProcessConnectionRequest(NULL);
            NtAcceptConnectPort(&hConnectedPort, &ulPortIdentifier, &ReceiveMsg.h, TRUE, NULL, NULL);
            NtCompleteConnectPort(hConnectedPort);
            continue;
        }
        }
    }

    NtTerminateThread(NtCurrentThread(),Status);

    return 0;
}
/* EOF */
