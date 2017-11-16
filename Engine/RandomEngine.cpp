#include "RandomEngine.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

void clamp(int &x, int minVal, int maxVal)
{
	x = std::min(std::max(x, minVal), maxVal);
}

void RandomEngine::Init()
{
	// Initialize random seed
	srand(time(NULL));
}


bool RandomEngine::GetSuccessWithChance(int percentage)
{
	clamp(percentage, 1, 100);
	return ((rand() % 100 + 1) <= percentage);
}


int RandomEngine::GetNumberInRange(int low, int high)
{
	return rand() % (high - low + 1) + low;
}