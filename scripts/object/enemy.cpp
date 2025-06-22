#include "enemy.h"

inline float Lerp(float a, float b, float t) {
	if (t < 0.0f) {
		t = 0.0f;
	}
	else if (t > 1.0f) {
		t = 1.0f;
	}

	return (a * (1.0 - t)) + (b * t);
}

Enemy::Enemy(string name, GameObject* player) : GameObject(name) {
	m_player = dynamic_cast<Player*>(player);

	unique_ptr<Transform> transform = make_unique<Transform>();
	transform->scale = XMFLOAT3{ 0.5f, 0.5f, 1.0f };
	m_transform = transform.get();

	unique_ptr<Collider> collider = make_unique<Collider>(ColliderTag::EnemyTag, XMFLOAT2{ 0.0f, 0.0f }, XMFLOAT2{ 0.5f, 0.5f });
	m_collider = collider.get();

	unique_ptr<Model> model = make_unique<Model>(10, MeshManager::GetMesh(MeshType::PlayerMesh), TextureManager::GetTexture(TextureType::PlayerTexture));
	m_model = model.get();

	InsertComponent(move(transform));
	InsertComponent(move(collider));
	InsertComponent(move(model));

	m_rotation = Random::GetRandom(0.0f, PI2);
	m_elapsedTime = 0.0f;
	m_currentHealth = ENEMY_HEALTH;
	m_damageCooldown = ENEMY_DAMAGE_COOLDOWN;
	
	m_targetPosition = XMFLOAT3();
}

void Enemy::Frame() {
	GameState gameState = dynamic_cast<InGame*>(SceneManager::GetCurrentScene())->GetGameState();

	if (gameState == GameState::Menu) {
		return;
	}

	m_elapsedTime += Time::GetDeltaTime();

	// 피격 효과 처리
	if (m_damageCooldown < ENEMY_DAMAGE_COOLDOWN) {
		m_damageCooldown += Time::GetDeltaTime();

		if (m_damageCooldown >= ENEMY_DAMAGE_COOLDOWN) {
			GetComponent<Model>()->SetMultiplicativeColor(XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
		}
		else if (int(m_damageCooldown * 5.0f) % 2 == 0) {
			GetComponent<Model>()->SetMultiplicativeColor(XMFLOAT4{ 1.0f, 1.0f, 1.0f, 0.2f });
		}
		else {
			GetComponent<Model>()->SetMultiplicativeColor(XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
		}
	}

	// 목표 위치로 이동 (플레이어를 중심으로 타원형)
	m_targetPosition = m_player->GetComponent<Transform>()->position;

	float rotX = cos(m_elapsedTime) * 1.6f;
	float rotY = sin(m_elapsedTime);

	m_targetPosition.x += cos(m_rotation) * rotX - sin(m_rotation) * rotY;
	m_targetPosition.y += sin(m_rotation) * rotX + cos(m_rotation) + rotY + 0.05f;

	m_transform->position.x = Lerp(m_transform->position.x, m_targetPosition.x, Time::GetDeltaTime() * 1.0f);
	m_transform->position.y = Lerp(m_transform->position.y, m_targetPosition.y, Time::GetDeltaTime() * 1.0f);

	if (m_collider->CheckCollision(m_player->GetComponent<Collider>()->GetRect())) {
		m_player->DecreaseHealth(ENEMY_DAMAGE);
	}
}

void Enemy::DecreaseHealth(float amount) {
	if (m_damageCooldown < ENEMY_DAMAGE_COOLDOWN) {
		return;
	}

	m_damageCooldown = 0.0f;
	m_currentHealth -= amount;

	if (m_currentHealth < 0.0f) {
		Destroy();
	}
}
