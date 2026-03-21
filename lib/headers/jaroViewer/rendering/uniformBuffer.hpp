#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sys/types.h>

namespace JaroViewer {
	class UniformBuffer {
	public:
		UniformBuffer(uint position, uint size, GLenum usage);
		void updateData(const void* data);

	private:
		uint mSize;
		uint mID;
	};
} // namespace JaroViewer
