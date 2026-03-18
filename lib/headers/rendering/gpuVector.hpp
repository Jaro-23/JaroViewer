#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

namespace JaroViewer {
	class GpuVector {
	public:
		GpuVector();

		size_t size() const;

		void copy(const std::vector<float>& data, size_t offset);
		void move(size_t from, size_t to);
		void move(size_t from, size_t to, size_t count);

	private:
		void enlarge(float factor = 2.0);
		void copy(GLuint fromBuffer, GLuint toBuffer);
		void genDataTex(GLuint* tex, GLuint* tbo, size_t size);

		size_t mCount;
		GLuint mTexture;
		GLuint mBuffer;
	};
} // namespace JaroViewer
