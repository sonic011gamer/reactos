/*
 * PROJECT:     ReactOS Display Driver Model
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     ReactOS Private Dxgkrnl Structures
 * COPYRIGHT:   Copyright 2023 Justin Miller <justin.miller@reactos.org>
 */

#pragma once

/*
 * In WDDM there's many internal structures as you can imagine.
 * 
 * This file is for the private structures that need to exist but we don't directly
 * match windows with. This is intentional.
 * 
 * Many of these change with updates inside windows.
 * 
 * DO NOT! make these directly match windows until we can cleanly shove our dxgkrnl
 * into windows, which requires us supporting watchdog.
 *
 * I WANT TO KEEP TRACK WHAT REACTOS ISN'T MATCHING WINDOWS ON >:(
 */

/* VidPn Object handle */
typedef struct _REACTOS_VIDPN_HANDLE
{
    BOOLEAN IsActive = FALSE;
    VIDPNMGR VidPnMgr = VIDPNMGR();
} REACTOS_VIDPN_HANDLE, *PREACTOS_VIDPN_HANDLE;

/* Vidpn Topology Handle*/
typedef struct _REACTOS_VIDPN_TOPOLOGY_HANDLE
{
    BOOLEAN IsActive = FALSE;
} REACTOS_VIDPN_TOPOLOGY_HANDLE, *PREACTOS_VIDPN_TOPOLOGY_HANDLE;

/* D3DKMT Object handle */
typedef struct _REACTOS_D3DKMT_HANDLE
{
    BOOLEAN IsActive = FALSE;
} REACTOS_D3DKMT_HANDLE, *PREACTOS_D3DKMT_HANDLE;


/* VIDPN SOURCE INTERFACE Object handle */
typedef struct _REACTOS_HVIDPN_SOURCE_MODE
{
    BOOLEAN IsActive = FALSE;
} REACTOS_HVIDPN_SOURCE_MODE, *PREACTOS_HVIDPN_SOURCE_MODE;

typedef struct _REACTOS_HVIDPN_TARGET_MODE
{
    BOOLEAN IsActive = FALSE;
} REACTOS_HVIDPN_TARGET_MODE, *PREACTOS_HVIDPN_TARGET_MODE;