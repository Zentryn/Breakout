#pragma once
#include "Powerups.h"
#include <map>

const int LIVES_AT_GAME_START = 3;

class GameManager {
public:
	static void SetGameOver(bool value);
	static bool IsGameOver();

	static void SetGameWon(bool value);
	static bool IsGameWon();

	static void ReduceLife();
	static int GetLives();

	static std::map<Powerup, bool> Powerups;
private:
	static bool m_gameOver;
	static bool m_gameWon;
	static int m_lives;
};