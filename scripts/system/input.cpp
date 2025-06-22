#include "input.h"

map<KeyCode, bool> Input::m_keyMap;
HWND Input::m_hwnd;

float Input::m_cooldown = 0.0f;

void Input::Start(HWND hwnd) {
	m_hwnd = hwnd;
}

void Input::Frame() {
	if (m_cooldown > 0.0f) {
		m_cooldown -= Time::GetDeltaTime();

		return;
	}

	if (m_hwnd != GetForegroundWindow()) {
		auto it = m_keyMap.begin();

		while (it != m_keyMap.end()) {
			it->second = false;
			it++;
		}

		return;
	}

	if (GetAsyncKeyState('W') & 0x8000) {
		m_keyMap[KeyCode::W] = true;
	}
	else {
		m_keyMap[KeyCode::W] = false;
	}

	if (GetAsyncKeyState('A') & 0x8000) {
		m_keyMap[KeyCode::A] = true;
	}
	else {
		m_keyMap[KeyCode::A] = false;
	}

	if (GetAsyncKeyState('S') & 0x8000) {
		m_keyMap[KeyCode::S] = true;
	}
	else {
		m_keyMap[KeyCode::S] = false;
	}

	if (GetAsyncKeyState('D') & 0x8000) {
		m_keyMap[KeyCode::D] = true;
	}
	else {
		m_keyMap[KeyCode::D] = false;
	}

	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
		m_keyMap[KeyCode::Esc] = true;
	}
	else {
		m_keyMap[KeyCode::Esc] = false;
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		m_keyMap[KeyCode::Space] = true;
	}
	else {
		m_keyMap[KeyCode::Space] = false;
	}

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
		m_keyMap[KeyCode::LeftMouse] = true;
	}
	else {
		m_keyMap[KeyCode::LeftMouse] = false;
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
		m_keyMap[KeyCode::RightMouse] = true;
	}
	else {
		m_keyMap[KeyCode::RightMouse] = false;
	}
}

bool Input::GetKeyDown(KeyCode keyCode) {
	return m_keyMap[keyCode];
}

float Input::GetMouseX() {
	POINT point;
	RECT rect;

	GetCursorPos(&point);
	GetWindowRect(m_hwnd, &rect);

	return point.x - rect.left;
}

float Input::GetMouseY() {
	POINT point;
	RECT rect;

	GetCursorPos(&point);
	GetWindowRect(m_hwnd, &rect);

	return point.y - rect.top;
}

void Input::StartCooldown() {
	m_cooldown = 0.1f;

	auto it = m_keyMap.begin();

	while (it != m_keyMap.end()) {
		it->second = false;
		it++;
	}
}

void Input::SetKeyDown(KeyCode keyCode, bool isKeyDown) {
	m_keyMap[keyCode] = isKeyDown;
}