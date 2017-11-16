#pragma once
#include "Resources.h"
#include "ShaderProgram.h"
#include <GL/glew.h>

namespace Engine {

class TextRenderer
{
public:
	static void Init();
	static void Render(const std::string &text, float x, float y, float scale, const glm::vec3 &color, float windowWidth, float windowHeight);
private:
	static ShaderProgram m_Shader;
	static GLuint m_VAO;
	static GLuint m_VBO;
};

}