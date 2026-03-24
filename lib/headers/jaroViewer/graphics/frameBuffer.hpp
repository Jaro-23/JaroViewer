#pragma once

#include "glad/glad.h"
#include <sys/types.h>

namespace JaroViewer {
	struct FrameBufferArgs {
		int width;
		int height;
		bool readableColor;
		bool readableDepthStencil;
		GLenum formatType;
	};

	class FrameBuffer {
	public:
		FrameBuffer(const FrameBufferArgs& args);
		~FrameBuffer();

		void bind() const;
		void unbind() const;

		void resize(uint width, uint height);
		void clear(float r, float g, float b, float a) const;

		uint getColor() const;
		uint getDepthStencil() const;

	private:
		void genBuffer();
		uint genTexure(GLenum formatType);
		uint genRenderBuffer(GLenum formatType);
		uint bindTexture(GLenum usage, GLenum formatType);
		uint bindRenderBuffer(GLenum usage);
		void createStorage(bool readable, uint* textureStorage, uint* renderStorage, GLenum usage, GLenum formatType);

		uint mID;
		uint mColorTexture;
		uint mDepthStencilTexture;
		uint mColorRBO;
		uint mDepthStencilRBO;

		int mWidth;
		int mHeight;
		bool mReadableColor;
		bool mReadableDepthStencil;
		GLenum mFormatType;
	};
} // namespace JaroViewer
