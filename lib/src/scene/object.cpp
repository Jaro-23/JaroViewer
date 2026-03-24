#include "jaroViewer/scene/object.hpp"
#include "jaroViewer/core/eventSender.hpp"
#include "jaroViewer/modifiers/modifier.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>

using namespace JaroViewer;

Object::Object()
  : mTranslation(0.0f),
    mAngleX(0.0f),
    mAngleY(0.0f),
    mAngleZ(0.0f),
    mScale(1.0f),
    mVisibility(true) {}

Object::Object(Object&& other) noexcept
  : EventSender<Object, ObjectEvent>(std::move(other)),
    mTranslation(other.mTranslation),
    mAngleX(other.mAngleX),
    mAngleY(other.mAngleY),
    mAngleZ(other.mAngleZ),
    mScale(other.mScale),
    mVisibility(other.mVisibility),
    mLatestCallback(other.mLatestCallback),
    mModifiers(std::move(other.mModifiers)) {}

Object& Object::operator=(Object&& other) noexcept {
	if (this == &other) return *this;
	EventSender<Object, ObjectEvent>::operator=(std::move(other));
	mTranslation    = other.mTranslation;
	mAngleX         = other.mAngleX;
	mAngleY         = other.mAngleY;
	mAngleZ         = other.mAngleZ;
	mScale          = other.mScale;
	mVisibility     = other.mVisibility;
	mModifiers      = std::move(other.mModifiers);
	mLatestCallback = other.mLatestCallback;
	return *this;
}

Object::~Object() { send(this, ObjectEvent::DELETE); }

void Object::setVisibility(bool visibility) {
	mVisibility = visibility;
	send(this, ObjectEvent::VISIBILITY);
}

bool Object::getVisibility() const { return mVisibility; }

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

void Object::addClickCallback(std::function<void()> callback) {
	mLatestCallback = callback;
	send(this, ObjectEvent::CALLBACK);
}

std::function<void()> Object::getLatestCallback() const {
	return mLatestCallback;
}

/**
 * Adds a translation to the current translation
 * @param translation The offset from the current translation
 */
void Object::addTranslation(const glm::vec3& translation) {
	mTranslation += translation;
	send(this, ObjectEvent::TRANSFORM);
}

/**
 * Adds a rotation to the current rotation
 * @param angleX The new angle offset around the X-axis
 * @param angleY The new angle offset around the Y-axis
 * @param angleZ The new angle offset around the Z-axis
 */
void Object::addRotation(float angleX, float angleY, float angleZ) {
	mAngleX += angleX;
	mAngleY += angleY;
	mAngleZ += angleZ;
	normalizeAngles();
	send(this, ObjectEvent::TRANSFORM);
}

/**
 * Adds a scaling offset to the current scale
 * @param scale A vec3 with x component the x scaling, y-component the y scaling and z-component the z scaling
 */
void Object::addScale(const glm::vec3& scale) {
	mScale += scale;
	send(this, ObjectEvent::TRANSFORM);
}

/**
 * Adds a scaling offset to the current scale
 * @param scale A float value with which each scale value will be offset
 */
void Object::addScale(float scale) {
	mScale += glm::vec3(scale);
	send(this, ObjectEvent::TRANSFORM);
}

/**
 * Sets the component to a position
 * @param translation The new position
 */
void Object::setTranslation(const glm::vec3& translation) {
	mTranslation = translation;
	send(this, ObjectEvent::TRANSFORM);
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
	send(this, ObjectEvent::TRANSFORM);
}

/**
 * Sets the scale of the component
 * @param scale The new scale of each axis for the component
 */
void Object::setScale(const glm::vec3& scale) {
	mScale = scale;
	send(this, ObjectEvent::TRANSFORM);
}

/**
 * Sets a the scale values to one specific value
 * @param scale The new scale value for each axis
 */
void Object::setScale(float scale) {
	mScale = glm::vec3(scale);
	send(this, ObjectEvent::TRANSFORM);
}

void Object::addModifier(std::shared_ptr<Modifier> modifier) {
	modifier->addListener([this](Modifier*, ModifierEvent event) {
		if (event == ModifierEvent::UPDATE) this->send(this, ObjectEvent::MODIFIER);
	});

	mModifiers.push_back(modifier);
	modifier->send(modifier.get(), ModifierEvent::UPDATE);
}

ModifierStack Object::getStack() const {
	ModifierStack stack{(uint)this->mModifiers.size(), {}};
	for (auto& modifier : this->mModifiers) {
		std::vector<float> params = modifier->getParams();
		stack.params.insert(stack.params.end(), params.begin(), params.end());
	}
	return stack;
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
