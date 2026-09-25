#pragma once

// The engine uses Direct Input for all keyboard, mouse, etc. inputs.

// Define the Direct Input Version
#define DIRECTINPUT_VERSION 0x0800

// Link libraries for Direct Input
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include <wrl/client.h> // Include for ComPtr

#include "SYS_d3d_exception.h"

class InputClass
{
public:
	InputClass(HINSTANCE, HWND, int, int);
	~InputClass() = default;

	void Frame();

	void GetMouseLocation(int&, int&);
	
public:
	bool IsEscapePressed() const;
	bool IsSpacePressed() const;
	
	bool IsMousePressed();

private:
	void ReadKeyboard();
	void ReadMouse();
	void ProcessInput();

private:
	Microsoft::WRL::ComPtr<IDirectInput8> m_directInput;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> m_keyboard;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight, m_mouseX, m_mouseY;
};