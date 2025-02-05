#include "../header/Mesh.h"
#include "../header/Tools.h"

using namespace JaroViewer;

Mesh::Mesh(const std::vector<float> &vertices, std::vector<unsigned int> &indices, std::vector<Material> &materials) :
	mMaterials{materials}
{
	// Setup buffers
	mNumIndices = indices.size();

	glGenVertexArrays(1, &mVaoBuffer);
	glBindVertexArray(mVaoBuffer);
	
	Tools::generateBuffer(vertices, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	Tools::generateBuffer(indices, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Mesh::Draw(const std::shared_ptr<Shader> &shader) {
	shader->use();
	shader->setInt("numTextures", mMaterials.size());
	glBindVertexArray(mVaoBuffer);
	for (unsigned int i = 0; i < mMaterials.size(); i++)
		mMaterials.at(i).loadIntoArray(shader, i);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT, 0);
}
