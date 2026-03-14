#include "rendering/uniformBuffer.hpp"

using namespace JaroViewer;

UniformBuffer::UniformBuffer(uint position, uint size, GLenum usage) {
	mSize = size;
	glGenBuffers(1, &mID);
	glBindBuffer(GL_UNIFORM_BUFFER, mID);
	glBufferData(GL_UNIFORM_BUFFER, mSize, NULL, usage);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, position, mID, 0, size);
}

void UniformBuffer::updateData(const void* data) {
	glBindBuffer(GL_UNIFORM_BUFFER, mID);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, mSize, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
