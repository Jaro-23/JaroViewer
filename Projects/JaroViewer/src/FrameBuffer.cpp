#include "../header/FrameBuffer.h"
#include <iostream>
#include <ostream>

using namespace JaroViewer;

/**
 * Creates a framebuffer with a certain size
 * @param width The width of the framebuffer
 * @param height The height of the framebuffer
 * @param readableColor If it is possible to read the color buffer
 * @param readableDepthStencil If it is possible to read the depth2_stencil8 buffer
 */
FrameBuffer::FrameBuffer(int width, int height, bool readableColor, bool readableDepthStencil) {
	mWidth = width;
	mHeight = height;
	
	genBuffer();
	bind();
	createStorage(readableColor, &mColorTexture, &mColorRBO, GL_COLOR_ATTACHMENT0);
	createStorage(readableDepthStencil, &mDepthStencilTexture, &mDepthStencilRBO, GL_DEPTH_STENCIL_ATTACHMENT);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER::NOT_COMPLETE" << std::endl;
	unbind();
}

FrameBuffer::~FrameBuffer() {

}

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

/**
 * Returns the color buffer as a texture id
 * @return The id of the texture with all the data
 */
unsigned int FrameBuffer::getColor() const {
	if (mColorTexture && mID != 0)
		return mColorTexture;
	std::cout << "ERROR::FRAMEBUFFER::TRYING_TO_READ_RENDER_BUFFER_COLOR";
	return 0;
}

/**
 * Returns the depth stencil buffer as a texture id
 * @return The id of the texture with all the data
 */
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
	glFramebufferTexture2D(GL_FRAMEBUFFER, usage, GL_TEXTURE_2D, texture, 0);
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
void FrameBuffer::createStorage(bool readable, unsigned int *texureStorage, unsigned int *renderStorage, GLenum usage) {
	if (readable)
		*texureStorage = bindTexture(usage);
	else
		*renderStorage = bindRenderBuffer(usage);
}
