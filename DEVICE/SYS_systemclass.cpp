#include "SYS_systemclass.h"
#

SystemClass::SystemClass() {}

bool SystemClass::Initialize()
{
	// Create window.
	int screenWidth{ 800 }, screenHeight{ 600 };
	m_Window = std::make_unique<WindowClass>(screenWidth, screenHeight, FULL_SCREEN);

	// Create application class.
	m_Application = std::make_unique<ApplicationClass>();
	if (!m_Application->Initialize(screenWidth, screenHeight, m_Window->GetHwnd()))
		return false;
	
	// Create input class.
	m_Input = std::make_unique<InputClass>();
	if (!m_Input->Initialize(m_Window->GetInstance(), m_Window->GetHwnd(), screenWidth, screenHeight))
		return false;

	return true;
}

// while not done
// check for windows system messages
// process system messages
// process application loop
// check if user wanted to quit during the frame processing 
void SystemClass::Start()
{
	MSG msg{}; // Windows system message
	ZeroMemory(&msg, sizeof(MSG));

	bool done{ false };
	bool frame_result;

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
	if (!m_Input->Frame())
		return false;

	// Do the frame processing of the application class.
	if (!m_Application->Frame(m_Input.get()))
		return false;

	return true;
}