#include "ingame.h"

InGame::InGame(string name) : Scene(name) {
	m_player = 0;
	m_resumeButton = 0;
	m_quitButton = 0;

	m_healthBar = 0;
	m_healthBarBackground = 0;

	m_firstUpgradeButton = 0;
	m_secondUpgradeButton = 0;

	m_firstUpgradeSprite = 0;
	m_secondUpgradeSprite = 0;

	m_gameOverDisplay = 0;

	m_upgradeCooldown = 0.0f;

	m_firstUpgradeType = UpgradeType::Health;
	m_secondUpgradeType = UpgradeType::Health;

	m_gameState = GameState::Playing;

	m_enemySpawnCooldown = 0.0f;
	m_enemySpawnCoeff = 0;
}

void InGame::Initialize() {
	// 플랫폼
	unique_ptr<Floor> gameObject;

	for (int i = -12; i <= 12; i += 2) {
		gameObject = make_unique<Floor>("Floor", XMFLOAT3{ (float)i, -1.0f, Random::GetRandom(-0.1f, 0.1f) });
		InsertGameObject(move(gameObject));
	}

	for (int i = 8; i <= 20; i += 2) {
		gameObject = make_unique<Floor>("Floor", XMFLOAT3{ (float)i, 1.0f, Random::GetRandom(-0.1f, 0.1f) });
		InsertGameObject(move(gameObject));

		gameObject = make_unique<Floor>("Floor", XMFLOAT3{ -(float)i, 1.0f, Random::GetRandom(-0.1f, 0.1f) });
		InsertGameObject(move(gameObject));
	}

	for (int i = -6; i <= 6; i += 2) {
		gameObject = make_unique<Floor>("Floor", XMFLOAT3{ (float)i, 3.0f, Random::GetRandom(-0.1f, 0.1f) });
		InsertGameObject(move(gameObject));
	}

	for (int i = 4; i <= 16; i += 2) {
		gameObject = make_unique<Floor>("Floor", XMFLOAT3{ (float)i, 5.0f, Random::GetRandom(-0.1f, 0.1f) });
		InsertGameObject(move(gameObject));

		gameObject = make_unique<Floor>("Floor", XMFLOAT3{ -(float)i, 5.0f, Random::GetRandom(-0.1f, 0.1f) });
		InsertGameObject(move(gameObject));
	}

	for (int i = -6; i <= 6; i += 2) {
		gameObject = make_unique<Floor>("Floor", XMFLOAT3{ (float)i, 7.0f, Random::GetRandom(-0.1f, 0.1f) });
		InsertGameObject(move(gameObject));
	}

	for (int i = 8; i <= 20; i += 2) {
		gameObject = make_unique<Floor>("Floor", XMFLOAT3{ (float)i, 9.0f, Random::GetRandom(-0.1f, 0.1f) });
		InsertGameObject(move(gameObject));

		gameObject = make_unique<Floor>("Floor", XMFLOAT3{ -(float)i, 9.0f, Random::GetRandom(-0.1f, 0.1f) });
		InsertGameObject(move(gameObject));
	}

	// 플레이어
	unique_ptr<Player> player = make_unique<Player>("Player", this, XMFLOAT3{ 0.0f, 1.0f, 0.0f });
	m_player = player.get();
	InsertGameObject(move(player));

	// 배경 모델
	unique_ptr<GameObject> background = make_unique<GameObject>("Background");
	background->InsertComponent(make_unique<Transform>(XMFLOAT3{ 0.0f, 0.0f, 80.0f }));
	background->InsertComponent(make_unique<Model>(0, MeshManager::GetMesh(MeshType::BackgroundMesh), TextureManager::GetTexture(TextureType::BackgroundTexture)));
	InsertGameObject(move(background));

	// 재개 버튼
	unique_ptr<GameObject> resume = make_unique<GameObject>("Sprite");
	resume->InsertComponent(make_unique<Transform>(XMFLOAT3{ 0.0f, 80.0f, 0.0f }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f }, XMFLOAT3{ 4.0f, 0.8f, 1.0f }));
	resume->InsertComponent(make_unique<Sprite>(0, TextureManager::GetTexture(TextureType::PlayerTexture)));
	resume->InsertComponent(make_unique<Button>(resume.get()));
	resume->SetActive(false);
	m_resumeButton = resume->GetComponent<Button>();
	InsertGameObject(move(resume));

	// 종료 버튼
	unique_ptr<GameObject> quit = make_unique<GameObject>("Sprite");
	quit->InsertComponent(make_unique<Transform>(XMFLOAT3{ 0.0f, -70.0f, 0.0f }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f }, XMFLOAT3{ 4.0f, 0.8f, 1.0f }));
	quit->InsertComponent(make_unique<Sprite>(0, TextureManager::GetTexture(TextureType::PlayerTexture)));
	quit->InsertComponent(make_unique<Button>(quit.get()));
	quit->SetActive(false);
	m_quitButton = quit->GetComponent<Button>();
	InsertGameObject(move(quit));

	// 체력 바
	unique_ptr<GameObject> healthBar = make_unique<GameObject>("Sprite");
	healthBar->InsertComponent(make_unique<Transform>(XMFLOAT3{ 0.0f, -200.0f, 0.0f }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f }, XMFLOAT3{ 3.95f, 0.24f, 1.0f }));
	healthBar->InsertComponent(make_unique<Sprite>(1, TextureManager::GetTexture(TextureType::HealthBarTexture)));
	m_healthBar = healthBar->GetComponent<Transform>();
	InsertGameObject(move(healthBar));

	// 체력 바 배경
	unique_ptr<GameObject> healthBarBackground = make_unique<GameObject>("Sprite");
	healthBarBackground->InsertComponent(make_unique<Transform>(XMFLOAT3{ 0.0f, -200.0f, 0.0f }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f }, XMFLOAT3{ 4.0f, 0.3f, 1.0f }));
	healthBarBackground->InsertComponent(make_unique<Sprite>(0, TextureManager::GetTexture(TextureType::HealthBarBackgroundTexture)));
	m_healthBarBackground = healthBarBackground->GetComponent<Transform>();
	InsertGameObject(move(healthBarBackground));

	// 업그레이드 버튼 1
	unique_ptr<GameObject> firstUpgrade = make_unique<GameObject>("Sprite");
	firstUpgrade->InsertComponent(make_unique<Transform>(XMFLOAT3{ -60.0f, -290.0f, 0.0f }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f }, XMFLOAT3{ 0.7f, 0.7f, 1.0f }));
	firstUpgrade->InsertComponent(make_unique<Sprite>(0, TextureManager::GetTexture(TextureType::IntervalTexture)));
	firstUpgrade->InsertComponent(make_unique<Button>(firstUpgrade.get()));
	firstUpgrade->SetActive(false);
	m_firstUpgradeButton = firstUpgrade->GetComponent<Button>();
	m_firstUpgradeSprite = firstUpgrade->GetComponent<Sprite>();
	InsertGameObject(move(firstUpgrade));

	// 업그레이드 버튼 2
	unique_ptr<GameObject> secondUpgrade = make_unique<GameObject>("Sprite");
	secondUpgrade->InsertComponent(make_unique<Transform>(XMFLOAT3{ 60.0f, -290.0f, 0.0f }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f }, XMFLOAT3{ 0.7f, 0.7f, 1.0f }));
	secondUpgrade->InsertComponent(make_unique<Sprite>(0, TextureManager::GetTexture(TextureType::HealthTexture)));
	secondUpgrade->InsertComponent(make_unique<Button>(secondUpgrade.get()));
	secondUpgrade->SetActive(false);
	m_secondUpgradeButton = secondUpgrade->GetComponent<Button>();
	m_secondUpgradeSprite = secondUpgrade->GetComponent<Sprite>();
	InsertGameObject(move(secondUpgrade));

	// 게임 오버 화면
	unique_ptr<GameObject> gameOver = make_unique<GameObject>("Sprite");
	gameOver->InsertComponent(make_unique<Transform>(XMFLOAT3{ 0.0f, 100.0f, 0.0f }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f }, XMFLOAT3{ 6.0f, 1.2f, 1.0f }));
	gameOver->InsertComponent(make_unique<Sprite>(0, TextureManager::GetTexture(TextureType::PlayerTexture)));
	gameOver->SetActive(false);
	m_gameOverDisplay = gameOver.get();
	InsertGameObject(move(gameOver));

	m_upgradeCooldown = 1.0f;
	m_enemySpawnCooldown = ENEMY_SPAWN_COOLDOWN / INITIAL_COOLDOWN_MODIFIER;
	m_enemySpawnCoeff = INITIAL_ENEMY_COEFF;
}

