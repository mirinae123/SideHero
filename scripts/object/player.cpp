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

Player::Player(string name, Scene* scene, XMFLOAT3 position) : GameObject(name) {
	m_isAttacking = false;
	m_attackTargetDuration = 0.0f;
	m_attackDuration = 0.0f;
	m_attackCooldown = 0.0f;

	unique_ptr<Transform> transform = make_unique<Transform>(position);
	unique_ptr<Collider> collider = make_unique<Collider>(ColliderTag::None, XMFLOAT2{ 0.0f, 0.0f }, XMFLOAT2{ 0.1f, 0.5f });

	m_transform = transform.get();
	m_collider = collider.get();

	InsertComponent(move(transform));
	InsertComponent(move(collider));

	// 바디 오브젝트 생성
	unique_ptr<PlayerPart> head = make_unique<PlayerPart>("Head", this, BodyType::Head);
	m_playerParts.push_back(head.get());
	scene->InsertGameObject(move(head));

	unique_ptr<PlayerPart> body = make_unique<PlayerPart>("Body", this, BodyType::Body);
	m_playerParts.push_back(body.get());
	scene->InsertGameObject(move(body));

	unique_ptr<PlayerPart> hip = make_unique<PlayerPart>("Hip", this, BodyType::Hip);
	m_playerParts.push_back(hip.get());
	scene->InsertGameObject(move(hip));

	unique_ptr<PlayerPart> leftHand = make_unique<PlayerPart>("LeftHand", this, BodyType::LeftHand);
	m_playerParts.push_back(leftHand.get());
	scene->InsertGameObject(move(leftHand));

	unique_ptr<PlayerPart> rightHand = make_unique<PlayerPart>("RightHand", this, BodyType::RightHand);
	m_playerParts.push_back(rightHand.get());
	scene->InsertGameObject(move(rightHand));

	m_xVelocity = 0.0f;
	m_yVelocity = 0.0f;

	m_isGrounded = false;

	m_headTarget = m_transform->position + XMFLOAT3{ 0.0f, 0.8f, 0.0f };
	m_bodyTarget = m_transform->position + XMFLOAT3{ 0.0f, 0.4f, 0.0f };
	m_hipTarget = m_transform->position + XMFLOAT3{ 0.0f, 0.2f, 0.0f };
	m_leftHandTarget = m_transform->position + XMFLOAT3{ -0.5f, 0.0f, 0.0f };
	m_rightHandTarget = m_transform->position + XMFLOAT3{ 0.5f, 0.0f, 0.0f };

	m_currentHealth = 100.0f;
	m_attackInterval = PLAYER_INITIAL_INTERVAL;
	m_attackSpeed = PLAYER_INITIAL_SPEED;

	m_damageCooldown = PLAYER_DAMAGE_COOLDOWN;

	m_cameraPosition = m_transform->position;
}

