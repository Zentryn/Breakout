#include "GameManager.h"

bool GameManager::m_gameOver = false;
bool GameManager::m_gameWon = false;
bool GameManager::m_inMenu = true;
bool GameManager::m_exitGame = false;
int GameManager::m_lives = LIVES_AT_GAME_START;
std::map<Powerup, bool> GameManager::Powerups = {
	{ STICKY_PADDLE, true },
	{ SPEED, false },
	{ WIDTH, false },
	{ BREAKTHROUGH, false },
	{ CHAOS, false },
	{ CONFUSE, false },
	{ SHAKE, false }
};

void GameManager::SetGameOver(bool value)
{
	GameManager::m_gameOver = value;
}

bool GameManager::IsGameOver()
{
	return (GameManager::m_lives <= 0 || GameManager::m_gameOver);
}

void GameManager::SetInMenu(bool value)
{
	GameManager::m_inMenu = value;
}

bool GameManager::IsInMenu()
{
	return GameManager::m_inMenu;
}

void GameManager::SetGameWon(bool value)
{
	GameManager::m_gameWon = value;
}

bool GameManager::IsGameWon()
{
	return GameManager::m_gameWon;
}

void GameManager::ReduceLife()
{
	GameManager::m_lives--;
}

int GameManager::GetLives()
{
	return GameManager::m_lives;
}

void GameManager::SetLives(int lives)
{
	GameManager::m_lives = lives;
}

bool GameManager::ShouldExitGame()
{
	return GameManager::m_exitGame;
}

void GameManager::SetExitGame(bool value)
{
	GameManager::m_exitGame = value;
}