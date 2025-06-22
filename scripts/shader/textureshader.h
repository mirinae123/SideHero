// 참고 자료 : https://rastertek.com/

#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
#include "../core/direct3d.h"

using namespace DirectX;
using namespace std;
using Microsoft::WRL::ComPtr;

class TextureShader {
private:
	struct MatrixBufferType {
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct ColorBufferType {
		XMFLOAT4 additiveColor;
		XMFLOAT4 multiplicateColor;
	};

public:
	static bool Start();
	static bool Render(int, XMMATRIX, XMMATRIX, XMMATRIX, ComPtr<ID3D11ShaderResourceView>, XMFLOAT4, XMFLOAT4);

private:
	static ComPtr<ID3D11VertexShader> m_vertexShader;
	static ComPtr<ID3D11PixelShader> m_pixelShader;
	static ComPtr<ID3D11InputLayout> m_layout;
	static ComPtr<ID3D11Buffer> m_matrixBuffer;
	static ComPtr<ID3D11Buffer> m_colorBuffer;
	static ComPtr<ID3D11SamplerState> m_sampleState;
};