#include "random.h"

mt19937 Random::m_generator;

void Random::Start() {
	std::random_device randomeDevice;
	m_generator = mt19937(randomeDevice());
}

int Random::GetRandom(int a, int b) {
	if (a > b) {
		swap(a, b);
	}

	std::uniform_int_distribution<int> distribution(a, b);

	return distribution(m_generator);
}

float Random::GetRandom(float a, float b) {
	if (a > b) {
		swap(a, b);
	}

	std::uniform_real_distribution<float> distribution(a, b);

	return distribution(m_generator);
}