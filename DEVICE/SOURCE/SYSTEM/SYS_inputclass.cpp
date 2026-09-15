#include "SYS_inputclass.h"

InputClass::InputClass(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight) 
{
	HRESULT hr;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_mouseX = 0; m_mouseY = 0;

	// KEYBOARD CREATION //
	// Initialize the main direct input interface
	D3D_THROW(DirectInput8Create(
		hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL
	));

	// Initialize the direct input interface for the keyboard.
	D3D_THROW(m_directInput->CreateDevice(
		GUID_SysKeyboard, &m_keyboard, NULL
	));

	// Set the data format. In this case since it is a keyboard we can use the predefined data format.
	D3D_THROW(m_keyboard->SetDataFormat(
		&c_dfDIKeyboard
	));

	// Set the cooperative level of the keyboard to not share with other programs.
	D3D_THROW(m_keyboard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE
	));

	D3D_THROW(m_keyboard->Acquire());
	// END OF KEYBOARD CREATION //


	// MOUSE CREATION
	// Initialize the direct input interface for the mouse.
	D3D_THROW(m_directInput->CreateDevice(
		GUID_SysMouse, &m_mouse, NULL
	));

	// Set the data format for the mouse using the pre-defined mouse data format.
	D3D_THROW(m_mouse->SetDataFormat(
		&c_dfDIMouse
	));

	// Set the cooperative level of the mouse to share with other programs.
	D3D_THROW(m_mouse->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
	));

	// Acquire the mouse.
	D3D_THROW(m_mouse->Acquire());
	// END OF MOUSE CREATION
}

void InputClass::Frame()
{
	// Read the current state of the keyboard.
	ReadKeyboard();

	// Read the current state of the keyboard.
	ReadMouse();

	// Process the changes in the mouse and keyboard.
	ProcessInput();
}

void InputClass::ReadKeyboard()
{
	HRESULT hr;

	// Read the keyboard device.
	hr = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			D3D_THROW(hr);
		}
	}
}

void InputClass::ReadMouse()
{
	HRESULT hr;

	// Read the mouse device.
	hr = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(hr))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			D3D_THROW(hr);
		}
	}
}

void InputClass::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// Ensure the mouse location doesn't exceed the screen width or height.
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }

	return;
}

// This method shows how to read from the keyboard state.
bool InputClass::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
		return true;

	return false;
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}

bool InputClass::IsMousePressed()
{
	// Check the left mouse button state.
	if (m_mouseState.rgbButtons[0] & 0x80)
	{
		return true;
	}

	return false;
}