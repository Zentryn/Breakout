#include "TextRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

ShaderProgram TextRenderer::m_Shader = ShaderProgram();
GLuint TextRenderer::m_VAO = -1;
GLuint TextRenderer::m_VBO = -1;

void TextRenderer::Init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_Shader.compile("Shaders/glyphShader.vert", "Shaders/glyphShader.frag");

	glGenVertexArrays(1, &TextRenderer::m_VAO);
	glGenBuffers(1, &TextRenderer::m_VBO);

	glBindVertexArray(TextRenderer::m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, TextRenderer::m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW); // 2 triangles = 6 vertices * pos * TexCoords
	
	// Position
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	// Texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}


void TextRenderer::Render(const std::string &text, float x, float y, float scale, const glm::vec3 &color, float windowWidth, float windowHeight)
{
	glm::mat4 projection = glm::ortho(0.0f, windowWidth, 0.0f, windowHeight);

	TextRenderer::m_Shader.use();
	TextRenderer::m_Shader.setVec3("textColor", color);
	TextRenderer::m_Shader.setMat4("projection", projection);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(TextRenderer::m_VAO);

	// Iterate through all characters and render them one by one
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = Resources::Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);

		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, TextRenderer::m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

}