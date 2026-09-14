#include "SYS_systemclass.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass system;

	if(system.Initialize())
		system.Start();

	return 0;
}