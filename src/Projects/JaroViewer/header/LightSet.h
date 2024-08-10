#pragma once

#include <GLM/glm.hpp>
#include <memory>
#include <array>

#include "DirectionalLight.h"
#include "PointLight.h"

namespace JaroViewer {
	class LightSet {
		public:
			static const unsigned int mMAXNUMPOINTLIGHTS = 20;

			struct LightSetStruct {
				DirectionalLight::DirectionalLightStruct directionalLight;
				PointLight::PointLightStruct pointLights[mMAXNUMPOINTLIGHTS];
				int numPointLights;
			};

			LightSet(std::shared_ptr<DirectionalLight> dirLight);
			void addPointLight(std::shared_ptr<PointLight> pointLight);

			LightSetStruct getStruct() const;
			DirectionalLight* getDirectionalLight() const;

		private:
			std::shared_ptr<DirectionalLight> mDirectionalLight;
			std::array<std::shared_ptr<PointLight>, mMAXNUMPOINTLIGHTS> mPointLights;

			int mNumPointLights;
	};
}
