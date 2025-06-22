// 참고 자료 : https://rastertek.com/

#include "camera.h"

XMFLOAT3 Camera::m_position{ 0.0f, 0.0f, 0.0f };
XMFLOAT3 Camera::m_rotation{ 0.0f, 0.0f, 0.0f };

void Camera::SetPosition(const XMFLOAT3 position) {
	m_position = position;
}

void Camera::SetRotation(const XMFLOAT3 rotation) {
	m_rotation = rotation;
}

XMFLOAT3 Camera::GetPosition() {
	return m_position;
}

XMFLOAT3 Camera::GetRotation() {
	return m_rotation;
}

XMMATRIX Camera::GetViewMatrix() {
	XMMATRIX rotationMatrix;

	XMVECTOR upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR positionVector = XMLoadFloat3(&m_position);
	XMVECTOR lookAtVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	float pitch = m_rotation.x * 0.0174532925f;
	float yaw = m_rotation.y * 0.0174532925f;
	float roll = m_rotation.z * 0.0174532925f;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	return XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

XMMATRIX Camera::GetDefaultMatrix() {
	XMVECTOR pos = XMVectorSet(0.0f, 0.0f, -3.0f, 0.0f);
	XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	return XMMatrixLookAtLH(pos, lookAt, up);
}
