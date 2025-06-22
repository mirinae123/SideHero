#pragma once

#include "../system/gameobject.h"
#include "../system/scenemanager.h"
#include "../system/input.h"
#include "../system/time.h"

#include "../object/playerpart.h"
#include "../scene/ingame.h"

#include "../component/transform.h"
#include "../component/model.h"
#include "../component/collider.h"

#include "cmath"

#define PI 3.141592f
#define PI2 6.283184f

const float PLAYER_HEALTH = 100.0f;			// 최대 체력
const float PLAYER_DAMAGE_COOLDOWN = 1.0f;	// 피격 쿨타임
const float PLAYER_FALL_DAMAGE = 20.0f;		// 추락 시 체력 감소량

const float PLAYER_INITIAL_SPEED = 0.5f;	// 최초 공격 속력
const float PLAYER_INITIAL_INTERVAL = 2.0f;	// 최초 공격 간격
const float PLAYER_SPEED_COEFF = 0.5f;		// 속력 업그레이드 값
const float PLAYER_INTERVAL_COEFF = 0.8f;	// 간격 업그레이드 값

/// <summary>
/// 플레이어 오브젝트
/// </summary>
class Player : public GameObject {
public:
	Player(string, Scene*, XMFLOAT3);

	void Frame();

	/// <summary>
	/// 바디 오브젝트가 이동해야 할 위치를 반환한다.
	/// </summary>
	/// <param name="">바디 종류</param>
	/// <returns>위치</returns>
	XMFLOAT3 GetTargetPosition(BodyType);
	XMFLOAT2 GetVelocity();

	float GetCurrentHealth();
	void IncreaseHealth(float);
	void DecreaseHealth(float);

	bool GetIsAttacking();

	void IncreaseSpeed();
	void DecreaseInterval();

private:
	Transform* m_transform;
	Collider* m_collider;

	vector<PlayerPart*> m_playerParts;

	float m_xVelocity, m_yVelocity;
	bool m_isGrounded;

	XMFLOAT3 m_headTarget;
	XMFLOAT3 m_bodyTarget;
	XMFLOAT3 m_hipTarget;

	XMFLOAT3 m_leftHandTarget;
	XMFLOAT3 m_rightHandTarget;

	XMFLOAT3 m_cameraPosition;

	float m_currentHealth;
	float m_attackInterval;
	float m_attackSpeed;

	float m_attackCooldown;
	float m_attackDuration;
	float m_attackTargetDuration;

	float m_damageCooldown;

	bool m_isAttacking;
};