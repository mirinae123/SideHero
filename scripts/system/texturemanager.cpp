#include "texturemanager.h"

unordered_map<TextureType, unique_ptr<Texture>> TextureManager::m_textures;

void TextureManager::Start() {
	m_textures[TextureType::PlayerTexture] = make_unique<Texture>("data\\empty.tga");
	m_textures[TextureType::FloorTexture] = make_unique<Texture>("data\\floor.tga");
	m_textures[TextureType::BackgroundTexture] = make_unique<Texture>("data\\background.tga");

	m_textures[TextureType::HealthBarTexture] = make_unique<Texture>("data\\healthbar.tga");
	m_textures[TextureType::HealthBarBackgroundTexture] = make_unique<Texture>("data\\healthbarbackground.tga");

	m_textures[TextureType::HealthTexture] = make_unique<Texture>("data\\health.tga");
	m_textures[TextureType::SpeedTexture] = make_unique<Texture>("data\\speed.tga");
	m_textures[TextureType::IntervalTexture] = make_unique<Texture>("data\\interval.tga");
}

Texture* TextureManager::GetTexture(TextureType textureType) {
	return m_textures[textureType].get();
}
