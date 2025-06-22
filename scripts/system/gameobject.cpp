#include "gameobject.h"
#include "component.h"

GameObject::GameObject(string name) {
	m_name = name;
	m_isActive = true;
}

GameObject::~GameObject() {
	this->End();
}

string GameObject::GetName() {
	return m_name;
}

void GameObject::SetActive(bool isActive) {
	m_isActive = isActive;
}

bool GameObject::GetActive() {
	return m_isActive;
}

void GameObject::Start() {
	for (const unique_ptr<Component>& component : m_components) {
		component->Start();
	}
}

void GameObject::Frame() {
	for (const unique_ptr<Component>& component : m_components) {
		if (component->GetActive()) {
			component->Frame();
		}
	}
}

void GameObject::Render() {
	for (const unique_ptr<Component>& component : m_components) {
		if (component->GetActive()) {
			component->Render();
		}
	}
}

void GameObject::End() {
	for (const unique_ptr<Component>& component : m_components) {
		component->End();
	}
}

void GameObject::Initialize(...) {
}

void GameObject::InsertComponent(unique_ptr<Component> component) {
	component->SetGameObject(this);
	m_components.insert(move(component));
}

void GameObject::EraseComponent(Component* component) {
	component->End();

	for (const unique_ptr<Component>& comp : m_components) {
		if (comp.get() == component) {
			m_components.erase(comp);
			break;
		}
	}
}

void GameObject::Destroy() {
	m_isDestroyed = true;
}

bool GameObject::IsDestroyed() {
	return m_isDestroyed;
}