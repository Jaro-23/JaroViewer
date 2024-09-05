#include "../header/UniformBuffer.h"

using namespace JaroViewer;

/**
 * Create a uniform buffer object
 * @param position The position of the uniform buffer
 * @param size The size of the uniform buffer
 * @param usage The usage of the uniform buffer
 */
UniformBuffer::UniformBuffer(unsigned int position, unsigned int size, GLenum usage) {
	mSize = size;
	glGenBuffers(1, &mID); 
	glBindBuffer(GL_UNIFORM_BUFFER, mID);
	glBufferData(GL_UNIFORM_BUFFER, mSize, NULL, usage);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, position, mID, 0, size);
}

/**
 * Updates the data in the uniform buffer
 * @param data A pointer to a struct containing the data
 */
void UniformBuffer::UpdateData(const void* data) {
	glBindBuffer(GL_UNIFORM_BUFFER, mID);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, mSize, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
