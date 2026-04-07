#include "jaroViewer/graphics/texture2D.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cassert>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace JaroViewer;

/**
 * Creates a textures based on the image provided
 * @param filepath The path to the image
 * @param type The type of GL texture
 * @pre filepath != nullptr
 */

Texture2D::Texture2D(const std::string& filepath) : Texture2D{filepath, true} {}

/**
 * Creates a textures based on the image provided
 * @param filepath The path to the image
 * @param type The type of GL texture
 * @param flip The image needs to be flipped for the texture
 * @pre filepath != nullptr
 */
Texture2D::Texture2D(const std::string& filepath, bool flip) {
	genTexture();
	setupParameters();
	loadImageToTexture(filepath.c_str(), flip);
}

Texture2D::Texture2D(const glm::vec4 color) {
	genTexture();
	setupParameters();

	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureID, 0);

	glViewport(0, 0, 512, 512);

	glClearColor(color.x, color.y, color.z, color.a);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 * Binds the texture to a certain position
 * @param position The position the texture has to be placed in
 * @pre position < 32
 */
void Texture2D::bind(unsigned int position) const {
	assert(position < 32);
	glActiveTexture(GL_TEXTURE0 + position);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}

unsigned int Texture2D::getID() const { return mTextureID; }

// Private methods
/**
 * Generates a gl texture and sets id to mTextureID
 */
void Texture2D::genTexture() {
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}

/**
 * Sets how the texture will be wrapped
 */
void Texture2D::setupParameters() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/**
 * Load the image for the texture
 * @param filepath The path to the image
 * @param flip If the image needs to be flipped
 */
void Texture2D::loadImageToTexture(const char* filepath, bool flip) {
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* imageData = stbi_load(filepath, &width, &height, &nrChannels, 0);

	if (imageData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, getFileFormat(nrChannels), GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "[Texture 2D] Error: Unable to load image => " << filepath << std::endl;
		std::cout << stbi_failure_reason() << std::endl;
	}
	stbi_image_free(imageData);
}

uint Texture2D::getFileFormat(int numChannels) const {
	switch (numChannels) {
	case 1: return GL_RED;
	case 3: return GL_RGB;
	case 4: return GL_RGBA;
	default:
		std::cout << "[Texture 2D] Error: Unknown number of channels => "
		          << numChannels << std::endl;
		assert(false);
	}
}
