#pragma once

#include "memory"
#include "map"

#include "../system/mesh.h"

using namespace std;

/// <summary>
/// 메시 종류
/// </summary>
enum MeshType { PlayerMesh, FloorMesh, BackgroundMesh };

/// <summary>
/// 메시들을 관리하는 클래스
/// </summary>
class MeshManager {
public:
	static void Start();
	static Mesh* GetMesh(MeshType);

private:
	static map<MeshType, unique_ptr<Mesh>> m_meshes;
};