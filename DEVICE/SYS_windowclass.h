#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class WindowClass
{
public:
	WindowClass(int screenWidth, int screenHeight, bool FULL_SCREEN);
	~WindowClass();

	HWND GetHwnd() const;
	HINSTANCE GetHinstance() const;

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