#include "playerpart.h"
#include "player.h"

inline float Lerp(float a, float b, float t) {
	if (t < 0.0f) {
		t = 0.0f;
	}
	else if (t > 1.0f) {
		t = 1.0f;
	}

	return (a * (1.0 - t)) + (b * t);
}

PlayerPart::PlayerPart(string name, GameObject* player, BodyType bodyType) : GameObject(name) {
	unique_ptr<Transform> transform = make_unique<Transform>();
	m_transform = transform.get();
	m_collider = 0;

	m_player = dynamic_cast<Player*>(player);

	m_bodyType = bodyType;
	m_damageCooldown = 0.0f;

	InsertComponent(move(transform));

	switch (m_bodyType) {
	case BodyType::Head: {
		InsertComponent(make_unique<Model>(4, MeshManager::GetMesh(MeshType::PlayerMesh), TextureManager::GetTexture(TextureType::PlayerTexture)));
		m_transform->scale = XMFLOAT3{ 0.4f, 0.4f, 0.4f };
		break;
	}
	case BodyType::Body: {
		InsertComponent(make_unique<Model>(3, MeshManager::GetMesh(MeshType::PlayerMesh), TextureManager::GetTexture(TextureType::PlayerTexture)));
		m_transform->scale = XMFLOAT3{ 0.8f, 0.4f, 1.0f };
		break;
	}
	case BodyType::Hip: {
		InsertComponent(make_unique<Model>(1, MeshManager::GetMesh(MeshType::PlayerMesh), TextureManager::GetTexture(TextureType::PlayerTexture)));
		m_transform->scale = XMFLOAT3{ 0.4f, 0.4f, 1.0f };
		break;
	}
	case BodyType::LeftHand: {
		InsertComponent(make_unique<Model>(4, MeshManager::GetMesh(MeshType::PlayerMesh), TextureManager::GetTexture(TextureType::PlayerTexture)));
		m_transform->scale = XMFLOAT3{ 0.2f, 0.2f, 0.2f };

		unique_ptr<Collider> collider = make_unique<Collider>(ColliderTag::None, XMFLOAT2{ 0.0f, 0.0f }, XMFLOAT2{ 0.5f, 0.5f });
		m_collider = collider.get();
		InsertComponent(move(collider));
		break;
	}
	default: {
		InsertComponent(make_unique<Model>(4, MeshManager::GetMesh(MeshType::PlayerMesh), TextureManager::GetTexture(TextureType::PlayerTexture)));
		m_transform->scale = XMFLOAT3{ 0.2f, 0.2f, 0.2f };

		unique_ptr<Collider> collider = make_unique<Collider>(ColliderTag::None, XMFLOAT2{ 0.0f, 0.0f }, XMFLOAT2{ 0.5f, 0.5f });
		m_collider = collider.get();
		InsertComponent(move(collider));
		break;
	}
	};
}

void PlayerPart::Frame() {
	if (dynamic_cast<InGame*>(SceneManager::GetCurrentScene())->GetGameState() == GameState::GameOver) {
		return;
	}

	// 피격 효과 처리
	if (m_damageCooldown < 1.0f) {
		m_damageCooldown += Time::GetDeltaTime();

		if (m_damageCooldown >= 1.0f) {
			GetComponent<Model>()->SetMultiplicativeColor(XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
		}
		else if (int(m_damageCooldown * 5.0f) % 2 == 0) {
			GetComponent<Model>()->SetMultiplicativeColor(XMFLOAT4{ 1.0f, 1.0f, 1.0f, 0.2f });
		}
		else {
			GetComponent<Model>()->SetMultiplicativeColor(XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
		}
	}

	// 위치 갱신
	switch (m_bodyType) {
		case BodyType::Head: {
			m_transform->position.x = m_player->GetTargetPosition(BodyType::Head).x;
			m_transform->position.y = Lerp(m_transform->position.y, m_player->GetTargetPosition(BodyType::Head).y, Time::GetDeltaTime() * (m_player->GetVelocity().y > 0 ? 64.0f : 16.0f));
			break;
		}
		case BodyType::Body: {
			m_transform->position.x = Lerp(m_transform->position.x, m_player->GetTargetPosition(BodyType::Body).x, Time::GetDeltaTime() * 32.0f);
			m_transform->position.y = Lerp(m_transform->position.y, m_player->GetTargetPosition(BodyType::Body).y, Time::GetDeltaTime() * 32.0f);
			break;
		}
		case BodyType::Hip: {
			m_transform->position.x = Lerp(m_transform->position.x, m_player->GetTargetPosition(BodyType::Hip).x, Time::GetDeltaTime() * 16.0f);
			m_transform->position.y = Lerp(m_transform->position.y, m_player->GetTargetPosition(BodyType::Hip).y, Time::GetDeltaTime() * (m_player->GetVelocity().y > 0 ? 16.0f : 64.0f));
			break;
		}
		case BodyType::LeftHand: {
			m_transform->position.x = Lerp(m_transform->position.x, m_player->GetTargetPosition(BodyType::LeftHand).x, Time::GetDeltaTime() * 16.0f);
			m_transform->position.y = Lerp(m_transform->position.y, m_player->GetTargetPosition(BodyType::LeftHand).y, Time::GetDeltaTime() * 16.0f);
			break;
		}
		default: {
			m_transform->position.x = Lerp(m_transform->position.x, m_player->GetTargetPosition(BodyType::RightHand).x, Time::GetDeltaTime() * 16.0f);
			m_transform->position.y = Lerp(m_transform->position.y, m_player->GetTargetPosition(BodyType::RightHand).y, Time::GetDeltaTime() * 16.0f);
			break;
		}
	};

	// 바디 타입이 LeftHand, RightHand인 경우, 적 공격 처리
	GameState gameState = dynamic_cast<InGame*>(SceneManager::GetCurrentScene())->GetGameState();

	if (gameState == GameState::Playing && m_player->GetIsAttacking() && (m_bodyType == BodyType::LeftHand || m_bodyType == BodyType::RightHand)) {
		for (const unique_ptr<GameObject>& gameObject : SceneManager::GetCurrentScene()->GetGameObjects()) {
			Collider* otherCollider = gameObject->GetComponent<Collider>();

			if (otherCollider != nullptr && otherCollider->tag == ColliderTag::EnemyTag && m_collider->CheckCollision(otherCollider->GetRect())) {
				dynamic_cast<Enemy*>(otherCollider->GetGameObject())->DecreaseHealth(10.0f);
			}
		}
	}
}

void PlayerPart::ApplyDamage() {
	m_damageCooldown = 0.0f;
}
