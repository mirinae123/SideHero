#pragma once

#include <memory>
#include <unordered_map>

#include "../system/texture.h"

using namespace std;

/// <summary>
/// 텍스처 종류
/// </summary>
enum TextureType { PlayerTexture, FloorTexture, BackgroundTexture, HealthBarTexture, HealthBarBackgroundTexture, HealthTexture, SpeedTexture, IntervalTexture };

/// <summary>
/// 텍스처들을 관리하는 클래스
/// </summary>
class TextureManager {
public:
	static void Start();
	static Texture* GetTexture(TextureType);

private:
	static unordered_map<TextureType, unique_ptr<Texture>> m_textures;
};