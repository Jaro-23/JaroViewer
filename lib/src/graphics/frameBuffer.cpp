#include "jaroViewer/graphics/frameBuffer.hpp"

#include <glad/glad.h>

#include <iostream>
#include <ostream>

using namespace JaroViewer;

FrameBuffer::FrameBuffer(const FrameBufferArgs& args) {
	mWidth                = args.width;
	mHeight               = args.height;
	mReadableColor        = args.readableColor;        // save these
	mReadableDepthStencil = args.readableDepthStencil; // save these
	mFormatType           = args.formatType;

	genBuffer();
	bind();
	createStorage(
	  args.readableColor, &mColorTexture, &mColorRBO, GL_COLOR_ATTACHMENT0, args.formatType
	);
	createStorage(args.readableDepthStencil, &mDepthStencilTexture, &mDepthStencilRBO, GL_DEPTH_STENCIL_ATTACHMENT, GL_DEPTH24_STENCIL8);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "[FrameBuffer] Error: Buffer is not complete" << std::endl;
	unbind();
}

FrameBuffer::~FrameBuffer() {}

// Binds the framebuffer so it will be drawn on
void FrameBuffer::bind() const { glBindFramebuffer(GL_FRAMEBUFFER, mID); }

// UnBinds the framebuffer so all draw calls will go to the screen
void FrameBuffer::unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

/**
 * Clears the framebuffer with a certain color
 * @param r The R component of the RGBA color
 * @param g The G component of the RGBA color
 * @param b The B component of the RGBA color
 * @param a The A component of the RGBA color
 */
void FrameBuffer::clear(float r, float g, float b, float a) const {
	int draw = 0, read = 0;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &draw);
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &read);

	glBindFramebuffer(GL_FRAMEBUFFER, mID);
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, read);
}

void FrameBuffer::resize(uint width, uint height) {
	mWidth  = width;
	mHeight = height;

	// Delete existing attachments
	if (mColorTexture) glDeleteTextures(1, &mColorTexture);
	if (mDepthStencilTexture) glDeleteTextures(1, &mDepthStencilTexture);
	if (mColorRBO) glDeleteRenderbuffers(1, &mColorRBO);
	if (mDepthStencilRBO) glDeleteRenderbuffers(1, &mDepthStencilRBO);
	if (mID) glDeleteFramebuffers(1, &mID);

	mColorTexture = mDepthStencilTexture = 0;
	mColorRBO = mDepthStencilRBO = mID = 0;

	// Recreate with new dimensions
	genBuffer();
	bind();
	createStorage(mReadableColor, &mColorTexture, &mColorRBO, GL_COLOR_ATTACHMENT0, mFormatType);
	createStorage(mReadableDepthStencil, &mDepthStencilTexture, &mDepthStencilRBO, GL_DEPTH_STENCIL_ATTACHMENT, GL_DEPTH24_STENCIL8);
	unbind();
}

/**
 * Returns the color buffer as a texture id
 * @return The id of the texture with all the data
 */
uint FrameBuffer::getColor() const {
	if (mColorTexture && mID != 0) return mColorTexture;
	std::cout << "ERROR::FRAMEBUFFER::TRYING_TO_READ_RENDER_BUFFER_COLOR";
	return 0;
}

/**
 * Returns the depth stencil buffer as a texture id
 * @return The id of the texture with all the data
 */
uint FrameBuffer::getDepthStencil() const {
	if (mDepthStencilTexture && mID != 0) return mDepthStencilTexture;
	std::cout << "ERROR::FRAMEBUFFER::TRYING_TO_READ_RENDER_BUFFER_DEPTH_STENCIL";
	return 0;
}

void FrameBuffer::genBuffer() { glGenFramebuffers(1, &mID); }

/**
 * Create an empty texture
 * @param formatType The format for the texture
 */
uint FrameBuffer::genTexure(GLenum formatType) {
	GLenum format;
	GLenum type;

	if (formatType == GL_RGB) {
		format = GL_RGB;
		type   = GL_UNSIGNED_BYTE;
	} else if (formatType == GL_DEPTH24_STENCIL8) {
		format = GL_DEPTH_STENCIL;
		type   = GL_UNSIGNED_INT_24_8;
	} else if (formatType == GL_R32UI) {
		format = GL_RED_INTEGER;
		type   = GL_UNSIGNED_INT;
	} else {
		std::cout << "Unsupported texture format!" << std::endl;
		return 0;
	}

	uint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, formatType, mWidth, mHeight, 0, format, type, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return texture;
}

/**
 * Create an empty render buffer
 * @param formatType The format for the render buffer
 * @pre formatType == GL_RGB || formatType == GL_DEPTH24_STENCIL8
 */
uint FrameBuffer::genRenderBuffer(GLenum formatType) {
	uint buffer;
	glGenRenderbuffers(1, &buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, formatType, mWidth, mHeight);
	return buffer;
}

/**
 * Creates a texture and attaches it to the framebuffer
 * @param usage The usage of the texture
 * @return The id of the new texture that was bound to the framebuffer
 */
uint FrameBuffer::bindTexture(GLenum usage, GLenum formatType) {
	uint texture = genTexure(formatType);
	glFramebufferTexture2D(GL_FRAMEBUFFER, usage, GL_TEXTURE_2D, texture, 0);
	return texture;
}

/**
 * Creates a buffer and attaches it to the framebuffer
 * @param usage The usage of the buffer
 * @return The render buffer id
 * @pre usage == GL_COLOR_ATTACHMENT{num} || usage == GL_DEPTH_STENCIL_ATTACHMENT
 */
uint FrameBuffer::bindRenderBuffer(GLenum usage) {
	uint buffer = (usage == GL_DEPTH_STENCIL_ATTACHMENT) ?
	  genRenderBuffer(GL_DEPTH24_STENCIL8) :
	  genRenderBuffer(GL_RGB);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, usage, GL_RENDERBUFFER, buffer);
	return buffer;
}

/**
 * Creates storage for the framebuffer
 * @param readable if it needs to be readable
 * @param textureStorage A pointer to the place to store the texture if readble
 * @param renderStorage A pointer to the place to store the renderbuffer if not readble
 * @param usage If it is used as color attachment or depth stencil
 */
void FrameBuffer::createStorage(bool readable, uint* textureStorage, uint* renderStorage, GLenum usage, GLenum formatType) {
	if (readable)
		*textureStorage = bindTexture(usage, formatType);
	else
		*renderStorage = bindRenderBuffer(usage);
}
