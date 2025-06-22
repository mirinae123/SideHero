#include "model.h"
#include "../component/transform.h"

Model::Model(int order, Mesh* mesh, Texture* texture) {
	m_order = order;

	m_mesh = mesh;
	m_texture = texture;

	m_additiveColor = XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_multiplicativeColor = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };

	D3D11_BUFFER_DESC vertexBufferDesc;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_mesh->GetVertices().size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;

	vertexData.pSysMem = m_mesh->GetVertices().data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	Direct3D::GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.GetAddressOf());

	D3D11_BUFFER_DESC indexBufferDesc;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_mesh->GetIndices().size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;

	indexData.pSysMem = m_mesh->GetIndices().data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	Direct3D::GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, m_indexBuffer.GetAddressOf());
}

void Model::Start() {
	RenderManager::Insert(m_order, this);
}

void Model::Render() {
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

	TextureShader::Render(m_mesh->GetIndices().size(), worldMatrix, Camera::GetViewMatrix(), Direct3D::GetProjectionMatrix(), m_texture->GetTexture(), m_additiveColor, m_multiplicativeColor);
}

void Model::End() {
	RenderManager::Erase(this);
}

void Model::SetAdditiveColor(XMFLOAT4 additiveColor) {
	m_additiveColor = additiveColor;
}

void Model::SetMultiplicativeColor(XMFLOAT4 multiplicativeColor) {
	m_multiplicativeColor = multiplicativeColor;
}

XMFLOAT4 Model::GetAdditiveColor() {
	return m_additiveColor;
}

XMFLOAT4 Model::GetMultiplicativeColor() {
	return m_multiplicativeColor;
}
