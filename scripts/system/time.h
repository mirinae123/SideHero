// 참고 자료 : https://rastertek.com/

#pragma once

#include <windows.h>

/// <summary>
/// 시간 클래스
/// </summary>
class Time {
public:
	/// <summary>
	/// 마지막 프레임으로부터 지난 시간을 반환한다.
	/// </summary>
	static float GetDeltaTime();

	/// <summary>
	/// 어플리케이션 시작으로부터 지난 시간을 반환한다.
	/// </summary>
	static float GetTimeSinceStart();

private:
	static bool Start();
	static void Frame();

private:
	static float m_deltaTime;
	static float m_frequency;

	static INT64 m_previousTime;
	static float m_timeSinceStart;

friend class Core;
};