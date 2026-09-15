#include "SYS_systemclass.h"
#include "SYS_DEVICE_exception.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass system;

	try
	{
		system.Start();
	}
	// Handle exceptions.
	// This program defines DEVICE_Exception, a custom class that inherits from std::exception
	// The program mostly handles exceptions from the DX11 API. When a device or device context
	// fails and its HRESULT variable gets that error value, an exception is thrown.
	// Not more complicated than that.
	catch (const DEVICE_Exception& e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}


	return 0;
}