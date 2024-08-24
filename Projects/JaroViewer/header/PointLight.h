#pragma once

#include "Component3D.h"
#include "Tools.h"
#include <GLM/glm.hpp>

namespace JaroViewer {
	class PointLight : public JaroViewer::Component3D {
		public:
			struct PointLightStruct{
				glm::vec3 position;
				bool enable;
				glm::vec3 ambient;
				float constant;
				glm::vec3 diffuse;
				float linear;
				glm::vec3 specular;
				float quadratic;
			};

			PointLight(const Shader &shader, Tools::LightColor lightColor, Tools::AttenuationParams params);

			void enable(bool enable);
			PointLightStruct getStruct() const;
			void load() override;

		private:
			Tools::LightColor mLightColor;	
			bool mEnable;

			float mConstant;
			float mLinear;
			float mQuadratic;
	};
}
