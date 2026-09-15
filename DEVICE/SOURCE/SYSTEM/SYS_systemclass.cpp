#include "SYS_systemclass.h"

SystemClass::SystemClass() 
{
	// Create window.
	int screenWidth{ 800 }, screenHeight{ 600 };
	m_Window = std::make_unique<WindowClass>(screenWidth, screenHeight, FULL_SCREEN);

	// Create application class.
	m_Application = std::make_unique<ApplicationClass>(screenWidth, screenHeight, m_Window->GetHwnd());

	// Create input class.
	m_Input = std::make_unique<InputClass>(m_Window->GetInstance(), m_Window->GetHwnd(), screenWidth, screenHeight);
}

void SystemClass::Start()
{
	MSG msg{}; // Windows system message
	ZeroMemory(&msg, sizeof(MSG));

	bool done{ false };
	bool frame_result;

	// Main loop of the program. 
	// Window messages are processed first, then the rest of the application.
	// Frame returns false if the user wanted to close the application, i.e. press ESC.
	while (!done)
	{
		// Handle windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Check if the application should close.
		if (msg.message == WM_QUIT)
			done = true;
		else
		{
			frame_result = Frame();
			if (!frame_result)
				done = true;
		}
	}

	return;
}

bool SystemClass::Frame()
{
	// First do the input frame processing.
	m_Input->Frame();

	// Do the frame processing of the application class.
	return m_Application->Frame(m_Input.get());
}