#pragma once

#include <GLAD/glad.h>
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

			/**
			 * Generates a uniform buffer
			 * @param location The location of that the uniform buffer will be placed on
			 * @param size The size of the uniform buffer
			 * @param usage The usage of the buffer
			 */
			static unsigned int generateUniformBuffer(unsigned int location, unsigned int size, GLenum usage) {
				unsigned int id;
				glGenBuffers(1, &id);
				glBindBuffer(GL_UNIFORM_BUFFER, id);
				glBufferData(GL_UNIFORM_BUFFER, size, NULL, usage);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
				glBindBufferRange(GL_UNIFORM_BUFFER, location, id, 0, size);
				return id;
			}

			/**
			 * Loads data into a uniform buffer
			 * @param id The id of the uniform buffer
			 * @param data A pointer to the data struct
			 * @param size The size of the data
			 */
			static void updateUniformBufferData(unsigned int id, const void* data, unsigned int size) {
				glBindBuffer(GL_UNIFORM_BUFFER, id);
				glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
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
