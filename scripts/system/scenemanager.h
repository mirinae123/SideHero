#pragma once

#include <memory>
#include <vector>

#include "scene.h"
#include "mesh.h"
#include "texture.h"

#include "../component/model.h"
#include "../component/transform.h"

/// <summary>
/// 씬 종류
/// </summary>
enum SceneType { TitleScene, InGameScene };

/// <summary>
/// 씬들을 관리하는 클래스
/// </summary>
class SceneManager {
private:
	static void Start();
	static void Frame();
	static void Render();
	static void End();

public:
	/// <summary>
	/// 씬을 변경한다.
	/// </summary>
	/// <param name="">변경할 씬</param>
	static void ChangeScene(int);

	/// <summary>
	/// 현재 씬을 반환한다.
	/// </summary>
	/// <returns>씬에 대한 Raw Pointer</returns>
	static Scene* GetCurrentScene();

private:
	static vector<unique_ptr<Scene>> m_scenes;
	static int m_currentScene;

friend class Core;
};