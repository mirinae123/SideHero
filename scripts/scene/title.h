#pragma once

#include "../system/scenemanager.h"

#include "../system/scene.h"
#include "../system/camera.h"

#include "../component/transform.h"
#include "../component/model.h"
#include "../component/sprite.h"
#include "../component/button.h"

/// <summary>
/// 타이틀 씬
/// </summary>
class Title : public Scene {
public:
	Title(string);
	void Initialize();

private:
	void Frame();

private:
	Button* m_startButton;
	Button* m_quitButton;
};