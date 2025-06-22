// 참고 자료 : https://rastertek.com/

#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "direct3d.h"

#include "../system/input.h"
#include "../system/time.h"
#include "../system/scenemanager.h"
#include "../system/rendermanager.h"
#include "../system/meshmanager.h"
#include "../system/texturemanager.h"
#include "../system/random.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

/// <summary>
/// 코어 클래스
/// </summary>
class Core {
public:
	static bool Start();
	static void Run();
	static void End();

	static LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	static bool Frame();
	static bool Render();

	static LPCWSTR m_applicationName;
	static HINSTANCE m_hinstance;
	static HWND m_hwnd;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);