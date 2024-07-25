#include "../header/Texture.h"

#include <GL/gl.h>
#include <cassert>
#include <iostream>
#include <STB_IMAGE/stb_image.h>

using namespace JaroViewer;

/**
 * Creates a textures based on the image provided
 * @param filepath The path to the image
 * @param type The type of GL texture
 * @param fileFormat The format of the image (png => GL_RGBA, jpg => GL_RGB)
 * @pre filepath != nullptr
 */

Texture::Texture(const char *filepath, GLenum type, GLenum fileFormat) : Texture{filepath, type, fileFormat, false} {}

/**
 * Creates a textures based on the image provided
 * @param filepath The path to the image
 * @param type The type of GL texture
 * @param fileFormat The format of the image (png => GL_RGBA, jpg => GL_RGB)
 * @param flip The image needs to be flipped for the texture
 * @pre filepath != nullptr
 */
Texture::Texture(const char *filepath, GLenum type, GLenum fileFormat, bool flip) :
	mType{type}
{
	assert(filepath != nullptr);
	genTexture();
	setupParameters();
	loadImageToTexture(filepath, fileFormat, flip);
}

/**
 * Binds the texture to a certain position
 * @param position The position the texture has to be placed in
 * @pre position < 32
 */
void Texture::bind(unsigned int position) {
	assert(position < 32);
	glActiveTexture(GL_TEXTURE0 + position);
	glBindTexture(mType, mTextureID);
}

unsigned int Texture::getID() const { return mTextureID; }

// Private methods
/**
 * Generates a gl texture and sets id to mTextureID
 */
void Texture::genTexture() {
	glGenTextures(1, &mTextureID);
	glBindTexture(mType, mTextureID);
}

/**
 * Sets how the texture will be wrapped
 */
void Texture::setupParameters() {
	glTexParameteri(mType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(mType, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(mType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(mType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/**
 * Load the image for the texture
 * @param filepath The path to the image
 * @param fileFormat The format that the file is in (png => GL_RGBA, jpg => GL_RGB)
 * @param flip If the image needs to be flipped
 */
void Texture::loadImageToTexture(const char *filepath, GLenum fileFormat, bool flip) {
	glBindTexture(mType, mTextureID);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(flip);
	unsigned char *imageData = stbi_load(filepath, &width, &height, &nrChannels, 0);

	if (imageData) {
		glTexImage2D(mType, 0, GL_RGB, width, height, 0, fileFormat, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(mType);
	}
	else {
		std::cout << "ERROR::TEXTURE::UNABLE_TO_LOAD_IMAGE" << std::endl;
		std::cout << stbi_failure_reason() << std::endl;
	}
	stbi_image_free(imageData);
}
