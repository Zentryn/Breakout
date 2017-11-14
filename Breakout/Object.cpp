#include "Object.h"
#include <Engine/Logger.h>

Object::Object()
{
	
}


Object::Object(float XPos, float YPos, int Width, int Height, glm::vec3 Color, int Health, bool IsSolid, bool IsStatic, bool IsDestructible, const glm::vec2 &dir /*= glm::vec2(0.0f)*/)
{
	xPos = XPos;
	yPos = YPos;
	width = Width;
	height = Height;
	color = Color;
	health = Health;
	isSolid = IsSolid;
	isStatic = IsStatic;
	isDestructible = IsDestructible;
	direction = dir;
}


Object::~Object()
{
}


void Object::init(float XPos, float YPos, int Width, int Height, glm::vec3 Color, int Health, bool IsSolid, bool IsStatic, bool IsDestructible, const glm::vec2 &dir /*= glm::vec2(0.0f)*/)
{
	xPos = XPos;
	yPos = YPos;
	width = Width;
	height = Height;
	color = Color;
	health = Health;
	isSolid = IsSolid;
	isStatic = IsStatic;
	isDestructible = IsDestructible;
	direction = dir;
}


void Object::init(bool IsPaddle, float XPos, float YPos, int Width, int Height, float Speed, glm::vec3 Color)
{
	isPaddle = IsPaddle;
	xPos = XPos;
	yPos = YPos;
	width = Width;
	height = Height;
	color = Color;
	speed = Speed;
}


void Object::move(float xChange, float yChange, float deltaTime, float Speed /*= 1.0f*/)
{
	xPos += xChange * Speed * deltaTime;
	yPos += yChange * Speed * deltaTime;
}


void Object::update(float deltaTime)
{
	xPos += direction.x * speed * deltaTime;
	yPos += direction.y * speed * deltaTime;
}


void Object::update(float deltaTime, float Speed)
{
	xPos += direction.x * Speed * deltaTime;
	yPos += direction.y * Speed * deltaTime;
}