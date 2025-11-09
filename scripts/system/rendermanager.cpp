#include "rendermanager.h"
#include "../component/model.h"
#include "../component/sprite.h"

#include "../system/input.h"

map<int, list<Model*>> RenderManager::m_models;
map<int, list<Sprite*>> RenderManager::m_sprites;

bool RenderManager::Render() {
	Direct3D::BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	Direct3D::EnableAlphaBlending();

	// Model 컴포넌트 렌더링
	for (auto v : m_models) {
		Direct3D::ClearZBuffer();

		for (auto c : v.second) {
			c->Render();
		}
	}

	Direct3D::TurnZBufferOff();

	// Sprite 컴포넌트 렌더링
	for (auto v : m_sprites) {
		for (auto c : v.second) {
			c->Render();
		}
	}

	Direct3D::TurnZBufferOn();

	Direct3D::DisableAlphaBlending();

	Direct3D::EndScene();

	return true;
}