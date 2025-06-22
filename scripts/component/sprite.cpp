#include "sprite.h"
#include "../component/transform.h"

Sprite::Sprite(int order, Texture* texture) {
	m_order = order;

	m_texture = texture;

	m_additiveColor = XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_multiplicativeColor = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };

	vector<VertexType> vertices(4);
	vertices[0] = { XMFLOAT3{ -64.0f, -64.0f, 0.0f }, XMFLOAT2{ 0.0f, 0.0f } };
	vertices[1] = { XMFLOAT3{ -64.0f, 64.0f, 0.0f }, XMFLOAT2{ 0.0f, 1.0f } };
	vertices[2] = { XMFLOAT3{ 64.0f, -64.0f, 0.0f }, XMFLOAT2{ 1.0f, 0.0f } };
	vertices[3] = { XMFLOAT3{ 64.0f, 64.0f, 0.0f }, XMFLOAT2{ 1.0f, 1.0f } };

	vector<int> indices(6);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	D3D11_BUFFER_DESC vertexBufferDesc;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;

	vertexData.pSysMem = vertices.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	Direct3D::GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.GetAddressOf());

	D3D11_BUFFER_DESC indexBufferDesc;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * 6;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;

	indexData.pSysMem = indices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	Direct3D::GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, m_indexBuffer.GetAddressOf());
}

void Sprite::Start() {
	RenderManager::Insert(m_order, this);
}

void Sprite::Render() {
	if (!m_gameObject->GetActive() || !m_isActive) {
		return;
	}

	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	Direct3D::GetDeviceContext()->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	Direct3D::GetDeviceContext()->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	Direct3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX worldMatrix = Direct3D::GetWorldMatrix();
	Transform* transform = m_gameObject->GetComponent<Transform>();

	if (transform != nullptr) {
		worldMatrix = XMMatrixMultiply(worldMatrix, transform->GetMatrix());
	}

	TextureShader::Render(6, worldMatrix, Camera::GetDefaultMatrix(), Direct3D::GetOrthoMatrix(), m_texture->GetTexture(), m_additiveColor, m_multiplicativeColor);
}

void Sprite::End() {
	RenderManager::Erase(this);
}

void Sprite::SetTexture(Texture* texture) {
	m_texture = texture;
}

void Sprite::SetAdditiveColor(XMFLOAT4 additiveColor) {
	m_additiveColor = additiveColor;
}

void Sprite::SetMultiplicativeColor(XMFLOAT4 multiplicativeColor) {
	m_multiplicativeColor = multiplicativeColor;
}

XMFLOAT4 Sprite::GetAdditiveColor() {
	return m_additiveColor;
}

XMFLOAT4 Sprite::GetMultiplicativeColor() {
	return m_multiplicativeColor;
}