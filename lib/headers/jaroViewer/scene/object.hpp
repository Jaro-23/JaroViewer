#pragma once

#include "glm/fwd.hpp"
#include "jaroViewer/modifiers/modifier.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <sys/types.h>

namespace JaroViewer {
	struct ModifierStack {
		uint count;
		std::vector<float> params;
	};

	enum ObjectEvent { CALLBACK, MODIFIER, DELETE, TRANSFORM, VISIBILITY };

	class Object : public EventSender<Object, ObjectEvent> {
	public:
		Object();
		Object(const Object&)            = delete;
		Object& operator=(const Object&) = delete;
		Object(Object&& other) noexcept;
		Object& operator=(Object&& other) noexcept;
		~Object();

		void setVisibility(bool visibility);
		bool getVisibility() const;
		glm::mat4 getModelMatrix() const;
		void addClickCallback(std::function<void()> callback);
		std::function<void()> getLatestCallback() const;

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
		ModifierStack getStack() const;

	protected:
		void normalizeAngles();

		// Data
		glm::vec3 mTranslation;
		float mAngleX, mAngleY, mAngleZ;
		glm::vec3 mScale;
		bool mVisibility;
		std::function<void()> mLatestCallback;

		// Modifiers
		std::vector<std::shared_ptr<Modifier>> mModifiers;
	};
} // namespace JaroViewer
