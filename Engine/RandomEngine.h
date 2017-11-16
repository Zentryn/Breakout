#pragma once
class RandomEngine
{
public:
	static void Init();

	// Returns true if a random number with _percentage_ chance was correct
	static bool GetSuccessWithChance(int percentage);
	static int GetNumberInRange(int low, int high);
};