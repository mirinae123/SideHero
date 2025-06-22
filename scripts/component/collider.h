#pragma once

#include <DirectXMath.h>

#include "../system/component.h"
#include "../component/transform.h"

using namespace DirectX;

/// <summary>
/// 콜라이더 컴포넌트
/// </summary>
enum ColliderTag { None, FloorTag, EnemyTag };

class Collider : public Component {
public:
	Collider(ColliderTag tag = ColliderTag::None, XMFLOAT2 center = XMFLOAT2{ 0.0f, 0.0f }, XMFLOAT2 size = XMFLOAT2{ 1.0f, 1.0f });

	/// <summary>
	/// 콜라이더가 나타내는 범위를 반환한다.
	/// </summary>
	/// <returns>콜라이더 범위</returns>
	RECT GetRect();

	/// <summary>
	/// 다른 콜라이더와의 충돌을 계산한다.
	/// </summary>
	/// <param name="">다른 콜라이더</param>
	/// <returns>충돌 여부</returns>
	bool CheckCollision(RECT);

public:
	ColliderTag tag;

	XMFLOAT2 center;
	XMFLOAT2 size;
};