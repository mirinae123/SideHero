#pragma once

#include "../system/gameobject.h"
#include "../system/scenemanager.h"
#include "../system/input.h"
#include "../system/time.h"

#include "../component/transform.h"
#include "../component/model.h"
#include "../component/collider.h"

#include "vector"

enum BodyType { Head, Body, Hip, LeftHand, RightHand };

class Player;

/// <summary>
/// 플레이어 바디 오브젝트
/// </summary>
class PlayerPart : public GameObject {
public:
	PlayerPart(string, GameObject*, BodyType);

	void Frame();

	/// <summary>
	/// 피격 효과를 적용한다.
	/// </summary>
	void ApplyDamage();

private:
	Transform* m_transform;
	Collider* m_collider;
	Player* m_player;

	BodyType m_bodyType;

	float m_damageCooldown;
};