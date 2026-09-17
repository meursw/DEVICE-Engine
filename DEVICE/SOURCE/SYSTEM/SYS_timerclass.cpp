#include "SYS_timerclass.h"

TimerClass::TimerClass()
{
	INT64 frequency{};

	// Retrieves the frequency of the performance counter. 
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	if (frequency == 0)
	{
		// Convert the error that would occur into an HRESULT.
		HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
		D3D_THROW(hr);
	}

	// Store the frequency as a floating point.
	m_frequency = (float)frequency;

	// Retrieves the current value of the performance counter, which is the initial start time.
	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

}

// The Frame method is called for every single loop of execution by the main program.
// This way we can calculate the difference of time between loops and determine the time
// it took to execute this frame.

void TimerClass::Frame()
{
	INT64 currentTime{};
	INT64 elapsedTicks{};

	// Query current time.
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	// Calculate the difference in time since the last time we queried for the current time.
	elapsedTicks = currentTime - m_startTime;

	// Calculate the frame time.
	m_deltaTime = (float)elapsedTicks / m_frequency;

	// Restart the timer.
	m_startTime = currentTime;
}

float TimerClass::GetDeltaTime() const
{
	return m_deltaTime;
}