void Player::Frame() {
	GameState gameState = dynamic_cast<InGame*>(SceneManager::GetCurrentScene())->GetGameState();

	if (gameState == GameState::Menu) {
		return;
	}

	// 좌우 입력 처리
	if (Input::GetKeyDown(KeyCode::A) && gameState != GameState::GameOver) {
		m_xVelocity -= 0.3f;
	}

	if (Input::GetKeyDown(KeyCode::D) && gameState != GameState::GameOver) {
		m_xVelocity += 0.3f;
	}

	if (m_xVelocity < -3.0f) {
		m_xVelocity = -3.0f;
	}
	else if (m_xVelocity > 3.0f) {
		m_xVelocity = 3.0f;
	}

	// 좌우 속력 감소
	if ((!Input::GetKeyDown(KeyCode::A) && !Input::GetKeyDown(KeyCode::D)) || gameState == GameState::GameOver) {
		m_xVelocity *= 0.7f;
	}

	m_transform->position.x += m_xVelocity * Time::GetDeltaTime();

	// 수직 이동 처리
	if (!m_isGrounded) {
		m_yVelocity -= 0.3f;

		if (m_yVelocity < -14.0f) {
			m_yVelocity = -14.0f;
		}
	}
	else if (Input::GetKeyDown(KeyCode::Space) && gameState != GameState::GameOver) {
		if (Input::GetKeyDown(KeyCode::S)) {
			m_transform->position.y -= 0.001f;
		}
		else {
			m_yVelocity = 8.5f;
		}
	}

	// 플랫폼 충돌 판정
	set<unique_ptr<GameObject>>& gameObjects = SceneManager::GetCurrentScene()->GetGameObjects();
	m_isGrounded = false;

	for (const unique_ptr<GameObject>& gameObject : gameObjects) {
		if (gameObject.get() == this) {
			continue;
		}

		Transform* otherTransform = gameObject->GetComponent<Transform>();
		Collider* otherCollider = gameObject->GetComponent<Collider>();

		if (otherCollider != nullptr && otherCollider->GetActive() && otherCollider->tag == ColliderTag::FloorTag) {
			bool horizontalCheck = m_transform->position.x + m_collider->center.x - m_collider->size.x <= otherTransform->position.x + otherCollider->center.x + otherCollider->size.x &&
				m_transform->position.x + m_collider->center.x + m_collider->size.x >= otherTransform->position.x + otherCollider->center.x - otherCollider->size.x;
			bool verticalCheck = m_transform->position.y + m_collider->center.y - m_collider->size.y >= otherTransform->position.y &&
				m_yVelocity * Time::GetDeltaTime() + m_transform->position.y + m_collider->center.y - m_collider->size.y <= otherTransform->position.y;
			if (horizontalCheck && verticalCheck) {

				m_isGrounded = true;
				m_yVelocity = 0.0f;

				m_transform->position.y = otherTransform->position.y + m_collider->size.y;

				break;
			}
		}
	}

	m_transform->position.y += m_yVelocity * Time::GetDeltaTime();

	// 추락 판정
	if (m_transform->position.y < -5.0f) {
		m_transform->position = XMFLOAT3{ 0.0f, 0.0f, 0.0f };

		m_attackCooldown = m_attackInterval;
		m_isAttacking = false;

		DecreaseHealth(PLAYER_FALL_DAMAGE);
	}

	// 카메라 이동
	m_cameraPosition.x = Lerp(m_cameraPosition.x, m_transform->position.x, Time::GetDeltaTime() * 8.0f);
	m_cameraPosition.y = Lerp(m_cameraPosition.y, m_transform->position.y + 1.0f, Time::GetDeltaTime() * 8.0f);
	m_cameraPosition.z = Lerp(m_cameraPosition.z, m_transform->position.z - 10.0f, Time::GetDeltaTime() * 8.0f);

	// 게임이 진행 중인 경우
	if (gameState == GameState::Playing) {
		m_headTarget = m_transform->position + XMFLOAT3{ 0.0f, 0.8f, 0.0f };
		m_bodyTarget = m_transform->position + XMFLOAT3{ 0.0f, 0.4f, 0.0f };
		m_hipTarget = m_transform->position + XMFLOAT3{ 0.0f, 0.2f, 0.0f };

		// 피격 쿨타임 처리
		if (m_damageCooldown < PLAYER_DAMAGE_COOLDOWN) {
			m_damageCooldown += Time::GetDeltaTime();
		}

		// 공격 처리
		if (m_isAttacking) {
			m_attackDuration += Time::GetDeltaTime();

			if (m_attackDuration >= m_attackTargetDuration) {
				m_attackCooldown = m_attackInterval;
				m_isAttacking = false;
			}
			else {
				m_leftHandTarget = m_transform->position + XMFLOAT3{ cos(Lerp(0.0f, PI2, m_attackDuration / m_attackTargetDuration) * m_attackSpeed + PI),
					sin(Lerp(0.0f, PI2, m_attackDuration / m_attackTargetDuration) * m_attackSpeed + PI) + 0.5f, 0.0f };
				m_rightHandTarget = m_transform->position + XMFLOAT3{ cos(Lerp(0.0f, PI2, m_attackDuration / m_attackTargetDuration) * m_attackSpeed),
					sin(Lerp(0.0f, PI2, m_attackDuration / m_attackTargetDuration) * m_attackSpeed) + 0.5f, 0.0f };
			}
		}
		else {
			m_leftHandTarget = m_transform->position + XMFLOAT3{ -0.5f, 0.0f, 0.0f };
			m_rightHandTarget = m_transform->position + XMFLOAT3{ 0.5f, 0.0f, 0.0f };

			m_attackCooldown -= Time::GetDeltaTime();

			if (m_attackCooldown <= 0.0f) {
				m_attackDuration = 0.0f;
				m_attackTargetDuration = 2.0f;
				m_isAttacking = true;
			}
		}
	}
	// 게임 오버된 경우
	else if (gameState == GameState::GameOver) {
		m_headTarget = m_transform->position + XMFLOAT3{ 1.0f, -0.3f, 0.0f };
		m_bodyTarget = m_transform->position + XMFLOAT3{ 0.0f, -0.3f, 0.0f };
		m_hipTarget = m_transform->position + XMFLOAT3{ -0.5f, -0.3f, 0.0f };
		m_leftHandTarget = m_transform->position + XMFLOAT3{ 1.5f, -0.2f, 0.0f };
		m_rightHandTarget = m_transform->position + XMFLOAT3{ 0.4f, -0.2f, 0.0f };
	}

	Camera::SetPosition(m_cameraPosition);
}

XMFLOAT3 Player::GetTargetPosition(BodyType bodyType)
{
	switch (bodyType) {
	case BodyType::Head: {
		return m_headTarget;
	}
	case BodyType::Body: {
		return m_bodyTarget;
	}
	case BodyType::Hip: {
		return m_hipTarget;
	}
	case BodyType::LeftHand: {
		return m_leftHandTarget;
	}
	default: {
		return m_rightHandTarget;
	}
	};
}

XMFLOAT2 Player::GetVelocity() {
	return XMFLOAT2{ m_xVelocity, m_yVelocity };
}

float Player::GetCurrentHealth() {
	return m_currentHealth;
}

bool Player::GetIsAttacking() {
	return m_isAttacking;
}

void Player::IncreaseHealth(float amount) {
	m_currentHealth += amount;

	if (m_currentHealth > PLAYER_HEALTH) {
		m_currentHealth = PLAYER_HEALTH;
	}
}

void Player::DecreaseHealth(float amount) {
	if (m_damageCooldown < PLAYER_DAMAGE_COOLDOWN) {
		return;
	}

	m_damageCooldown = 0.0f;
	m_currentHealth -= amount;

	for (PlayerPart* part : m_playerParts) {
		part->ApplyDamage();
	}

	if (m_currentHealth < 0.0f) {
		m_currentHealth = 0.0f;
	}
}

void Player::IncreaseSpeed() {
	m_attackSpeed += PLAYER_SPEED_COEFF;
}

void Player::DecreaseInterval() {
	m_attackInterval *= PLAYER_INTERVAL_COEFF;
}
