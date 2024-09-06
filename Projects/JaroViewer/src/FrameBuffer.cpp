#include "../header/FrameBuffer.h"
#include <iostream>
#include <ostream>

using namespace JaroViewer;

FrameBuffer::FrameBuffer(int width, int height, bool readableColor, bool readableDepthStencil) {
	mWidth = width;
	mHeight = height;
	
	genBuffer();
	createStorage(readableColor, &mColorTexture, &mColorRBO, GL_COLOR_ATTACHMENT0);
	createStorage(readableDepthStencil, &mDepthStencilTexture, &mDepthStencilRBO, GL_DEPTH_STENCIL_ATTACHMENT);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER::NOT_COMPLETE" << std::endl;
	unbind();
}

FrameBuffer::~FrameBuffer() {

}

void FrameBuffer::bind() const { glBindFramebuffer(GL_FRAMEBUFFER, mID); }
void FrameBuffer::unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

unsigned int FrameBuffer::getTexture() const {
	if (mColorTexture && mID != 0)
		return mColorTexture;
	std::cout << "ERROR::FRAMEBUFFER::TRYING_TO_READ_RENDER_BUFFER_COLOR";
	return 0;
}

unsigned int FrameBuffer::getDepthStencil() const {
	if (mDepthStencilTexture && mID != 0)
		return mDepthStencilTexture;
	std::cout << "ERROR::FRAMEBUFFER::TRYING_TO_READ_RENDER_BUFFER_DEPTH_STENCIL";
	return 0;
}

void FrameBuffer::genBuffer() {
	glGenFramebuffers(1, &mID);
}

/**
 * Create an empty texture
 * @param formatType The format for the texture 
 * @pre formatType == GL_RGB || formatType == GL_DEPTH24_STENCIL8
 */
unsigned int FrameBuffer::genTexure(GLenum formatType) {
	GLenum usage = (formatType == GL_RGB) ? GL_RGB : GL_DEPTH_STENCIL;
	GLenum variableType = (formatType == GL_RGB) ? GL_UNSIGNED_BYTE : GL_UNSIGNED_INT_24_8;

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, formatType, mWidth, mHeight, 0, usage, variableType, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return texture;
}

/**
 * Create an empty render buffer
 * @param formatType The format for the render buffer
 * @pre formatType == GL_RGB || formatType == GL_DEPTH24_STENCIL8
 */
unsigned int FrameBuffer::genRenderBuffer(GLenum formatType) {
	unsigned int buffer;
	glGenRenderbuffers(1, &buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, formatType, mWidth, mHeight);
	return buffer;
}

/**
 * Creates a texture and attaches it to the framebuffer
 * @param usage The usage of the texture
 * @return The id of the new texture that was bound to the framebuffer
 * @pre usage == GL_COLOR_ATTACHMENT{NUM} || usage == GL_DEPTH_STENCIL_ATTACHMENT
 */
unsigned int FrameBuffer::bindTexture(GLenum usage) {
	unsigned int texture = (usage == GL_DEPTH_STENCIL_ATTACHMENT) ? genTexure(GL_DEPTH24_STENCIL8) : genTexure(GL_RGB);
	bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, usage, GL_TEXTURE_2D, texture, 0);
	unbind();
	return texture;
}

/**
 * Creates a buffer and attaches it to the framebuffer
 * @param usage The usage of the buffer
 * @return The render buffer id
 * @pre usage == GL_COLOR_ATTACHMENT{num} || usage == GL_DEPTH_STENCIL_ATTACHMENT
 */
unsigned int FrameBuffer::bindRenderBuffer(GLenum usage) {
	unsigned int buffer = (usage == GL_DEPTH_STENCIL_ATTACHMENT) ? genRenderBuffer(GL_DEPTH24_STENCIL8) : genRenderBuffer(GL_RGB);
	bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, usage, GL_RENDERBUFFER, buffer);
	unbind();
	return buffer;
}

void FrameBuffer::createStorage(bool readable, unsigned int *texureStorage, unsigned int *renderStorage, GLenum usage) {
	if (readable)
		*texureStorage = bindTexture(usage);
	else
		*renderStorage = bindRenderBuffer(usage);
}
