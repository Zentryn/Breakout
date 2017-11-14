#include "Renderer.h"
#include "Window.h"
#include "ShaderProgram.h"
#include "Logger.h"

namespace Engine {

Renderer::Renderer()
{
	// Empty
}


Renderer::~Renderer()
{
	// Empty
}


void Renderer::init()
{
	float vertices[] = {
		 // Pos              // Tex
		0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

		0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		1.0f, 0.0f, 0.0f,  1.0f, 0.0f
	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void Renderer::render(GLTexture2D texture, ShaderProgram *shader, int x, int y, int width, int height, Window *window)
{
	float widthDiff = width / texture.width;
	float heightDiff = height / texture.height;

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(x, y, 0.0f));

	//model = glm::translate(model, glm::vec3(0.5f * width, 0.5f * height, 0.0f));
	//model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::translate(model, glm::vec3(-0.5f * width, -0.5f * height, 0.0f));

	model = glm::scale(model, glm::vec3(width, height, 1.0f));

	glm::mat4 projection = glm::ortho(
		0.0f,
		static_cast<GLfloat>(window->getWidth()),
		static_cast<GLfloat>(window->getHeight()),
		0.0f,
		-1.0f,
		1.0f
	);

	shader->use();
	shader->setMat4("model", model);
	shader->setMat4("projection", projection);
	shader->setVec3("spriteColor", glm::vec3(1.0f, 0.0f, 0.0f));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.id);

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	shader->unuse();
}


void Renderer::render(GLTexture2D texture, ShaderProgram *shader, int x, int y, Window *window)
{
	render(texture, shader, x, y, texture.width, texture.height, window);
}

}