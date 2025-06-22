// 참고 자료 : https://rastertek.com/

#include "core.h"

LPCWSTR Core::m_applicationName;
HINSTANCE Core::m_hinstance;
HWND Core::m_hwnd;

bool Core::Start() {
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;

	float screenWidth, screenHeight;
	int posX, posY;

	m_applicationName = L"Side Hero";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DROPSHADOW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN) {
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;

	}
	else {
		screenWidth = 1366;
		screenHeight = 768;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
			 posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	if (!Direct3D::Start(screenWidth, screenHeight, VSYNC_ENABLED, m_hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR)) {
		return false;
	}

	if (!TextureShader::Start()) {
		return false;
	}

	if (!Time::Start()) {
		return false;
	}

	MeshManager::Start();
	TextureManager::Start();

	Input::Start(m_hwnd);
	Random::Start();

	SceneManager::Start();

	return true;
}

void Core::End() {
	SceneManager::End();
}

void Core::Run() {
	MSG msg;

	ZeroMemory(&msg, sizeof(MSG));

	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) {
			break;
		}

		bool result = Frame();
		if (!result) break;
	}

	return;
}

bool Core::Frame() {
	Time::Frame();
	SceneManager::Frame();
	Input::Frame();

	if (!Render()) {
		return false;
	}
	
	return true;
}

bool Core::Render() {
	RenderManager::Render();

	return true;
}

LRESULT CALLBACK Core::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg) {
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}
		case WM_CLOSE: {
			PostQuitMessage(0);
			return 0;
		}
		default: {
			return Core::MessageHandler(hwnd, umsg, wparam, lparam);
		}
	}
}