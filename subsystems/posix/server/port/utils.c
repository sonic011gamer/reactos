/* $Id: utils.c,v 1.4 2003/12/21 20:11:46 ea Exp $
 *
 * PROJECT    : ReactOS / POSIX+ Environment Subsystem Server
 * FILE       : reactos/subsys/psx/server/port/utils.c
 * DESCRIPTION: LPC port utilities.
 * DATE       : 2002-04-07
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
 *	PsxCheckConnectionRequest/2
 *
 * DESCRIPTION
 *	Check if we can accept the connection request sent to
 *      an LPC port. Protocol version and ConnectionType MUST match.
 */
NTSTATUS NTAPI
PsxCheckConnectionRequest (
    IN OUT PPSX_CONNECT_PORT_DATA  pConnectData,
    IN     PSX_CONNECTION_TYPE     ConnectionType
    )
{
    /* Check if the caller is ConnectionType */
    if (ConnectionType != pConnectData->ConnectionType)
    {
        debug_print("psxsrv: %s: ConnectionType=%d, expected %d\n",
                    __FUNCTION__,
                    pConnectData->ConnectionType,
                    ConnectionType
                    );
        pConnectData->ConnectionType = ConnectionType;
        return STATUS_SUCCESS;
    }

    return STATUS_SUCCESS;
}
/* EOF */
