#pragma once

#include "shader.hpp"
#include "texture2D.hpp"
#include <memory>
#include <string>

namespace JaroViewer {
	struct MaterialArgs {
		std::string diffusePath;
		std::string specularPath;
		float shininess;
	};
	struct ColorMaterialArgs {
		glm::vec4 diffuse;
		glm::vec4 specular;
		float shininess;
	};

	class Material {
	public:
		Material(const Texture2D& diffuse, const Texture2D& specular, float shininess);
		Material(const MaterialArgs& args);
		Material(const ColorMaterialArgs& args);

		void load(const Shader* shader) const;
		void loadIntoArray(const std::shared_ptr<Shader>& shader, int arrayIndex) const;

	private:
		std::string mVariableName;
		std::string mArrayName;

		Texture2D mDiffuse;
		Texture2D mSpecular;
		float mShininess;
	};
} // namespace JaroViewer
