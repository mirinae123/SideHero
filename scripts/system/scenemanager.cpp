#include "scenemanager.h"

#include "../system/input.h"

#include "../scene/title.h"
#include "../scene/ingame.h"

vector<unique_ptr<Scene>> SceneManager::m_scenes;
int SceneManager::m_currentScene = -1;

void SceneManager::Start() {
	m_currentScene = 0;

	unique_ptr<Title> title = make_unique<Title>("Title");
	m_scenes.push_back(move(title));

	unique_ptr<InGame> inGame = make_unique<InGame>("InGame");
	m_scenes.push_back(move(inGame));

	m_scenes[m_currentScene]->Start();
}

void SceneManager::Frame() {
	m_scenes[m_currentScene]->Frame();
}

void SceneManager::Render() {
	m_scenes[m_currentScene]->Render();
}

void SceneManager::End() {
	if (m_currentScene != -1) {
		m_scenes[m_currentScene]->End();
	}
}

void SceneManager::ChangeScene(int newScene) {
	Input::StartCooldown();

	m_scenes[m_currentScene]->End();
	m_currentScene = newScene;
	m_scenes[m_currentScene]->Start();
}

Scene* SceneManager::GetCurrentScene() {
	return m_scenes[m_currentScene].get();
}
