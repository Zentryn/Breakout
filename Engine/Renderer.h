#pragma once

#include "Window.h"
#include "ShaderProgram.h"
#include "GLTexture2D.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

class Renderer
{
public:
	Renderer();
	~Renderer();

	void init();
	void render(GLTexture2D texture, ShaderProgram *shader, int x, int y, int width, int height, Window *window);
	void render(GLTexture2D texture, ShaderProgram *shader, int x, int y, Window *window);

private:
	GLuint m_vao;
	GLuint m_vbo;
};

}