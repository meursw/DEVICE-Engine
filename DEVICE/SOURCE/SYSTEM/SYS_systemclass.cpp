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

	// Do the frame processing of the application class.
	m_Application->Frame(m_Input.get());
}