#include "Resources.h"
#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

namespace Engine {

	Resources::Resources()
	{

	}


	Resources::~Resources()
	{

	}


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