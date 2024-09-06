#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace JaroViewer {
	class FrameBuffer {
		public:
			FrameBuffer(int width, int height, bool readableColor, bool readableDepthStencil);
			~FrameBuffer();

			void bind() const;
			void unbind() const;
			
			unsigned int getTexture() const;
			unsigned int getDepthStencil() const;

			void deleteBuffer();

		private:
			void genBuffer();
			unsigned int genTexure(GLenum formatType);
			unsigned int genRenderBuffer(GLenum formatType);
			unsigned int bindTexture(GLenum usage);
			unsigned int bindRenderBuffer(GLenum usage);
			void createStorage(bool readable, unsigned int *texureStorage, unsigned int *renderStorage, GLenum usage);

			unsigned int mID;
			unsigned int mColorTexture;
			unsigned int mDepthStencilTexture;
			unsigned int mColorRBO;
			unsigned int mDepthStencilRBO;

			int mWidth;
			int mHeight;
	};
}
