#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

namespace JaroViewer {
	class Texture2D {
	public:
		Texture2D(const std::string& filepath);
		Texture2D(const std::string& filepath, bool flip);
		Texture2D(const glm::vec4 color);

		void bind(unsigned int position) const;
		unsigned int getID() const;

	private:
		void genTexture();
		void setupParameters();
		void loadImageToTexture(const char* filepath, bool flip);
		GLenum getFileFormat(int numChannels) const;

		unsigned int mTextureID;
	};
} // namespace JaroViewer
