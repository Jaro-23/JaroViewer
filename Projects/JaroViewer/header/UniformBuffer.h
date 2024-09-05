#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace JaroViewer {
	class UniformBuffer {
		public:
			UniformBuffer(unsigned int position, unsigned int size, GLenum usage);
			void UpdateData(const void* data);

		private:
			unsigned int mSize;
			unsigned int mID;
	};
}
