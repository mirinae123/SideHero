#pragma once

#include <memory>
#include <list>
#include <map>
#include <type_traits>
#include <algorithm>

#include "../core/direct3d.h"

using namespace std;

class Model;
class Sprite;

/// <summary>
/// 렌더 처리 클래스
/// </summary>
class RenderManager {
private:
	static bool Render();

public:
	/// <summary>
	/// Model 또는 Sprite 컴포넌트를 렌더 매니저에 등록한다.
	/// </summary>
	/// <typeparam name="T">컴포넌트 타입</typeparam>
	/// <param name="order">렌더 순서</param>
	/// <param name="component">컴포넌트에 대한 Raw Pointer</param>
	template <typename T>
	static void Insert(int order, T* component) {
		if constexpr (is_same<T, Model>::value) {
			for (auto& v : m_models) {
				for (auto c : v.second) {
					if (c == component) {
						return;
					}
				}
			}

			m_models[order].push_back(component);
		}
		else if constexpr (is_same<T, Sprite>::value) {
			for (auto& v : m_sprites) {
				for (auto c : v.second) {
					if (c == component) {
						return;
					}
				}
			}

			m_sprites[order].push_back(component);
		}
	}
	
	/// <summary>
	/// Model 또는 Sprite 컴포넌트의 렌더 순서를 갱신한다.
	/// </summary>
	/// <typeparam name="T">컴포넌트 타입</typeparam>
	/// <param name="order">렌더 순서</param>
	/// <param name="component">컴포넌트에 대한 Raw Pointer</param>
	template <typename T>
	static void Update(int order, T* component) {
		if constexpr (is_same<T, Model>::value) {
			for (auto& v : m_models) {
				auto iterator = find(v.second.begin(), v.second.end(), component);

				if (iterator != v.second.end()) {
					v.second.erase(iterator);
				}
			}

			m_models[order].push_back(component);
		}
		else if constexpr (is_same<T, Sprite>::value) {
			for (auto& v : m_sprites) {
				auto iterator = find(v.second.begin(), v.second.end(), component);

				if (iterator != v.second.end()) {
					v.second.erase(iterator);
				}
			}

			m_sprites[order].push_back(component);
		}
	}

	/// <summary>
	/// Model 또는 Sprite 컴포넌트를 렌더 매니저에서 제거한다.
	/// </summary>
	/// <typeparam name="T">컴포넌트 타입</typeparam>
	/// <param name="component">컴포넌트에 대한 Raw Pointer</param>
	template <typename T>
	static void Erase(T* component) {
		if constexpr (is_same<T, Model>::value) {
			for (auto& v : m_models) {
				auto iterator = find(v.second.begin(), v.second.end(), component);

				if (iterator != v.second.end()) {
					v.second.erase(iterator);
				}
			}
		}
		else if constexpr (is_same<T, Sprite>::value) {
			for (auto& v : m_sprites) {
				auto iterator = find(v.second.begin(), v.second.end(), component);

				if (iterator != v.second.end()) {
					v.second.erase(iterator);
				}
			}
		}
	}

private:
	static map<int, list<Model*>> m_models;
	static map<int, list<Sprite*>> m_sprites;

	friend class Core;
};