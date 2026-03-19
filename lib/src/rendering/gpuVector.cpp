#include "rendering/gpuVector.hpp"
#include <algorithm>
#include <cassert>
#include <cstring>

using namespace JaroViewer;

GpuVector::GpuVector() {
	genDataTex(&mTexture, &mBuffer, 1024);
} // TODO: What should be the starting size

size_t GpuVector::size() const {
	GLint64 size;
	GLint prevBuffer = 0;

	glGetIntegerv(GL_TEXTURE_BINDING_BUFFER, &prevBuffer);

	glBindBuffer(GL_TEXTURE_BUFFER, mBuffer);
	glGetBufferParameteri64v(GL_TEXTURE_BUFFER, GL_BUFFER_SIZE, &size);

	return static_cast<size_t>(size);
}

void GpuVector::load(uint position) const {
	assert(position < 32);
	glActiveTexture(GL_TEXTURE0 + position);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}

void GpuVector::copy(const std::vector<float>& data, size_t offset) {
	mCount = std::max(offset + data.size(), mCount);
	if (mCount > size()) enlarge();

	glBindBuffer(GL_TEXTURE_BUFFER, mBuffer);
	float* ptr = (float*)glMapBufferRange(
	  GL_TEXTURE_BUFFER, offset * sizeof(float), data.size() * sizeof(float),
	  GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT
	);
	std::memcpy(ptr, data.data(), data.size() * sizeof(float));
	glUnmapBuffer(GL_TEXTURE_BUFFER);
}

void GpuVector::move(size_t from, size_t to) { move(from, to, mCount - from); }
void GpuVector::move(size_t from, size_t to, size_t count) {
	mCount = std::max(to + count, mCount);
	if (mCount > size()) enlarge();

	GLuint temp;
	glGenBuffers(1, &temp);
	glBindBuffer(GL_COPY_WRITE_BUFFER, temp);
	glBufferData(GL_COPY_WRITE_BUFFER, count, nullptr, GL_STATIC_DRAW);

	glBindBuffer(GL_COPY_READ_BUFFER, mBuffer);
	glBindBuffer(GL_COPY_WRITE_BUFFER, temp);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, from, 0, count);

	glBindBuffer(GL_COPY_READ_BUFFER, temp);
	glBindBuffer(GL_COPY_WRITE_BUFFER, mBuffer);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, to, count);

	glDeleteBuffers(1, &temp);
}

void GpuVector::enlarge(float factor) {
	GLuint newTex, newBuf;
	genDataTex(&newTex, &newBuf, size() * factor);
	copy(mBuffer, newBuf);

	glDeleteBuffers(1, &mBuffer);
	glDeleteTextures(1, &mTexture);
	mBuffer  = newBuf;
	mTexture = newTex;
}

void GpuVector::copy(GLuint fromBuffer, GLuint toBuffer) {
	glBindBuffer(GL_COPY_READ_BUFFER, fromBuffer);
	glBindBuffer(GL_COPY_WRITE_BUFFER, toBuffer);

	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, mCount);
}

void GpuVector::genDataTex(GLuint* tex, GLuint* tbo, size_t size) {
	glGenBuffers(1, tbo);
	glBindBuffer(GL_TEXTURE_BUFFER, *tbo);
	glBufferData(GL_TEXTURE_BUFFER, size * sizeof(float), nullptr, GL_STATIC_DRAW);

	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_BUFFER, *tex);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, *tbo);
}
