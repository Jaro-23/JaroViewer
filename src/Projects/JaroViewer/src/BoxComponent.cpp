#include "../header/BoxComponent.h"
#include <iostream>
#include <GLM/gtx/string_cast.hpp>

using namespace JaroViewer;

BoxComponent::BoxComponent(const Shader &shader) :
	Component3D(shader)
{}

void BoxComponent::load() {
	std::vector<float> vertices = {
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,

		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
	};

	std::vector<unsigned int> indices = {
		1, 3, 0,
		1, 2, 3,
		2, 7, 3,
		2, 6, 7,
		6, 4, 7,
		6, 5, 4,
		5, 0, 4,
		5, 1, 0,
		3, 4, 0,
		3, 7, 4,
		1, 6, 2,
		1, 5, 6
	};

	// Create the VAO
	glGenVertexArrays(1, &mVaoBuffer);
	glBindVertexArray(mVaoBuffer);
	
	unsigned int vboBuffer;
	glGenBuffers(1, &vboBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vboBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	unsigned int indicesBuffer;
	glGenBuffers(1, &indicesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
