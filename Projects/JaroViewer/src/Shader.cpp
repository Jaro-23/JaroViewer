#include "../header/Shader.h"
#include "../header/Tools.h"

#include <cstdlib>
#include <iostream>
#include <ostream>

using namespace JaroViewer;

/**
 * Creates a shader from 2 string containing the vertex and fragment code
 * @param code A struct with all code for the shader
 */
Shader::Shader(const ShaderCode &code) {
	unsigned int vertex = createShaderFromString(GL_VERTEX_SHADER, code.vertexCode->c_str(), "VERTEX");
	unsigned int geometry = (code.geometryCode) ? createShaderFromString(GL_GEOMETRY_SHADER, code.geometryCode->c_str(), "GEOMETRY") : 0;
	unsigned int fragment = createShaderFromString(GL_FRAGMENT_SHADER, code.fragmentCode->c_str(), "FRAGMENT");

	createProgram(vertex, geometry, fragment);
}

/**
 * Creates a shader by merging all files together for 1 type
 * @param vertexPaths The paths to all vertex files to be merged in the shader
 * @param fragmentPaths The paths to all fragment files to be merged in the shader
 */
Shader::Shader(const std::vector<std::string> &vertexPaths, const std::vector<std::string> &fragmentPaths) {
	unsigned int vertex = createShaderFromFile(GL_VERTEX_SHADER, &vertexPaths, "VERTEX");
	unsigned int fragment = createShaderFromFile(GL_FRAGMENT_SHADER, &fragmentPaths, "FRAGMENT");

	createProgram(vertex, 0, fragment);
}

/**
 * Creates a shader by merging all files together for 1 type
 * @param vertexPaths The paths to all vertex files to be merged in the shader
 * @param fragmentPaths The paths to all fragment files to be merged in the shader
 * @param geometryPaths The paths to all geometry files to be merged in the shader
 */
Shader::Shader(const std::vector<std::string> &vertexPaths, const std::vector<std::string> &geometryPaths, const std::vector<std::string> &fragmentPaths) {
	unsigned int vertex = createShaderFromFile(GL_VERTEX_SHADER, &vertexPaths, "VERTEX");
	unsigned int geometry = createShaderFromFile(GL_GEOMETRY_SHADER, &geometryPaths, "GEOMETRY");
	unsigned int fragment = createShaderFromFile(GL_FRAGMENT_SHADER, &fragmentPaths, "FRAGMENT");

	createProgram(vertex, geometry, fragment);
}

void Shader::use() { glUseProgram(mProgramID); }
void Shader::setBool(const std::string &name, bool value) const { glUniform1i(getLocation(name), value); }
void Shader::setInt(const std::string &name, int value) const { glUniform1i(getLocation(name), value); }
void Shader::setUniformBuffer(const std::string &name, int position) const {
	unsigned int id = glGetUniformBlockIndex(mProgramID, name.c_str());
	glUniformBlockBinding(mProgramID, id, position);
}

void Shader::setFloat1(const std::string &name, float x) const { return glUniform1f(getLocation(name), x); }
void Shader::setFloat2(const std::string &name, float x, float y) const { return glUniform2f(getLocation(name), x, y); }
void Shader::setFloat3(const std::string &name, float x, float y, float z) const { return glUniform3f(getLocation(name), x, y, z); }
void Shader::setFloat4(const std::string &name, float x, float y, float z, float w) const { return glUniform4f(getLocation(name), x, y, z, w); }

void Shader::setVec3(const std::string &name, glm::vec3 vec) const { glUniform3fv(getLocation(name), 1, glm::value_ptr(vec)); }
void Shader::setMat3(const std::string &name, glm::mat3 mat) const { glUniformMatrix3fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(mat)); }
void Shader::setMat4(const std::string &name, glm::mat4 mat) const { glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(mat)); }

/**
 * Creates a gl shader based on the contents of files
 * @param shaderType The type of shader that needs to be created
 * @param sources The paths to the source files containing the code of the shader
 * @param errorName The name that will be displayed if there is an error
 */
unsigned int Shader::createShaderFromFile(GLenum shaderType, const std::vector<std::string> *sources, const std::string &errorName) {
	// Read the code from the file
	std::string fullFile = "";
	std::string out;
	for (int i = 0; i < sources->size(); i++) {
		Tools::readFile(sources->at(i), &out);
		fullFile += out;
	}
	const char *code = fullFile.c_str();
	return createShaderFromString(shaderType, code, errorName);
}

/**
 * Creates a gl shader based on the contents of files
 * @param shaderType The type of shader that needs to be created
 * @param code The code for the shader
 * @param errorName The name that will be displayed if there is an error
 */
unsigned int Shader::createShaderFromString(GLenum shaderType, const char* code, const std::string &errorName) {
	unsigned int id = glCreateShader(shaderType);
	glShaderSource(id, 1, &code, NULL);
	glCompileShader(id);
	checkCompilingError(id, errorName);
	return id;
}

/**
 * Creates a program that contains the different shaders
 * @param vertexID The id for the vertexShader
 * @param geometryID The id for the geometryShader
 * @param fragmentID The id for the fragmentShader
 */
void Shader::createProgram(unsigned int vertexID, unsigned int geometryID, unsigned int fragmentID) {
	mProgramID = glCreateProgram();
	glAttachShader(mProgramID, vertexID);
	if (geometryID != 0) glAttachShader(mProgramID, geometryID);
	glAttachShader(mProgramID, fragmentID);
	glLinkProgram(mProgramID);
	checkLinkingError(mProgramID);

	glDeleteShader(vertexID);
	if (geometryID != 0) glDeleteShader(geometryID);
	glDeleteShader(fragmentID);
}

/**
 * Checks if the shader had compiling errors
 * @param shaderID The id of the shader
 * @param shaderName The name that will be printed if there is an error
 */
void Shader::checkCompilingError(unsigned int shaderID, const std::string &shaderName) const {
	int success;
	char infoLog[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(mProgramID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << shaderName << "::COMPILATION_ERROR\n" << infoLog << std::endl;
	}
}

/**
 * Checks if there is an error with linking the shaders to the program
 * @param programID The id of the program to be checked
 */
void Shader::checkLinkingError(unsigned int programID) const {
	int success;
	char infoLog[512];
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(mProgramID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}

GLint Shader::getLocation(const std::string &name) const { return glGetUniformLocation(mProgramID, name.c_str()); }
