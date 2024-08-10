#include "../header/Component3D.h"
#include "GLM/ext/matrix_transform.hpp"
#include "GLM/fwd.hpp"
#include "GLM/matrix.hpp"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <vector>

using namespace JaroViewer;

Component3D::Component3D(const Shader &shader, const Shader &wireframeShader) :
	mShader{ shader },
	mWireframeShader{ wireframeShader },
	mChildren{std::vector<std::shared_ptr<Component3D>>()},
	mWireframeMode{false},
	mUseIndices{true},
	mTranslation{glm::vec3(0.0f)},
	mAngleX{0.0f}, mAngleY{0.0f}, mAngleZ{0.0f},
	mScale{glm::vec3(1.0f)},
	mMaterials{std::vector<Material>()}
{
	mWireframeShader.setUniformBuffer("Transformation", 0);
}

void Component3D::setWireframeMode(bool enable) { mWireframeMode = enable; }

void Component3D::addTranslation(const glm::vec3 &translation) { mTranslation += translation; }
void Component3D::addRotation(float angleX, float angleY, float angleZ) {
	mAngleY += angleX;
	mAngleY += angleY;
	mAngleZ += angleZ;
	normalizeAngles();
}
void Component3D::addScale(const glm::vec3 &scale) { mScale += scale; }
void Component3D::addScale(float scale) { mScale += glm::vec3(scale); }

void Component3D::setTranslation(const glm::vec3 &translation) { mTranslation = translation; }
void Component3D::setRotation(float angleX, float angleY, float angleZ) {
	mAngleX = angleX;
	mAngleY = angleY;
	mAngleZ = angleZ;
	normalizeAngles();
}
void Component3D::setScale(const glm::vec3 &scale) { mScale = scale; }
void Component3D::setScale(float scale) { mScale = glm::vec3(scale); }

void Component3D::addComponent(std::shared_ptr<Component3D> &component) {
	mChildren.push_back(component);
	component->setTranslation(mTranslation);
	component->setRotation(mAngleX, mAngleY, mAngleZ);
	component->setScale(mScale);
	component->load();
}

/**
 * Adds a texture to the component
 * @param texture The texture to be added to the component
 */
void Component3D::addMaterial(const Material &material) {
	if (mMAXMATERIALS <= mMaterials.size()) {
		std::cout << "ERROR::COMPONENT::MAX_MATERIALS_AMOUNT" << std::endl;
		return;
	}
	mMaterials.push_back(material);
}

/**
 * Renders the component
 * @param data The renderdata that applies to this component
 */
void Component3D::render(const RenderData &data) {
	if (mVaoBuffer == 0) return;

	glBindVertexArray(mVaoBuffer);
	if (mWireframeMode)
		wireframeRender(data);
	else
		defaultRender(data);

	if (mUseIndices)
		glDrawElements(GL_TRIANGLES, mNumLines, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, mNumLines);

	for (int i = 0; i < mChildren.size(); i++) mChildren.at(i)->render(data);
}

/**
 * Returns the model matrix with all the transformations for this component
 */
glm::mat4 Component3D::getModelMatrix() const {
	glm::mat4 model(1.0f);
	model = glm::translate(model, mTranslation);
	model = glm::rotate(model, mAngleX, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, mAngleY, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, mAngleZ, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, mScale);
	return model;
}

glm::mat3 Component3D::getNormalModelMatrix(const glm::mat4 &model) const {
	return glm::mat3(glm::transpose(glm::inverse(model)));
};

glm::vec3 Component3D::getPosition() const {
	return mTranslation;
}

void Component3D::setUseIndices(bool use) {
	mUseIndices = use;
}

/**
 * Loads the materials in mMaterials into the shader
 * @param shader The shader the materials will be loaded into
 */
void Component3D::bindMaterials(const Shader &shader) const {
	if (mMaterials.size() == 1) {
		mMaterials.at(0).load(&shader);
		return;
	}
	for (unsigned int i = 0; i < mMaterials.size(); i++)
		mMaterials.at(i).loadIntoArray(&shader, i);
}

// Private methods

/**
 * Renders the wireframe of the component
 * @param data The data needed to render the component
 */
void Component3D::wireframeRender(const RenderData &data) {
	mWireframeShader.use();
	mWireframeShader.setMat4("model", getModelMatrix());	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

/**
 * Renders the component normally
 * @param data The data needed to render the component
 */
void Component3D::defaultRender(const RenderData &data) {
	mShader.use();
	glm::mat4 model = getModelMatrix();
	mShader.setMat4("model", model);
	mShader.setMat3("normalModel", getNormalModelMatrix(model));
	mShader.setVec3("viewPos", data.viewPos);
	bindMaterials(mShader);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}	

/**
 * Places the angles between 0 and 360 degree
 * @post mAngleX, mAngleY and mAngleZ are between 0 and 360
 */
void Component3D::normalizeAngles() {
	while (mAngleX < 0.0f) mAngleX += 360.0f;
	while (mAngleX > 360.0f) mAngleX -= 360.0f;

	while (mAngleY < 0.0f) mAngleY += 360.0f;
	while (mAngleY > 360.0f) mAngleY -= 360.0f;

	while (mAngleZ < 0.0f) mAngleZ += 360.0f;
	while (mAngleZ > 360.0f) mAngleZ -= 360.0f;
}
