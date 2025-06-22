#include "rendermanager.h"
#include "../component/model.h"
#include "../component/sprite.h"

#include "../system/input.h"

map<int, list<Model*>> RenderManager::m_models;
map<int, list<Sprite*>> RenderManager::m_sprites;

bool RenderManager::Render() {
	Direct3D::BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	Direct3D::EnableAlphaBlending();

	// Model ÄÄÆ÷³ÍÆ® ·»´õ¸µ
	for (auto v : m_models) {
		Direct3D::ClearZBuffer();

		for (auto c : v.second) {
			c->Render();
		}
	}

	Direct3D::TurnZBufferOff();

	// Sprite ÄÄÆ÷³ÍÆ® ·»´õ¸µ
	for (auto v : m_sprites) {
		if (Input::GetKeyDown(KeyCode::W)) {
			v.second.size();
		}

		for (auto c : v.second) {
			c->Render();
		}
	}

	Direct3D::TurnZBufferOn();

	Direct3D::DisableAlphaBlending();

	Direct3D::EndScene();

	return true;
}