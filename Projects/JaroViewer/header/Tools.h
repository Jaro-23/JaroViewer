#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

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
			static unsigned int generateBuffer(std::vector<T, Allocator> &data, GLenum bufferType, GLenum usage) {
				unsigned int buffer;
				glGenBuffers(1, &buffer);
				glBindBuffer(bufferType, buffer);
				glBufferData(bufferType, data.size() * sizeof(T), &data[0], usage);
				return buffer;
			}

			static glm::mat3 getNormalModelMatrix(const glm::mat4 &model) {
				return glm::mat3(glm::transpose(glm::inverse(model)));
			};

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
