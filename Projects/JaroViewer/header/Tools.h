#pragma once

#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

#include <iostream>
#include <sstream>
#include <vector>

namespace JaroViewer {
	class Tools {
		public:
			/**
			 * Creates a buffer of the given type
			 * @param data The data that will be placed in the buffer
			 * @param bufferType The type of buffer that will be created
			 * @param usage The usage of the buffer
			 */
			template <typename T, typename Allocator>
			static unsigned int generateBuffer(const std::vector<T, Allocator> &data, GLenum bufferType, GLenum usage) {
				unsigned int buffer;
				glGenBuffers(1, &buffer);
				glBindBuffer(bufferType, buffer);
				glBufferData(bufferType, data.size() * sizeof(T), &data[0], usage);
				return buffer;
			}

			static glm::mat3 getNormalModelMatrix(const glm::mat4 &model) {
				return glm::mat3(glm::transpose(glm::inverse(model)));
			};

			/**
			 * Reads a file into a std::string
			 * @param filePath The path to the file
			 * @param out The output where in content of the file will be placed in
			 */
			static void readFile(const std::string &filePath, std::string *out) {
				std::ifstream fileStream;
				fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

				try {
					fileStream.open(filePath);
					std::stringstream stringStream;
					stringStream << fileStream.rdbuf();
					fileStream.close();
					*out = stringStream.str();
				}
				catch (std::ifstream::failure) {
					std::cout << "ERROR::TOOLS::FILE_NOT_SUCCESFULLY_READ::" << filePath << std::endl;
				}
			}

			struct LightColor {
				glm::vec3 ambient;
				glm::vec3 diffuse;
				glm::vec3 specular;
			};

			struct AttenuationParams{
				float constant;
				float linear;
				float quadratic;
			};
	};
}
