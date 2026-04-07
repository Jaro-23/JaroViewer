#include "jaroViewer/rendering/shader.hpp"
#include "jaroViewer/core/tools.hpp"

#include <glad/glad.h>

#include <cstdlib>
#include <iostream>
#include <ostream>
#include <sys/types.h>

using namespace JaroViewer;

Shader::Shader(const ShaderCode& code) {
	uint vertex = createShaderFromString(GL_VERTEX_SHADER, code.vertexCode.c_str(), "vertex");
	uint geometry = (code.geometryCode != "") ?
	  createShaderFromString(GL_GEOMETRY_SHADER, code.geometryCode.c_str(), "geometry") :
	  0;
	uint fragment =
	  createShaderFromString(GL_FRAGMENT_SHADER, code.fragmentCode.c_str(), "fragment");

	createProgram(vertex, geometry, fragment);
}

Shader::Shader(const ShaderPaths& paths) {
	uint vertex = createShaderFromFile(GL_VERTEX_SHADER, &paths.vertexPaths, "vertex");
	uint geometry = (paths.geometryPaths.size() > 0) ?
	  createShaderFromFile(GL_GEOMETRY_SHADER, &paths.geometryPaths, "geometry") :
	  0;
	uint fragment = createShaderFromFile(GL_FRAGMENT_SHADER, &paths.fragmentPaths, "fragment");

	createProgram(vertex, geometry, fragment);
}

void Shader::use() const { glUseProgram(mProgramId); }
void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(getLocation(name), value);
}
void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(getLocation(name), value);
}
void Shader::setUniformBuffer(const std::string& name, int position) const {
	uint id = glGetUniformBlockIndex(mProgramId, name.c_str());
	glUniformBlockBinding(mProgramId, id, position);
}

void Shader::setFloat1(const std::string& name, float x) const {
	return glUniform1f(getLocation(name), x);
}
void Shader::setFloat2(const std::string& name, float x, float y) const {
	return glUniform2f(getLocation(name), x, y);
}
void Shader::setFloat3(const std::string& name, float x, float y, float z) const {
	return glUniform3f(getLocation(name), x, y, z);
}
void Shader::setFloat4(const std::string& name, float x, float y, float z, float w) const {
	return glUniform4f(getLocation(name), x, y, z, w);
}

void Shader::setVec3(const std::string& name, glm::vec3 vec) const {
	glUniform3fv(getLocation(name), 1, glm::value_ptr(vec));
}
void Shader::setMat3(const std::string& name, glm::mat3 mat) const {
	glUniformMatrix3fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::setMat4(const std::string& name, glm::mat4 mat) const {
	glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

uint Shader::createShaderFromFile(GLenum shaderType, const std::vector<std::string>* sources, const std::string& errorName) {
	std::string fullFile = "";
	std::string out;
	for (size_t i = 0; i < sources->size(); i++) {
		Tools::readFile(sources->at(i), &out);
		fullFile += out;
	}
	const char* code = fullFile.c_str();
	return createShaderFromString(shaderType, code, errorName);
}

uint Shader::createShaderFromString(GLenum shaderType, const char* code, const std::string& errorName) {
	uint id = glCreateShader(shaderType);
	glShaderSource(id, 1, &code, NULL);
	glCompileShader(id);
	checkCompilingError(id, errorName);
	return id;
}

void Shader::createProgram(uint vertexID, uint geometryID, uint fragmentID) {
	mProgramId = glCreateProgram();
	glAttachShader(mProgramId, vertexID);
	if (geometryID != 0) glAttachShader(mProgramId, geometryID);
	glAttachShader(mProgramId, fragmentID);
	glLinkProgram(mProgramId);
	checkLinkingError(mProgramId);

	glDeleteShader(vertexID);
	if (geometryID != 0) glDeleteShader(geometryID);
	glDeleteShader(fragmentID);
}

/**
 * Checks if the shader had compiling errors
 * @param shaderID The id of the shader
 * @param shaderName The name that will be printed if there is an error
 */
void Shader::checkCompilingError(uint shaderID, const std::string& shaderName) const {
	int success;
	char infoLog[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(mProgramId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << shaderName << "::COMPILATION_ERROR\n"
		          << infoLog << std::endl;
	}
}

void Shader::checkLinkingError(uint programID) const {
	int success;
	char infoLog[512];
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(mProgramId, 512, NULL, infoLog);
		std::cout << "[Shader] Error: Linking failed (" << infoLog << ")" << std::endl;
	}
}

GLint Shader::getLocation(const std::string& name) const {
	return glGetUniformLocation(mProgramId, name.c_str());
}
