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

	static void SetInMenu(bool value);
	static bool IsInMenu();

	static void ReduceLife();
	static int GetLives();
	static void SetLives(int lives);

	static bool ShouldExitGame();
	static void SetExitGame(bool value);

	static std::map<Powerup, bool> Powerups;
private:
	static bool m_exitGame;
	static bool m_gameOver;
	static bool m_gameWon;
	static bool m_inMenu;
	static int m_lives;
};