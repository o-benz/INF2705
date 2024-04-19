#include "utils.h"

#include <random>
#include <chrono>

double rand01()
{
	static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	static std::uniform_real_distribution<double> distribution(0, 1);
	return distribution(generator);
}


void getGroupRandomPos(int groupId, const int N_ROWS, float& x, float& z)
{
	const float GROUP_RADIUS = 3.5;
	const float GROUP_WIDTH = 2 * GROUP_RADIUS;
	const float DELTA_DIST_WITH_MARGIN = GROUP_WIDTH + 1;

	x = (float)(groupId % N_ROWS - N_ROWS / 2);
	z = (float)(groupId / N_ROWS - N_ROWS / 2);

	float randPosX = x * DELTA_DIST_WITH_MARGIN + (-GROUP_RADIUS + rand01() * GROUP_WIDTH);
	float randPosY = z * DELTA_DIST_WITH_MARGIN + (-GROUP_RADIUS + rand01() * GROUP_WIDTH);

	x = randPosX;
	z = randPosY;
}