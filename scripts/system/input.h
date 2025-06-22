#pragma once

#include "../system/time.h"

#include <windows.h>
#include <map>

using namespace std;

enum KeyCode { W, A, S, D, Space, Esc, LeftMouse, RightMouse };

/// <summary>
/// 입력 값 처리 클래스
/// </summary>
class Input {
public:
	static void Start(HWND);
	static void Frame();

	/// <summary>
	/// 주어진 키가 눌려 있는지 확인한다.
	/// </summary>
	/// <param name="">키 코드</param>
	/// <returns>입력 여부</returns>
	static bool GetKeyDown(KeyCode);

	/// <summary>
	/// 마우스 커서의 X 좌표를 반환한다. (Client Area 기준)
	/// </summary>
	/// <returns>X 좌표</returns>
	static float GetMouseX();

	/// <summary>
	/// 마우스 커서의 Y 좌표를 반환한다. (Client Area 기준)
	/// </summary>
	/// <returns>Y 좌표</returns>
	static float GetMouseY();

	/// <summary>
	/// 일정 시간 동안 입력을 비활성화한다.
	/// </summary>
	static void StartCooldown();

private:
	static void SetKeyDown(KeyCode, bool);

private:
	static HWND m_hwnd;
	static map<KeyCode, bool> m_keyMap;

	static float m_cooldown;

	friend class Core;
};