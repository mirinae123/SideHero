#include "floor.h"

Floor::Floor(string name, XMFLOAT3 position) : GameObject(name) {
	InsertComponent(make_unique<Model>(0, MeshManager::GetMesh(MeshType::FloorMesh), TextureManager::GetTexture(TextureType::FloorTexture)));
	InsertComponent(make_unique<Transform>(position));
	InsertComponent(make_unique<Collider>(ColliderTag::FloorTag, XMFLOAT2{ 0.0f, 0.0f }, XMFLOAT2{ 1.0f, 0.0f }));
}