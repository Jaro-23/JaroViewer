#pragma once

#include "glm/fwd.hpp"
#include "modifiers/modifier.hpp"
#include <functional>
#include <glm/glm.hpp>
#include <memory>
#include <sys/types.h>

namespace JaroViewer {
	struct ModifierStack {
		uint size;
		std::vector<glm::vec4> params;
	};

	class Object {
	public:
		Object();
		Object(const Object&)            = delete;
		Object& operator=(const Object&) = delete;
		Object(Object&& other) noexcept;
		Object& operator=(Object&& other) noexcept;
		~Object();

		void setVisibility(bool visibility);
		glm::mat4 getModelMatrix() const;

		// Addition transform
		void addTranslation(const glm::vec3& translation);
		void addRotation(float angleX, float angleY, float angleZ);
		void addScale(const glm::vec3& scale);
		void addScale(float scale);

		// Set transform
		void setTranslation(const glm::vec3& translation);
		void setRotation(float angleX, float angleY, float angleZ);
		void setScale(const glm::vec3& scale);
		void setScale(float scale);

		// Modifiers
		void addModifier(std::shared_ptr<Modifier> modifier);
		ModifierStack getModifierStack() const;

		// Events
		void subscribeDelete(std::function<void()> callback);
		void subscribeTransform(std::function<void(glm::mat4)> callback);
		void subscribeVisibility(std::function<void(bool)> callback);

	protected:
		void normalizeAngles();

		// Data
		glm::vec3 mTranslation;
		float mAngleX, mAngleY, mAngleZ;
		glm::vec3 mScale;

		// Modifiers
		std::vector<std::shared_ptr<Modifier>> mModifiers;

		// Event callbacks
		std::vector<std::function<void()>> mDeleteCallbacks;
		std::vector<std::function<void(glm::mat4)>> mTransformCallbacks;
		std::vector<std::function<void(bool)>> mVisibilityCallbacks;
	};
} // namespace JaroViewer
