#include "ShaderProgram.h"
#include "Logger.h"
#include "IOManager.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

ShaderProgram::ShaderProgram() : m_programID(0), m_vertexShaderID(0), m_geometryShaderID(0), m_fragmentShaderID(0)
{
	// Empty
}

ShaderProgram::~ShaderProgram()
{
	// Empty
}

void ShaderProgram::compile(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath, const std::string& geometryShaderFilePath /* = "" */)
{
	std::string vertexSource;
	std::string geometrySource;
	std::string fragmentSource;
	m_vertexFilePath = vertexShaderFilePath;
	m_geometryFilePath = geometryShaderFilePath;
	m_fragmentFilePath = fragmentShaderFilePath;

	bool vertexSuccess = IOManager::readFileToBuffer(vertexShaderFilePath, vertexSource);
	bool fragmentSuccess = IOManager::readFileToBuffer(fragmentShaderFilePath, fragmentSource);
	bool geometrySuccess;
	if (geometryShaderFilePath != "") {
		geometrySuccess = IOManager::readFileToBuffer(geometryShaderFilePath, geometrySource);
		if (!geometrySuccess) Logger::Error("Geometry shader " + geometryShaderFilePath + " failed to be loaded.");
	}
	else {
		geometrySource = "";
	}

	if (!vertexSuccess) Logger::Error("Vertex shader " + vertexShaderFilePath + " failed to be loaded.");
	if (!fragmentSuccess) Logger::Error("Fragment shader " + fragmentShaderFilePath + " failed to be loaded.");

	compileShadersFromSource(vertexSource.c_str(), fragmentSource.c_str(), geometrySource.c_str());
}

void ShaderProgram::compileShadersFromSource(const char* vertexSource, const char* fragmentSource, const char* geometrySource /* = "" */)
{
	// Create the program
	m_programID = glCreateProgram();

	// Create the vertex shader
	m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (m_vertexShaderID == 0) {
		Logger::Error("Failed to create vertex shader.");
	}

	if (geometrySource != "") {
		m_geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		if (m_geometryShaderID == 0) {
			Logger::Error("Failed to create geometry shader");
		}
	}
	else {
		m_geometryShaderID = 0;
	}

	// Create the fragment shader
	m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (m_fragmentShaderID == 0) {
		Logger::Error("Failed to create fragment shader");
	}

	// Compile the shaders
	compileShader(vertexSource, "Vertex Shader", m_vertexShaderID);
	compileShader(fragmentSource, "Fragment Shader", m_fragmentShaderID);
	if (m_geometryShaderID != 0) compileShader(geometrySource, "Geometry Shader", m_geometryShaderID);

	linkShaders();
}

void ShaderProgram::compileShader(const char* source, const std::string& name, GLuint id)
{
	// Tell OpenGL that we want to use source as the contents of the shader
	glShaderSource(id, 1, &source, nullptr);

	// Compile the shader
	glCompileShader(id);

	// Check for errors
	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		GLint maxLenght = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLenght);

		// The maxLength includes the NULL character
		std::vector<char> errorLog(maxLenght);
		glGetShaderInfoLog(id, maxLenght, &maxLenght, &errorLog[0]);

		glDeleteShader(id);

		std::printf("%s\n", &errorLog[0]);
		Logger::Error(name + " '" + m_fragmentFilePath.substr(0, m_fragmentFilePath.length() - 5) + "' failed to compile.");
	}
}

void ShaderProgram::linkShaders()
{
	// Attach the shaders to the program
	glAttachShader(m_programID, m_vertexShaderID);
	glAttachShader(m_programID, m_fragmentShaderID);
	if (m_geometryFilePath != "")
		glAttachShader(m_programID, m_geometryShaderID);

	// Link the program
	glLinkProgram(m_programID);

	// Error checking
	GLint isLinked = 0;
	glGetProgramiv(m_programID, GL_LINK_STATUS, (int *)&isLinked);

	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

		// We don't need to program anymore
		glDeleteProgram(m_programID);

		std::printf("%s\n", &errorLog[0]);
		Logger::Error("Linking the shaders failed");
	}

	// Detach shaders after successful link
	glDetachShader(m_programID, m_vertexShaderID);
	glDetachShader(m_programID, m_fragmentShaderID);
	if (m_geometryFilePath != "")
		glDetachShader(m_programID, m_geometryShaderID);
	

	// Delete shaders as they are linked to the program
	glDeleteShader(m_vertexShaderID);
	glDeleteShader(m_fragmentShaderID);
	if (m_geometryFilePath != "")
		glDeleteShader(m_geometryShaderID);
}

GLint ShaderProgram::getUniformLocation(const std::string& uniformName)
{
	return glGetUniformLocation(m_programID, uniformName.c_str());
}

void ShaderProgram::use()
{
	glUseProgram(m_programID);
}

void ShaderProgram::unuse()
{
	glUseProgram(0);
}

void ShaderProgram::setUniformTexturesInitlist(std::initializer_list<std::string> textureNames)
{
	m_uniformTextures.clear();

	unsigned int index = 0;
	this->use();
	for (auto t : textureNames) {
		m_uniformTextures.push_back(t);
		setInt(t.c_str(), index++);
	}
	this->unuse();
}

void ShaderProgram::reload()
{
	// Re-compile and link shaders
	glDeleteProgram(m_programID);
	compile(m_vertexFilePath, m_fragmentFilePath, m_geometryFilePath);
	linkShaders();
	
	// Set texture uniforms
	this->use();
	for (GLuint i = 0; i < m_uniformTextures.size(); i++) {
		setInt(m_uniformTextures[i].c_str(), i);
	}
	this->unuse();
}

void ShaderProgram::setInt(const char* name, const int& value)
{
	glUniform1i(getUniformLocation(name), value);
}

void ShaderProgram::setFloat(const char* name, const float& value)
{
	glUniform1f(getUniformLocation(name), value);
}

void ShaderProgram::setVec2(const char* name, const glm::vec2& value)
{
	glUniform2f(getUniformLocation(name), value.x, value.y);
}

void ShaderProgram::setVec3(const char* name, const glm::vec3& value)
{
	glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
}

void ShaderProgram::setVec3(const char* name, const float& v0, const float& v1, const float& v2)
{
	setVec3(name, glm::vec3(v0, v1, v2));
}

void ShaderProgram::setMat4(const char* name, const glm::mat4& value)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setKernel(const char* name, const float value[9][2])
{
	glUniform2fv(getUniformLocation(name), 9, (GLfloat*)value);
}

void ShaderProgram::setIntArray(const char* name, const int nrElements, const int value[])
{
	glUniform1iv(getUniformLocation(name), nrElements, value);
}

void ShaderProgram::setFloatArray(const char* name, const int nrElements, const float value[])
{
	glUniform1fv(getUniformLocation(name), nrElements, value);
}

}