#pragma once

#include <memory>
#include <list>
#include <string>

#include "gameobject.h"

using namespace std;

/// <summary>
/// 씬 클래스
/// </summary>
class Scene : public enable_shared_from_this<Scene> {
public:
	Scene(string);
	Scene(const Scene&) = delete;
	~Scene();

	string GetSceneName();

	/// <summary>
	/// 씬에 존재하는 게임 오브젝트들을 반환한다.
	/// </summary>
	/// <returns>게임 오브젝트 Set</returns>
	list<unique_ptr<GameObject>>& GetGameObjects();

	/// <summary>
	/// 씬에 게임 오브젝트를 추가한다.
	/// </summary>
	/// <param name="">게임 오브젝트에 대한 Unique Pointer</param>
	void InsertGameObject(unique_ptr<GameObject>);

	/// <summary>
	/// 씬에서 게임 오브젝트를 제거한다.
	/// </summary>
	/// <param name="">게임 오브젝트에 대한 Unique Pointer</param>
	void EraseGameObject(unique_ptr<GameObject>);

	virtual void Initialize();

protected:
	virtual void Start();
	virtual void Frame();
	virtual void Render();
	virtual void End();

protected:
	string m_sceneName;
	list<unique_ptr<GameObject>> m_gameObjects;

	bool m_isInitialized = false;

friend class SceneManager;
};