#pragma once

#include "SYS_WinFilter.h"

class WindowClass
{
public:
	WindowClass(int& screenWidth, int& screenHeight, bool FULL_SCREEN);
	~WindowClass();

	// Static so that this method process messages for all windows.
	static bool ProcessMessages();

	HWND GetHwnd() const;
	HINSTANCE GetInstance() const;

	void CenterCursor() const;

private:
	// Application name.
	LPCWSTR m_applicationName;

	// Handle to the instance that contains the win proc.
	HINSTANCE m_hinstance;

	// Handle to the window.
	HWND m_hwnd;

	bool m_fullscreen;
};

// Window procedure that processes messages sent to a window.
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);