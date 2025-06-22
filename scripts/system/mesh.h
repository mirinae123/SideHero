#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <vector>

using namespace std;
using namespace DirectX;

struct VertexType {
	XMFLOAT3 position;
	XMFLOAT2 texture;
};

/// <summary>
/// 메시 클래스
/// </summary>
class Mesh {
public:
	Mesh(const vector<VertexType>, const vector<int>);

	vector<VertexType>& GetVertices();
	vector<int>& GetIndices();

private:
	vector<VertexType> m_vertices;
	vector<int> m_indices;
};