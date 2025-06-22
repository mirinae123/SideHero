#include "meshmanager.h"

map<MeshType, unique_ptr<Mesh>> MeshManager::m_meshes;

void MeshManager::Start() {
	vector<VertexType> vertices;
	vector<int> indices;

	vertices = vector<VertexType>(4);
	vertices[0] = { XMFLOAT3{ -0.5, -0.5f, 0.0f }, XMFLOAT2{ 0.0f, 0.0f } };
	vertices[1] = { XMFLOAT3{ -0.5f, 0.5f, 0.0f }, XMFLOAT2{ 0.0f, 1.0f } };
	vertices[2] = { XMFLOAT3{ 0.5f, -0.5f, 0.0f }, XMFLOAT2{ 1.0f, 0.0f } };
	vertices[3] = { XMFLOAT3{ 0.5f, 0.5f, 0.0f }, XMFLOAT2{ 1.0f, 1.0f } };

	indices = vector<int>(6);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	m_meshes[MeshType::PlayerMesh] = make_unique<Mesh>(vertices, indices);

	vertices = vector<VertexType>(8);
	vertices[0] = { XMFLOAT3{ -1.0f, 0.0f, -1.0f }, XMFLOAT2{ 0.0f, 0.0f } };
	vertices[1] = { XMFLOAT3{ -1.0f, 0.0f, 1.0f }, XMFLOAT2{ 0.0f, 1.0f } };
	vertices[2] = { XMFLOAT3{ 1.0f, 0.0f, -1.0f }, XMFLOAT2{ 1.0f, 0.0f } };
	vertices[3] = { XMFLOAT3{ 1.0f, 0.0f, 1.0f }, XMFLOAT2{ 1.0f, 1.0f } };
	vertices[4] = { XMFLOAT3{ -1.0f, -0.5f, -1.0f }, XMFLOAT2{ 0.0f, 0.0f } };
	vertices[5] = { XMFLOAT3{ -1.0f, -0.5f, 1.0f }, XMFLOAT2{ 0.0f, 1.0f } };
	vertices[6] = { XMFLOAT3{ 1.0f, -0.5f, -1.0f }, XMFLOAT2{ 1.0f, 0.0f } };
	vertices[7] = { XMFLOAT3{ 1.0f, -0.5f, 1.0f }, XMFLOAT2{ 1.0f, 1.0f } };

	indices = vector<int>(30);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	indices[6] = 6;
	indices[7] = 5;
	indices[8] = 4;
	indices[9] = 7;
	indices[10] = 5;
	indices[11] = 6;

	indices[12] = 4;
	indices[13] = 0;
	indices[14] = 6;
	indices[15] = 6;
	indices[16] = 0;
	indices[17] = 2;

	indices[18] = 5;
	indices[19] = 1;
	indices[20] = 4;
	indices[21] = 4;
	indices[22] = 1;
	indices[23] = 0;

	indices[24] = 6;
	indices[25] = 2;
	indices[26] = 7;
	indices[27] = 7;
	indices[28] = 2;
	indices[29] = 3;

	m_meshes[MeshType::FloorMesh] = make_unique<Mesh>(vertices, indices);

	vertices = vector<VertexType>(4);
	vertices[0] = { XMFLOAT3{ -100.0f, -100.0f, 0.0f }, XMFLOAT2{ 0.0f, 0.0f } };
	vertices[1] = { XMFLOAT3{ -100.0f, 100.0f, 0.0f }, XMFLOAT2{ 0.0f, 1.0f } };
	vertices[2] = { XMFLOAT3{ 100.0f, -100.0f, 0.0f }, XMFLOAT2{ 1.0f, 0.0f } };
	vertices[3] = { XMFLOAT3{ 100.0f, 100.0f, 0.0f }, XMFLOAT2{ 1.0f, 1.0f } };

	indices = vector<int>(6);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	m_meshes[MeshType::BackgroundMesh] = make_unique<Mesh>(vertices, indices);
}

Mesh* MeshManager::GetMesh(MeshType meshType) {
	return m_meshes[meshType].get();
}
