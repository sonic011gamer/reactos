/* $Id: print.c,v 1.3 2002/10/29 04:45:54 rex Exp $ */
#include <windows.h>
#include <stdio.h>
#include <debug.h>
VOID NTAPI debug_print (LPSTR Template, ...)
{
   CHAR   Buffer [1024];
   va_list ArgumentPointer;
   
   va_start(ArgumentPointer, Template);
   vsprintf(Buffer, Template, ArgumentPointer);
   va_end(ArgumentPointer);
#ifdef __PSXSS_ON_W32__
   printf("%s", Buffer);
#else
#error TODO
#endif
}
/* EOF */
