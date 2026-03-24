#include "jaroViewer/scene/object.hpp"
#include "jaroViewer/core/eventSender.hpp"
#include "jaroViewer/modifiers/modifier.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>

using namespace JaroViewer;

RawObject::RawObject()
  : mTranslation(0.0f),
    mAngleX(0.0f),
    mAngleY(0.0f),
    mAngleZ(0.0f),
    mScale(1.0f),
    mVisibility(true) {}

RawObject::RawObject(RawObject&& other) noexcept
  : EventSender<RawObject, ObjectEvent>(std::move(other)),
    mTranslation(other.mTranslation),
    mAngleX(other.mAngleX),
    mAngleY(other.mAngleY),
    mAngleZ(other.mAngleZ),
    mScale(other.mScale),
    mVisibility(other.mVisibility),
    mModifiers(std::move(other.mModifiers)) {}

RawObject& RawObject::operator=(RawObject&& other) noexcept {
	if (this == &other) return *this;
	EventSender<RawObject, ObjectEvent>::operator=(std::move(other));
	mTranslation = other.mTranslation;
	mAngleX      = other.mAngleX;
	mAngleY      = other.mAngleY;
	mAngleZ      = other.mAngleZ;
	mScale       = other.mScale;
	mVisibility  = other.mVisibility;
	mModifiers   = std::move(other.mModifiers);
	return *this;
}

RawObject::~RawObject() { send(this, ObjectEvent::DELETE); }

void RawObject::setVisibility(bool visibility) {
	mVisibility = visibility;
	send(this, ObjectEvent::VISIBILITY);
}

bool RawObject::getVisibility() const { return mVisibility; }

/**
 * Returns the model matrix with all the transformations for this component
 */
glm::mat4 RawObject::getModelMatrix() const {
	glm::mat4 model(1.0f);
	model = glm::translate(model, mTranslation);
	model = glm::rotate(model, mAngleX, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, mAngleY, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, mAngleZ, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, mScale);
	return model;
}

glm::vec3 RawObject::getPosition() const { return mTranslation; }

/**
 * Adds a translation to the current translation
 * @param translation The offset from the current translation
 */
void RawObject::addTranslation(const glm::vec3& translation) {
	mTranslation += translation;
	send(this, ObjectEvent::TRANSFORM);
}

/**
 * Adds a rotation to the current rotation
 * @param angleX The new angle offset around the X-axis
 * @param angleY The new angle offset around the Y-axis
 * @param angleZ The new angle offset around the Z-axis
 */
void RawObject::addRotation(float angleX, float angleY, float angleZ) {
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
void RawObject::addScale(const glm::vec3& scale) {
	mScale += scale;
	send(this, ObjectEvent::TRANSFORM);
}

/**
 * Adds a scaling offset to the current scale
 * @param scale A float value with which each scale value will be offset
 */
void RawObject::addScale(float scale) {
	mScale += glm::vec3(scale);
	send(this, ObjectEvent::TRANSFORM);
}

/**
 * Sets the component to a position
 * @param translation The new position
 */
void RawObject::setTranslation(const glm::vec3& translation) {
	mTranslation = translation;
	send(this, ObjectEvent::TRANSFORM);
}

/**
 * Sets the rotation of the component
 * @param angleX The rotation around the X-axis
 * @param angleY The rotation around the Y-axis
 * @param angleZ The rotation around the Z-axis
 */
void RawObject::setRotation(float angleX, float angleY, float angleZ) {
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
void RawObject::setScale(const glm::vec3& scale) {
	mScale = scale;
	send(this, ObjectEvent::TRANSFORM);
}

/**
 * Sets a the scale values to one specific value
 * @param scale The new scale value for each axis
 */
void RawObject::setScale(float scale) {
	mScale = glm::vec3(scale);
	send(this, ObjectEvent::TRANSFORM);
}

void RawObject::addModifier(std::shared_ptr<Modifier> modifier) {
	modifier->addListener([this](Modifier*, ModifierEvent event) {
		if (event == ModifierEvent::UPDATE) this->send(this, ObjectEvent::MODIFIER);
	});

	mModifiers.push_back(modifier);
	modifier->send(modifier.get(), ModifierEvent::UPDATE);
}

ModifierStack RawObject::getStack() const {
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
void RawObject::normalizeAngles() {
	while (mAngleX < 0.0f) mAngleX += 360.0f;
	while (mAngleX > 360.0f) mAngleX -= 360.0f;

	while (mAngleY < 0.0f) mAngleY += 360.0f;
	while (mAngleY > 360.0f) mAngleY -= 360.0f;

	while (mAngleZ < 0.0f) mAngleZ += 360.0f;
	while (mAngleZ > 360.0f) mAngleZ -= 360.0f;
}
