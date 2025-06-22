#pragma once

#include <memory>
#include "../system/gameobject.h"

using namespace std;

/// <summary>
/// 컴포넌트 클래스
/// </summary>
class Component {
public:
	Component();
	~Component();

	/// <summary>
	/// 컴포넌트를 소유한 게임 오브젝트를 지정한다.
	/// </summary>
	/// <param name="">게임 오브젝트</param>
	void SetGameObject(GameObject*);

	/// <summary>
	/// 컴포넌트를 소유한 게임 오브젝트를 반환한다.
	/// </summary>
	/// <returns>게임 오브젝트</returns>
	GameObject* GetGameObject();

	void SetActive(bool);
	bool GetActive();

	virtual void Start();
	virtual void Frame();
	virtual void Render();
	virtual void End();

protected:
	GameObject* m_gameObject;
	bool m_isActive = true;
};