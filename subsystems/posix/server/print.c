/* $Id: print.c,v 1.3 2002/10/29 04:45:54 rex Exp $ */
#include "include/psxss.h"
#include <stdio.h>
#include <debug.h>
VOID NTAPI debug_print (LPSTR Template, ...)
{
   CHAR   Buffer [1024];
   va_list ArgumentPointer;
   
   va_start(ArgumentPointer, Template);
   vsprintf(Buffer, Template, ArgumentPointer);
   va_end(ArgumentPointer);
   DbgPrint(Buffer);
}
/* EOF */
