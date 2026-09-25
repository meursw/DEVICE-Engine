#include "SYS_systemclass.h"

SystemClass::SystemClass() 
{
	// Initialize for WIC functionality
	HRESULT hr;
	D3D_THROW(CoInitializeEx(nullptr, COINIT_MULTITHREADED));

	// Create window.
	int screenWidth{ (int)(800 * 1.5) }, screenHeight{ (int)(600 * 1.5) };
	m_Window = std::make_unique<WindowClass>(screenWidth, screenHeight, FULL_SCREEN);

	// Create application class.
	m_Application = std::make_unique<ApplicationClass>(screenWidth, screenHeight, m_Window->GetHwnd());

	// Create input class.
	m_Input = std::make_unique<InputClass>(m_Window->GetInstance(), m_Window->GetHwnd(), screenWidth, screenHeight);

	m_Timer = std::make_unique<TimerClass>();

}

void SystemClass::Start()
{
	// Main loop of the program. 
	// Window messages are processed first, then the rest of the application.
	// Frame returns false if the user wanted to close the application, i.e. press ESC.
	
	// If Process Messages returns false it means that there was a QUIT message.
	while (m_Window->ProcessMessages())
		Frame();

	return;
}

void SystemClass::Frame()
{
	// First do the input frame processing.
	m_Input->Frame();

	// Check if the user wants to close the application.
	if (m_Input->IsEscapePressed())
	{
		PostQuitMessage(0);
		return;
	}

	// Update timer status.
	m_Timer->Frame();

	// Do the frame processing of the application class.
	m_Application->Frame(m_Input.get(), m_Timer->GetDeltaTime());

}