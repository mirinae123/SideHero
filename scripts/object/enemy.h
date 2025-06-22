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

const float ENEMY_DAMAGE_COOLDOWN = 0.6f;	// 적 피격 쿨타임
const float ENEMY_HEALTH = 20.0f;			// 적 최대 체력
const float ENEMY_DAMAGE = 5.0f;			// 적 공격력

/// <summary>
/// 적 오브젝트
/// </summary>
class Enemy : public GameObject {
public:
	Enemy(string, GameObject*);

	void Frame();

	void DecreaseHealth(float);

private:
	Player* m_player;

	Transform* m_transform;
	Collider* m_collider;
	Model* m_model;

	XMFLOAT3 m_targetPosition;

	float m_rotation;
	float m_elapsedTime;
	float m_currentHealth;
	float m_damageCooldown;
};