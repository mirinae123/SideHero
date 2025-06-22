#pragma once

#include <memory>
#include <d3d11.h>
#include <directxmath.h>
#include <wrl/client.h>

#include "../system/camera.h"
#include "../system/rendermanager.h"
#include "../system/component.h"
#include "../system/mesh.h"
#include "../system/texture.h"
#include "../shader/textureshader.h"

using namespace std;
using namespace DirectX;
using Microsoft::WRL::ComPtr;

/// <summary>
/// 스프라이트 컴포넌트
/// </summary>
class Sprite : public Component {
public:
	Sprite(int, Texture*);

	void Start();
	void Render();
	void End();

	void SetTexture(Texture*);

	void SetAdditiveColor(XMFLOAT4);
	void SetMultiplicativeColor(XMFLOAT4);

	XMFLOAT4 GetAdditiveColor();
	XMFLOAT4 GetMultiplicativeColor();

private:
	int m_order;

	Texture* m_texture;

	XMFLOAT4 m_additiveColor;
	XMFLOAT4 m_multiplicativeColor;

	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
};