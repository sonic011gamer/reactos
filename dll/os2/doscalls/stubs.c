#include <ros2.h>
#include <debug.h>

ULONG __stdcall DosSleep (ULONG ulInterval)
{
	 DPRINT1("Calling OS/2 Func: DosSleep\n");

	return 0;
}
ULONG __stdcall DosBeep (ULONG ulFrequency, ULONG ulDuration)
{
    DPRINT1("Calling OS/2 Func: DosBeep\n");
    return 0;
}
VOID __stdcall DosExit (ULONG ulAction, ULONG ulResult)
{
    DPRINT1("Calling OS/2 Func: DosExit\n");
}
BOOL __stdcall DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	switch (ul_reason_for_call)
	{
	case 1://DLL_PROCESS_ATTACH:
	case 2://DLL_THREAD_ATTACH:
	case 3://DLL_THREAD_DETACH:
	case 0://DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

