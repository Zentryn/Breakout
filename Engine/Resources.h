#pragma once

#include "Logger.h"
#include "GLTexture2D.h"
#include "ShaderProgram.h"
#include <unordered_map>
#include <string>

namespace Engine {

	class Resources
	{
	public:
		Resources();
		~Resources();

		static GLTexture2D LoadImage(const std::string &path);

		ShaderProgram GetShader(std::string &name);
		void LoadShader(std::string &name, const char *vertexPath, const char *fragmentPath, const char *geometryPath = "");

	private:
		std::unordered_map<std::string, ShaderProgram> m_shaders;
	};

}