#pragma once

#include <memory>

#include "SYS_windowclass.h"
#include "SYS_inputclass.h"
#include "SYS_timerclass.h"

#include "GFX_applicationclass.h"

class SystemClass
{
public:
	SystemClass();
	~SystemClass() = default;

	void Start();
	void Frame();

private:
	std::unique_ptr<WindowClass> m_Window;
	std::unique_ptr<InputClass> m_Input;
	std::unique_ptr<TimerClass> m_Timer;
	
	std::unique_ptr<ApplicationClass> m_Application;
};

