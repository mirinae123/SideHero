#pragma once

#include <DirectXMath.h>

#include "../system/component.h"
#include "../system/gameObject.h"

using namespace DirectX;

/// <summary>
/// 변환 컴포넌트
/// </summary>
class Transform : public Component {
public:
	Transform(XMFLOAT3 position = XMFLOAT3{ 0.0f, 0.0f, 0.0f }, XMFLOAT4 rotation = XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f }, XMFLOAT3 scale = XMFLOAT3{ 1.0f, 1.0f, 1.0f });

	/// <summary>
	/// 변환 행렬을 반환한다.
	/// </summary>
	/// <returns>변환 행렬</returns>
	XMMATRIX GetMatrix();

public:
	XMFLOAT3 position;
	XMFLOAT4 rotation;
	XMFLOAT3 scale;
};