#include "button.h"

Button::Button(GameObject* gameObject) {
	m_sprite = gameObject->GetComponent<Sprite>();
	m_transform = gameObject->GetComponent<Transform>();

	m_isHover = false;
	m_isClick = false;
}

void Button::SetActive(bool isActive) {
	Component::SetActive(isActive);

	m_isHover = false;
	m_isClick = false;

	m_sprite->SetAdditiveColor(XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f });
}

bool Button::GetHover() {
	return m_isHover;
}

bool Button::GetClick() {
	return m_isClick;
}

void Button::Frame() {
	if (!m_gameObject->GetActive() || !m_isActive) {
		return;
	}

	float centerX = Direct3D::GetScreenWidth() / 2.0f + m_transform->position.x;
	float centerY = Direct3D::GetScreenHeight() / 2.0f - m_transform->position.y;

	float width = 64.0f * m_transform->scale.x;
	float height = 64.0f * m_transform->scale.y;

	float x = Input::GetMouseX();
	float y = Input::GetMouseY();

	if (centerX - width <= Input::GetMouseX() && Input::GetMouseX() <= centerX + width &&
		centerY - height <= Input::GetMouseY() && Input::GetMouseY() <= centerY + height) {
		m_isHover = true;
		m_isClick = Input::GetKeyDown(KeyCode::LeftMouse);

		m_sprite->SetAdditiveColor(XMFLOAT4{ 0.2f, 0.2f, 0.2f, 0.0f });
	}
	else {
		m_isHover = false;
		m_isClick = false;

		m_sprite->SetAdditiveColor(XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f });
	}
}
