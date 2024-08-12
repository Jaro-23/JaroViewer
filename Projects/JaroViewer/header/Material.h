#pragma once

#include "Shader.h"
#include "Texture2D.h"
#include <string>

namespace JaroViewer {
	class Material {
		public:
			Material(const Texture2D &diffuse, const Texture2D &specular, float shininess);
			Material(const std::string &diffusePath, const std::string &specularPath, float shininess);

			void load(const Shader *shader) const;
			void loadIntoArray(const Shader *shader, int arrayIndex) const;

		private:
			std::string mVariableName;
			std::string mArrayName;

			Texture2D mDiffuse;
			Texture2D mSpecular;
			float mShininess;
	};
}
