#include "../header/Shader.h"
#include "../header/Config.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>

using namespace JaroViewer;

/**
 * Creates a shader by merging all files together for 1 type
 * @param vertexPaths The paths to all vertex files to be merged in the shader
 * @param fragmentPaths The paths to all fragment files to be merged in the shader
 */
Shader::Shader(const std::vector<std::string> &vertexPaths, const std::vector<std::string> &fragmentPaths) {
	unsigned int vertex = createShader(GL_VERTEX_SHADER, &vertexPaths, "VERTEX");
	unsigned int fragment = createShader(GL_FRAGMENT_SHADER, &fragmentPaths, "FRAGMENT");

	createProgram(vertex, 0, fragment);
}

/**
 * Creates a shader by merging all files together for 1 type
 * @param vertexPaths The paths to all vertex files to be merged in the shader
 * @param fragmentPaths The paths to all fragment files to be merged in the shader
 * @param geometryPaths The paths to all geometry files to be merged in the shader
 */
Shader::Shader(const std::vector<std::string> &vertexPaths, const std::vector<std::string> &fragmentPaths, const std::vector<std::string> &geometryPaths) {
	unsigned int vertex = createShader(GL_VERTEX_SHADER, &vertexPaths, "VERTEX");
	unsigned int geometry = createShader(GL_GEOMETRY_SHADER, &geometryPaths, "GEOMETRY");
	unsigned int fragment = createShader(GL_FRAGMENT_SHADER, &fragmentPaths, "FRAGMENT");

	createProgram(vertex, geometry, fragment);
}

void Shader::use() { glUseProgram(mProgramID); }
void Shader::setBool(const std::string &name, bool value) const { glUniform1i(getLocation(name), value); }
void Shader::setInt(const std::string &name, int value) const { glUniform1i(getLocation(name), value); }

void Shader::setFloat1(const std::string &name, float x) const { return glUniform1f(getLocation(name), x); }
void Shader::setFloat2(const std::string &name, float x, float y) const { return glUniform2f(getLocation(name), x, y); }
void Shader::setFloat3(const std::string &name, float x, float y, float z) const { return glUniform3f(getLocation(name), x, y, z); }
void Shader::setFloat4(const std::string &name, float x, float y, float z, float w) const { return glUniform4f(getLocation(name), x, y, z, w); }

void Shader::setVec3(const std::string &name, glm::vec3 vec) const { glUniform3fv(getLocation(name), 1, glm::value_ptr(vec)); }
void Shader::setMat4(const std::string &name, glm::mat4 mat) const { glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(mat)); }

/**
 * Reads a file into a std::string
 * @param filePath The path to the file
 * @param out The output where in content of the file will be placed in
 */
void Shader::readFile(const std::string &filePath, std::string *out) const {
	std::ifstream fileStream;
	fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		fileStream.open(JAROVIEWER_PATH + "/" + filePath);
		std::stringstream stringStream;
		stringStream << fileStream.rdbuf();
		fileStream.close();
		*out = stringStream.str();
	}
	catch (std::ifstream::failure) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ::" << filePath << std::endl;
	}
}

/**
 * Creates a gl shader based on the contents of files
 * @param shaderType The type of shader that needs to be created
 * @param sources The paths to the source files containing the code of the shader
 * @param errorName The name that will be displayed if there is an error
 */
unsigned int Shader::createShader(GLenum shaderType, const std::vector<std::string> *sources, const std::string &errorName) {
	// Read the code from the file
	std::string fullFile = "";
	std::string out;
	for (int i = 0; i < sources->size(); i++) {
		readFile(sources->at(i), &out);
		fullFile += out;
	}
	const char *code = fullFile.c_str();

	// Compiling the shader
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
