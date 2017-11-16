#pragma once

#include "Powerups.h"
#include <glm/glm.hpp>

class Object
{
public:
	Object();
	Object(float XPos, float YPos, int Width, int Height, glm::vec3 Color, int Health, bool IsSolid, bool IsStatic, bool IsDestructible, Powerup aPowerup = NONE, const glm::vec2 &dir = glm::vec2(0.0f));
	~Object();

	void init(float XPos, float YPos, int Width, int Height, glm::vec3 Color, int Health, bool IsSolid, bool IsStatic, bool IsDestructible, Powerup aPowerup = NONE, const glm::vec2 &dir = glm::vec2(0.0f));
	void init(bool IsPaddle, float XPos, float YPos, int Width, int Height, float Speed, glm::vec3 Color);
	void move(float xChange, float yChange, float deltaTime, float Speed = 1.0f);
	void update(float deltaTime);
	void update(float deltaTime, float Speed);

	bool isPaddle = false;
	bool isSolid;
	bool isStatic;
	bool isDestructible;

	float xPos;
	float yPos;
	float speed;
	int width;
	int height;
	glm::vec2 direction;
	glm::vec3 color;
	int health;

	Powerup powerup;
};

