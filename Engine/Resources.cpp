#include "Resources.h"
#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Engine {

Resources::Resources()
{

}


Resources::~Resources()
{

}

std::map<GLchar, Character> Resources::Characters = std::map<GLchar, Character>();

GLTexture2D Resources::LoadImage(const std::string &path)
{
	//stbi_set_flip_vertically_on_load(true);

	GLTexture2D texture;

	// Generate texture
	glGenTextures(1, &texture.id);

	int nrChannels;
	unsigned char *data = stbi_load(path.c_str(), &texture.width, &texture.height, &nrChannels, 0);
	if (data) {
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;

		else if (nrChannels == 3)
			format = GL_RGB;

		else if (nrChannels == 4)
			format = GL_RGBA;

		// Load data into the texture
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, texture.width, texture.height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Logger::Log("Loaded texture " + path);
	}
	else {
		Logger::Warning("Could not load the following image: " + path);
	}

	stbi_image_free(data);

	return texture;
}


void Resources::LoadFont(const std::string &name, int height)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		Logger::Error("Could not initialize FreeType library!");

	std::string path = "Fonts/" + name;

	FT_Face face;
	if (FT_New_Face(ft, path.c_str(), 0, &face))
		Logger::Error("Failed to load font!");

	FT_Set_Pixel_Sizes(face, 0, height);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of the ASCII character set
	for (GLubyte c = 0; c < 128; c++) {
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			Logger::Warning("Freetype failed to load glyph!");
			continue;
		}

		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Store character
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Resources::Characters.insert(std::pair<GLchar, Character>(c, character));
	}

	// Free memory
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}


void Resources::LoadShader(std::string &name, const char *vertexPath, const char *fragmentPath, const char *geometryPath /* = "" */)
{
	ShaderProgram shader;
	shader.compile(vertexPath, fragmentPath, geometryPath);
	m_shaders[name] = shader;
}


ShaderProgram Resources::GetShader(std::string &name)
{
	auto it = m_shaders.find(name);
	if (it != m_shaders.end())
		return it->second;
	else
		Logger::Error("A shader with name '" + name + "' was not found!");
}

}