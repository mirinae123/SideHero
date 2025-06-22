#pragma once

#include <random>

using namespace std;

/// <summary>
/// 랜덤 값 처리 클래스
/// </summary>
class Random {
private:
	static void Start();

public:
	static int GetRandom(int, int);
	static float GetRandom(float, float);

private:
	static mt19937 m_generator;

	friend class Core;
};