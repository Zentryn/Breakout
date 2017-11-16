#pragma once

#include "Logger.h"
#include "GLTexture2D.h"
#include "ShaderProgram.h"
#include <unordered_map>
#include <map>
#include <string>
#include <GL/glew.h>

namespace Engine {

struct Character {
	GLuint	   TextureID; // Handle to the glyph texture
	glm::ivec2 Size;      // The size of the glyph
	glm::ivec2 Bearing;   // Offset from baseline to top/left of glyph
	GLuint     Advance;   // Distance to the next glyph
};

class Resources
{
public:
	Resources();
	~Resources();

	static GLTexture2D LoadImage(const std::string &path);
	static void LoadFont(const std::string &name, int height);

	ShaderProgram GetShader(std::string &name);
	void LoadShader(std::string &name, const char *vertexPath, const char *fragmentPath, const char *geometryPath = "");

	static std::map<GLchar, Character> Characters;
private:
	std::unordered_map<std::string, ShaderProgram> m_shaders;
};

}