#include "transform.h"

Transform::Transform(XMFLOAT3 position, XMFLOAT4 rotation, XMFLOAT3 scale) {
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

XMMATRIX Transform::GetMatrix() {
	XMMATRIX matrix = XMMatrixIdentity();

	matrix = XMMatrixMultiply(matrix, XMMatrixScaling(scale.x, scale.y, scale.z));
	matrix = XMMatrixMultiply(matrix, XMMatrixRotationQuaternion(XMLoadFloat4(&(rotation))));
	matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(position.x, position.y, position.z));

	return matrix;
}