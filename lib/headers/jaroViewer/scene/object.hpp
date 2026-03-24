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

	enum ObjectEvent { MODIFIER, DELETE, TRANSFORM, VISIBILITY };

	class RawObject : public EventSender<RawObject, ObjectEvent> {
	public:
		RawObject();
		RawObject(const RawObject&)            = delete;
		RawObject& operator=(const RawObject&) = delete;
		RawObject(RawObject&& other) noexcept;
		RawObject& operator=(RawObject&& other) noexcept;
		~RawObject();

		void setVisibility(bool visibility);
		bool getVisibility() const;
		glm::mat4 getModelMatrix() const;
		glm::vec3 getPosition() const;

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

		// Modifiers
		std::vector<std::shared_ptr<Modifier>> mModifiers;
	};

	using Object = std::shared_ptr<RawObject>;
} // namespace JaroViewer
