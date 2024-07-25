#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

namespace JaroViewer {
	class Texture {
		public:
			Texture(const char *filepath, GLenum type, GLenum fileformat);
			Texture(const char *filepath, GLenum type, GLenum fileFormat, bool flip);

			void bind(unsigned int position);
			unsigned int getID() const;

		private:
			void genTexture();
			void setupParameters();
			void loadImageToTexture(const char *filepath, GLenum fileFormat, bool flip);

			GLenum mType;
			unsigned int mTextureID;
	};
}
