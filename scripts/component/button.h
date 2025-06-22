#pragma once

#include "../core/direct3d.h"

#include "../system/component.h"
#include "../system/input.h"

#include "sprite.h"
#include "transform.h"

/// <summary>
/// 버튼 컴포넌트
/// </summary>
class Button : public Component {
public:
	Button(GameObject* gameObject);

	void SetActive(bool);

	/// <summary>
	/// 마우스 호버 여부를 반환한다.
	/// </summary>
	/// <returns>호버 여부</returns>
	bool GetHover();

	/// <summary>
	/// 버튼 클릭 여부를 반환한다.
	/// </summary>
	/// <returns>클릭 여부</returns>
	bool GetClick();
	
private:
	void Frame();

private:
	Sprite* m_sprite;
	Transform* m_transform;

	bool m_isHover;
	bool m_isClick;
};