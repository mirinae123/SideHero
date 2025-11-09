#pragma once

#include <list>
#include <memory>
#include <string>
#include <vector>

#include "meshmanager.h"
#include "texturemanager.h"

using namespace std;

class Component;

/// <summary>
/// 게임 오브젝트 클래스
/// </summary>
class GameObject : public enable_shared_from_this<GameObject> {
public:
	GameObject(string);
	~GameObject();

	string GetName();

	void SetActive(bool);
	bool GetActive();

	virtual void Start();
	virtual void Frame();
	virtual void Render();
	virtual void End();

	/// <summary>
	/// 필요한 초기화 과정을 수행한다.
	/// </summary>
	virtual void Initialize(...);

	/// <summary>
	/// 게임 오브젝트에 포함된 컴포넌트를 반환한다.
	/// </summary>
	/// <typeparam name="T">컴포넌트 타입</typeparam>
	/// <returns>컴포넌트에 대한 Raw Pointer</returns>
	template<typename T>
	T* GetComponent() {
		T* retPointer;

		for (const unique_ptr<Component>& component : m_components) {
			if (retPointer = dynamic_cast<T*>(component.get())) {
				return retPointer;
			}
		}

		return nullptr;
	}

	/// <summary>
	/// 현재 게임 오브젝트에 컴포넌트를 삽입한다.
	/// </summary>
	/// <param name="">컴포넌트에 대한 Unique Pointer</param>
	void InsertComponent(unique_ptr<Component>);

	/// <summary>
	/// 현재 게임 오브젝트에서 컴포넌트를 제거한다.
	/// </summary>
	/// <param name="">컴포넌트에 대한 Raw Pointer</param>
	void EraseComponent(Component*);

	/// <summary>
	/// 현재 게임 오브젝트의 제거 플래그를 설정한다.
	/// 제거 처리는 씬에서 별도로 진행한다.
	/// </summary>
	void Destroy();
	bool IsDestroyed();

protected:
	string m_name;

	bool m_isActive;
	bool m_isDestroyed = false;

	list<unique_ptr<Component>> m_components;
};
