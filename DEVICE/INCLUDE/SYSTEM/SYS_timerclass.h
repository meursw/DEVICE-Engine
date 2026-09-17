#pragma once
#include <windows.h>
#include "SYS_d3d_exception.h"

/// <summary>
/// Measures the exact time between frames of execution.
/// Used for objects that require a standard time frame for movement.
/// </summary>

class TimerClass
{
public:
    TimerClass();
    ~TimerClass() = default;

    void Frame();

    float GetDeltaTime() const;

private:
    INT64 m_startTime;
    float m_frequency;
    float m_deltaTime;
};
