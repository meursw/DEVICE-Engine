#pragma once

#include <memory>

#include "SYS_windowclass.h"
#include "GFX_applicationclass.h"
#include "SYS_inputclass.h"

class SystemClass
{
public:
	SystemClass();
	~SystemClass() = default;

	void Start();
	void Frame();

private:
	std::unique_ptr<WindowClass> m_Window;
	std::unique_ptr<ApplicationClass> m_Application;
	std::unique_ptr<InputClass> m_Input;
};

