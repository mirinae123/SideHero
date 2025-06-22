#include "component.h"

Component::Component() {
}

Component::~Component() {
	this->End();
}

void Component::SetActive(bool isActive) {
	m_isActive = isActive;
}

bool Component::GetActive() {
	return m_isActive;
}

void Component::Start() {
}

void Component::Frame() {
}

void Component::Render() {
}

void Component::End() {
}

void Component::SetGameObject(GameObject* gameObject) {
	m_gameObject = gameObject;
}

GameObject* Component::GetGameObject() {
	return m_gameObject;
}
