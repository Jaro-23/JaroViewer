#include "jaroViewer/core/tools.hpp"

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>

using namespace JaroViewer;

template<typename T, typename Allocator>
unsigned int
  Tools::generateBuffer(const std::vector<T, Allocator>& data, unsigned int bufferType, unsigned int usage) {
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(bufferType, buffer);
	glBufferData(bufferType, data.size() * sizeof(T), &data[0], usage);
	return buffer;
}

template unsigned int Tools::generateBuffer(const std::vector<float>&, unsigned int, unsigned int);
template unsigned int
  Tools::generateBuffer(const std::vector<unsigned int>&, unsigned int, unsigned int);

glm::mat3 Tools::getNormalModelMatrix(const glm::mat4& model) {
	return glm::mat3(glm::transpose(glm::inverse(model)));
}

void Tools::readFile(const std::string& filePath, std::string* out) {
	std::ifstream fileStream;
	fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		fileStream.open(filePath);
		std::stringstream stringStream;
		stringStream << fileStream.rdbuf();
		fileStream.close();
		*out = stringStream.str();
	} catch (std::ifstream::failure&) {
		std::cout << "[Tools] Error: Coudn't read " << filePath << std::endl;
	}
}
