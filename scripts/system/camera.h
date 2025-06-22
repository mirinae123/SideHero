// 참고 자료 : https://rastertek.com/

#pragma once

#include <directxmath.h>

using namespace DirectX;

/// <summary>
/// 카메라 클래스
/// </summary>
class Camera {
public:
	/// <summary>
	/// 카메라 위치를 설정한다.
	/// </summary>
	/// <param name="">위치 (x, y, z)</param>
	static void SetPosition(const XMFLOAT3);

	/// <summary>
	/// 카메라 회전을 설정한다.
	/// </summary>
	/// <param name="">회전 (pitch, yaw, roll)</param>
	static void SetRotation(const XMFLOAT3);

	static XMFLOAT3 GetPosition();
	static XMFLOAT3 GetRotation();

	static XMMATRIX GetViewMatrix();
	static XMMATRIX GetDefaultMatrix();

private:
	static XMFLOAT3 m_position;
	static XMFLOAT3 m_rotation;
};