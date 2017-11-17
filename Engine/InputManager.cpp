#include "InputManager.h"

namespace Engine {

InputManager::InputManager() : m_mouseCoords(0.0f), m_previousMouseCoords(0.0f)
{
}


InputManager::~InputManager()
{
}


void InputManager::update()
{
	for (auto &it : m_keymap) {
		m_prevKeymap[it.first] = it.second;
	}

	m_previousMouseCoords = m_mouseCoords;
}


void InputManager::setMouseCoords(const glm::vec2 &coords)
{
	setMouseCoords(coords.x, coords.y);
}


void InputManager::setMouseCoords(const float x, const float y)
{
	m_mouseCoords.x = x;
	m_mouseCoords.y = y;
}


void InputManager::pressKey(unsigned int keyID)
{
	m_keymap[keyID] = true;
}


void InputManager::releaseKey(unsigned int keyID)
{
	m_keymap[keyID] = false;
}


bool InputManager::isKeyDown(unsigned int keyID)
{
	auto it = m_keymap.find(keyID);

	if (it != m_keymap.end())
		return it->second;

	return false;
}


bool InputManager::wasKeyPressed(unsigned int keyID)
{
	return (isKeyDown(keyID) && !m_prevKeymap[keyID]);
}

}