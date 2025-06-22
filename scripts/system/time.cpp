#include "time.h"

float Time::m_deltaTime;
float Time::m_frequency;

INT64 Time::m_previousTime;
float Time::m_timeSinceStart;

float Time::GetDeltaTime() {
    return m_deltaTime;
}

float Time::GetTimeSinceStart() {
    return m_timeSinceStart;
}

bool Time::Start() {
    INT64 frequency;

    QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
    if (frequency == 0) {
        return false;
    }

    m_frequency = (float)frequency;

    QueryPerformanceCounter((LARGE_INTEGER*)&m_previousTime);

    return true;

}

void Time::Frame() {
    INT64 currentTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

    INT64 elapsedTicks = currentTime - m_previousTime;

    m_deltaTime = (float)elapsedTicks / m_frequency;
    m_timeSinceStart += m_deltaTime;

    m_previousTime = currentTime;

    return;

}