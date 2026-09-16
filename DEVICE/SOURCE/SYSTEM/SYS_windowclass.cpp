#include "SYS_windowclass.h"

WindowClass::WindowClass(int screenWidth, int screenHeight, bool FULL_SCREEN)
{
	// Create window class.
	m_applicationName = L"DEVICE";
	m_hinstance = GetModuleHandle(nullptr);
	m_fullscreen = FULL_SCREEN;

	WNDCLASSEX wc{ 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;

	// Register window class to the operating system.
	RegisterClassEx(&wc);

	// Check if the window should be full screen.
	// Set the position of the window.
	int posX, posY;
	if (m_fullscreen)
	{
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		DEVMODE dm;
		memset(&dm, 0, sizeof(dm));
		dm.dmSize = sizeof(dm);
		dm.dmPelsWidth = (unsigned long)screenWidth;
		dm.dmPelsHeight = (unsigned long)screenHeight;
		dm.dmBitsPerPel = 32;
		dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dm, CDS_FULLSCREEN);

		posX = posY = 0;

	}
	else
	{
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create an instance of the window and get a handle to it.
	m_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL
	);

	/*m_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL
	);*/

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);
}

WindowClass::~WindowClass()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	if(m_fullscreen)
		ChangeDisplaySettings(NULL, 0);

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;
}

bool WindowClass::ProcessMessages()
{
	MSG msg{}; // Windows system message

	// Handle windows messages.
	// We use PeekMessage instead of GetMessage.
	// This is because GetMessage blocks when a message is not being sent.
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
	{
		// Check if the application should close.
		if (msg.message == WM_QUIT)
			return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	
	return true;
}

// Here we handle the messages concerned with destroying the window or closing the window
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	// Check if window is being destroyed and close the application.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if window is being closed and close the application.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Here we pass the message to the default window procedure function
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);

	}
}

HWND WindowClass::GetHwnd() const
{
	return m_hwnd;
}

HINSTANCE WindowClass::GetInstance() const
{
	return m_hinstance;
}