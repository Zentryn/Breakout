#pragma once

#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Engine {

class InputManager
{
public:
	InputManager();
	~InputManager();

	void update();
	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);
	void setMouseCoords(const glm::vec2 &coords);
	void setMouseCoords(const float x, const float y);

	bool isKeyDown(unsigned int keyID);
	bool wasKeyPressed(unsigned int keyID);
	glm::vec2 getMouseCoords() { return m_mouseCoords; };
private:
	glm::vec2 m_mouseCoords;
	glm::vec2 m_previousMouseCoords;
	std::unordered_map<unsigned int, bool> m_keymap;
	std::unordered_map<unsigned int, bool> m_prevKeymap;
};

}