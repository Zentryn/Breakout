#pragma once

#include <glm/glm.hpp>
#include <map>

#define COLOR_NONE 0
#define COLOR_BLUE 1
#define COLOR_YELLOW 2
#define COLOR_GREEN 3
#define COLOR_ORANGE 4

const std::map<int, glm::vec3> COLOR_CODES = {
	{ 0, glm::vec3(255.0f) },
	{ 1, glm::vec3(74.0f,  135.0f, 147.0f) },
	{ 2, glm::vec3(178.0f, 178.0f, 0.0f) },
	{ 3, glm::vec3(25.0f,  140.0f, 25.0f) },
	{ 4, glm::vec3(204.0f, 132.0f, 0.0f) }
};