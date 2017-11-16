#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Engine {

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	void compile(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath, const std::string& geometryShaderFilePath = "");

	void use();
	void unuse();
	void reload(); // Re-compiles the shader

	template <typename ...T>
	void setUniformTextures(T ...textureNames) {
		setUniformTexturesInitlist({ textureNames... });
	}

	void setInt(const char* name, const int& value);
	void setFloat(const char* name, const float& value);
	void setVec2(const char* name, const glm::vec2& value);
	void setVec3(const char* name, const glm::vec3& value);
	void setVec3(const char* name, const float& v0, const float& v1, const float& v2);
	void setMat4(const char* name, const glm::mat4& value);
	void setIntArray(const char* name, const int nrElements, const int value[]);
	void setFloatArray(const char* name, const int nrElements, const float value[]);
	void setKernel(const char* name, const float value[9][2]);

	GLuint getProgramID() const { return m_programID; }
	std::string getVertexFilePath() { return m_vertexFilePath; }

private:
	void linkShaders();
	GLint getUniformLocation(const std::string& uniformName);
	void compileShadersFromSource(const char* vertexSource, const char* fragmentSource, const char* geometrySource = "");
	void compileShader(const char* source, const std::string& name, GLuint id);
	void setUniformTexturesInitlist(std::initializer_list<std::string> textureNames);

	std::string m_vertexFilePath;
	std::string m_geometryFilePath;
	std::string m_fragmentFilePath;

	std::vector<std::string> m_uniformTextures;

	GLuint m_programID;
	GLuint m_vertexShaderID;
	GLuint m_geometryShaderID;
	GLuint m_fragmentShaderID;
};

}