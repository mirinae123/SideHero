#include "scene.h"

Scene::Scene(string sceneName) {
	m_sceneName = sceneName;
}

Scene::~Scene() {
	this->End();
}

string Scene::GetSceneName() {
	return m_sceneName;
}

list<unique_ptr<GameObject>>& Scene::GetGameObjects() {
	return m_gameObjects;
}

void Scene::InsertGameObject(unique_ptr<GameObject> gameObject) {
	if (m_isInitialized && gameObject->GetActive()) {
		gameObject->Start();
	}
	m_gameObjects.push_back(move(gameObject));
}

void Scene::EraseGameObject(unique_ptr<GameObject> gameObject) {
	m_gameObjects.remove(gameObject);
}

void Scene::Initialize() {
}

void Scene::Start() {
	m_gameObjects.clear();
	Initialize();

	for (const unique_ptr<GameObject>& gameObject : m_gameObjects) {
		gameObject->Start();
	}

	m_isInitialized = true;
}

void Scene::Frame() {
	for (const unique_ptr<GameObject>& gameObject : m_gameObjects) {
		if (gameObject->GetActive()) {
			gameObject->Frame();
		}
	}

	// 제거 플래그가 지정된 게임 오브젝트를 제거한다.
	auto it = m_gameObjects.begin();

	while (it != m_gameObjects.end()) {
		if ((*it)->IsDestroyed()) {
			(*it)->End();
			it = m_gameObjects.erase(it);
		}
		else {
			it++;
		}
	}
}

void Scene::Render() {
	for (const unique_ptr<GameObject>& gameObject : m_gameObjects) {
		if (gameObject->GetActive()) {
			gameObject->Render();
		}
	}
}

void Scene::End() {
	for (const unique_ptr<GameObject>& gameObject : m_gameObjects) {
		gameObject->End();
	}
}