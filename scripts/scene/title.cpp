#include "title.h"

Title::Title(string name) : Scene(name) {
	m_startButton = 0;
	m_quitButton = 0;
}

void Title::Initialize() {
	Camera::SetPosition(XMFLOAT3{ 0.0f, 0.0f, 0.0f });

	// 시작 버튼
	unique_ptr<GameObject> start = make_unique<GameObject>("Sprite");
	start->InsertComponent(make_unique<Transform>(XMFLOAT3{ 0.0f, -100.0f, 0.0f }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f }, XMFLOAT3{ 4.0f, 0.8f, 1.0f }));
	start->InsertComponent(make_unique<Sprite>(0, TextureManager::GetTexture(TextureType::PlayerTexture)));
	start->InsertComponent(make_unique<Button>(start.get()));
	m_startButton = start->GetComponent<Button>();
	InsertGameObject(move(start));

	// 종료 버튼
	unique_ptr<GameObject> quit = make_unique<GameObject>("Sprite");
	quit->InsertComponent(make_unique<Transform>(XMFLOAT3{ 0.0f, -240.0f, 0.0f }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f }, XMFLOAT3{ 4.0f, 0.8f, 1.0f }));
	quit->InsertComponent(make_unique<Sprite>(0, TextureManager::GetTexture(TextureType::PlayerTexture)));
	quit->InsertComponent(make_unique<Button>(quit.get()));
	m_quitButton = quit->GetComponent<Button>();
	InsertGameObject(move(quit));

	// 배경 모델
	unique_ptr<GameObject> background = make_unique<GameObject>("Background");
	background->InsertComponent(make_unique<Transform>(XMFLOAT3{ 0.0f, 0.0f, 80.0f }));
	background->InsertComponent(make_unique<Model>(0, MeshManager::GetMesh(MeshType::BackgroundMesh), TextureManager::GetTexture(TextureType::BackgroundTexture)));
	InsertGameObject(move(background));
}

void Title::Frame() {
	Scene::Frame();

	// 시작 버튼 클릭
	if (m_startButton->GetClick()) {
		SceneManager::ChangeScene(SceneType::InGameScene);
	}

	// 종료 버튼 클릭
	if (m_quitButton->GetClick()) {
		PostQuitMessage(0);
	}
}
