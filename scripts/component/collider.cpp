#include "collider.h"

Collider::Collider(ColliderTag tag, XMFLOAT2 center, XMFLOAT2 size) {
	this->tag = tag;
	this->center = center;
	this->size = size;
}

RECT Collider::GetRect() {
	RECT rect;

	Transform* transform = m_gameObject->GetComponent<Transform>();

	XMFLOAT3 position = transform->position;
	XMFLOAT3 scale = transform->scale;

	rect.left = position.x + (center.x - size.x) * scale.x;
	rect.right = position.x + (center.x + size.x) * scale.x;
	rect.top = position.y + (center.y + size.y) * scale.y;
	rect.bottom = position.y + (center.y - size.y) * scale.y;

	return rect;
}

bool Collider::CheckCollision(RECT other) {
	RECT rect = GetRect();

	if (other.right < rect.left || rect.right < other.left) {
		return false;
	}

	if (other.top < rect.bottom || rect.top < other.bottom) {
		return false;
	}

	return true;
}