GameState InGame::GetGameState() {
	return m_gameState;
}

void InGame::Start() {
	Scene::Start();

	Camera::SetRotation(XMFLOAT3(8.0f, 0.0f, 0.0f));

	m_gameState = GameState::Playing;

	m_resumeButton->GetGameObject()->SetActive(false);
	m_quitButton->GetGameObject()->SetActive(false);
}

void InGame::Frame() {
	Scene::Frame();

	// 메뉴가 켜져 있는 경우
	if (m_gameState == GameState::Menu) {
		if (m_resumeButton->GetClick()) {
			m_gameState = GameState::Playing;

			m_resumeButton->GetGameObject()->SetActive(false);
			m_quitButton->GetGameObject()->SetActive(false);

			m_firstUpgradeButton->SetActive(true);
			m_secondUpgradeButton->SetActive(true);
		}
		
		if (m_quitButton->GetClick()) {
			SceneManager::ChangeScene(0);
		}
	}
	// 게임이 진행 중인 경우
	else if (m_gameState == GameState::Playing) {
		m_healthBar->position.x = (PLAYER_HEALTH - m_player->GetCurrentHealth()) * -2.56f;
		m_healthBar->scale.x = m_player->GetCurrentHealth() / PLAYER_HEALTH * 3.95f;

		// 게임 오버 처리
		if (m_player->GetCurrentHealth() <= 0.0f) {
			m_gameState = GameState::GameOver;

			m_healthBar->GetGameObject()->SetActive(false);
			m_healthBarBackground->GetGameObject()->SetActive(false);
			m_firstUpgradeButton->GetGameObject()->SetActive(false);
			m_secondUpgradeButton->GetGameObject()->SetActive(false);

			m_quitButton->GetGameObject()->GetComponent<Transform>()->position.y = -150.0f;
			m_quitButton->GetGameObject()->SetActive(true);

			m_gameOverDisplay->SetActive(true);
		}

		// 업그레이드 항목 관리
		if (m_upgradeCooldown > 0.0f) {
			m_upgradeCooldown -= Time::GetDeltaTime();

			if (m_upgradeCooldown <= 0.0f) {
				m_firstUpgradeType = (UpgradeType)Random::GetRandom(0, 2);
				m_secondUpgradeType = (UpgradeType)Random::GetRandom(0, 2);

				while (m_firstUpgradeType == m_secondUpgradeType) {
					m_secondUpgradeType = (UpgradeType)Random::GetRandom(0, 2);
				}

				switch (m_firstUpgradeType) {
					case UpgradeType::Health: {
						m_firstUpgradeSprite->SetTexture(TextureManager::GetTexture(TextureType::HealthTexture));
						break;
					}
					case UpgradeType::Speed: {
						m_firstUpgradeSprite->SetTexture(TextureManager::GetTexture(TextureType::SpeedTexture));
						break;
					}
					case UpgradeType::Interval: {
						m_firstUpgradeSprite->SetTexture(TextureManager::GetTexture(TextureType::IntervalTexture));
						break;
					}
				}

				switch (m_secondUpgradeType) {
				case UpgradeType::Health: {
					m_secondUpgradeSprite->SetTexture(TextureManager::GetTexture(TextureType::HealthTexture));
					break;
				}
				case UpgradeType::Speed: {
					m_secondUpgradeSprite->SetTexture(TextureManager::GetTexture(TextureType::SpeedTexture));
					break;
				}
				case UpgradeType::Interval: {
					m_secondUpgradeSprite->SetTexture(TextureManager::GetTexture(TextureType::IntervalTexture));
					break;
				}
				}

				m_firstUpgradeButton->GetGameObject()->SetActive(true);
				m_secondUpgradeButton->GetGameObject()->SetActive(true);
			}
		}
		else {
			if (m_firstUpgradeButton->GetClick()) {
				m_firstUpgradeButton->GetGameObject()->SetActive(false);
				m_secondUpgradeButton->GetGameObject()->SetActive(false);

				switch (m_firstUpgradeType) {
					case UpgradeType::Health: {
						m_player->IncreaseHealth(HEALTH_COEFF);
						break;
					}
					case UpgradeType::Speed: {
						m_player->IncreaseSpeed();
						break;
					}
					case UpgradeType::Interval: {
						m_player->DecreaseInterval();
						break;
					}
				}

				m_upgradeCooldown = 3.0f;
			}
			else if (m_secondUpgradeButton->GetClick()) {
				m_firstUpgradeButton->GetGameObject()->SetActive(false);
				m_secondUpgradeButton->GetGameObject()->SetActive(false);

				switch (m_secondUpgradeType) {
				case UpgradeType::Health: {
					m_player->IncreaseHealth(HEALTH_COEFF);
					break;
				}
				case UpgradeType::Speed: {
					m_player->IncreaseSpeed();
					break;
				}
				case UpgradeType::Interval: {
					m_player->DecreaseInterval();
					break;
				}
				}

				m_player->DecreaseInterval();
				m_upgradeCooldown = 3.0f;
			}
		}

		// 메뉴 열기 처리
		if (Input::GetKeyDown(KeyCode::Esc)) {
			m_gameState = GameState::Menu;

			m_resumeButton->GetGameObject()->SetActive(true);
			m_quitButton->GetGameObject()->SetActive(true);

			m_firstUpgradeButton->SetActive(false);
			m_secondUpgradeButton->SetActive(false);
		}

		m_enemySpawnCooldown -= Time::GetDeltaTime();

		// 적 스폰 처리
		if (m_enemySpawnCooldown < 0.0f) {
			for (int i = 0; i < m_enemySpawnCoeff; i++) {
				float rand = Random::GetRandom(0.0f, PI2);

				XMFLOAT3 newPosition = m_player->GetComponent<Transform>()->position;
				newPosition.x += cos(rand) * 16.0f;
				newPosition.y += sin(rand) * 16.0f;

				unique_ptr<Enemy> enemy = make_unique<Enemy>("Enemy", m_player);
				enemy->Initialize(m_player);
				enemy->GetComponent<Transform>()->position = newPosition;
				InsertGameObject(move(enemy));
			}

			m_enemySpawnCooldown += ENEMY_SPAWN_COOLDOWN;
			m_enemySpawnCoeff += ENEMY_COEFF_INCREASEMENT;
		}
	}
	// 게임 오버된 경우
	else {
		if (m_quitButton->GetClick()) {
			SceneManager::ChangeScene(0);
		}
	}
}
