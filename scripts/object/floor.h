#pragma once

#include "../system/gameobject.h"

#include "../component/transform.h"
#include "../component/model.h"
#include "../component/collider.h"

/// <summary>
/// 플랫폼 오브젝트
/// </summary>
class Floor : public GameObject {
public:
	Floor(string, XMFLOAT3);
};