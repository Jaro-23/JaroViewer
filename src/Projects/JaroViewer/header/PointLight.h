#pragma once

#include "Component3D.h"
#include "Tools.h"
#include <GLM/glm.hpp>

namespace JaroViewer {
	class PointLight : public JaroViewer::Component3D {
		public:
			struct PointLightStruct{
				glm::vec3 direction;
				float pad0;
				glm::vec3 ambient;
				float constant;
				glm::vec3 diffuse;
				float linear;
				glm::vec3 specular;
				float quadratic;
			};

			PointLight(const Shader &shader, const Shader &wireframeShader, Tools::LightColor lightColor, float constant, float linear, float quadratic);

			PointLightStruct getStruct() const;
			void load() override;

		private:
			Tools::LightColor mLightColor;	

			float mConstant;
			float mLinear;
			float mQuadratic;
	};
}
