#include "mesh.h"

Mesh::Mesh(const vector<VertexType> vertices, const vector<int> indices)
{
	m_vertices = vertices;
	m_indices = indices;
}

vector<VertexType>& Mesh::GetVertices()
{
	return m_vertices;
}

vector<int>& Mesh::GetIndices()
{
	return m_indices;
}
