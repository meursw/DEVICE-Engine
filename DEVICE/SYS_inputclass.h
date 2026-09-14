#pragma once

// The engine uses Direct Input for all keyboard, mouse, etc. inputs.

// Define the Direct Input Version
#define DIRECTINPUT_VERSION 0x0800

// Link libraries for Direct Input
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include <wrl/client.h> // Include for ComPtr

class InputClass
{
public:
	InputClass();
	~InputClass() = default;

	bool Initialize(HINSTANCE, HWND, int, int);

	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);
	bool IsMousePressed();

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	Microsoft::WRL::ComPtr<IDirectInput8> m_directInput;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> m_keyboard;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight, m_mouseX, m_mouseY;
};