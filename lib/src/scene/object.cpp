#include "jaroViewer/scene/object.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "jaroViewer/core/eventSender.hpp"
#include "jaroViewer/modifiers/modifier.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>

using namespace JaroViewer;

RawObject::RawObject(glm::vec3 minPoint, glm::vec3 maxPoint)
  : mMinPoint(minPoint),
    mMaxPoint(maxPoint),
    mTranslation(0.0f),
    mRotation(glm::identity<glm::quat>()),
    mScale(1.0f),
    mVisibility(true) {}

RawObject::RawObject(RawObject&& other) noexcept
  : EventSender<RawObject, ObjectEvent>(std::move(other)),
    mChildren(other.mChildren),
    mMinPoint(other.mMinPoint),
    mMaxPoint(other.mMaxPoint),
    mTranslation(other.mTranslation),
    mRotation(other.mRotation),
    mScale(other.mScale),
    mVisibility(other.mVisibility),
    mModifiers(std::move(other.mModifiers)) {}

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
	model *= glm::mat4_cast(mRotation);
	model = glm::scale(model, mScale);
	return model;
}

glm::vec3 RawObject::getPosition() const { return mTranslation; }

glm::vec3 RawObject::getEulerAngles() const {
	return glm::eulerAngles(mRotation) * glm::pi<float>() / 180.f;
}

glm::quat RawObject::getQuaternion() const { return mRotation; }

void RawObject::addChild(Object child) { mChildren.push_back(child); }

void RawObject::removeChild(Object child) {
	auto place = std::find(mChildren.begin(), mChildren.end(), child);
	mChildren.erase(place);
}

/**
 * Adds a translation to the current translation
 * @param translation The offset from the current translation
 */
void RawObject::addTranslation(const glm::vec3& translation) {
	mTranslation += translation;
	send(this, ObjectEvent::TRANSFORM);
	for (auto& child : mChildren) child->addTranslation(translation);
}

/**
 * Adds a rotation to the current rotation
 * @param angleX The new angle offset around the X-axis
 * @param angleY The new angle offset around the Y-axis
 * @param angleZ The new angle offset around the Z-axis
 */
void RawObject::addRotation(float angleX, float angleY, float angleZ) {
	glm::quat delta = glm::quat(glm::radians(glm::vec3(angleX, angleY, angleZ)));

	mRotation = glm::normalize(delta * mRotation);
	send(this, ObjectEvent::TRANSFORM);

	for (auto& child : mChildren) {
		child->applyRotationRecursive(delta, mTranslation);
	}
}

/**
 * Adds a scaling offset to the current scale
 * @param scale A vec3 with x component the x scaling, y-component the y scaling and z-component the z scaling
 */
void RawObject::addScale(const glm::vec3& scale) {
	mScale *= scale;

	send(this, ObjectEvent::TRANSFORM);

	for (auto& child : mChildren) {
		child->applyScaleRecursive(scale, mTranslation);
	}
}

/**
 * Adds a scaling offset to the current scale
 * @param scale A float value with which each scale value will be offset
 */
void RawObject::addScale(float scale) { addScale(glm::vec3(scale)); }

/**
 * Sets the component to a position
 * @param translation The new position
 */
void RawObject::setTranslation(const glm::vec3& translation) {
	glm::vec3 offset = translation - mTranslation;
	mTranslation     = translation;
	send(this, ObjectEvent::TRANSFORM);
	for (auto& child : mChildren) child->addTranslation(offset);
}

/**
 * Sets the rotation of the component
 * @param angleX The rotation around the X-axis
 * @param angleY The rotation around the Y-axis
 * @param angleZ The rotation around the Z-axis
 */
void RawObject::setRotation(float angleX, float angleY, float angleZ) {
	glm::quat newRot = glm::quat(glm::radians(glm::vec3(angleX, angleY, angleZ)));

	glm::quat delta = newRot * glm::inverse(mRotation);
	mRotation       = newRot;

	send(this, ObjectEvent::TRANSFORM);

	for (auto& child : mChildren) {
		child->applyRotationRecursive(delta, mTranslation);
	}
}

/**
 * Sets the scale of the component
 * @param scale The new scale of each axis for the component
 */
void RawObject::setScale(const glm::vec3& scale) {
	glm::vec3 ratio = glm::vec3(
	  mScale.x != 0.0f ? scale.x / mScale.x : 1.0f,
	  mScale.y != 0.0f ? scale.y / mScale.y : 1.0f,
	  mScale.z != 0.0f ? scale.z / mScale.z : 1.0f
	);
	mScale = scale;
	send(this, ObjectEvent::TRANSFORM);

	for (auto& child : mChildren) {
		child->applyScaleRecursive(ratio, mTranslation);
	}
}

/**
 * Sets a the scale values to one specific value
 * @param scale The new scale value for each axis
 */
void RawObject::setScale(float scale) { setScale(glm::vec3(scale)); }

void RawObject::addModifier(std::shared_ptr<Modifier> modifier) {
	size_t index = mModifiers.size();
	modifier->addListener([this, index](Modifier*, ModifierEvent event) {
		if (event == ModifierEvent::UPDATE) this->send(this, ObjectEvent::MODIFIER);
		if (event == ModifierEvent::OUTPUT_CHANGE) {
			for (size_t i = index; i < this->mModifiers.size(); ++i) {
				if (i == 0)
					this->mModifiers.at(i)->updateData({.minPoint = mMinPoint, .maxPoint = mMaxPoint});
				else
					this->mModifiers.at(i)->updateData(this->mModifiers.at(i - 1)->getOutputData());
			}
		}
	});

	if (index == 0)
		modifier->updateData({.minPoint = mMinPoint, .maxPoint = mMaxPoint});
	else
		modifier->updateData(mModifiers.back()->getOutputData());

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

glm::mat4 RawObject::getRotationMatrix(const glm::quat& q) {
	return glm::mat4_cast(q);
}

void RawObject::applyRotationRecursive(const glm::quat& delta, const glm::vec3& pivot) {
	glm::mat4 rotMat = glm::mat4_cast(delta);

	glm::vec3 dir = mTranslation - pivot;
	dir           = glm::vec3(rotMat * glm::vec4(dir, 1.0f));
	mTranslation  = pivot + dir;
	mRotation     = glm::normalize(delta * mRotation);
	send(this, ObjectEvent::TRANSFORM);

	for (auto& child : mChildren) {
		child->applyRotationRecursive(delta, pivot);
	}
}

void RawObject::applyScaleRecursive(const glm::vec3& scale, const glm::vec3& pivot) {
	glm::vec3 dir = mTranslation - pivot;
	dir *= scale;
	mTranslation = pivot + dir;
	mScale *= scale;
	send(this, ObjectEvent::TRANSFORM);

	for (auto& child : mChildren) {
		child->applyScaleRecursive(scale, pivot);
	}
}
