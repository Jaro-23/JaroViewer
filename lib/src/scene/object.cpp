#include "scene/object.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>

using namespace JaroViewer;

Object::Object()
  : mTranslation(0.0f), mAngleX(0.0f), mAngleY(0.0f), mAngleZ(0.0f), mScale(1.0f) {}

Object::Object(Object&& other) noexcept
  : mTranslation(other.mTranslation),
    mAngleX(other.mAngleX),
    mAngleY(other.mAngleY),
    mAngleZ(other.mAngleZ),
    mScale(other.mScale),
    mModifiers(std::move(other.mModifiers)),
    mDeleteCallbacks(std::move(other.mDeleteCallbacks)),
    mTransformCallbacks(std::move(other.mTransformCallbacks)),
    mVisibilityCallbacks(std::move(other.mVisibilityCallbacks)) {}

Object& Object::operator=(Object&& other) noexcept {
	if (this == &other) return *this;
	mTranslation         = other.mTranslation;
	mAngleX              = other.mAngleX;
	mAngleY              = other.mAngleY;
	mAngleZ              = other.mAngleZ;
	mScale               = other.mScale;
	mModifiers           = std::move(other.mModifiers);
	mDeleteCallbacks     = std::move(other.mDeleteCallbacks);
	mTransformCallbacks  = std::move(other.mTransformCallbacks);
	mVisibilityCallbacks = std::move(other.mVisibilityCallbacks);
	return *this;
}

Object::~Object() {
	for (auto& callback : mDeleteCallbacks) callback();
}

void Object::setVisibility(bool visibility) {
	for (auto& callback : mVisibilityCallbacks) callback(visibility);
}

/**
 * Returns the model matrix with all the transformations for this component
 */
glm::mat4 Object::getModelMatrix() const {
	glm::mat4 model(1.0f);
	model = glm::translate(model, mTranslation);
	model = glm::rotate(model, mAngleX, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, mAngleY, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, mAngleZ, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, mScale);
	return model;
}

/**
 * Adds a translation to the current translation
 * @param translation The offset from the current translation
 */
void Object::addTranslation(const glm::vec3& translation) {
	mTranslation += translation;
	for (auto& callback : mTransformCallbacks) callback(getModelMatrix());
}

/**
 * Adds a rotation to the current rotation
 * @param angleX The new angle offset around the X-axis
 * @param angleY The new angle offset around the Y-axis
 * @param angleZ The new angle offset around the Z-axis
 */
void Object::addRotation(float angleX, float angleY, float angleZ) {
	mAngleY += angleX;
	mAngleY += angleY;
	mAngleZ += angleZ;
	normalizeAngles();
	for (auto& callback : mTransformCallbacks) callback(getModelMatrix());
}

/**
 * Adds a scaling offset to the current scale
 * @param scale A vec3 with x component the x scaling, y-component the y scaling and z-component the z scaling
 */
void Object::addScale(const glm::vec3& scale) {
	mScale += scale;
	for (auto& callback : mTransformCallbacks) callback(getModelMatrix());
}

/**
 * Adds a scaling offset to the current scale
 * @param scale A float value with which each scale value will be offset
 */
void Object::addScale(float scale) {
	mScale += glm::vec3(scale);
	for (auto& callback : mTransformCallbacks) callback(getModelMatrix());
}

/**
 * Sets the component to a position
 * @param translation The new position
 */
void Object::setTranslation(const glm::vec3& translation) {
	mTranslation = translation;
	for (auto& callback : mTransformCallbacks) callback(getModelMatrix());
}

/**
 * Sets the rotation of the component
 * @param angleX The rotation around the X-axis
 * @param angleY The rotation around the Y-axis
 * @param angleZ The rotation around the Z-axis
 */
void Object::setRotation(float angleX, float angleY, float angleZ) {
	mAngleX = angleX;
	mAngleY = angleY;
	mAngleZ = angleZ;
	normalizeAngles();
	for (auto& callback : mTransformCallbacks) callback(getModelMatrix());
}

/**
 * Sets the scale of the component
 * @param scale The new scale of each axis for the component
 */
void Object::setScale(const glm::vec3& scale) {
	mScale = scale;
	for (auto& callback : mTransformCallbacks) callback(getModelMatrix());
}

/**
 * Sets a the scale values to one specific value
 * @param scale The new scale value for each axis
 */
void Object::setScale(float scale) {
	mScale = glm::vec3(scale);
	for (auto& callback : mTransformCallbacks) callback(getModelMatrix());
}

void Object::addModifier(std::shared_ptr<Modifier> modifier) {
	mModifiers.push_back(modifier);
}

// TODO: Change this so it only updates the vector on modifier change and
// propagate event to object manager So it only updates the modifier stack
// shader on a change Does the count need to be changeable after creation?
ModifierStack Object::getModifierStack() const {
	ModifierStack out{(uint)mModifiers.size(), {}};
	for (auto& modifier : mModifiers) {
		std::vector<float> params = modifier->getParams();
		out.params.insert(out.params.end(), params.begin(), params.end());
	}

	return out;
}

void Object::subscribeDelete(std::function<void()> callback) {
	mDeleteCallbacks.push_back(callback);
}

void Object::subscribeTransform(std::function<void(glm::mat4)> callback) {
	mTransformCallbacks.push_back(callback);
}

void Object::subscribeVisibility(std::function<void(bool)> callback) {
	mVisibilityCallbacks.push_back(callback);
}

/**
 * Places the angles between 0 and 360 degree
 * @post mAngleX, mAngleY and mAngleZ are between 0 and 360
 */
void Object::normalizeAngles() {
	while (mAngleX < 0.0f) mAngleX += 360.0f;
	while (mAngleX > 360.0f) mAngleX -= 360.0f;

	while (mAngleY < 0.0f) mAngleY += 360.0f;
	while (mAngleY > 360.0f) mAngleY -= 360.0f;

	while (mAngleZ < 0.0f) mAngleZ += 360.0f;
	while (mAngleZ > 360.0f) mAngleZ -= 360.0f;
}
