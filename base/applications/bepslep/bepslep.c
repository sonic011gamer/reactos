
#define INCL_DOSPROCESS
#include <ros2.h>

void Eingang()
{
	DosBeep(3000,300);
	DosSleep(1000);
	DosBeep(4000,200);
	DosExit(0,0);
}

void WinMainCRTStartup()
{
	Eingang();
}
