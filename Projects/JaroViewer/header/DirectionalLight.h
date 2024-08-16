#pragma once

#include "Tools.h"
#include <GLM/glm.hpp>

namespace JaroViewer {
	class DirectionalLight {
		public:
			struct DirectionalLightStruct {
				glm::vec3 direction;
				bool enable;
				glm::vec3 ambient;
				float pad1;
				glm::vec3 diffuse;
				float pad2;
				glm::vec3 specular;
				float pad3;
			};

			DirectionalLight(glm::vec3 direction, Tools::LightColor lightColor);

			void enable(bool enable);
			DirectionalLightStruct getStruct() const;

		private:
			glm::vec3 mDirection;
			Tools::LightColor mLightColor;	
			bool mEnable;
	};
}
